/*
 * Femto3DCF.h
 *
 *  Created on: 12-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTO3DCF_H_
#define HALFEMTO3DCF_H_

#include "FemtoConst.h"

#include "Array.h"
#include "DividedHisto.h"

#include <TString.h>

namespace Hal {
  class CorrFit3DCF;
  class FemtoCFPainter;
  /**
   * basic class for storing 3-dim correlation functions
   */
  class Femto3DCF : public DividedHisto3D {
    friend class FemtoSerializationInterface3D;
    FemtoCFPainter* fPainter = {nullptr};  //!
    /**
     * frame of pairs in CF
     */
    const Femto::EKinematics fFrame;
    /**
     * set assumed names of axes
     * @param h - adress of numerator or denominator
     */
    virtual void SetAxisNames(TH1* h);

  public:
    /**
     * constructor for streamer
     */
    Femto3DCF() : DividedHisto3D(), fFrame(Femto::EKinematics::kLCMS) {};
    /**
     * default constructor
     * @param name name of CF's
     * @param frame frame of pairs
     */
    Femto3DCF(TString name, Femto::EKinematics frame = Femto::EKinematics::kLCMS);
    Femto3DCF(TString name,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Double_t minY,
              Double_t maxY,
              Int_t binsZ,
              Double_t minZ,
              Double_t maxZ,
              Femto::EKinematics frame = Femto::EKinematics::kLCMS);
    Femto3DCF(TString name, Int_t binsX, Double_t minX, Double_t maxX, Femto::EKinematics frame = Femto::EKinematics::kLCMS);
    Femto3DCF(const Femto3DCF& other);
    /**
     *
     * @return pair kinematics
     */
    Femto::EKinematics GetFrame() const { return fFrame; };
    virtual void FillNumObj(TObject* ob);
    virtual void FillDenObj(TObject* obj);
    virtual void AddNum(TH1* h, Option_t* opt = "");
    virtual void AddDen(TH1* h, Option_t* opt = "");
    /**
     * fit this function
     */
    void Fit(CorrFit3DCF* fit);
    /**
     * make "dummy fit" (allow to draw function )
     * does not perform any fit
     * @param fit
     */
    void FitDummy(CorrFit3DCF* fit);
    virtual void Browse(TBrowser* b);
    /**
     * see @see #Hal::Femto3DCFPainter#SetOptionInternal @see #Hal::FemtoCFPainter#SetOptionInternal
     * @param option
     */
    virtual void Draw(Option_t* opt = "cf+rgb+norm");
    /**
     * draw scaled projection of CF
     * @param scale scale (norm)
     * @param opt option like for TH1
     */
    virtual void DrawScaled(Double_t scale, Option_t* opt);
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual TH1D** GetDiagProj(Option_t* opt = "diag1", Bool_t normalized = kTRUE) const;
    virtual void Print(Option_t* opt = "") const;
    virtual FemtoCFPainter* GetPainter() const { return fPainter; }
    virtual TObject* GetSpecial(TString opt) const;
    virtual ~Femto3DCF();
    ClassDef(Femto3DCF, 4)
  };
}  // namespace Hal
#endif /* HALFEMTO3DCF_H_ */
