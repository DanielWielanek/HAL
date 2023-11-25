// $Id: TofLinkDef.h,v 1.3 2006/03/07 11:51:55 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
/**
 * Data field
 */

#pragma link C++ namespace HalDbg;
#pragma link C++ namespace Dbg;
#pragma link C++ class HalDbg::ComplexEvent + ;
#pragma link C++ class Dbg::RecoTrack + ;
#pragma link C++ class Dbg::McTrack + ;
#pragma link C++ class Dbg::RecoEvent + ;
#pragma link C++ class Dbg::McEvent + ;
#pragma link C++ class HalDbg::IOManager + ;
#pragma link C++ class HalDbg::McEvent + ;
#pragma link C++ class HalDbg::McTrack + ;
#pragma link C++ class HalDbg::McEventInterface + ;
#pragma link C++ class HalDbg::Reader + ;
#pragma link C++ class HalDbg::RecoEvent + ;
#pragma link C++ class HalDbg::RecoTrack + ;
#pragma link C++ class HalDbg::RecoEventInterface + ;
#pragma link C++ class HalDbg::Source + ;
#endif
