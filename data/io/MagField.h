/*
 * Field.h
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_STEER_HALFIELD_H_
#define HAL_ANALYSIS_BASE_STEER_HALFIELD_H_

#include <TObject.h>
#include <TVector3.h>

/**
 * represents magnetic field
 */
namespace Hal {
  class MagField : public TObject {
  public:
    MagField();
    /**
     * return magnetic field at x,y,z
     * @param x
     * @param y
     * @param z
     * @return
     */
    virtual TVector3 GetField(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) const { return TVector3(0, 0, 0); }
    /**
     * alternative method to get field
     */
    void GetFieldValue(const Double_t point[3], Double_t* bFiel) const;
    virtual ~MagField();
    ClassDef(MagField, 1)
  };

  class ConstMagField : public MagField {
    Float_t fBx = {0};
    Float_t fBy = {0};
    Float_t fBz = {0};

  public:
    ConstMagField() {};
    void SetField(Float_t x, Float_t y, Float_t z);
    ConstMagField(Float_t x, Float_t y, Float_t z) : fBx(x), fBy(y), fBz(z) {};
    virtual TVector3 GetField(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) const { return TVector3(fBx, fBy, fBz); }
    /**
     * alternative method to get field
     */
    void GetFieldValue(const Double_t point[3], Double_t* bFiel) const;
    virtual ~ConstMagField() {};
    ClassDef(ConstMagField, 1)
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_BASE_STEER_HALFIELD_H_ */
