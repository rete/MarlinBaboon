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
#include "Utilities/Globals.hh"

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
	StringVector energies;
	string treeName;
	StringVector coreThresholdDensities;
	string pathToRootFiles;

	RawCfgParser *parser = new RawCfgParser();
	parser->Read( cfgFileArg.getValue() );
	parser->GetValue("config","treeName",&treeName);
	parser->GetValue("config","coreThresholdDensities",&coreThresholdDensities);
	parser->GetValue("config","energies",&energies);
	parser->GetValue("config","pathToRootFiles",&pathToRootFiles);

	vector<TObject*> storage;

	// Graphs and histograms declaration
	vector<TGraph*> coreMultiplicityGraphs;
	vector<TGraph*> meanMainCoreSizeGraphs;
	vector<TGraph*> meanCoreSizeGraphs;
	vector<TGraph*> coreSizeLossGraphs;

	for( unsigned int e=0 ; e<energies.size() ; e++ ) {

		coreMultiplicityGraphs.push_back( NewTGraph( coreThresholdDensities.size() , e+1 ) );
		meanMainCoreSizeGraphs.push_back( NewTGraph( coreThresholdDensities.size() , e+1 ) );
		meanCoreSizeGraphs.push_back( NewTGraph( coreThresholdDensities.size() , e+1 ) );
		coreSizeLossGraphs.push_back( NewTGraph( coreThresholdDensities.size() , e+1 ) );

		storage.push_back( coreMultiplicityGraphs.back() );
		storage.push_back( meanMainCoreSizeGraphs.back() );
		storage.push_back( meanCoreSizeGraphs.back() );
		storage.push_back( coreSizeLossGraphs.back() );
	}

	TMultiGraph *coreMultiplicityGraph = new TMultiGraph();
	TMultiGraph *meanCoreSizeGraph = new TMultiGraph();
	TMultiGraph *meanMainCoreSizeGraph = new TMultiGraph();
	TMultiGraph *coreSizeLossGraph = new TMultiGraph();

	TLegend *coreMultiplicityLegend = NewTLegend(0.55,0.65,0.76,0.82);
	TLegend *meanCoreSizeLegend = NewTLegend(0.55,0.65,0.76,0.82);
	TLegend *meanMainCoreSizeLegend = NewTLegend(0.55,0.65,0.76,0.82);
	TLegend *coreSizeLossLegend = NewTLegend(0.55,0.65,0.76,0.82);

	for( unsigned int e=0 ; e<energies.size() ; e++ ) {
		for( unsigned int thr=0 ; thr<coreThresholdDensities.size() ; thr++ ) {

			ostringstream ss;
			ss << pathToRootFiles << "/pi-_" << energies.at(e) << "GeV_" << coreThresholdDensities.at(thr) << "CoreThrDens.root";

			TFile *rootFile = TFile::Open( ss.str().c_str() );
			TTree *tree = (TTree *) rootFile->Get( treeName.c_str() );

			InputTTreeWrapper *wrapper = new InputTTreeWrapper( tree );

			int nbOfEntries = wrapper->GetNbOfEntries();
			double coreMultiplicity = 0;
			double meanCoreSize = 0;
			double meanMainCoreSize = 0;

			for( int entry=0 ; entry<nbOfEntries ; entry++ ) {

				wrapper->LoadEntry( entry );

				int nbOfCores = 0;
				vector<int> coreSizes;
				int mainCoreSize = 0;

				wrapper->GetValue( "nbOfCores" , nbOfCores );
				wrapper->GetValue( "coreSizes" , coreSizes );
				wrapper->GetValue( "mainCoreSize" , mainCoreSize );

				coreMultiplicity += nbOfCores;
				meanMainCoreSize += mainCoreSize;


//				nbOfClustersHisto->Fill( nbOfClusters );
				for( unsigned int i=0 ; i<coreSizes.size() ; i++ ) {
					meanCoreSize += coreSizes.at(i);
				}
				if( !coreSizes.empty() )
					meanCoreSize /= coreSizes.size();
			}

			coreMultiplicity /= nbOfEntries;
			meanCoreSize /= nbOfEntries;
			meanMainCoreSize /= nbOfEntries;

			coreMultiplicityGraphs.at( e )->SetPoint( thr , atof(coreThresholdDensities.at(thr).c_str()) , coreMultiplicity );
			meanMainCoreSizeGraphs.at( e )->SetPoint( thr , atof(coreThresholdDensities.at(thr).c_str()) , meanMainCoreSize );
			meanCoreSizeGraphs.at( e )->SetPoint( thr , atof(coreThresholdDensities.at(thr).c_str()) , meanCoreSize );

			rootFile->Close();
			delete rootFile;

		}


		coreMultiplicityGraph->Add( coreMultiplicityGraphs.at( e ) );
		meanCoreSizeGraph->Add( meanCoreSizeGraphs.at( e ) );
		meanMainCoreSizeGraph->Add( meanMainCoreSizeGraphs.at( e ) );

		coreMultiplicityLegend->AddEntry( coreMultiplicityGraphs.at( e ) , (energies.at(e) + " Gev" ).c_str() , "l" );
		meanCoreSizeLegend->AddEntry( meanCoreSizeGraphs.at( e ) , (energies.at(e) + " Gev" ).c_str() , "l" );
		meanMainCoreSizeLegend->AddEntry( meanMainCoreSizeGraphs.at( e ) , (energies.at(e) + " Gev" ).c_str() , "l" );
	}

/*
	for( unsigned int e=0 ; e<energies.size() ; e++ ) {

		int nbOfEntries = 2;

		int coreSizeStep1 = 0;
		int coreSizeStep2 = 0;

		vector<double> meanCoreSizeLoss(coreThresholdDensities.size()-1 ,0 );

		for( int entry=0 ; entry<nbOfEntries ; entry++  ) {

			for( unsigned int thr=0 ; thr<coreThresholdDensities.size() ; thr++ ) {

				ostringstream ss;
				ss << pathToRootFiles << "/pi-_" << energies.at(e) << "GeV_" << coreThresholdDensities.at(thr) << "CoreThrDens.root";

				TFile *rootFile = TFile::Open( ss.str().c_str() );
				TTree *tree = (TTree *) rootFile->Get( treeName.c_str() );

				InputTTreeWrapper *wrapper = new InputTTreeWrapper( tree );

				nbOfEntries = wrapper->GetNbOfEntries();

				wrapper->LoadEntry( entry );

				int mainCoreSize = 0;
				wrapper->GetValue( "mainCoreSize" , mainCoreSize );

				if( thr == 0 ) {
					coreSizeStep1 = mainCoreSize;
				}
				else {
					coreSizeStep1 = coreSizeStep2;
					coreSizeStep2 = mainCoreSize;
					meanCoreSizeLoss.at( thr-1 ) += double(coreSizeStep1 - coreSizeStep2) / coreSizeStep1 ;
//					coreSizeLossGraphs.at(e)->SetPoint( thr , atof(coreThresholdDensities.at(thr).c_str()) + .5 , double(coreSizeStep1 - coreSizeStep2) / coreSizeStep1  );
				}

				rootFile->Close();
				delete rootFile;
				delete wrapper;
			}
		}

		for( unsigned int thr=1 ; thr<coreThresholdDensities.size() ; thr++ ) {

			coreSizeLossGraphs.at(e)->SetPoint( thr-1 , atof(coreThresholdDensities.at(thr).c_str()) - .5 , meanCoreSizeLoss.at(thr-1) );
		}

		coreSizeLossGraph->Add( coreSizeLossGraphs.at( e ) );
		coreSizeLossLegend->AddEntry( coreSizeLossGraphs.at( e ) , (energies.at(e) + " Gev" ).c_str() , "l" );
	}

*/


	TCanvas *coreMultiplicityCanvas = new TCanvas("coreMultiplicityCanvas","Master Canvas for WaitPrimitive()");
	TCanvas *meanCoreSizeCanvas = new TCanvas("meanCoreSizeCanvas");
	TCanvas *meanMainCoreSizeCanvas = new TCanvas("meanMainCoreSizeCanvas");
	TCanvas *coreSizeLossCanvas = new TCanvas("coreSizeLossCanvas");



	storage.push_back( coreMultiplicityCanvas );
	storage.push_back( meanCoreSizeCanvas );
	storage.push_back( meanMainCoreSizeCanvas );
	storage.push_back( coreSizeLossCanvas );


//	CaliceStyle();

	coreMultiplicityCanvas->cd();
	coreMultiplicityGraph->Draw("alp");
	coreMultiplicityGraph->SetTitle( "Core Multiplicity" );
	coreMultiplicityGraph->GetXaxis()->SetTitle( "Core density threshold" );
	coreMultiplicityGraph->GetYaxis()->SetTitle( "Core multiplicity" );
	coreMultiplicityLegend->Draw();

	meanCoreSizeCanvas->cd();
	meanCoreSizeGraph->Draw("alp");
	meanCoreSizeGraph->SetTitle( "Mean core size" );
	meanCoreSizeGraph->GetXaxis()->SetTitle( "Core density threshold" );
	meanCoreSizeGraph->GetYaxis()->SetTitle( "Mean core size" );
	meanCoreSizeLegend->Draw();

	meanMainCoreSizeCanvas->cd();
	meanMainCoreSizeGraph->Draw("alp");
	meanMainCoreSizeGraph->SetTitle( "Mean main core size" );
	meanMainCoreSizeGraph->GetXaxis()->SetTitle( "Core density threshold" );
	meanMainCoreSizeGraph->GetYaxis()->SetTitle( "Mean main core size" );
	meanMainCoreSizeLegend->Draw();

	coreSizeLossCanvas->cd();
	coreSizeLossGraph->Draw("alp");
	coreSizeLossGraph->SetTitle( "Core size loss" );
	coreSizeLossGraph->GetXaxis()->SetTitle( "Core density threshold" );
	coreSizeLossGraph->GetYaxis()->SetTitle( "Core size loss (%)" );
	coreSizeLossLegend->Draw();

	coreMultiplicityCanvas->Update();
	meanCoreSizeCanvas->Update();
	meanMainCoreSizeCanvas->Update();
	coreSizeLossCanvas->Update();

	coreMultiplicityCanvas->WaitPrimitive();

	for( unsigned int i=0 ; i<storage.size() ; i++ )
		delete storage.at(i);

	storage.clear();

	return 0;
}

