/*
 * CutMonitorComplex.h
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUTMONITORCOMPLEX_H_
#define HAL_CUTS_CUTMONITORS_CUTMONITORCOMPLEX_H_

#include "CutMonitor.h"

namespace Hal {

  class CutMonitorReal : public CutMonitor {
    Hal::CutMonitor* fMonitor = {nullptr};

  public:
    CutMonitorReal() {};
    CutMonitorReal(const CutMonitorReal& other);
    CutMonitorReal(Hal::CutMonitor& mon);
    virtual ~CutMonitorReal();
    virtual Bool_t ObjMonitor() const;
    virtual Bool_t AreSimilar(const Hal::CutMonitor& other) const;
    virtual Hal::Package* Report() const;
    virtual void Update(Bool_t, TObject*);
    virtual Bool_t Init(Int_t);
    virtual Hal::CutMonitor* MakeCopy() const;
  };
  class CutMonitorImag : public CutMonitor {
    Hal::CutMonitor* fMonitor = {nullptr};

  public:
    CutMonitorImag() {};
    CutMonitorImag(const CutMonitorReal& other);
    CutMonitorImag(Hal::CutMonitor& mon);
    virtual ~CutMonitorImag();
    virtual Bool_t ObjMonitor() const;
    virtual Bool_t AreSimilar(const Hal::CutMonitor& other) const;
    virtual Hal::Package* Report() const;
    virtual void Update(Bool_t, TObject*);
    virtual Bool_t Init(Int_t);
    virtual Hal::CutMonitor* MakeCopy() const;
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUTMONITORCOMPLEX_H_ */
