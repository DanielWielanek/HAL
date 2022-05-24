/*
 * QAPlotReport.h
 *
 *  Created on: 12 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALQAPLOTREPORT_H_
#define HALQAPLOTREPORT_H_

#include "HistogramManager.h"
#include "HtmlTable.h"
#include "Parameter.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TString.h>

#include "Std.h"

/**
 * base class for storing plots from QAPlot based  task, this class only
 * stores histograms and information about operations that have to be performed
 * on histograms. Using QAPlotReport instead of saving custon QAPlot allow
 * to keep simplicity of output files. To extract such files please use
 * QAExtractor.
 */

namespace Hal {
  class QAPlot;

  class QAPlotReport : public Object {
    friend class QAPlot;

  private:
    std::vector<TString> f1dFlags;
    std::vector<TString> f2dFlags;
    std::vector<TString> f3dFlags;

    Object* fExtraInfo;
    ParameterString fOriginName;

  protected:
    HistogramManager_1_1D<TH1D>* f1dHistos;
    HistogramManager_1_2D<TH2D>* f2dHistos;
    HistogramManager_1_3D<TH3D>* f3dHistos;
    /**
     * check if flag can be set to given histogram
     * @param no histogram that we define
     * @param c operator "+" -adding "-" subtracking "/" division
     * @param h1 first histogram
     * @param h2 second histogram
     * @param dim dimension 1 for 1D histograms, etc.
     * @return
     */
    Bool_t CheckFlag(Int_t no, Char_t c, Int_t h1, Int_t h2, Int_t dim) const;
    /**
     * set flag to histogram, flag is a recipe how to recalculate given histogram
     * from other histograms, for example SetFlag1D(1,'+',2,3) means that
     * histogram number 1 is a sum of histogram number 2 and 3 (1=2+3)
     * @param no number of histogram to recalculate
     * @param c operation + - o r /
     * @param h1 first histogram
     * @param h2 second histogram
     */
    void SetFlag1D(Int_t no, Char_t c = ' ', Int_t h1 = 0, Int_t h2 = 0);
    /**
     * set flag to histogram, flag is a recipe how to recalculate given histogram
     * from other histograms, for example SetFlag2D(1,'+',2,3) means that
     * histogram number 1 is a sum of histogram number 2 and 3 (1=2+3)
     * @param no number of histogram to recalculate
     * @param c operation + - o r /
     * @param h1 first histogram
     * @param h2 second histogram
     */
    void SetFlag2D(Int_t no, Char_t c = ' ', Int_t h1 = 0, Int_t h2 = 0);
    /**
     * set flag to histogram, flag is a recipe how to recalculate given histogram
     * from other histograms, for example SetFlag3D(1,'+',2,3) means that
     * histogram number 1 is a sum of histogram number 2 and 3 (1=2+3)
     * @param no number of histogram to recalculate
     * @param c operation + - o r /
     * @param h1 first histogram
     * @param h2 second histogram
     */
    void SetFlag3D(Int_t no, Char_t c = ' ', Int_t h1 = 0, Int_t h2 = 0);
    /**
     * translate flag to values used in opeartor
     * @param flag input flag
     * @param hist number of first histogram in equation
     * @param hist2 number of second histogram in equation
     * @param op the equation
     */
    void TranslateFlag(TString flag, Int_t& hist, Int_t& hist2, Char_t& op) const;
    /**
     * calculate h0 histogram from h1 and h2 histogram with operation defined by
     * op
     * @param h0
     * @param h1
     * @param h2
     * @param op
     */
    void Operate(TH1* h0, TH1* h1, TH1* h2, Char_t op) const;
    /**
     * export single histogram to file
     * @param path
     * @param count
     * @param h
     * @return
     */
    TString ExportHistogramToFile(TString path, Int_t count, TH1* h) const;
    /**
     * export single histogram to file in subdir
     * @param path
     * @param dir
     * @param count
     * @param h
     * @return
     */
    TString ExportHistogramToFile(TString path, TString dir, Int_t count, TH1* h) const;
    /**
     * constructor that should be used to all QA plots
     * @param name
     * @param oneDim
     * @param twoDim
     * @param threeDIm
     * @param upd
     */
    QAPlotReport(TString name, Int_t oneDim, Int_t twoDim, Int_t threeDIm);

    HistogramManager_1_1D<TH1D>* GetHistArray1D() const { return f1dHistos; };
    HistogramManager_1_2D<TH2D>* GetHistArray2D() const { return f2dHistos; };
    HistogramManager_1_3D<TH3D>* GetHistArray3D() const { return f3dHistos; };

  public:
    QAPlotReport();
    QAPlotReport(const QAPlot& other);
    QAPlotReport(const QAPlotReport& other);
    QAPlotReport& operator=(const QAPlotReport& other);
    virtual TString HTMLExtract(Int_t no, TString dir = "") const;
    virtual void HTMLExtractIntoTable(Int_t no, HtmlTable& obj, TString dir = "", TString rel_dir = "") const;
    virtual Bool_t CanBeTableElement() const { return kTRUE; };
    virtual void Print(Option_t* option = "") const;
    /**
     * set extra information about this plot
     * @param p
     */
    virtual void SetExtraInfo(Object* p);
    virtual void Add(const Object* pack);
    virtual void Browse(TBrowser* b);
    /**
     * set histogram flag
     * @param no histogram number
     * @param flag flag
     * @param dim number of dimensions
     */
    void SetFlag(Int_t no, TString flag, Int_t dim);
    /** finish this task, recalculate histograms if needed **/
    virtual void Recalculate();
    /**
     * set name of this plots
     * @param name
     */
    void SetOriginClass(TString name) { fOriginName.SetValue(name); };
    /**
     * set name of this class
     * @return
     */
    TString GetOriginClass() const { return fOriginName.GetValue(); };
    /**
     *
     * @return number of 1-D histograms
     */
    inline Int_t GetSize1D() const { return f1dFlags.size(); };
    /**
     *
     * @return number of 2-D histograms
     */
    inline Int_t GetSize2D() const { return f2dFlags.size(); };
    /**
     *
     * @return number of 3-D histograms
     */
    inline Int_t GetSize3D() const { return f3dFlags.size(); };
    /**
     * return 1D histogram, note: doesn't check if histogram exists
     * @param no id of histogram
     * @return
     */
    inline TH1D* Get1D(Int_t no) const { return f1dHistos->At(no); };
    /**
     * return 2D histogram, note: doesn't check if histogram exists
     * @param no id of histogram
     * @return
     */
    inline TH2D* Get2D(Int_t no) const { return f2dHistos->At(no); };
    /**
     * return 3D histogram, note: doesn't check if histogram exists
     * @param no id of histogram
     * @return
     */
    inline TH3D* Get3D(Int_t no) const { return f3dHistos->At(no); };
    /**
     *
     * @return extra informatio about this QA plots
     */
    Object* GetExtraInfo() const { return fExtraInfo; };
    virtual Long64_t Merge(TCollection* collection);
    virtual ~QAPlotReport();
    ClassDef(QAPlotReport, 1)
  };
}  // namespace Hal
#endif /* HALQAPLOTREPORT_H_ */
