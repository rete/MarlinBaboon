  /// \file ShowerSplitterProcessor.cc
/*
 *
 * ShowerSplitterProcessor.cc source template generated by fclass
 * Creation date : jeu. mai 9 2013
 * Copyright (c) CNRS , IPNL
 *
 * All Right Reserved.
 * Use and copying of these libraries and preparation of derivative works
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * @author : rete
 */


#include "MonitoringTestProcessor.hh"

#include "Utilities/Globals.hh"

#include "UTIL/CellIDDecoder.h"
#include "EVENT/CalorimeterHit.h"
#include "marlin/Global.h"


#include "Algorithm/EventPreparation.hh"

MonitoringTestProcessor aMonitoringTestProcessor;


using namespace std;
using namespace baboon;
using namespace cfgparser;
using namespace EVENT;

using namespace marlin;

MonitoringTestProcessor::MonitoringTestProcessor()
	: Processor("MonitoringTestProcessor") {

	  _description = "MonitoringTestProcessor for event display in SDHCAL";

	  registerProcessorParameter("decoderString" ,
				     "decoder string for cell ID decoder" ,
				     decoderString,
				     string("M:3,S-1:3,I:9,J:9,K-1:6"));

	  registerProcessorParameter("SDHCALCollectionName" ,
				     "collection name for SDHCAL hits" ,
				     SDHCALCollectionName,
				     string("HCALBarrel"));

	  vector<string> ijkVec;
	  ijkVec.push_back("I");
	  ijkVec.push_back("J");
	  ijkVec.push_back("K-1");

	  registerProcessorParameter("IJKEncoding",
				     "I J K hit encoding",
				     IJKEncoding,
				     ijkVec);

	  registerProcessorParameter("displayMode" ,
				     "Display mode" ,
				     displayMode,
				     string("threshold") );

}

MonitoringTestProcessor::~MonitoringTestProcessor() {}


void MonitoringTestProcessor::init() {

	monitoring = BaboonMonitoring::GetInstance();

	BaboonMonitoring::SetEnable( true );

	detectorManager = DetectorManager::GetInstance();

	if( !detectorManager->DetectorIsRegistered("SDHCAL") ) {

		sdhcal = new SDHCAL();
		BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , detectorManager->RegisterDetector( sdhcal ) );

	}
	else {
		sdhcal = (SDHCAL *) detectorManager->GetDetector("SDHCAL");
	}

	if( displayMode == "thresholds" )
		sdhcal->SetHitDisplayMode( kDisplayThresholds );
	else if( displayMode == "showers" )
		sdhcal->SetHitDisplayMode( kDisplayShowers );
	else if( displayMode == "tags" )
		sdhcal->SetHitDisplayMode( kDisplayTags );
	else if( displayMode == "uniform" )
		sdhcal->SetHitDisplayMode( kDisplayUniform );
	else if( displayMode == "density" )
		sdhcal->SetHitDisplayMode( kCustom );
	else if( displayMode == "custom" )
		sdhcal->SetHitDisplayMode( kCustom );
	else
		sdhcal->SetHitDisplayMode( kDisplayThresholds );

	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , detectorManager->Init( marlin::Global::GEAR ) );


	algorithmManager = AlgorithmManager::GetInstance();
	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->RegisterAlgorithm( new EventPreparation() ) );

	algorithmManager->SetConfigFileName("/home/remi/ilcsoft/SDHCAL/Baboon/config/Algorithm.cfg");

	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , algorithmManager->Initialize() );

}

void MonitoringTestProcessor::processRunHeader( LCRunHeader* run ) {


}

void MonitoringTestProcessor::processEvent( EVENT::LCEvent * evt ) {

	cout << "evt " << evt->getEventNumber() << endl;

	BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , this->LoadSDHCALHits( sdhcal , evt ) );

	CaloHitCollection *caloHitCollection = sdhcal->GetCaloHitCollection();

	if( caloHitCollection->empty() )
		StopProcessingException( this );

	if( algorithmManager->AlgorithmIsRegistered( "EventPreparation" ) )
		algorithmManager->GetAlgorithm( "EventPreparation" )->Process();


	if( displayMode == "density" ) {

		for( unsigned int h=0 ; h<caloHitCollection->size() ; h++ ) {

			double density = caloHitCollection->at( h )->GetDensity();
			int color = BaboonMonitoring::GetColorBetweenMinAndMax( std::log(1.0) , std::log( 4.0 ) , std::log( density + 1.0 ) );
			caloHitCollection->at( h )->SetColor( color );
		}

	}

	monitoring->ViewEvent();


	sdhcal->ClearContent();
}


void MonitoringTestProcessor::check( LCEvent *evt ) {

}

void MonitoringTestProcessor::end() {

}


baboon::Return MonitoringTestProcessor::LoadSDHCALHits( SDHCAL *sdhcal , EVENT::LCEvent *evt ) {

	try {

		LCCollection *collection = evt->getCollection( SDHCALCollectionName );
		UTIL::CellIDDecoder<CalorimeterHit>::setDefaultEncoding( decoderString );
		UTIL::CellIDDecoder<CalorimeterHit> cellIdDecoder( collection );

		for( unsigned int i=0 ; i<collection->getNumberOfElements() ; i++ ) {

			if( collection->getTypeName() != LCIO::CALORIMETERHIT )
				return  BABOON_INVALID_PARAMETER("Collection name must be a CalorimeterHit collection");

			CalorimeterHit *hit = static_cast<CalorimeterHit*> ( collection->getElementAt(i) );

			CaloHit *caloHit = new CaloHit( fSemiDigitalCaloHit );
			if( hit->getEnergy() == 1.0 ) caloHit->SetThreshold( fCaloHitThr1 );
			else if( hit->getEnergy() == 2.0 ) caloHit->SetThreshold( fCaloHitThr2 );
			else if( hit->getEnergy() == 3.0 ) caloHit->SetThreshold( fCaloHitThr3 );
			else
				return BABOON_INVALID_PARAMETER("Energy of SDHCAL hit is not in threshold unit (1,2,3)");

			ThreeVector position;
			position.setX( hit->getPosition()[0] );
			position.setY( hit->getPosition()[1] );
			position.setZ( hit->getPosition()[2] );
			caloHit->SetPosition( position );
			int I = cellIdDecoder(hit)[IJKEncoding.at(0).c_str()];
			int J = cellIdDecoder(hit)[IJKEncoding.at(1).c_str()];
			int K = cellIdDecoder(hit)[IJKEncoding.at(2).c_str()];
			caloHit->SetIJK( I , J , K );
			caloHit->SetTypeID( hit->getType() );
			caloHit->SetTime( hit->getTime() );

			BABOON_THROW_RESULT_IF( BABOON_SUCCESS() , != , sdhcal->AddCaloHit( caloHit ) );
		}
	}
	catch ( DataNotAvailableException &e ) {

		return BABOON_ERROR( "LCIO exception thrown : " + string(e.what()) );
	}
	catch ( baboon::Exception &e ) {

		return BABOON_ERROR( "Failed to create hit. " + string(e.what()) );
	}

	return BABOON_SUCCESS();

}

