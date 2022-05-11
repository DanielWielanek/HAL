/*
 * QAPlot.h
 *
 *  Created on: 11 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_QA_HALQAPLOT_H_
#define HALFEMTO_ANALYSIS_QA_HALQAPLOT_H_

#include "QAHistoSettings.h"
#include "QAPlotReport.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "HalStd.h"


/**
 * base class for QA plots, base on QAReport that is representation of
 * this class in form of report This class contains additional informations
 * about configuration of histograms
 */

namespace Hal{
class Track;
class Event;

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
  virtual void FillTrackCustom1D(Track* track, TH1D* h, Int_t no) {};
  /**
   * @param event event to fill
   * @param h histogram to fill
   * @param no id filled histogram
   */
  virtual void FillEventCustom1D(Event* event, TH1D* h, Int_t no) {};
  /**
   * @param track track to fill
   * @param h histogram to fill
   * @param no id filled histogram
   */
  virtual void FillTrackCustom2D(Track* track, TH2D* h, Int_t no) {};
  /**
   * @param event event to fill
   * @param h histogram to fill
   * @param no id filled histogram
   */
  virtual void FillEventCustom2D(Event* event, TH2D* h, Int_t no) {};
  /**
   * @param track track to fill
   * @param h histogram to fill
   * @param no id filled histogram
   */
  virtual void FillTrackCustom3D(Track* track, TH3D* h, Int_t no) {};
  /**
   * @param event event to fill
   * @param h histogram to fill
   * @param no id filled histogram
   */
  virtual void FillEventCustom3D(Event* event, TH3D* h, Int_t no) {};

public:
  QAPlot();
  QAPlot(ECutUpdate upd);
  /**
   *
   * @param name name of  QA
   * @param oneDimHist number of 1d histograms
   * @param twoDimHist number of 2d histograms
   * @param threeDimHist number of 3d histograms
   */
  QAPlot(TString name, Int_t oneDimHist, Int_t twoDimHist, Int_t threeDimHist, ECutUpdate upd);
  /**
   * adds TH1 histogram
   * @param name
   * @param flagIDx - ID of variable filled on x-axis, set negative to define histogram as "customized"
   * @param flag flag that define operations on QA plots e.g.:
   * 0/1 mean this histogram is equal to histo[0]/histo[1]
   * @return number of created histogram
   */
  Int_t AddTH1(TString name, Int_t flagIDx, TString flag = "");
  /**
   * adds TH2 histogram
   * @param name
   * @param flag flag that define operations on QA plots e.g.:
   * 0/1 mean this histogram is equal to histo[0]/histo[1]
   * @param flagIDx - ID of variable filled on x-axis, set negative to define histogram as "customized"
   * @param flagIDy - ID of variable filled on y-axis, set negative to define histogram as "customized"
   * @return number of created histogram
   */
  Int_t AddTH2(TString name, Int_t flagIDx, Int_t flagIDy, TString flag = "");
  /**
   * adds TH3 histogram
   * @param name
   * @param flag flag that define operations on QA plots e.g.:
   * 0/1 mean this histogram is equal to histo[0]/histo[1]
   * @param flagIDx - ID of variable filled on x-axis, set negative to define histogram as "customized"
   * @param flagIDy - ID of variable filled on y-axis, set negative to define histogram as "customized"
   * @param flagIDz - ID of variable filled onz-axis, set negative to define histogram as "customized"
   * @return number of created histogram
   */
  Int_t AddTH3(TString name, Int_t flagIDx, Int_t flagIDy, Int_t flagIDz, TString flag = "");
  /**
   * adds TH1 histogram
   * @param name
   * @param flagIDx - ID of variable filled on x-axis, set negative to define histogram as "customized"
   * @param flag flag that define operations on QA plots e.g.:
   * 0/1 mean this histogram is equal to histo[0]/histo[1]
   *    * @param nBinsX - number of bins in X-axis
   * @param minX - minimum on X-axis
   * @param maxX - maximum on X-axis
   * @return number of created histogram
   */
  Int_t AddTH1(TString name, Int_t flagIDx, Int_t nbinsX, Double_t min, Double_t max, TString flag = "");
  /**
   * adds TH2 histogram
   * @param name
   * @param flag flag that define operations on QA plots e.g.:
   * 0/1 mean this histogram is equal to histo[0]/histo[1]
   * @param flagIDx - ID of variable filled on x-axis, set negative to define histogram as "customized"
   * @param flagIDy - ID of variable filled on y-axis, set negative to define histogram as "customized"
   *    * @param nBinsX - number of bins in X-axis
   * @param minX - minimum on X-axis
   * @param maxX - maximum on X-axis
   * @param nBinsY - number of bins in Y-axis
   * @param minY - minimum on Y-axis
   * @param maxY - maximum on Y-axis
   * @return number of created histogram
   */
  Int_t AddTH2(TString name,
               Int_t flagIDx,
               Int_t flagIDy,
               Int_t nbinsX,
               Double_t minX,
               Double_t maxX,
               Int_t nbinsY,
               Double_t minY,
               Double_t maxY,
               TString flag = "");
  /**
   * adds TH3 histogram
   * @param name
   * @param flag flag that define operations on QA plots e.g.:
   * 0/1 mean this histogram is equal to histo[0]/histo[1]
   * @param flagIDx - ID of variable filled on x-axis, set negative to define histogram as "customized"
   * @param flagIDy - ID of variable filled on y-axis, set negative to define histogram as "customized"
   * @param flagIDz - ID of variable filled onz-axis, set negative to define histogram as "customized"
   * @param nBinsX - number of bins in X-axis
   * @param minX - minimum on X-axis
   * @param maxX - maximum on X-axis
   * @param nBinsY - number of bins in Y-axis
   * @param minY - minimum on Y-axis
   * @param maxY - maximum on Y-axis
   * @param nBinsZ - number of bins in Z-axis
   * @param minZ- minimum on Z-axis
   * @param maxZ - maximum on Z-axis
   * @return number of created histogram
   */
  Int_t AddTH3(TString name,
               Int_t flagIDx,
               Int_t flagIDy,
               Int_t flagIDz,
               Int_t nbinsX,
               Double_t minX,
               Double_t maxX,
               Int_t nbinsY,
               Double_t minY,
               Double_t maxY,
               Int_t nbinsZ,
               Double_t minZ,
               Double_t maxZ,
               TString flag = "");


  /**
   * makes copy of this class
   * @return
   */
  virtual QAPlot* MakeCopy() const { return (QAPlot*) this->Clone(); };
  /**
   * configure axies of 1D histogram
   * @param no
   * @param nbinsX
   * @param minX
   * @param maxX
   */
  void SetAxis1D(Int_t no, Int_t nbinsX, Double_t minX, Double_t maxX);
  /**
   * configure axes of 2D histogram
   * @param no
   * @param nbinsX
   * @param minX
   * @param maxX
   * @param nbinsY
   * @param minY
   * @param maxY
   */
  void SetAxis2D(Int_t no, Int_t nbinsX, Double_t minX, Double_t maxX, Int_t nbinsY, Double_t minY, Double_t maxY);
  /**
   * configure axes of 3D histogram
   * @param no
   * @param nbinsX
   * @param minX
   * @param maxX
   * @param nbinsY
   * @param minY
   * @param maxY
   * @param nbinsZ
   * @param minZ
   * @param maxZ
   */
  void SetAxis3D(Int_t no,
                 Int_t nbinsX,
                 Double_t minX,
                 Double_t maxX,
                 Int_t nbinsY,
                 Double_t minY,
                 Double_t maxY,
                 Int_t nbinsZ,
                 Double_t minZ,
                 Double_t maxZ);

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
}
#endif /* HALFEMTO_ANALYSIS_QA_HALQAPLOT_H_ */
