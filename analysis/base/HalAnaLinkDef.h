// $Id: TofLinkDef.h,v 1.3 2006/03/07 11:51:55 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Hal::EventAna + ;
#pragma link C++ class Hal::TrackAna + ;
#pragma link C++ class Hal::MemoryMapManager + ;
#pragma link C++ class Hal::MultiTrackAna + ;
#pragma link C++ class Hal::TwoTrackAna + ;
#pragma link C++ class Hal::EventArray + ;

#pragma link C++ class Hal::BoostTask + ;
#pragma link C++ class Hal::CopyDataTask + ;
#pragma link C++ class Hal::SmearTask + ;

#pragma link C++ class Hal::SmearAlgorithm + ;
#pragma link C++ class Hal::EventSmear + ;
#pragma link C++ class Hal::TrackSmear + ;
#pragma link C++ class Hal::MiniEventTask + ;
#pragma link C++ class Hal::EventSmearVirtual + ;
#pragma link C++ class Hal::TrackSmearVirtual + ;
#pragma link C++ class Hal::TrackSmearMomentumPercent + ;
#pragma link C++ class Hal::SplitedTrackToStatusTask + ;
#pragma link C++ class Hal::SplittingToStatusTask + ;

#pragma link C++ class Hal::EventAnaChain + ;
#pragma link C++ class Hal::TrackAnaChain + ;
#pragma link C++ class Hal::TwoTrackAnaChain + ;
#pragma link C++ class Hal::Reader + ;

#endif
