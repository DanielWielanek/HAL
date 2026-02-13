// $Id: TofLinkDef.h,v 1.3 2006/03/07 11:51:55 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
/**
 * Data field
 */

#pragma link C++ namespace HalOTF;
#pragma link C++ namespace OTF;
#pragma link C++ class HalOTF::ComplexEvent + ;
#pragma link C++ class OTF::RecoTrack + ;
#pragma link C++ class OTF::McTrack + ;
#pragma link C++ class OTF::RecoEvent + ;
#pragma link C++ class OTF::McEvent + ;
#pragma link C++ class HalOTF::IOManager + ;
#pragma link C++ class HalOTF::McEvent + ;
#pragma link C++ class HalOTF::McTrack + ;
#pragma link C++ class HalOTF::McEventInterface + ;
#pragma link C++ class HalOTF::Reader + ;
#pragma link C++ class HalOTF::EventGenerator + ;
#pragma link C++ class HalOTF::EventGeneratorDecay + ;
#pragma link C++ class HalOTF::RecoEvent + ;
#pragma link C++ class HalOTF::RecoTrack + ;
#pragma link C++ class HalOTF::RecoEventInterface + ;
#pragma link C++ class HalOTF::Source + ;
#pragma link C++ class HalOTF::EventGeneratorDecayer + ;

#pragma link C++ class HalOTF::DetectorHits + ;
#pragma link C++ class HalOTF::DetectorHitsA + ;
#pragma link C++ class HalOTF::DetectorHitsB + ;
#pragma link C++ class HalOTF::DetectorHitsC + ;
#pragma link C++ class HalOTF::DetectorHitsABCSum + ;
#pragma link C++ class HalOTF::EfficientyKin + ;
#pragma link C++ class HalOTF::EfficiencyYPt + ;
#pragma link C++ class HalOTF::EfficiencyEtaPt + ;
#pragma link C++ class HalOTF::Experiment + ;
#pragma link C++ class HalOTF::ExperimentSubTask + ;
#pragma link C++ class HalOTF::MisidentificationKin + ;
#pragma link C++ class HalOTF::MisidentificationYPt + ;
#pragma link C++ class HalOTF::MisidentificationEtaPt + ;
#pragma link C++ class HalOTF::MomentumSmearing + ;
#endif
