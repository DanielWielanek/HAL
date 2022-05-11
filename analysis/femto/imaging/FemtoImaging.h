/*
 * FemtoImaging.h
 *
 *  Created on: 20 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOIMAGING_H_
#define HALFEMTOIMAGING_H_
#include <TObject.h>
/**
 * abstract class for imaging CF's
 */
namespace Hal {
  class FemtoImaging : public TObject {
  protected:
    /**
     * flag for simmetrization 1 for bosons with spin 0, -1 for fermions
     */
    Int_t fSymmetrization;

  public:
    FemtoImaging();
    void SetFermions() { fSymmetrization = -1; };
    virtual ~FemtoImaging();
    ClassDef(FemtoImaging, 1)
  };
}  // namespace Hal

#endif /* HALROOT_ANALYSIS_FEMTO_IMAGING_HALFEMTOIMAGING_H_ */
