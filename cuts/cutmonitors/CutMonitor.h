/*
 * CutMonitor.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 */

#ifndef HALCUTMONITOR_H_
#define HALCUTMONITOR_H_

#include <TObject.h>
#include <TString.h>

#include "Std.h"

namespace Hal {
  class Cut;
  class Package;

  /**
   * abstract class for all cut monitors
   */
  class CutContainer;
  class CutMonitor : public TObject {
    friend class CutContainer;
    friend class CutOptions;

  protected:
    enum EFlagBit { kInit = 0, kExclusive = 1, kRe = 2, kIm = 3 };
    /**
     * number of axis in cut monitor
     */
    const Int_t fAxisNo;
    /**
     * number of currently added cuts
     */
    Int_t fCuts = {0};
    /**
     * collection number of this cut monitor
     */
    Int_t fCollectionID = {-1};
    /**
     * array with numbers of axis bins
     */
    Int_t* fAxisBins = {nullptr};  //[fAxisNo]
    /**
     * array with parameters numbers used from cuts for all axes
     */
    Int_t* fOptionAxis = {nullptr};  //[fAxisNo]
    /**
     * histogram with passed objects
     */
    TH1* fHistoPassed = {nullptr};
    /**
     * histogram with failed objects
     */
    TH1* fHistoFailed = {nullptr};
    /**
     * array with lower edges of axes
     */
    Double_t* fAxisMin = {nullptr};  //[fAxisNo]
    /**
     * array with upper edges of axes
     */
    Double_t* fAxisMax = {nullptr};  //[fAxisNo]
    /**
     *  flag for holding informations
     */
    Int_t fFlags = {0};
    /**
     * array with pointers to monitored cuts
     */
    Cut** fCut = {nullptr};  //[fAxisNo]
    /**
     * array with pointers to names of monitored cuts
     */
    TString* fCutNames = {nullptr};  //[fAxisNo]
    /**
     * update ratio of this cut monitor
     */
    ECutUpdate fUpdateRatio = {ECutUpdate::kNo};
    /**
     * allocate histograms
     */
    virtual void CreateHistograms();
    /**
     * make update of histogram
     * @param passed true if object goes to pass false otherwise
     */
    virtual void TrueUpdate(Bool_t passed);
    /**
     * confgiure axis of histogram
     * @param bins number of bins
     * @param min lower edge of axis
     * @param max upper edge of axis
     * @param opt axis numger
     */
    void SetAxis(Int_t bins, Double_t min, Double_t max, Int_t opt);
    /**
     * set axis from initializer list
     * @param axis {nbins, min, max}
     * @param opt
     */
    void SetAxisList(std::initializer_list<Double_t> axis, Char_t opt);
    /**
     *
     * @param i cut number
     * @return cut monitred by this cut monitor
     */
    Cut* GetCut(Int_t i) const;
    /**
     * fill histogram by hand
     * @param x
     * @param passed
     */
    void ManualFill1D(Double_t x, Bool_t passed);
    /**
     * fill histogram by hand
     * @param x
     * @param passed
     */
    void ManualFill2D(Double_t x, Double_t y, Bool_t passed);
    /**
     * fill histogram by hand
     * @param x
     * @param passed
     */
    void ManualFill3D(Double_t x, Double_t y, Double_t z, Bool_t passed);
    /**
     *
     * @return true if initialized
     */
    Bool_t IsInitialized() const { return TESTBIT(fFlags, EFlagBit::kInit); }
    /**
     * marks as initialized
     */
    void MarkAsInitialized() { SETBIT(fFlags, EFlagBit::kInit); }
    /**
     *
     * @return true if exclusive
     */
    inline Bool_t IsExclusive() const { return TESTBIT(fFlags, EFlagBit::kExclusive); }
    /**
     *
     * @return true if marked as real
     */
    Bool_t IsRe() const { return TESTBIT(fFlags, EFlagBit::kRe); }
    /**
     *
     * @return true if marked as imaginary
     */
    Bool_t IsIm() const { return TESTBIT(fFlags, EFlagBit::kIm); }

  public:
    /**
     * default constructor
     * @param size number of axes
     */
    CutMonitor(Int_t size = 0);
    /**
     * copy constructor
     * @param other object to copy
     */
    CutMonitor(const CutMonitor& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    CutMonitor& operator=(const CutMonitor& other);
    /**
     * reset this cut monitor
     */
    void Reset();
    /**
     * add cut to this monitor
     * @param cut cut name
     * @param parameter_no number of paramter that should be monitored from given
     * cut
     */
    void AddCut(TString cut, Int_t parameter_no = 0);
    /**
     * setup X axis
     * @param bins number of bins
     * @param min lower limit of axis
     * @param max upper limit of axis
     */
    void SetXaxis(Int_t bins, Double_t min, Double_t max);
    /**
     * setup Y axis
     * @param bins number of bins
     * @param min lower limit of axis
     * @param max upper limit of axis
     */
    void SetYaxis(Int_t bins, Double_t min, Double_t max);
    /**
     * setup Z axis
     * @param bins number of bins
     * @param min lower limit of axis
     * @param max upper limit of axis
     */
    void SetZaxis(Int_t bins, Double_t min, Double_t max);
    /**
     * add pointer to moniter cut, this option should be used inside of cut
     * monitors/applications not in ROOT macros
     * @param cut pointer to monitored cut
     * @param no axis number
     */
    void AddForcedCut(Cut* cut, Int_t no);
    /**
     * copy setting of axis from different cut
     * @param original cut with is used for taking values
     */
    void CopySettings(const CutMonitor* original);
    /**
     * set collection number for this cut monitor
     * @param i collection number
     */
    void SetCollectionID(Int_t i);
    /**
     * enable exclusive updates mode
     */
    void EnableExclusiveUpdate();
    /**
     *
     * @return this collection no
     */
    Int_t GetCollectionID() const { return fCollectionID; };
    /**
     *
     * @return number of axes
     */
    Int_t GetAxisNo() const { return fAxisNo; };
    /**
     *
     * @param i axis number / cut number
     * @return number of parameter monitored for given cut/axis
     */
    Int_t GetCutParameter(Int_t i) const;
    /**
     *
     * @param other
     * @return true if both monitors are similar
     */
    virtual Bool_t AreSimilar(const CutMonitor& other) const;
    /**
     *
     * @param i axes no
     * @return cut name
     */
    TString GetCutName(Int_t i) const { return fCutNames[i]; };
    /**
     *
     * @return update ratio of this monitor
     */
    ECutUpdate GetUpdateRatio() const;
    /**
     *
     * @return true if method used UpdateByObject instead of Update
     */
    virtual Bool_t ObjMonitor() const { return kFALSE; };
    /**
     * initialize this cut monitor
     * @param task_id task number
     */
    virtual Bool_t Init(Int_t task_id);
    /**
     * make update of cut monitor
     * @param passed state used for update
     * @param obj - passed object (not used in non-obj cuts
     */
    virtual void Update(Bool_t passed, TObject* obj);
    /**
     * make copy of this object
     * @return copy of this
     */
    virtual CutMonitor* MakeCopy() const;
    /**
     *
     * @return report from this cut monitor
     */
    virtual Package* Report() const;
    /**
     *
     * @param i axis no
     * @return pointer to linked cut
     */
    Cut* GetCutLink(Int_t i) const { return fCut[i]; };
    /**
     * mark as real cut (used by some property monitors)
     */
    void SetFlagRe() { SETBIT(fFlags, EFlagBit::kRe); }
    /**
     * mark as imaginary cut (used by some property monitors)
     */
    void SetFlagIm() { SETBIT(fFlags, EFlagBit::kIm); }
    virtual ~CutMonitor();
    ClassDef(CutMonitor, 2)
  };
}  // namespace Hal
#endif /* HALCUTMONITOR_H_ */
