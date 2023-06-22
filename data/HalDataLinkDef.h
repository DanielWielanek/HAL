// $Id: TofLinkDef.h,v 1.3 2006/03/07 11:51:55 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
/**
 * Data field
 */

#pragma link C++ enum Hal::DataFieldID::Track::EBasic;
#pragma link C++ enum Hal::DataFieldID::Track::EMc;
#pragma link C++ enum Hal::DataFieldID::Track::EExp;
#pragma link C++ enum Hal::DataFieldID::Track::EComplex;
#pragma link C++ enum Hal::DataFieldID::Event::EComplex;
#pragma link C++ enum Hal::DataFieldID::Event::EBasic;
#pragma link C++ enum Hal::DataFieldID::Event::EExp;
#pragma link C++ enum Hal::DataFieldID::Event::EMc;
#pragma link C++ enum Hal::DataFieldID::Event::EV0;
#pragma link C++ enum Hal::BranchInfo::EFlag;

#pragma link C++ class Hal::DataFormatManager + ;
#pragma link C++ class Hal::Event + ;
#pragma link C++ class Hal::McEvent + ;
#pragma link C++ class Hal::Track + ;
#pragma link C++ class Hal::McTrack + ;
#pragma link C++ class Hal::TwoTrack + ;
#pragma link C++ class Hal::SmearedEvent + ;
#pragma link C++ class Hal::SmearedTrack + ;
#pragma link C++ class Hal::DetectorTrack + ;
#pragma link C++ class Hal::DetectorEvent + ;
#pragma link C++ class Hal::ToFTrack + ;
#pragma link C++ class Hal::TpcTrack + ;
#pragma link C++ class Hal::ExpTrack + ;
#pragma link C++ class Hal::ExpTrackHelix + ;
#pragma link C++ class Hal::ExpEvent + ;
#pragma link C++ class Hal::ExpEventHelix + ;
#pragma link C++ class Hal::ComplexEvent + ;
#pragma link C++ class Hal::ComplexTrack + ;
#pragma link C++ class Hal::EventInterface + ;
#pragma link C++ class Hal::EventInterfaceAdvanced + ;
#pragma link C++ class Hal::ComplexEventInterface + ;
#pragma link C++ class Hal::VirtualEventInterface + ;
#pragma link C++ class Hal::TrackInterface + ;
#pragma link C++ class Hal::TrackInterfaceAdvanced + ;
#pragma link C++ class Hal::McTrackInterface + ;
#pragma link C++ class Hal::ExpEventInterface + ;
#pragma link C++ class Hal::VirtualTrack + ;
#pragma link C++ class Hal::VirtualEvent + ;
#pragma link C++ class Hal::TrackClones + ;
#pragma link C++ class Hal::V0Track + ;
#pragma link C++ namespace Hal::DetectorID;
#pragma link C++ namespace Hal::DataFieldID;

#pragma link C++ class Hal::Task + ;
#pragma link C++ class Hal::TriggerTask + ;
#pragma link C++ class Hal::Source + ;
#pragma link C++ class Hal::RootIOManager + ;
#pragma link C++ class Hal::MagField + ;
#pragma link C++ class Hal::DataManager + ;
#pragma link C++ class Hal::AnalysisManager + ;
#pragma link C++ class Hal::IOManager + ;
#pragma link C++ class Hal::RootSource + ;
#pragma link C++ class Hal::Reader + ;
#pragma link C++ class Hal::BranchInfo + ;
#pragma link C++ class std::vector < Hal::BranchInfo> + ;
#pragma link C++ class std::vector < Hal::TriggerTask> + ;
#endif
