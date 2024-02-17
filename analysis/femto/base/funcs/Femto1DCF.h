/*
 * Femto1DCF.h
 *
 *  Created on: 12-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *		Experimental class
 */

#ifndef HALFEMTO1DCF_H_
#define HALFEMTO1DCF_H_

#include "FemtoConst.h"

#include "Array.h"
#include "DividedHisto.h"

#include <TString.h>

namespace Hal {
  class CorrFit1DCF;
  /**
   * basic class for storing 1-dim correlation functions
   */
  class Femto1DCF : public DividedHisto1D {
    const Femto::EKinematics fFrame;
    friend class FemtoSerializationInterface1D;

  protected:
    virtual TString GetPic() const;
    /**
     * set assumed names of axes
     * @param h - adress of numerator or denominator
     */
    virtual void SetAxisNames(TH1* h);

  public:
    Femto1DCF();
    /**
     * default ctor
     * @param name name of function
     * @param frame frame
     */
    Femto1DCF(TString name, Femto::EKinematics frame = Femto::EKinematics::kLCMS);
    /**
     * another constructor
     * @param name
     * @param bins
     * @param min
     * @param max
     * @param frame
     */
    Femto1DCF(TString name, Int_t bins, Double_t min, Double_t max, Femto::EKinematics frame = Femto::EKinematics::kLCMS);
    /**
     * copy constructor
     * @param other
     */
    Femto1DCF(const Femto1DCF& other);
    /**
     *
     * @return kinematics mode
     */
    Femto::EKinematics GetFrame() const { return fFrame; };
    virtual void AddNum(TH1* h, Option_t* opt = "");
    virtual void AddDen(TH1* h, Option_t* opt = "");
    void FillNumObj(TObject* obj);
    void FillDenObj(TObject* obj);
    /**
     * fit this function
     */
    void Fit(CorrFit1DCF* fit);
    /**
     * make "dummy fit" (allow to draw function )
     * does not perform any fit
     * @param fit
     */
    void FitDummy(CorrFit1DCF* fit);
    virtual void Browse(TBrowser* b);
    /**
     * draw scaled C-f
     * @param scale scale (norm)
     * @param opt option like for TH1
     */
    virtual void DrawScaled(Double_t scale, Option_t* opt);
    /**
     * extract this CF into web page
     * @param counter number of this object
     * @param dir directory where extract object
     * @return url to extracted object
     */
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual void Print(Option_t* opt = "") const;
    virtual TObject* GetSpecial(TString opt) const;
    virtual ~Femto1DCF();
    ClassDef(Femto1DCF, 3)
  };
}  // namespace Hal
#endif /* HALFEMTO1DCF_H_ */
