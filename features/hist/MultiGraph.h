/*
 * HalMultiGraph.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMULTIGRAPH_H_
#define HALMULTIGRAPH_H_

#include <TObject.h>
#include <TString.h>

class TAxis;
class TH1;
class TGraph;
class TGraphAsymmErrors;
class TMultiGraph;

/**
 * class for creating multi graph without manual creating mutiple graphs
 */
namespace Hal {
  /**
   * class for more effective managing of graphs that are drawn together
   * can be handled in two ways:
   * - normal mode - each graphs represents the same data
   * - staty/sys mode- even graphs represents systematical uncertainties and odd graphs represents statistical uncertainties
   * syst. uncertainties are stored in even graphs, stat. are stored in odd histograms, to avoid setting to many styles user can
   * call: MakeStatSys - adds all even histograms with option "5" to draw rectangles and odd with option "P" user can overwrite
   * options for sys and stat graphs SetStatSysStyles - copy part of styles from odd histograms to event histograms
   *
   */
  class MultiGraph : public TObject {
    std::vector<TGraphAsymmErrors*> fGraphErrors;
    std::vector<TString> fAddingOptions;
    TMultiGraph* fMultiGraph;
    TAxis* fXaxis;
    TAxis* fYaxis;
    Double_t fMin[2];
    Double_t fMax[2];
    Bool_t fDrawn = {kFALSE};
    /**
     * return list of graps
     * @param opt - -1 - return all graphs, -2 return even graphs, -3 return odd graphs, otherwise return graph with given number
     * @return
     */
    std::vector<TGraphAsymmErrors*> GetListForOpt(Int_t opt) const;

  public:
    MultiGraph();
    /**
     * make multigraph with many graphs
     * @param graphs
     * @param option of adding graph
     */
    MultiGraph(Int_t graphs, Option_t* opt = "");
    /**
     * set x axis range
     * @param min
     * @param max
     */
    void SetXRange(Double_t min, Double_t max);
    /**
     * set y axis range
     * @param min
     * @param max
     */
    void SetYRange(Double_t min, Double_t max);
    /**
     * set marker style
     * @param marker marker style
     * @param no graph no if -1 then apply to all graphs
     */
    void SetMarkerStyle(Style_t marker, Int_t no = -1);
    /**
     * set marker size
     * @param size marker size
     * @param no graph no if -1 then apply to all graphs
     */
    void SetMarkerSize(Size_t size, Int_t no = -1);
    /**
     * set marker color
     * @param color marker color
     * @param no graph no if -1 then apply to all graphs
     */
    void SetMarkerColor(Color_t color, Int_t no = -1);
    /**
     * set line width
     * @param width line width
     * @param no graph no if -1 then apply to all graphs
     */
    void SetLineWidth(Width_t width, Int_t no = -1);
    /**
     * set line attributes
     * @param style line style
     * @param width line width
     * @param color line color
     * @param no graph no if -1 apply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only to odd
     * graphs (event)
     */
    void SetLineAttributes(Style_t style, Width_t width, Color_t color, Int_t no = -1);
    /**
     * set marker attributes
     * @param style marker style
     * @param size marker size
     * @param color marker color
     * @param no graph no, if -1 then aply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void SetMarkerAttributes(Style_t style, Size_t size, Color_t color, Int_t no = -1);
    /**
     * set marker style
     * @param color marker color
     * @param no graph no if -1 then apply to all graphs , if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void SetLineColor(Color_t color, Int_t no = -1);
    /**
     * set line style
     * @param style line style
     * @param no graph no if -1 then apply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void SetLineStyle(Style_t style, Int_t no = -1);
    /**
     * set fill style
     * @param style fill style
     * @param no graph no if -1 then apply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void SetFillStyle(Int_t style, Int_t no = -1);
    /**
     * set fill color
     * @param color fill color
     * @param no graph no if -1 then apply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void SetFillColor(Color_t color, Int_t no = -1);
    /**
     * makes graph with given draw options
     * @param opt gdraw option
     */
    void MakeGraph(Option_t* opt = "P");
    /**
     * makes fewgraph with given draw options
     * @param no number of graphs to make
     * @param opt gdraw option
     */
    void MakeGraphs(Int_t no, Option_t* opt = "P");
    /**
     * set point for given graph
     * @param graph graph no
     * @param point point no
     * @param x x-value
     * @param y y-value
     */
    void SetPoint(Int_t graph, Int_t point, Double_t x, Double_t y);
    /**
     * set point for given graph
     * @param graph graph no
     * @param point point no
     * @param x x-value
     * @param ex x-error
     * @param y y-value
     * @param ey y-error
     */
    void SetPoint(Int_t graph, Int_t point, Double_t x, Double_t ex, Double_t y, Double_t ey);
    /**
     *
     * @param graph
     * @param point
     * @param x
     * @param exLow
     * @param exHigh
     * @param y
     * @param eyLow
     * @param eyHigh
     */
    void
    SetPoint(Int_t graph, Int_t point, Double_t x, Double_t exLow, Double_t exHigh, Double_t y, Double_t eyLow, Double_t eyHigh);
    /**
     *  return pointer to temporary X-axs. Thanks to this its possible to set
     * title's/label's font size/font color/offset. However for setting range user
     * must call SetXRange.
     * @return pointer to axis
     */
    TAxis* GetXaxis();
    /**
     *  return pointer to temporary X-axs. Thanks to this its possible to set
     * title's/label's font size/font color/offset. However for setting range user
     * must call SetXRange.
     * @return pointer to axis
     */
    TAxis* GetYaxis();
    /**
     * return graph
     * @param no graph no
     * @return
     */
    TGraphAsymmErrors* GetGraph(Int_t no) const;
    /**
     * return pointer to drawn multi graph, if this object has not been drawn
     * - return NULL
     */
    TMultiGraph* GetMutliGraph() const;
    /**
     * draw this graph
     * @param opt draw option
     */
    void Draw(Option_t* opt = "");
    /**
     *
     * @return number of histograms
     */
    Int_t GetNGraphs() const { return fGraphErrors.size(); }
    /**
     * make this graph suitable to draw and systematic errors
     * by convention:
     * even - systematic uncertainty drawn with "5" option
     * odd - statistical uncertainty draw with options provided when call MakeGraph
     * @param optStat - default option for statistical uncertainty if empty keep option from MakeGraph
     * @param optSys - default option for systematical uncertainty if empty keep option from MakeGraph
     */
    void MakeStatSys(TString optStat = "", TString optSys = "5");
    /**
     * set default styles for syst errors graps by basing on statystical uncertainties
     * - the same line color
     * - set marker style 1 (dot)
     * @param opt flag of setting styles = "fill" copy fill color from marker ,"line" copy line color from marker ,"clear" set
     * fill style -1 (makes systematic uncertainties) transparent optionally set fill style by adding number in brackets eg.
     * {3023}
     */
    void SetStatSysStyles(TString opt = "line+clear");
    /**
     * add histogram to this multigraph
     * @param h
     * @param no
     */
    void AddHistogram(const TH1& h, Int_t no = -1);
    /**
     * set points in stat-sys style
     * @param pairGraph number of pair of the graps (even - systematic, odd -statistic error) - eg. for pairGraph = N
     * set sys data for graph = 2N and stat data for graph = 2N+1
     * @param point point number
     * @param x x-value
     * @param errXStat statistical uncertainty, if vector.size is zero - set errors to zero, if vector size is 1 set the same
     * upper and lower errors, if size is larger set lower (first element) and upper (second element errors)
     * @param errXsys set systematical uncertainty, if vector.size is zero - set errors to zero, if vector size is 1 set the same
     * upper and lower errors, if size is larger set lower (first element) and upper (second element errors)
     * @param y
     * @param errYStat like errXstat but for Y axis
     * @param errYsys like errXsys but for Y axis
     */
    void SetPointStatSys(Int_t pairGraph,
                         Int_t point,
                         Double_t x,
                         std::vector<Double_t> errXStat,
                         std::vector<Double_t> errXsys,
                         Double_t y,
                         std::vector<Double_t> errYStat,
                         std::vector<Double_t> errYsys);
    /**
     * set fixed errors on x, thanks to this the width of errors might be the same for logarithmic scale
     * @param ref reference to calculate logarithm
     * @param width upper error for reference
     * @param gr graph no if -1 then apply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void RecalculateXLogErrors(Double_t ref, Double_t widht, Int_t gr = -1);
    /**
     * set fixed errors on y, thanks to this the width of errors might be the same for logarithmic scale
     * @param ref reference to calculate logarithm
     * @param width upper error for reference
     * @param gr graph no if -1 then apply to all graphs if no == -2 then apply only to even graphs (sys), if no then apply only
     * to odd graphs (event)
     */
    void RecalculateYLogErrors(Double_t ref, Double_t widht, Int_t gr = -1);
    virtual ~MultiGraph();
    ClassDef(MultiGraph, 2)
  };
}  // namespace Hal
#endif /* HALMULTIGRAPH_H_ */
