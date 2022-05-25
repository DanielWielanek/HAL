// $Id: TofLinkDef.h,v 1.3 2006/03/07 11:51:55 friese Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
/**
 * Data field
 */

#pragma link C++ namespace Hal::Fair;
#pragma link C++ class Hal::Fair::Field + ;
#pragma link C++ class Hal::Fair::RootManager + ;
#pragma link C++ class Hal::Fair::QACoreManager + ;
#pragma link C++ class Hal::Fair::TaskManager + ;
#endif
