/*
 * Hal::StdHist.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSTDHIST_H_
#define HALSTDHIST_H_

#include <TString.h>
#include <TVector3.h>

class TH1;
class TH1D;
class TH2;
class TH2D;
class TH3;
class TH3D;
class TAxis;
class TVirtualPad;

namespace Hal {
  namespace Std {
    /**
     * remove nans from histogram
     * @param h histogram to check
     * @param fill value to set in bin if is nan
     * @param fill_e error to set if bin content is nan
     */
    void RemoveNan(TH1* h, Double_t fill = 0, Double_t fill_e = 1);
    /**
     * copy axis properties like title color etc. if they are not default
     * @param from copy from
     * @param to copy to
     * @param addition option if "!tit" do not copy  title
     */
    void CopyAxisProp(TAxis* from, TAxis* to, TString opt = "");
    /**
     * make diagonal projection
     * @param h
     * @param dir direction projection - e.g. "xy" mean project at const z (start)
     * "xyz+++" means project from min,min,min to max,max,max
     * "xyz-++" means project from max,min,min to min max max max
     * "x" mean projection at const y and z
     * @param start first constant (used xy, yz, xz projections)
     * @param start2 - second constant value (used in x, y, z projections)
     * @return
     */
    TH1D* GetDiagonalProjection1D(TH3* h, TString dir, Double_t start, Double_t start2 = 0.);
    /**
     * return projection
     * @param min projection lower range
     * @param max projection upper range
     * @param opt  if conain "bins" then min and max specify bin range used to make
     * projections, otherwise it specify balues. If "scale" is specified, then
     * projection is divided by number of bins in projection range, if "y" is
     * specified the projection is made along y axis, otherwise along x axis.
     * "sumw" call TH1::Sumw before scalling (if used).
     * ROOT new projection might overwrite old one (if it's on the same pad or in
     * the same directory). To avoid this add "autoname" option (set name accordig
     * to number of objects into gDirectory) or "randname" (set name based on random
     * value).
     * .
     * @return projection
     */
    TH1D* GetProjection1D(const TH2* histo, Double_t min, Double_t max, Option_t* opt);
    /**
     *
     * @param min1 mimum value for first axis
     * @param max1 maximum value for first axis
     * @param min2 minimum value for second axis
     * @param max2 maximum value for second axis
     * @param opt  If user don't specify "bins" then projection is made from
     * histogram range between min an max value, if "bins" is used then these value
     * describes range in bins units.. Option "x" "y" "z" can be used for specify
     * axis. If "scale" then histogram is divided by number of bins in projection
     * range. "sumw" call TH1::Sumw before scalling (if used) In ROOT new projection might overwrite old one (if it's on the same
     * pad or in the same directory). To avoid this add "autoname" option (set name
     * accordig to number of objects into gDirectory) or "randname" (set name based
     * on random value).
     * @return projection
     */
    TH1D* GetProjection1D(const TH3* histo, Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt);
    /**
     * "smears" histogram with given "smear matrix"
     * important note: this version doesn't take into account uncertaitnies of smear
     * matrix The [i][j] bin say that in i-th bin goes to j-th bin with given weight
     * in other words on X you plot mc data whreas on Y reconstructed data
     * therefore matrix of transformation should be filled like
     * Fill(old_val,new_val,prob) probability doesn't need to be normalized.
     * @param input_histogram
     * @param smear_matrix
     * @param opt
     * @return if REV then reverse equation, if "ov" normalize last martix elements
     * - assume that from overflow bins balance with if "forced" - try "magic map
     * rebinning"
     */
    TH1D* SmearHistogram(TH1D* input_histogram, TH2D* smear_matrix, Option_t* opt = "");
    /**
     * return projection
     * @param min projection lower range
     * @param max projection upper range
     * @param opt if "bins" then min and max are treated as
     * number of bins for projection. If "scale" is specified, then projection is
     * divided by number of bins in projection range. Additional option is
     * specyfication of projetction it might be xy, yx etc. (like option in
     * TH3D:Project3D)
     * "sumw" call TH1::Sumw before scalling (if used).
     * @return projection
     */
    TH2D* GetProjection2D(const TH3* histo, Double_t min, Double_t max, Option_t* opt);
    /**
     * set under/overflow bins along given egde
     * @param h histogram
     * @param option - set x,y, z to specify axis, "ov" then fill overlow if "uv"
     * then fill underflow histogram if opciont contains "val" - use value set by
     * value, otherwise copy value from first/last bin. If axis is speficed then all
     * bins along this axis are filed accroding to value specified by user
     * @param value value to fill
     * @return
     */
    void HistogramEdges(TH1* h, TString option = "x+uv", Double_t value = 0);
    /**
     * extend histogram along given axis
     * @param h
     * @param axis
     * @param factor
     */
    void HistogramExtend(TH1* h, Char_t axis = 'x', Double_t factor = 1);
    /**
     * check if histograms are similar - have the same ranges and bin numbers
     * @param x
     * @param y
     * @param classes if true require that class have exactly the same type
     * @return true if histograms are similar
     */
    Bool_t AreSimilar(const TH1* x, const TH1* y, Bool_t classes = kTRUE);
    /**
     * get parameters of axis
     * @param obj histogram
     * @param nbins number of bins
     * @param min lower range of axis
     * @param xmax upper range of axis
     * @param opt  - x, y or z (specify axis)
     */
    void GetAxisPar(const TH1& obj, Int_t& nbins, Double_t& min, Double_t& xmax, Option_t* opt = "x");
    /**
     * create new histogram
     * @param name
     * @param title
     * @param Xaxis X axis configuration -number of bins, Y - min, Z - max
     * @param type 'D' for TH1D, 'F' for TH1F, 'I' for TH1I
     * @return
     */
    TH1* MakeHisto1D(TString name, TString title, TVector3 Xaxis, Char_t type = 'D');
    /**
     * create new histogram
     * @param name
     * @param title
     * @param Xaxis X axis configuration -number of bins, Y - min, Z - max
     * @param Yaxis Y axis configuration -number of bins, Y - min, Z - max
     * @param type 'D' for TH1D, 'F' for TH1F, 'I' for TH1I
     * @return
     */
    TH2* MakeHisto2D(TString name, TString title, TVector3 Xaxis, TVector3 Yaxis, Char_t type = 'D');
    /**
     * create new histogram
     * @param name
     * @param title
     * @param Xaxis X axis configuration -number of bins, Y - min, Z - max
     * @param Yaxis Y axis configuration -number of bins, Y - min, Z - max
     * @param Zaxis Z axis configuration -number of bins, Y - min, Z - max
     * @param type 'D' for TH1D, 'F' for TH1F, 'I' for TH1I
     * @return
     */
    TH3* MakeHisto3D(TString name, TString title, TVector3 Xaxis, TVector3 Yaxis, TVector3 Zaxis, Char_t type = 'D');
    /**
     * set some global settings of painters
     */
    void MakeBeautiful();
    /**
     * makes folding of histogram around given point, point must be in the middle
     * of bin or in the edge of bin, if point is in the middle of bin then content
     * of such bin is multiplied by 2 and error by sqrt(2) in case of 2 and 3D
     * histograms makes folding around given plane
     * @param val value of folding (if value is not in the middle of axis,
     * assumption of periodical histogram is used
     * @param h histogram to fold
     */
    void Fold1D(Double_t val, TH1& h);
    /**
     * fold histogram around given axis
     * @param val axis value
     * @param h
     * @param opt axis - x or y
     */
    void Fold2D(Double_t val, TH2& h, TString opt = "x");
    /**
     * fold histogram around given axis
     * @param val
     * @param h
     * @param opt
     */
    void Fold3D(Double_t val, TH3& h, TString opt = "x");
    /**
     * set the same color of line and marker for histogram
     * @param color color to set
     * @param h histogram
     */
    void SetColor(TH1& h, Color_t color);
    /**
     * set the same color for line and maker for histogram, set histogram marker
     * @param color color to set
     * @param m marker style to set
     * @param h histogram
     * @param s size
     */
    void SetColorAndMarker(TH1& h, Color_t color, Marker_t m, Size_t s = 1);
    /**
     * set Z-axis for two dim histogram
     * @param h
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    void SetRainbow(TH2& h, Double_t x1, Double_t y1, Double_t x2, Double_t y2);
    /**
     * get index of oposite color
     * @param col
     * @return
     */
    Int_t GetAntiColor(Int_t col);
    /**
     * calculate the number of subpads
     * @param pad
     * @return
     */
    Int_t GetListOfSubPads(TVirtualPad* pad);
    /**
     * make new histogram only with copy of the original histogram
     * @param h
     * @param min
     * @param max
     * @param opt - vals - crop by value, bin - crop by bin
     * @return
     */
    TH1D* Crop1D(const TH1& h, Double_t min, Double_t max, TString opt = "vals");
    /**
     * make new histogram only with copy of the original histogram
     * @param h
     * @param minX
     * @param maxX
     * @param minY
     * @param maxY
     * @param opt - vals - crop by value, bin - crop by bin
     * @return
     */
    TH2D* Crop2D(const TH2& h, Double_t minX, Double_t maxX, Double_t minY, Double_t maxY, TString opt = "vals");
    /**
     * make new histogram only with copy of the original histogram
     * @param h
     * @param minX
     * @param maxX
     * @param minY
     * @param maxY
     * @param minZ
     * @param maxZ
     * @param opt - vals - crop by value, bin - crop by bin
     * @return
     */
    TH3D* Crop3D(const TH3& h,
                 Double_t minX,
                 Double_t maxX,
                 Double_t minY,
                 Double_t maxY,
                 Double_t minZ,
                 Double_t maxZ,
                 TString opt = "vals");
    std::vector<TObject*> GetPadChildren(TString objName, TString className = "", TVirtualPad* pad = nullptr);
  }  // namespace Std
}  // namespace Hal
#endif /* HALSTDHIST_H_ */
