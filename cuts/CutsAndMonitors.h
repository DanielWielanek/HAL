/*
 * Cuts.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCUTSANDMONITORS_HALCUTS_H_
#define HALCUTSANDMONITORS_HALCUTS_H_

#include "Cut.h"
#include "CutMonitor.h"
#include "CutMonitorRequest.h"

#include <TLorentzVector.h>
#include <TString.h>


class TObjString;
namespace Hal {
  class CutMonitorX;
  class CutMonitorYX;
  class CutMonitorXYZ;

  /**
   * class that stores collection of hits and cut monitors
   * after adding to task cuts/cut monitors are extracted and added with "local"
   * and "global" option. E.g. CutsAndMonitors::AddCut(some_cut,"{0}) and
   * CutsAndMonitors::SetOptionForAllCuts("re") is equivalent of
   * EventAna::AddCut(some_cut,"{0}+re"), @see TrackKinematicsCutAndMonitor as and example of
   * this class
   *
   */


  class CutsAndMonitors : public TObject {
    TObjArray* fCuts;
    TObjArray* fCutsOptions;
    TObjArray* fCutMonitors;
    TObjArray* fCutMonitorsOptions;
    TString fGlobalOptionCuts;
    TString fGlobalOptionsCutMonitors;
    std::vector<CutMonitorRequest> fCutMonitorRequests;

  protected:
    /**
     * return cut at i
     * @param i
     * @return
     */
    Cut* CutAt(Int_t i) const;
    /**
     *
     * @param i
     * @return monitor at
     */
    CutMonitor* MonitorAt(Int_t i) const;
    /**
     * clear cuts and cuts options
     */
    void ClearCuts();
    /**
     * clear cut monitors and their options
     */
    void ClearMonitors();
    /**
     * add  cut without copying the cut, cut will be owned by this class
     * @param cut
     * @param opt
     */
    void AddRawCut(Cut* cut, TObjString* opt = nullptr);
    /**
     * add cut monitor without copying, this monitor will be owned by this class
     * @param mon
     * @param opt
     */
    void AddRawCutMonitor(CutMonitor* mon, TObjString* opt = nullptr);
    /**
     * add request to cut monitor, cuts must be present at this time
     * @param x configuration of x axis
     */
    void AddCutMonitorRequest(const CutMonAxisConf& x);
    /**
     * add request to cut monitor, cuts must be present at this time
     * @param x configuration of x axis
     * @param y configuration of y axis
     */
    void AddCutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y);
    /**
     * add request to cut monitor, cuts must be present at this time
     * @param x configuration of x axis
     * @param y configuration of y axis
     * @param z configuration of z axis
     */
    void AddCutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y, const CutMonAxisConf& z);
    /**
     * add all cut monitor requests
     * @param option passed by @see MakeCutMonitors
     */
    virtual void AddAllCutMonitorRequests(Option_t* opt) = 0;
    /**
     * return cut monitor request
     * @param request_no
     * @return
     */
    CutMonitor* MakeCutMonitor(Int_t request_no) const;
    /**
     * find number of cut by it classname
     * @param classname
     * @return cut no if cut has been foudn, -1 otherwise
     */
    Int_t GetCutNo(TString classname) const;

  public:
    CutsAndMonitors();
    CutsAndMonitors(const CutsAndMonitors& other);
    CutsAndMonitors& operator=(const CutsAndMonitors& other);
    /**
     * set global option for all cuts
     * @param opt
     */
    void SetOptionForAllCuts(TString opt) { fGlobalOptionCuts = opt; };
    /**
     * set global option for all cut monitors
     * @param opt
     */
    void SetOptionForAllMonitors(TString opt) { fGlobalOptionsCutMonitors = opt; }
    /**
     * add cut to this container
     * @param cut
     * @param opt
     */
    void AddCut(const Cut& cut, Option_t* opt = "");
    /**
     * add cut monitor to this container note: this class doesn't have to containe
     * monitored cuts, they can be added later to the task
     * @param monitor
     * @param opt
     */
    void AddCutMonitor(const CutMonitor& monitor, Option_t* opt = "");
    /**
     * set collection id to all cuts
     * @param id
     */
    void SetCollectionID(Int_t id);
    /**
     *
     * @param i
     * @return cut
     */
    const Cut* GetCut(Int_t i) const;
    /**
     *
     * @param i
     * @return "local" option for given cut
     */
    TString GetCutOption(Int_t i) const;
    /**
     *
     * @return "global" option for all cuts
     */
    TString GetGlobalCutOption() { return fGlobalOptionCuts; };
    /**
     *
     * @param i
     * @return i-th monitor
     */
    const CutMonitor* GetMonitor(Int_t i) const;
    /**
     *
     * @return global cut monitor option
     */
    TString GetGlobalCutMonitorOption() { return fGlobalOptionsCutMonitors; };
    /**
     *
     * @param i
     * @return local cut monitor option
     */
    TString GetCutMonitorOption(Int_t i) const;
    /**
     *
     * @return number of cuts
     */
    Int_t GetNCuts() const;
    /**
     *
     * @return number of cut monitors
     */
    Int_t GetNCutMonitors() const;
    /**
     *
     * @return number of request for cut monitors
     */
    Int_t GetNRequest() const { return fCutMonitorRequests.size(); }
    /**
     *
     * @param i
     * @return given request for cut monitor
     */
    CutMonitorRequest& GetCutMonitorRequest(Int_t i) { return fCutMonitorRequests[i]; }
    /**
     * this function makes requested cut monitors, it must be called before adding to analysis
     */
    void MakeCutMonitors(Option_t* opt = "");
    virtual ~CutsAndMonitors();
    ClassDef(CutsAndMonitors, 1)
  };
}  // namespace Hal
#endif /* HALCUTSANDMONITORS_HALCUTS_H_ */
