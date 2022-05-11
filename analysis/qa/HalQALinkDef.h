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

#pragma link C++ class QAPlot + ;
#pragma link C++ class QAPlotReport + ;

#pragma link C++ class QAEventTask + ;
#pragma link C++ class QATrackTask + ;
#pragma link C++ class QACoreManager + ;

#pragma link C++ class QAHistoSettings + ;
#pragma link C++ class std::vector < QAHistoSettings> + ;
#endif
