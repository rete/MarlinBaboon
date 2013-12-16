#######################################################
# cmake file for building MarlinBaboon package
# @author rete , IPNL
#######################################################

# include directories
INCLUDE_DIRECTORIES( include )
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_BINARY_DIR}" ) # generated MarlinBaboonConfig.h


# require proper c++
ADD_DEFINITIONS( "-pedantic -Wunused-value -O2" )
ADD_DEFINITIONS("-Wno-long-long -Wreturn-type")
IF( OPENMP_FOUND )
	ADD_DEFINITIONS("-fopenmp")
ENDIF()


# --------------------- Cut Processor lib -------------------------------

FILE (
  GLOB_RECURSE
  CutProcessor_FILES
  include/CutProcessor.hh
  src/CutProcessor.cc
)
ADD_LIBRARY( CutProcessor_lib SHARED ${CutProcessor_FILES} )
SET_TARGET_PROPERTIES( CutProcessor_lib PROPERTIES OUTPUT_NAME CutProcessor )
INSTALL(
  TARGETS CutProcessor_lib
  LIBRARY DESTINATION lib 
)



# --------------------- Shower Processor lib -------------------------------

FILE (
  GLOB_RECURSE
  ShowerProcessor_FILES
  include/ShowerProcessor.hh
  src/ShowerProcessor.cc
)
ADD_LIBRARY( ShowerProcessor_lib SHARED ${ShowerProcessor_FILES} )
SET_TARGET_PROPERTIES( ShowerProcessor_lib PROPERTIES OUTPUT_NAME ShowerProcessor )
INSTALL(
  TARGETS ShowerProcessor_lib
  LIBRARY DESTINATION lib 
)


# --------------------- Shower Splitter lib -------------------------------

FILE (
	GLOB_RECURSE
	ShowerSplitterProcessor_FILES
	include/ShowerSplitterProcessor.hh
	src/ShowerSplitterProcessor.cc
)
ADD_LIBRARY( ShowerSpliter_lib SHARED ${ShowerSplitterProcessor_FILES} )
SET_TARGET_PROPERTIES( ShowerSpliter_lib PROPERTIES OUTPUT_NAME ShowerSplitter )
INSTALL(
	TARGETS ShowerSpliter_lib
	LIBRARY DESTINATION lib 
)


# --------------------- TestBeamCutsProcessor lib -------------------------------

FILE (
	GLOB_RECURSE
	TestBeamCutsProcessor_FILES
	include/TestBeamCutsProcessor.hh
	src/TestBeamCutsProcessor.cc
)
ADD_LIBRARY( TestBeamCutsProcessor_lib SHARED ${TestBeamCutsProcessor_FILES} )
SET_TARGET_PROPERTIES( TestBeamCutsProcessor_lib PROPERTIES OUTPUT_NAME TestBeamCutsProcessor )
INSTALL(
	TARGETS TestBeamCutsProcessor_lib
	LIBRARY DESTINATION lib 
)

# --------------------- Monitoring Test lib processor -------------------------------

FILE (
	GLOB_RECURSE
	MonitoringTestProcessor_FILES
	include/MonitoringTestProcessor.hh
	src/MonitoringTestProcessor.cc
)
ADD_LIBRARY( MonitoringTestProcessor_lib SHARED ${MonitoringTestProcessor_FILES} )
SET_TARGET_PROPERTIES( MonitoringTestProcessor_lib PROPERTIES OUTPUT_NAME MonitoringTestProcessor )
INSTALL(
	TARGETS MonitoringTestProcessor_lib
	LIBRARY DESTINATION lib 
)


# --------------------- TrackFinder lib processor -------------------------------

FILE (
	GLOB_RECURSE
	TrackFinderProcessor_FILES
	include/TrackFinderProcessor.hh
	src/TrackFinderProcessor.cc
)
ADD_LIBRARY( TrackFinderProcessor_lib SHARED ${TrackFinderProcessor_FILES} )
SET_TARGET_PROPERTIES( TrackFinderProcessor_lib PROPERTIES OUTPUT_NAME TrackFinderProcessor )
INSTALL(
	TARGETS TrackFinderProcessor_lib
	LIBRARY DESTINATION lib 
)


# --------------------- ShowerAgglomeration lib processor -------------------------------

FILE (
	GLOB_RECURSE
	ShowerAgglomerationProcessor_FILES
	include/ShowerAgglomerationProcessor.hh
	src/ShowerAgglomerationProcessor.cc
)
ADD_LIBRARY( ShowerAgglomerationProcessor_lib SHARED ${ShowerAgglomerationProcessor_FILES} )
SET_TARGET_PROPERTIES( ShowerAgglomerationProcessor_lib PROPERTIES OUTPUT_NAME ShowerAgglomerationProcessor )
INSTALL(
	TARGETS ShowerAgglomerationProcessor_lib
	LIBRARY DESTINATION lib 
)



# --------------------- EnergyMinimizerProcessor lib processor -------------------------------

FILE (
  GLOB_RECURSE
  EnergyMinimizerProcessor_FILES
  include/EnergyMinimizerProcessor.hh
  src/EnergyMinimizerProcessor.cc
)
ADD_LIBRARY( EnergyMinimizerProcessor_lib SHARED ${EnergyMinimizerProcessor_FILES} )
SET_TARGET_PROPERTIES( EnergyMinimizerProcessor_lib PROPERTIES OUTPUT_NAME EnergyMinimizerProcessor )
INSTALL(
  TARGETS EnergyMinimizerProcessor_lib
  LIBRARY DESTINATION lib 
)



# ------------------ Overlay Estimator Analysis --------------------
SET( 
	OVERLAY_ESTIMATOR_ANALYSIS_SRC_FILES 
	main/OverlayEstimatorAnalysis.cc 
)
ADD_EXECUTABLE( OverlayEstimatorAnalysis ${OVERLAY_ESTIMATOR_ANALYSIS_SRC_FILES} )


# ------------------ Overlay Reco Energy --------------------
SET( 
	OVERLAY_RECO_ENERGY_SRC_FILES 
	main/OverlayRecoEnergy.cc 
)
ADD_EXECUTABLE( OverlayRecoEnergy ${OVERLAY_RECO_ENERGY_SRC_FILES} )




#-------------------- install ----------------------- 
INSTALL( 
	TARGETS 
	OverlayEstimatorAnalysis OverlayRecoEnergy
	DESTINATION 
	bin
)

