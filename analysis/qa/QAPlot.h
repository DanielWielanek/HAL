/*
 * QAPlot.h
 *
 *  Created on: 11 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALQAPLOT_H_
#define HALQAPLOT_H_

#include "QAHistoSettings.h"
#include "QAPlotReport.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "Std.h"


/**
 * base class for QA plots, base on QAReport that is representation of
 * this class in form of report This class contains additional informations
 * about configuration of histograms
 */

namespace Hal {
  class Track;
  class Event;
  class QAPlotAxis;

  class QAPlot : public TNamed {
    ECutUpdate fUpdate;
    std::vector<QAHistoSettings> fSettings[3];
    QAPlotReport* fReport;

  protected:
    inline Int_t GetSize1D() const { return fSettings[0].size(); };
    /**
     *
     * @return number of 2-D histograms
     */
    inline Int_t GetSize2D() const { return fSettings[1].size(); };
    /**
     *
     * @return number of 3-D histograms
     */
    inline Int_t GetSize3D() const { return fSettings[2].size(); };
    /**
     * return 1D histogram, note: doesn't check if histogram exists
     * @param no id of histogram
     * @return
     */
    inline TH1D* Get1D(Int_t no) const { return fReport->Get1D(no); };
    /**
     * return 2D histogram, note: doesn't check if histogram exists
     * @param no id of histogram
     * @return
     */
    inline TH2D* Get2D(Int_t no) const { return fReport->Get2D(no); };
    /**
     * return 3D histogram, note: doesn't check if histogram exists
     * @param no id of histogram
     * @return
     */
    inline TH3D* Get3D(Int_t no) const { return fReport->Get3D(no); };
    void FillTrack(Track* track);
    void FillEvent(Event* event);
    /**
     * @param track track to fill
     * @param h histogram to fill
     * @param no id filled histogram
     */
    virtual void FillTrackCustom1D(Track* /*track*/, TH1D* /*h*/, Int_t /*no*/) {};
    /**
     * @param event event to fill
     * @param h histogram to fill
     * @param no id filled histogram
     */
    virtual void FillEventCustom1D(Event* /*event*/, TH1D* /*h*/, Int_t /*no*/) {};
    /**
     * @param track track to fill
     * @param h histogram to fill
     * @param no id filled histogram
     */
    virtual void FillTrackCustom2D(Track* /*track*/, TH2D* /*h*/, Int_t /*no*/) {};
    /**
     * @param event event to fill
     * @param h histogram to fill
     * @param no id filled histogram
     */
    virtual void FillEventCustom2D(Event* /*event*/, TH2D* /*h*/, Int_t /*no*/) {};
    /**
     * @param track track to fill
     * @param h histogram to fill
     * @param no id filled histogram
     */
    virtual void FillTrackCustom3D(Track* /*track*/, TH3D* /*h*/, Int_t /*no*/) {};
    /**
     * @param event event to fill
     * @param h histogram to fill
     * @param no id filled histogram
     */
    virtual void FillEventCustom3D(Event* /*event*/, TH3D* /*h*/, Int_t /*no*/) {};

  public:
    QAPlot();
    QAPlot(ECutUpdate upd);
    /**
     *
     * @param name name of  QA
     */
    QAPlot(TString name, ECutUpdate upd);
    /**
     * Add TH1 histogram
     * @param name name and title  of histogram
     * @param x
     */
    Int_t AddTH1(TString name, const QAPlotAxis& x, TString flag = "");
    /**
     * Add TH2 histogram
     * @param name name and title  of histogram
     * @param x
     * @param y
     * @param flag
     */
    Int_t AddTH2(TString name, const QAPlotAxis& x, const QAPlotAxis& y, TString flag = "");
    /**
     * Add TH3 histogram
     * @param name name and title  of histogram
     * @param x
     * @param y
     * @param z
     * @param flag
     */
    Int_t AddTH3(TString name, const QAPlotAxis& x, const QAPlotAxis& y, const QAPlotAxis& z, TString flag = "");
    /**
     * makes copy of this class
     * @return
     */
    virtual QAPlot* MakeCopy() const { return (QAPlot*) this->Clone(); };
    /**
     * init QA plot, if used in task then task id should be provided
     * @param id_task
     */
    virtual Bool_t Init(Int_t id_task = -1);
    /**
     * fill these QA plots
     * @param obj
     */
    virtual void Fill(TObject* obj);
    ECutUpdate GetUpdateRatio() const { return fUpdate; }
    virtual void Print(Option_t* opt = "") const;
    virtual ~QAPlot();
    QAPlotReport* GetReport() const;
    QAPlot(const QAPlot& other);
    QAPlot& operator=(const QAPlot& other);
    ClassDef(QAPlot, 1)
  };
}  // namespace Hal
#endif /* HALQAPLOT_H_ */
