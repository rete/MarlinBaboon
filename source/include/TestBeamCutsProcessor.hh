  /// \file TestBeamCutsProcessor.hh
/*
 *
 * TestBeamCutsProcessor.hh header template generated by fclass
 * Creation date : lun. mai 27 2013
 * Copyright (c) CNRS , IPNL
 *
 * All Right Reserved.
 * Use and copying of these libraries and preparation of derivative works
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * @author : rete
 */


#ifndef TESTBEAMCUTSPROCESSOR_HH
#define TESTBEAMCUTSPROCESSOR_HH


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

// lcio includes
#include "IOIMPL/LCFactory.h"
#include "IO/LCWriter.h"

/* 
 * Class TestBeamCutsProcessor
 * Inherits from base class marlin::Processor
 */ 

class TestBeamCutsProcessor : public marlin::Processor {

	public:

		/*!
		 *
		 * Default Constructor
		 *
		 */
		TestBeamCutsProcessor();

		/*!
		 *
		 * Default Destructor
		 *
		 */
		virtual ~TestBeamCutsProcessor();


		virtual Processor *newProcessor()
			{ return new TestBeamCutsProcessor(); }

		/*!
		 *
		 * @brief  Init the processor
		 *
		 */
		virtual void init();

		/*!
		 *
		 * @brief  Process the run header.
		 *
		 */
		virtual void processRunHeader( EVENT::LCRunHeader* run  );

		/*!
		 *
		 * @brief  Process an event
		 *
		 */
		virtual void processEvent( EVENT::LCEvent * );

		/*!
		 *
		 * @brief  Check the event.
		 *
		 */
		virtual void check( EVENT::LCEvent * evt );

		/*!
		 *
		 * @brief  Called after processing all the events.
		 *
		 */
		virtual void end();

		/*!
		 *
		 * @brief  Load the sdhcal hits in the calorimeter from the LCEvent object
		 *
		 */
		baboon::Return LoadSDHCALHits( baboon::SDHCAL *sdhcal , EVENT::LCEvent * );

	protected:

		// output file
		std::string slcioOutputFile;
		IO::LCWriter* lcWriter;

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
		std::string overlayEstimatorMethod;
		std::string decoderString;
		std::string SDHCALCollectionName;
		baboon::StringVector IJKEncoding;
		std::string gearFile;
		double electronConcentrationCut;
		double muonConcentrationCut;
		int totalNbOfHitsCut;
		double muonMultiplicityCut;
		int nbOfProcessedEvent;
		int nbOfCutOnMuon;
		int nbOfCutOnElectron;
		int nbOfCutOnHits;



};  // class 


#endif  //  TESTBEAMCUTSPROCESSOR_HH
