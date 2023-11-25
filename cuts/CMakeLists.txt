Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CUTS}
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
 ${BASE_INCLUDE_DIRECTORIES}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${GSL_LIBRARY_DIRS} 
  ${FAIRROOT_LIBRARY_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

 
Link_Directories( ${LINK_DIRECTORIES})

set(SRCS

SubCut.cxx
Cut.cxx
CutContainer.cxx
CutCollection.cxx
EventCut.cxx
TrackCut.cxx
TwoTrackCut.cxx
CutsAndMonitors.cxx
CutMonitorRequest.cxx
CutFuncs.cxx
#EVENT CUTS
eventcuts/mc/EventImpactParameterCut.cxx
eventcuts/EventModuloCut.cxx
eventcuts/general/EventMultiplicityCut.cxx
eventcuts/general/EventPrimaryTracksNoCut.cxx
eventcuts/general/EventPhiCut.cxx
eventcuts/general/EventPtMaxCut.cxx
eventcuts/general/EventVertexCut.cxx
eventcuts/general/EventMultiplicityZVertexCut.cxx
eventcuts/detector/EventRunCut.cxx
eventcuts/EventVirtualCut.cxx
eventcuts/EventComplexCut.cxx
eventcuts/EventBinningCut.cxx
eventcuts/EventExpCut.cxx
eventcuts/EventMcCut.cxx
#HAL TRACK CUT
trackcuts/detector/TrackBasicToFCut.cxx
trackcuts/detector/TrackToFMass2Cut.cxx
trackcuts/detector/TrackDCACut.cxx
trackcuts/detector/TrackTpcCut.cxx
trackcuts/detector/TrackExpCut.cxx
trackcuts/detector/TrackTpcHitCut.cxx
trackcuts/detector/TrackTpcToFCut.cxx
trackcuts/detector/TrackDCAPipeCut.cxx
trackcuts/detector/TrackTpcToFThresholdlessCut.cxx
trackcuts/detector/TrackChi2Cut.cxx

trackcuts/kinematics/TrackBetaCut.cxx
trackcuts/kinematics/TrackEtaCut.cxx
trackcuts/kinematics/TrackMassCut.cxx
trackcuts/kinematics/TrackMtCut.cxx
trackcuts/kinematics/TrackPCut.cxx
trackcuts/kinematics/TrackPhiCut.cxx
trackcuts/kinematics/TrackPtCut.cxx
trackcuts/kinematics/TrackYCut.cxx
trackcuts/kinematics/TrackPzCut.cxx
trackcuts/kinematics/TrackThetaCut.cxx
trackcuts/kinematics/TrackChargeCut.cxx
trackcuts/kinematics/TrackBoostedMomentaCuts.cxx
trackcuts/kinematics/TrackKinematicsCutAndMonitor.cxx

trackcuts/monprop/DCAPropertyMonitor.cxx
trackcuts/monprop/TofPropertyMonitor.cxx

trackcuts/mc/TrackBasicMCCut.cxx
trackcuts/mc/TrackFreezoutCut.cxx
trackcuts/mc/TrackMCCut.cxx
trackcuts/mc/TrackPdgCut.cxx
trackcuts/mc/TrackStartPositionCut.cxx
trackcuts/mc/TrackPdgBinCut.cxx

trackcuts/resolution/TrackDeltaMomCut.cxx
trackcuts/resolution/TrackDeltaMomentumCut.cxx

trackcuts/TrackOnlyPrimariesCut.cxx
trackcuts/TrackStatusCut.cxx
trackcuts/TrackComplexCut.cxx
trackcuts/TrackVirtualCut.cxx
trackcuts/TrackFlagCuts.cxx
trackcuts/TrackNullCut.cxx
#PAIR CUT
twotrackcuts/kinematics/TwoTrackEtaCut.cxx
twotrackcuts/kinematics/TwoTrackKtCut.cxx
twotrackcuts/kinematics/TwoTrackMinvCut.cxx
twotrackcuts/kinematics/TwoTrackPhiCut.cxx
twotrackcuts/kinematics/TwoTrackPtOrderCut.cxx
twotrackcuts/kinematics/TwoTrackDphiDetaCut.cxx
twotrackcuts/kinematics/TwoTrackPtSumCut.cxx
twotrackcuts/kinematics/TwoTrackRapidityCut.cxx
twotrackcuts/kinematics/TwoTrackDeltaEtaCut.cxx
twotrackcuts/kinematics/PairDeltaQinvCut.cxx
twotrackcuts/kinematics/TwoTrackLCMSCut.cxx
twotrackcuts/kinematics/TwoTrack3DCFCut.cxx
twotrackcuts/kinematics/PairEtaPtCut.cxx
twotrackcuts/compound/TwoTrackAntiSplittingCut.cxx
twotrackcuts/TwoTrackComplexCut.cxx
twotrackcuts/kinematics/TwoTrackFemtoQinvCut.cxx
twotrackcuts/detector/TwoTrackSailorCowboyCut.cxx

twotrackcuts/detector/TwoTrackRadialDistanceCut.cxx
twotrackcuts/TwoTrackDebugCut.cxx
twotrackcuts/mc/TwoTrackFemtoPRFCut.cxx
twotrackcuts/mc/PdgBinPairCut.cxx
twotrackcuts/TwoTrackVirtualCut.cxx
#CUT MONITORS
cutmonitors/CutMonitor.cxx
cutmonitors/CutMonitorX.cxx
cutmonitors/CutMonitorXY.cxx
cutmonitors/CutMonitorXYZ.cxx
cutmonitors/PropertyMonitorX.cxx
cutmonitors/PropertyMonitorXY.cxx
cutmonitors/PropertyMonitorXYZ.cxx
)

Set(HEADERS )
Set(LINKDEF HalCutLinkDef.h)
Set(LIBRARY_NAME HalCut)
Set(DEPENDENCIES HalData)

GENERATE_LIBRARY_HAL()

