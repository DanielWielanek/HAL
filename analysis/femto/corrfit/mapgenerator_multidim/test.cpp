/*
 * draw.C
 *
 *  Created on: 16 mar 2023
 *      Author: Daniel Wielanek
 *      E-mail: daniel.wielanek@gmail.com
 *      Warsaw University of Technology, Faculty of Physics
 */



#include "CorrFit.h"
#include "CorrFitDrawOptions.h"
#include "FemtoSHCF.h"
#include "CorrFitInfo.h"
#include "CorrFitSHCFMultiDim.h"

#include <Rtypes.h>
#include <RtypesCore.h>


#ifndef __CLING__

#endif
#include <TFile.h>
//#include "sh_pain.C"


int main() {
  auto file = new TFile("../merged_map3.root");
  auto info = (Hal::CorrFitInfo*)file->Get("Info");
  auto cf = (Hal::FemtoSHCF*)info->GetCf();
  //cf->RecalculateCF(1);
  //cf->RecalculateCF(1);
  //cf->RecalculateCF(-1);
  cf->Draw();

   /* sh_paint* paint = new sh_paint(kGreen, kOrange, 3);
    Double_t scale  = 1.0;
    paint->SetRadius(2. * scale, 3. * scale, 4. * scale);
    paint->SetLambda(1);
    paint->Repaint("SAME");*/
    auto fitter = new Hal::CorrFitSHCFMultiDim();
    fitter->SetFunctorFromMap("../merged_map3.root");
    fitter->SetLineColor(kGreen);
    cf->FitDummy(fitter);
    Hal::CorrFitDrawOptions opts({Hal::CorrFitDrawOptions::eCommon::kSame});
    fitter->SetDrawOption(opts);
    fitter->Draw();
    //fitter->Paint();
 //   Hal::CorrFitGUI *gui = new Hal::CorrFitGUI(fitter);

return 0;
}
