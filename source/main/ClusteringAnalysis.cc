/*
 *
 * //OverlayEstimatorAnalysis.cc main file
 * Copyright (c) CNRS / IPNL
 * All Right Reserved.
 *
 * Use and copying of this software and preparation of derivative works
 * based upon this software are permitted. Any copy of this software or
 * of any derivative work must include this paragraph.
 *
 * Written by : R. Eté
 *
 */


// std includes
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iomanip>

// CfgParser includes
#include "CfgParser/RawCfgParser.hh"
#include "CfgParser/Section.hh"

// baboon includes
#include "Analysis/InputTreeWrapper.hh"
#include "Analysis/BasicDraw.hh"
#include "Utilities/Numeric.hh"

// root includes
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TLegend.h"

// tclap includes
#include "tclap/CmdLine.h"

using namespace std;
using namespace cfgparser;
using namespace baboon;

// for debug messages (devel)
#define __DEBUG__ 0



int main (int argc ,char *argv[]) {


	cout << "Don't forget to source init_Baboon.sh script before running this..." << endl;
	/********************************************
	 * Grab the most useful environment variable
	 ********************************************/

	char *pathToBab = NULL;
	pathToBab = getenv("BABOON_HOME");
	if( pathToBab == NULL )
		throw runtime_error("'BABOON_HOME' env variable is not set.\n "
		"Please source init_Baboon.sh before running.");

	string pathToBaboon(pathToBab);

	/*********************************
	 * Define the command line parser
	 *********************************/

	string footer = "Please report bug to <rete@ipnl.in2p3.fr>";
	TCLAP::CmdLine cmd(footer, ' ', "1.0");

	TCLAP::ValueArg<std::string> cfgFileArg(
				"f"
				,"config"
				,"config file for analysis"
				,true
				,""
				,"string" );


	cmd.add( cfgFileArg );
	cmd.parse( argc, argv );

	// Mandatory for executables if graphs...
	TApplication app("appOfTheDeath",0,0);

	// Load the cfg file and grabs the variables
	string energy;
	string rootFileName;
	string treeName;

	RawCfgParser *parser = new RawCfgParser();
	parser->Read( cfgFileArg.getValue() );
	parser->GetValue("config","treeName",&treeName);
	parser->GetValue("config","rootFileName",&rootFileName);
	parser->GetValue("config","energy",&energy);

	vector<TObject*> storage;


	// Graphs and histograms declaration
	TH1D *nbOfClustersHisto = NewTH1D( "nbOfClustersHisto" , "" , 100 , 0 , 1 , kBlack );
	TH1D *clusterSizesHisto  = NewTH1D( "clusterSizesHisto"  , "" , 50 , 0 , 50 , kBlack );

	string nbOfClustersTitle = "Nb of clusters (E = " + energy + " GeV)";
	string clusterSizesTitle = "Cluster sizes (E = " + energy + " GeV)";

	gStyle->SetOptFit(1);
	gStyle->SetOptStat(1);

	nbOfClustersHisto->SetTitle( nbOfClustersTitle.c_str() );
	clusterSizesHisto->SetTitle( clusterSizesTitle.c_str() );

	storage.push_back( nbOfClustersHisto );
	storage.push_back( clusterSizesHisto );

	TFile *rootFile = TFile::Open( rootFileName.c_str() );
	TTree *tree = (TTree *) rootFile->Get( treeName.c_str() );

	InputTTreeWrapper *wrapper = new InputTTreeWrapper( tree );

	int nbOfEntries = wrapper->GetNbOfEntries();

	for( int entry=0 ; entry<nbOfEntries ; entry++ ) {

		wrapper->LoadEntry( entry );

		 int nbOfClusters = 0;
		 vector<int> clusterSizes;
		 int nbOfHits = 0;


		wrapper->GetValue( "nbOfClusters" , nbOfClusters );
		wrapper->GetValue( "clusterSizes" , clusterSizes );
		wrapper->GetValue( "nbOfHits" , nbOfHits );

		nbOfClustersHisto->Fill( double(nbOfClusters) / nbOfHits );
		for( unsigned int i=0 ; i<clusterSizes.size() ; i++ )
			clusterSizesHisto->Fill( clusterSizes.at(i) );
	}

	rootFile->Close();
	delete rootFile;

	TCanvas *nbOfClustersCanvas = new TCanvas("nbOfClustersCanvas","Master Canvas for WaitPrimitive()");
	TCanvas *clusterSizesCanvas = new TCanvas("clusterSizesCanvas");

	storage.push_back( nbOfClustersCanvas );
	storage.push_back( clusterSizesCanvas );

//	CaliceStyle();

	nbOfClustersCanvas->cd();
	nbOfClustersHisto->Draw();

	clusterSizesCanvas->cd();
	clusterSizesHisto->Draw();

	nbOfClustersCanvas->Update();
	clusterSizesCanvas->Update();

	nbOfClustersCanvas->WaitPrimitive();

	for( unsigned int i=0 ; i<storage.size() ; i++ )
		delete storage.at(i);

	storage.clear();

	return 0;
}

