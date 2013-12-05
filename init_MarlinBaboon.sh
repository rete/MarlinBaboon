##########################################
# init_MarlinBaboon.sh file to load environment
# @author : rete , IPNL / CNRS
# @date : 03/05/13
#!/bin/sh
##########################################


export MARLIN_BABOON_HOME="/home/remi/ilcsoft/SDHCAL/MarlinBaboon"

export MARLIN_BABOON_BIN_DIR=$MARLIN_BABOON_HOME"/bin"
export MARLIN_BABOON_LIB_DIR=$MARLIN_BABOON_HOME"/lib"

export PATH=$PATH:$MARLIN_BABOON_BIN_DIR
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARLIN_BABOON_LIB_DIR

export MARLIN_BABOON_DLL=$MARLIN_BABOON_LIB_DIR"/libShowerSplitter.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libOverlayEstimator.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libTestBeamCutsProcessor.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libMonitoringTestProcessor.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libShowerProcessor.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libTrackFinderProcessor.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libShowerAgglomerationProcessor.so"
export MARLIN_BABOON_DLL=$MARLIN_BABOON_DLL:$MARLIN_BABOON_LIB_DIR"/libEnergyMinimizerProcessor.so"

export MARLIN_DLL=$MARLIN_BABOON_DLL
