/*
 * CorrFitMap1D.h
 *
 *  Created on: 18 paź 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITMAPKSTARRSTAR_H_
#define HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITMAPKSTARRSTAR_H_

#include "Splines.h"

#include <TH2D.h>
#include <TNamed.h>
#include <TObject.h>

#include "FemtoConst.h"
#include "Object.h"
namespace Hal {
  class DividedHisto2D;

  class CorrFitMapKstarRstar : public Object {
  protected:
    TH2D* fHisto2d;
    Spline2D* fSplined;  //!
    Double_t fFrameScale;
    Femto::EKinematics fFrame;

  public:
    CorrFitMapKstarRstar(TString name = "CF_map");
    CorrFitMapKstarRstar(TString name,
                         Int_t binsKstar,
                         Double_t minKstar,
                         Double_t maxKstar,
                         Int_t binsR,
                         Double_t minR,
                         Double_t maxR,
                         Femto::EKinematics kin = Femto::EKinematics::kPRF);
    CorrFitMapKstarRstar(const TH2D& h, Femto::EKinematics kin = Femto::EKinematics::kPRF);
    CorrFitMapKstarRstar(const CorrFitMapKstarRstar& other);
    virtual void Recalc(TString extrOpt = "");
    virtual void Add(const Object* pack);
    void SetFrameScale(Femto::EKinematics kin);
    Int_t GetNRbins() const { return fHisto2d->GetYaxis()->GetNbins(); };
    Int_t GetNKstrbins() const { return fHisto2d->GetXaxis()->GetNbins(); };
    Double_t GetRmin() const { return fHisto2d->GetYaxis()->GetBinLowEdge(1); };
    Double_t GetRmax() const { return fHisto2d->GetYaxis()->GetBinUpEdge(GetNRbins()); };
    Double_t GetKstarMin() const { return fHisto2d->GetXaxis()->GetBinLowEdge(1); };
    Double_t GetKstarMax() const { return fHisto2d->GetXaxis()->GetBinUpEdge(GetNKstrbins()); };
    Double_t Eval(Double_t q, Double_t R) const;
    Double_t EvalNumError(Double_t q, Double_t R) const;
    Double_t EvalNumErrorBin(Int_t x, Int_t y) const { return fSplined->ErrorBin(x, y) * 1.5; }
    Femto::EKinematics GetFrame() const { return fFrame; };
    TH2D* GetHisto() const { return fHisto2d; }
    CorrFitMapKstarRstar& operator=(const CorrFitMapKstarRstar& other);
    virtual ~CorrFitMapKstarRstar();
    ClassDef(CorrFitMapKstarRstar, 1)
  };

  class CorrFitMapKstarRstarDiv : public CorrFitMapKstarRstar {
    DividedHisto2D* fHistoDiv2d;

  public:
    CorrFitMapKstarRstarDiv(TString name = "CF_map");
    CorrFitMapKstarRstarDiv(const CorrFitMapKstarRstarDiv& other);
    CorrFitMapKstarRstarDiv(const DividedHisto2D& other, Femto::EKinematics kin);
    CorrFitMapKstarRstarDiv& operator=(const CorrFitMapKstarRstarDiv& other);
    virtual void Recalc(TString extrOpt = "");
    virtual void Add(const Object* pack);
    virtual ~CorrFitMapKstarRstarDiv();
    ClassDef(CorrFitMapKstarRstarDiv, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITMAPKSTARRSTAR_H_ */
