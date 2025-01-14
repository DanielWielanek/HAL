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
    /**
     * number of axis in cut monitor
     */
    const Int_t fAxisNo;
    /**
     * number of currently added cuts
     */
    Int_t fCuts;
    /**
     * collection number of this cut monitor
     */
    Int_t fCollectionID;
    /**
     * array with numbers of axis bins
     */
    Int_t* fAxisBins;  //[fAxisNo]
    /**
     * array with parameters numbers used from cuts for all axes
     */
    Int_t* fOptionAxis;  //[fAxisNo]
    /**
     * histogram with passed objects
     */
    TH1* fHistoPassed;
    /**
     * histogram with failed objects
     */
    TH1* fHistoFailed;
    /**
     * array with lower edges of axes
     */
    Double_t* fAxisMin;  //[fAxisNo]
    /**
     * array with upper edges of axes
     */
    Double_t* fAxisMax;  //[fAxisNo]
    /**
     * init flag, true if monitor has been initialized
     */
    Bool_t fInit;
    /**
     * exclusive flag, true if monitor works in exclusive mode
     */
    Bool_t fExUpdate;
    /**
     * array with pointers to monitored cuts
     */
    Cut** fCut;  //[fAxisNo]
    /**
     * array with pointers to names of monitored cuts
     */
    TString* fCutNames;  //[fAxisNo]
    /**
     * update ratio of this cut monitor
     */
    ECutUpdate fUpdateRatio;
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
    Bool_t AreSimilar(CutMonitor* other) const;
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
    virtual ~CutMonitor();
    ClassDef(CutMonitor, 1)
  };
}  // namespace Hal
#endif /* HALCUTMONITOR_H_ */
