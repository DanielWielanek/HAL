#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all typedefs;

#ifdef __MAKECINT__
#pragma link C++ class Hal::Cut + ;
#pragma link C++ class Hal::SubCut + ;
#pragma link C++ class Hal::SubCutHisto + ;
#pragma link C++ class Hal::SubCutRectangle + ;
#pragma link C++ class Hal::CutContainer + ;
#pragma link C++ enum Hal::CutContainer::ELinkPolicy;
#pragma link C++ class Hal::CutCollection + ;
#pragma link C++ class Hal::EventCut + ;
#pragma link C++ class Hal::TrackCut + ;
#pragma link C++ class Hal::TwoTrackCut + ;
#pragma link C++ class Hal::TrackPhiCut + ;
#pragma link C++ class Hal::TrackTauCut + ;
#pragma link C++ class Hal::CutsAndMonitors + ;
#pragma link C++ class Hal::CutMonitorRequest + ;
#pragma link C++ class std::vector < Hal::CutMonitorRequest> + ;
#pragma link C++ class Hal::CutMonAxisConf + ;

// Cut monitors
#pragma link C++ class Hal::CutMonitor + ;
#pragma link C++ class Hal::CutMonitorX + ;
#pragma link C++ class Hal::CutMonitorXY + ;
#pragma link C++ class Hal::CutMonitorXYZ + ;
#pragma link C++ class Hal::PropertyMonitorX + ;
#pragma link C++ class Hal::PropertyMonitorXY + ;
#pragma link C++ class Hal::PropertyMonitorXYZ + ;
#pragma link C++ class Hal::EventFieldMonitorX + ;
#pragma link C++ class Hal::EventFieldMonitorXY + ;
#pragma link C++ class Hal::EventFieldMonitorXYZ + ;
#pragma link C++ class Hal::TrackFieldMonitorX + ;
#pragma link C++ class Hal::TrackFieldMonitorXY + ;
#pragma link C++ class Hal::TrackFieldMonitorXYZ + ;
// EventCut
#pragma link C++ class Hal::EventMultiplicityCut + ;
#pragma link C++ class Hal::EventMultiChargedCut + ;
#pragma link C++ class Hal::EventModuloCut + ;
#pragma link C++ class Hal::EventPtMaxCut + ;
#pragma link C++ class Hal::EventImpactParameterCut + ;
#pragma link C++ class Hal::EventPhiCut + ;
#pragma link C++ class Hal::EventImpactCentralityCut + ;
#pragma link C++ class Hal::EventComplexCut + ;
#pragma link C++ class Hal::EventRealCut + ;
#pragma link C++ class Hal::EventImaginaryCut + ;
#pragma link C++ class Hal::EventVertexCut + ;
#pragma link C++ class Hal::EventVertexZCut + ;
#pragma link C++ class Hal::EventVertexXYZCut + ;
#pragma link C++ class Hal::EventBinningCut + ;
#pragma link C++ class Hal::EventPrimaryTracksNoCut + ;
#pragma link C++ class Hal::EventExpCut + ;
#pragma link C++ class Hal::EventMcCut + ;
#pragma link C++ class Hal::EventRunCut + ;

// Track Cut
#pragma link C++ class Hal::TrackBasicToFCut + ;
#pragma link C++ class Hal::TrackDCACut + ;
#pragma link C++ class Hal::TrackExpCut + ;
#pragma link C++ class Hal::TrackTpcCut + ;
#pragma link C++ class Hal::TrackTpcToFThresholdlessCut + ;
#pragma link C++ class Hal::TrackToFMass2Cut + ;
#pragma link C++ class Hal::TrackTofMass2CutNoZeros + ;

#pragma link C++ class Hal::TrackYAssumedCut + ;
#pragma link C++ class Hal::TrackBetaCut + ;
#pragma link C++ class Hal::TrackEtaCut + ;
#pragma link C++ class Hal::TrackEtaAbsCut + ;
#pragma link C++ class Hal::TrackMassCut + ;
#pragma link C++ class Hal::TrackMtCut + ;
#pragma link C++ class Hal::TrackPCut + ;
#pragma link C++ class Hal::TrackPSignedCut + ;
#pragma link C++ class Hal::TrackPhiCut + ;
#pragma link C++ class Hal::TrackPtCut + ;
#pragma link C++ class Hal::TrackYCut + ;
#pragma link C++ class Hal::TrackPzCut + ;
#pragma link C++ class Hal::TrackThetaCut + ;
#pragma link C++ class Hal::TrackChargeCut + ;
#pragma link C++ class Hal::TrackChi2Cut + ;
#pragma link C++ class Hal::TrackBoostedPCut + ;
#pragma link C++ class Hal::TrackBoostedDeltaPCut + ;
#pragma link C++ class Hal::TrackBoostedKinematcisCut + ;
#pragma link C++ class Hal::TrackKinematicsCutAndMonitor + ;

#pragma link C++ class Hal::TrackBasicMCCut + ;
#pragma link C++ class Hal::TrackTFreezCut + ;
#pragma link C++ class Hal::TrackFreezoutCut + ;
#pragma link C++ class Hal::TrackPdgCut + ;
#pragma link C++ class Hal::TrackPdgAbsCut + ;
#pragma link C++ class Hal::TrackStartPositionCut + ;
#pragma link C++ class Hal::TrackMCCut + ;
#pragma link C++ class Hal::TrackPdgBinCut + ;
#pragma link C++ class Hal::TrackTpcHitCut + ;
#pragma link C++ class Hal::TrackTpcToFCut + ;
#pragma link C++ class Hal::TrackDCAPipeCut + ;
#pragma link C++ class Hal::TrackNullCut + ;

#pragma link C++ class Hal::TrackDeltaMomentumCut + ;
#pragma link C++ class Hal::TrackDeltaMomCut + ;
#pragma link C++ class Hal::TrackDeltaVectMomCut + ;
#pragma link C++ class Hal::TrackDeltaPtCut + ;
#pragma link C++ class Hal::TrackDeltaPCut + ;
#pragma link C++ class Hal::TrackDeltaPzCut + ;
#pragma link C++ class Hal::TrackDeltaPhiCut + ;
#pragma link C++ class Hal::TrackDeltaThetaCut + ;
#pragma link C++ class Hal::TrackDeltaVectPtCut + ;
#pragma link C++ class Hal::TrackDeltaVectPCut + ;
#pragma link C++ class Hal::TrackDeltaVectPhiCut + ;
#pragma link C++ class Hal::TrackDeltaVectThetaCut + ;
#pragma link C++ class Hal::TrackBoostedKinematcisCut + ;
#pragma link C++ class Hal::TwoTrack3DCFCut + ;
#pragma link C++ class Hal::PairEtaPtCut + ;

#pragma link C++ class Hal::TrackComplexCut + ;
#pragma link C++ class Hal::TrackRealCut + ;
#pragma link C++ class Hal::TrackImaginaryCut + ;
#pragma link C++ class Hal::TrackStatusCut + ;
#pragma link C++ class Hal::TrackOnlyPrimariesCut + ;

#pragma link C++ class Hal::TrackPrimaryFlagCut + ;
#pragma link C++ class Hal::TrackV0FlagCut + ;
#pragma link C++ class Hal::TrackGoodSecondaryFlagCut + ;
#pragma link C++ class Hal::TrackGlobalFlagCut + ;

#pragma link C++ class Hal::TofPropertyMonitor + ;
#pragma link C++ class Hal::DCAPropertyMonitor + ;


// TwoTrack Cut
#pragma link C++ class Hal::TwoTrackEtaCut + ;
#pragma link C++ class Hal::TwoTrackKtCut + ;
#pragma link C++ class Hal::TwoTrackMinvCut + ;
#pragma link C++ class Hal::TwoTrackPtSumCut + ;
#pragma link C++ class Hal::TwoTrackPhiCut + ;
#pragma link C++ class Hal::TwoTrackDeltaEtaCut + ;
#pragma link C++ class Hal::TwoTrackPtOrderCut + ;
#pragma link C++ class Hal::TwoTrackRapidityCut + ;
#pragma link C++ class Hal::TwoTrackLCMSCut + ;

#pragma link C++ class Hal::TwoTrackDphistarDetaCut + ;
#pragma link C++ class Hal::TwoTrackDphiDetaCut + ;
#pragma link C++ class Hal::TwoTrackRadialDistanceCut + ;
#pragma link C++ class Hal::TwoTrackDebugCut + ;
#pragma link C++ class Hal::TwoTrackFemtoPRFCut + ;
#pragma link C++ class Hal::TwoTrackFemtoQinvCut + ;
#pragma link C++ class Hal::TwoTrackSailorCowboyCut + ;
#pragma link C++ class Hal::PairDeltaQinvCut + ;
#pragma link C++ class Hal::PdgBinPairCut + ;
// Virtual cut
#pragma link C++ class Hal::EventVirtualCut + ;
#pragma link C++ class Hal::TrackVirtualCut + ;
#pragma link C++ class Hal::TwoTrackVirtualCut + ;
#pragma link C++ class Hal::TwoTrackComplexCut + ;
#pragma link C++ class Hal::TwoTrackRealCut + ;
#pragma link C++ class Hal::TwoTrackImaginaryCut + ;
#pragma link C++ class Hal::TwoTrackAntiSplittingCut + ;
/*
#pragma link C++ class Hal::EventClonedCut+;
#pragma link C++ class Hal::TrackClonedCut+;
#pragma link C++ class Hal::TwoTrackClonedCut+;
*/
#endif
#endif
