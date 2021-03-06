  /// \file ShowerAgglomerationProcessor.hh
/*
 *
 * ShowerAgglomerationProcessor.hh header template generated by fclass
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


#ifndef SHOWERAGGLOMERATIONPROCESSOR_HH
#define SHOWERAGGLOMERATIONPROCESSOR_HH

#include <iostream> 
#include <string> 
#include <cstdlib> 
#include <cmath> 
#include <vector> 

#include "marlin/Processor.h"
#include "marlin/Exceptions.h"

#include "Monitoring/BaboonMonitoring.hh"

#include "Managers/DetectorManager.hh"
#include "Managers/AlgorithmManager.hh"
#include "Managers/AnalysisManager.hh"
#include "Managers/ClusteringManager.hh"
#include "Managers/CoreManager.hh"
#include "Managers/ShowerManager.hh"
#include "Managers/TrackManager.hh"

#include "Detector/SDHCAL.hh"

#include "Utilities/Globals.hh"
#include "Utilities/Internal.hh"

/* 
 * Class ShowerAgglomerationProcessor
 * Inherits from base class marlin::Processor
 */ 


struct ConnectedHits {

	baboon::CaloHit *theHit;
	baboon::CaloHitCollection *otherHits;
	std::vector<double> distances;
	std::map<double,baboon::CaloHit*> distanceToCaloHitMap;
};

typedef std::vector<ConnectedHits*> ConnectedHitsCollection;

class ShowerAgglomerationProcessor : public marlin::Processor {

	public:

		/*! Default Constructor */
		ShowerAgglomerationProcessor();

		/*! Default Destructor */
		virtual ~ShowerAgglomerationProcessor();


		virtual Processor *newProcessor()
			{ return new ShowerAgglomerationProcessor(); }

		/*!
		 *
		 * @brief Must be defined by the user. Init the BABOON processor
		 *
		 */
		virtual void init();

		/*!
		 *
		 * @brief Must be defined by the user. Process the run header.
		 *
		 */
		virtual void processRunHeader( EVENT::LCRunHeader* run  );

		/*!
		 *
		 * @brief Must be defined by the user. Process an event in the BABOON framework
		 *
		 */
		virtual void processEvent( EVENT::LCEvent * );

		/*!
		 *
		 * @brief Must be defined by the user. Check the event.
		 *
		 */
		virtual void check( EVENT::LCEvent * evt );

		/*!
		 *
		 * @brief Must be defined by the user. Called after processing all the events.
		 *
		 */
		virtual void end();

		/*!
		 *
		 * @brief  Load the sdhcal hits in the calorimeter from the LCEvent object
		 *
		 */
		baboon::Return LoadSDHCALHits( baboon::SDHCAL *sdhcal , EVENT::LCEvent * );

		/*!
		 *
		 * @brief  Clear the content for managers and detectors
		 *
		 */
		baboon::Return ClearContent();

		/*!
		 *
		 * @brief Get the density of a sdhcal calo hit
		 *
		 */
		baboon::Return GetCaloHitDensity( baboon::CaloHit *caloHit , double &density );

		/*!
		 *
		 *
		 *
		 *
		 */
		void BuildShowersFromDistances();

		/*!
		 *
		 *
		 *
		 *
		 */
		baboon::Return KeepGoodConnectors( ConnectedHitsCollection *connectedHitsCollection );

		/*!
		 *
		 *
		 *
		 *
		 */
		baboon::Return KeepGoodConnection( ConnectedHits *connectedHits );



	protected:

		// The sdhcal calorimeter instance
		baboon::SDHCAL *sdhcal;

		// monitoring
		baboon::BaboonMonitoring *monitoring;

		// managers
		baboon::DetectorManager *detectorManager;
		baboon::AlgorithmManager *algorithmManager;
		baboon::AnalysisManager *analysisManager;
		baboon::ClusteringManager *clusteringManager;
		baboon::CoreManager *coreManager;
		baboon::ShowerManager *showerManager;
		baboon::TrackManager *trackManager;

		// processor parameters
		bool enableMonitoring;
		std::string algorithmConfigFileName;
		std::string displayMode;
		std::string decoderString;
		std::string SDHCALCollectionName;
		baboon::StringVector IJKEncoding;
		std::string gearFile;
		std::string rootOutputFile;

		double lowestDensityLimit;
		double highestDensityLimit;
		double densityStep;
		double scalarProductLimitBetweenShower;
		double pcaAngleLimit;
		double limitDistance;


};  // class 


#endif  //  SHOWERAGGLOMERATIONPROCESSOR_HH
