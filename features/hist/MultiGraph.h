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

class TAxis;
class TGraphErrors;
class TMultiGraph;

/**
 * class for creating multi graph without manual creating mutiple graphs
 */
namespace Hal {
  class MultiGraph : public TObject {
    Int_t fN;
    TGraphErrors** fGraphErrors;  //[fN]
    TMultiGraph* fMultiGraph;
    TAxis* fXaxis;
    TAxis* fYaxis;
    Double_t fMin[2];
    Double_t fMax[2];
    void CopyAxis(TAxis* from, TAxis* to);

  public:
    MultiGraph();
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
     * @param no graph no if -1 then aply to all graphs
     */
    void SetMarkerStyle(Style_t marker, Int_t no = -1);
    /**
     * set marker size
     * @param size marker size
     * @param no graph no if -1 then aply to all graphs
     */
    void SetMarkerSize(Size_t size, Int_t no = -1);
    /**
     * set marker color
     * @param color marker color
     * @param no graph no if -1 then aply to all graphs
     */
    void SetMarkerColor(Color_t color, Int_t no = -1);
    /**
     * set line width
     * @param width line width
     * @param no graph no if -1 then aply to all graphs
     */
    void SetLineWidth(Width_t width, Int_t no = -1);
    /**
     * set line attributes
     * @param style line style
     * @param width line width
     * @param color line color
     * @param no graph no if -1 aply to all graphs
     */
    void SetLineAttributes(Style_t style, Width_t width, Color_t color, Int_t no = -1);
    /**
     * set marker attributes
     * @param style marker style
     * @param size marker size
     * @param color marker color
     * @param no graph no, if -1 then aplly to all graphs
     */
    void SetMarkerAttributes(Style_t style, Size_t size, Color_t color, Int_t no = -1);
    /**
     * set marker style
     * @param color marker color
     * @param no graph no if -1 then aply to all graphs
     */
    void SetLineColor(Color_t color, Int_t no = -1);
    /**
     * set line style
     * @param style line style
     * @param no graph no if -1 then aply to all graphs
     */
    void SetLineStyle(Style_t style, Int_t no = -1);
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
    TGraphErrors* GetGraph(Int_t no) const;
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
    virtual ~MultiGraph();
    ClassDef(MultiGraph, 1)
  };
}  // namespace Hal
#endif /* HALMULTIGRAPH_H_ */
