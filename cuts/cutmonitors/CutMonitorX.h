/*
 * CutMonitorX.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef HALCUTMONITORX_H_
#define HALCUTMONITORX_H_

#include "CutMonitor.h"
/**
 * class for monitoring one cut/one paramter
 */

namespace Hal {
  class CutMonitorX : public CutMonitor {
  protected:
    void TrueUpdate(Bool_t);
    virtual void CreateHistograms();

  public:
    CutMonitorX();

    /**
     *
     * @param cut_name name of monitored cut
     * @param parameter_no number of monitored parameter
     */
    CutMonitorX(TString cut_name, Int_t parameter_no = 0);
    /**
     * copy ct-or
     * @param other object that should be copied
     */
    CutMonitorX(const CutMonitorX& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    CutMonitorX& operator=(const CutMonitorX& other);
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual CutMonitor* MakeCopy() const;
    virtual ~CutMonitorX();
    ClassDef(CutMonitorX, 1)
  };
}  // namespace Hal
#endif /* HALCUTMONITORX_H_ */
