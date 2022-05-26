/*
 * SpectraLinkDef.h
 *
 *  Created on: 21-07-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Hal::QAPlot + ;
#pragma link C++ class Hal::QAPlotReport + ;

#pragma link C++ class Hal::QAEventTask + ;
#pragma link C++ class Hal::QATrackTask + ;
#pragma link C++ class Hal::QAManager + ;

#pragma link C++ class Hal::QAHistoSettings + ;
#pragma link C++ class std::vector < Hal::QAHistoSettings> + ;
#endif
