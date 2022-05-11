/*
 * Femto3DCFQinv.h
 *
 *  Created on: 19 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO3DCFQINV_H_
#define HALFEMTO3DCFQINV_H_

#include "Femto3DCF.h"
/**
 * 3-d function with addritional map for qinv values
 */
namespace Hal {
  class Femto3DCFQinv : public Femto3DCF {
    TH3D* fPureNum;
    TH3D* fQinvNum;

  public:
    Femto3DCFQinv();
    Femto3DCFQinv(TString name, Femto::EKinematics frame = Femto::EKinematics::kLCMS);
    Femto3DCFQinv(TString name,
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
    Femto3DCFQinv(const Femto3DCFQinv& other);
    Femto3DCFQinv& operator=(const Femto3DCFQinv& other);
    virtual void FillNumObj(TObject* obj);
    /**
     * @return map filled with weight*q_inv if divided by numerator - equal to
     * average qinv
     */
    TH3D* GetQinvHist(TString opt = "av") const;
    virtual void AddNum(TH1* num, Option_t* opt = "");
    virtual void AddDen(TH1* den, Option_t* opt = "");
    virtual Long64_t Merge(TCollection* collection);
    virtual void Add(const Object* h);
    virtual void Browse(TBrowser* b);
    virtual ~Femto3DCFQinv();
    ClassDef(Femto3DCFQinv, 2)
  };
}  // namespace Hal
#endif /* HALFEMTO3DCFQINV_H_ */
