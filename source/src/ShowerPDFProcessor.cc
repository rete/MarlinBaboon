  /// \file ShowerPDFProcessor.cc
/*
 *
 * ShowerPDFProcessor.cc source template generated by fclass
 * Creation date : ven. mai 31 2013
 * Copyright (c) CNRS , IPNL
 *
 * All Right Reserved.
 * Use and copying of these libraries and preparation of derivative works
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * @author : rete
 */


#include "MarlinProcessor/ShowerPDFProcessor.hh"
#include "Algorithm/Calorimetry/ClusteringAlgorithm.hh"
#include "Algorithm/Calorimetry/CoreFinderAlgorithm.hh"
#include "Algorithm/Calorimetry/HoughTransformAlgorithm.hh"
#include "Algorithm/Calorimetry/IsolationTaggingAlgorithm.hh"


ShowerPDFProcessor aShowerPDFProcessor;

using namespace std;
using namespace baboon;
using namespace EVENT;

ShowerPDFProcessor::ShowerPDFProcessor()
	: BaboonProcessor("ShowerPDFProcessor") {

	  _description = "ShowerPDFProcessor for shower splitting in SDHCAL";

}

ShowerPDFProcessor::~ShowerPDFProcessor() {}


Return ShowerPDFProcessor::Init() {

	// Add the Hough Transform Algorithm for track reconstruction within the sdhcal
	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->RegisterAlgorithm( new HoughTransformAlgorithm() ) );

	// Add isolation tagging algorithm
	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->RegisterAlgorithm( new IsolationTaggingAlgorithm() ) );

	// Add clustering (2D) algorithm
	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->RegisterAlgorithm( new ClusteringAlgorithm() ) );

	// Add core finder algorithm
	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->RegisterAlgorithm( new CoreFinderAlgorithm() ) );

	// Add cone beginning algorithm
//	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->RegisterAlgorithm( new ConeBeginningAlgorithm() ) ;

	return BABOON_SUCCESS();
}

Return ShowerPDFProcessor::ProcessRunHeader( LCRunHeader* run ) {

	return BABOON_SUCCESS();
}

Return ShowerPDFProcessor::ProcessEvent( EVENT::LCEvent * evt ) {


	HitCollection *hitCollection = hitManager->GetHitCollection();

	if( hitCollection->empty() )
		return BABOON_NOT_INITIALIZED("No hit in the current event. Framework not initialized!");


	if( algorithmManager->AlgorithmIsRegistered("IsolationTaggingAlgorithm") ) {

		cout << "IsolationTaggingAlgorithm found" << endl;
		IsolationTaggingAlgorithm* isolationAlgo = (IsolationTaggingAlgorithm *) algorithmManager->GetAlgorithm("IsolationTaggingAlgorithm");
		isolationAlgo->Process();
	}

	if( algorithmManager->AlgorithmIsRegistered("ClusteringAlgorithm") ) {

		cout << "ClusteringAlgorithm found" << endl;
		ClusterCollection *clustCol = new ClusterCollection();
		ClusteringAlgorithm* clusteringAlgo = (ClusteringAlgorithm *) algorithmManager->GetAlgorithm("ClusteringAlgorithm");
		clusteringAlgo->SetClusteringMode( fClustering2D );
		clusteringAlgo->SetTaggingMode( fClusterTagMode );
		clusteringAlgo->SetClusterCollection( clustCol );
		clusteringAlgo->Process();

		for( unsigned int i=0 ; i<clustCol->size() ; i++ )
			BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , clusteringManager->AddCluster( clustCol->at(i) ) );

		clustCol->clear();
		delete clustCol;
	}

	if( algorithmManager->AlgorithmIsRegistered("HoughTransformAlgorithm") ) {

		cout << "HoughTransformAlgorithm found" << endl;
		HoughTransformAlgorithm *houghTransform = (HoughTransformAlgorithm*) algorithmManager->GetAlgorithm("HoughTransformAlgorithm");
		houghTransform->Process();
	}

	HitCollection *subHitCollection = new HitCollection();

	for( unsigned int i=0 ; i<hitCollection->size() ; i++ ) {

		Tag tag = hitCollection->at(i)->GetHitTag();

		if( tag == fTrack || tag == fIsolated )
			continue;
		subHitCollection->push_back( hitCollection->at(i) );
	}

	ThreeVector barycenter = subHitCollection->GetBarycenter();
//	cout << "subHitCollection->GetBarycenter() : " << barycenter << endl;

	int showerLayerBegin = -1000;

	for( unsigned int i=0 ; i<hitCollection->size() ; i++ ) {

		IntVector ijk = hitCollection->at(i)->GetIJK();

		if( abs(ijk.at(0)-int(barycenter.x()) )  || abs(ijk.at(1)-int(barycenter.y()) )  )
			continue;
		int count = 0;

		for( unsigned int j=0 ; j<hitCollection->size() ; j++ ) {

			if(i==j) continue;
			IntVector ijk2 = hitCollection->at(j)->GetIJK();
			if( ijk.at(2) == ijk2.at(2)
			&& abs(ijk2.at(0)-int(barycenter.x()) )
			&& abs(ijk2.at(1)-int(barycenter.y()) ) )
				count++;
		}

		if( count < 5 ) continue;

		int count2[3];

		for(int j=0; j<3; j++)
			count2[j]=0;

		for( unsigned int j=0 ; j<hitCollection->size() ; j++ ) {

			IntVector ijk2 = hitCollection->at(j)->GetIJK();
			if( abs(ijk2.at(0)-int(barycenter.x()) ) > 5
			 && abs(ijk2.at(1)-int(barycenter.y()) ) > 5   )
				continue;

	      if(ijk2.at(2)==ijk.at(2)+1) count2[0]++;
	      if(ijk2.at(2)==ijk.at(2)+2) count2[1]++;
	      if(ijk2.at(2)==ijk.at(2)+3) count2[2]++;
		}

	    if( count2[0]>3 && count2[1]>3 && count2[2]>3 ) {
	    	showerLayerBegin = ijk.at(2);
	      break;
	    }
	}

//	cout << "shower beginning : " << showerLayerBegin << endl;

	ThreeVector showerBeginning( barycenter.x() , barycenter.y() , showerLayerBegin );

	vector<double> zPos;
	vector<double> xPos;
	vector<double> rPos;

	for( unsigned int i=0 ; i<hitCollection->size() ; i++ ) {

		Tag tag = hitCollection->at(i)->GetHitTag();

		if( tag == fTrack || tag == fIsolated )
			continue;

		if( hitCollection->at(i)->GetIJK().at(2) < showerLayerBegin )
			continue;

		zPos.push_back( hitCollection->at(i)->GetIJK().at(2) - showerBeginning.z() );
		xPos.push_back( hitCollection->at(i)->GetIJK().at(0) - showerBeginning.x() );
		double r = sqrt( pow(hitCollection->at(i)->GetIJK().at(0)-barycenter.x(),2) + pow(hitCollection->at(i)->GetIJK().at(1)-barycenter.y(),2) );
		rPos.push_back( r );
	}

	analysisManager->Set("ShowerPDF","zPos",&zPos);
	analysisManager->Set("ShowerPDF","xPos",&xPos);
	analysisManager->Set("ShowerPDF","rPos",&rPos);

	analysisManager->Fill("ShowerPDF");

	delete subHitCollection;
	return BABOON_SUCCESS();
}


Return ShowerPDFProcessor::Check( LCEvent *evt ) {

	return BABOON_SUCCESS();
}

Return ShowerPDFProcessor::End() {

	return BABOON_SUCCESS();
}


