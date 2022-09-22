/*
 * CutMonitorXYZ.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef HALCUTMONITORXYZ_H_
#define HALCUTMONITORXYZ_H_

#include "CutMonitor.h"
/**
 * class for monitoring 3 cuts/paramters
 */

namespace Hal {
  class CutMonitorXYZ : public CutMonitor {
#ifdef MPPCUTFULL
    Int_t fWeightFlag;
#endif
  protected:
    void TrueUpdate(Bool_t pass);
    void CreateHistograms();

  public:
    CutMonitorXYZ();
    /**
     *
     * @param cut_A name of cut monitored on X axis
     * @param cut_B name of cut monitored on Y axis
     * @param cut_C name of cut monitored on Z axis
     */
    CutMonitorXYZ(TString cut_A, TString cut_B, TString cut_C);
    /**
     *
     * @param cut_A name of cut monitored on X axis
     * @param par_A number of parameter monitored on X axis
     * @param xAxis x-axis configuration {bins,min,max}
     * @param cut_B name of cut monitored on B axis
     * @param par_B number of parameter monitored on Y axis
     * @param yAxis y-axis configuration {bins,min,max}
     * @param cut_C name of cut monitored on C axis
     * @param par_C number of parameter monitored on Z axis
     * @param zAxis z-axis configuration {bins,min,max}
     */
    CutMonitorXYZ(TString cut_A,
                  Int_t par_A,
                  std::initializer_list<Double_t> xAxis,
                  TString cut_B,
                  Int_t par_B,
                  std::initializer_list<Double_t> yAxis,
                  TString cut_C,
                  Int_t par_C,
                  std::initializer_list<Double_t> zAxis);
    /**
     *
     * @param cut_A name of cut monitored on X axis
     * @param par_A number of parameter monitored on X axis
     * @param cut_B name of cut monitored on B axis
     * @param par_B number of parameter monitored on Y axis
     * @param cut_C name of cut monitored on C axis
     * @param par_C number of parameter monitored on Z axis
     */
    CutMonitorXYZ(TString cut_A, Int_t par_A, TString cut_B, Int_t par_B, TString cut_C, Int_t par_C);
    /**
     * copy c-tor
     * @param other object to copy
     */
    CutMonitorXYZ(const CutMonitorXYZ& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    CutMonitorXYZ& operator=(const CutMonitorXYZ& other);
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t pass, TObject* obj);
    virtual CutMonitor* MakeCopy() const;
    virtual ~CutMonitorXYZ();
    ClassDef(CutMonitorXYZ, 1)
  };
}  // namespace Hal
#endif /* HALCUTMONITORXYZ_H_ */
