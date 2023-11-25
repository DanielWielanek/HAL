/*
 * HalDividedHisto1D.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALDIVIDEDHISTO1D_H_
#define HALDIVIDEDHISTO1D_H_

#include "Object.h"

#include <TH2.h>
#include <TH3.h>
#include <TString.h>


/**
 * Basic class for string two histograms numerator and denominator. During using
 * user have two option of adding numerator and denominator. Default option just
 * copy value of pointer, in such case deleting HalDividedHisto don't release
 * memory of all internal histograms, histograms added with option "clone" are
 * copied inside of this class. In such case HalDividedHisto delete those
 * histograms if delete was called or if numerator/denominator is overwritten by
 * new histogram.
 */
namespace Hal {
  class DividedHisto1D : public Object {
  private:
    /**
     * true if numerator was added by cloning
     */
    Bool_t fNumIsCloned;
    /**
     * true if denominator was added by cloning
     */
    Bool_t fDenIsCloned;

  protected:
    /**
     * numerator histogram
     */
    TH1* fNum;
    /**
     * denominator histogram
     */
    TH1* fDen;
    /**
     * if true then Histo histogram is divided with "B" option
     */
    Bool_t fBinomial;
    /**
     * number of histogram dimensions
     */
    const Int_t fDim;
    /**
     * array of lower edges of ranges for normalization
     */
    Double_t* fNormMin;  //[fDim]
    /**
     * array of upper edges of ranges for normalization
     */
    Double_t* fNormMax;  //[fDim]
    /**
     * scale, if 0 - then scale is not used
     */
    Double_t fScale;
    /**
     * name of axis for divided histogram
     */
    TString fAxisName;
    /**
     * comment to this object
     */
    TString fComment;
    /**
     * labels -additional information about this object
     */
    TList* fLabels;
    /**
     * @param name name
     * @param dim number of dimensions of histograms
     */
    DividedHisto1D(TString name, const Int_t dim);
    /**
     * make folding around given axis
     * @param val value of folding
     * @param axis axis label (x,y or z)
     */
    virtual void Folding1D(Double_t val, Option_t* axis);
    /**
     * make normalization for histogram
     * @param h histogram for normalization
     */
    virtual void Normalize(TH1* h) const;
    /**
     * make common part of HTML report for all derived classes
     * @param counter used for numerating objects in the same directory
     * @param dir palce where report should be extracted
     * @return linkt to report
     */
    virtual TString CommonExtract(Int_t counter, TString dir) const;
    /**
     * set titles for histogram axis
     * @param h histogram to set titles on axis
     */
    virtual void SetHistoName(TH1* h) const;
    /**
     * return line with pictures that show this class
     */
    virtual TString GetPic() const;

  public:
    /**
     * default constructor - should not be used!
     */
    DividedHisto1D();
    /**
     * constructor with names - note histograms are not allocated!
     * @param name,
     */
    DividedHisto1D(TString name) : DividedHisto1D(name, 1) {};
    /**
     * main constructor
     * @param name name of this object
     * @param nbins number of bins
     * @param min min-x
     * @param max max-x
     * @param type type - 'D' for TH1D, 'I' for TH1I etc.
     */
    DividedHisto1D(TString name, Int_t nbins, Double_t min, Double_t max, Char_t type = 'D');
    /**
     * copy constructor
     * @param other object to copy
     */
    DividedHisto1D(const DividedHisto1D& other);
    /**
     * operator =
     * @param other
     * @return
     */
    DividedHisto1D& operator=(const DividedHisto1D& other);
    /**
     * set scale for normalization if 0 - then scale is not used
     * @param scale scale
     */
    void SetScale(Double_t scale);
    /**
     * set name for axis of divided histogram
     * @param name axis name
     */
    void SetAxisName(TString name);
    /**
     *
     * @return numerator
     */
    TH1* GetNum() const;
    /**
     *
     * @return denominator
     */
    TH1* GetDen() const;
    /**
     *
     * @param normalized if true then normalization is performed
     * @return numerator divide by denominator
     */
    TH1* GetHist(Bool_t normalized = kTRUE) const;
    /**
     * set ownership of numerator
     * @param own
     */
    void SetOwnerNum(Bool_t own) { fNumIsCloned = own; };
    /**
     * set ownership of numerator
     * @param own
     */
    void SetOwnerDen(Bool_t own) { fDenIsCloned = own; };
    /**
     * set owhership of numerator and denominator
     * @param own
     */
    void SetOwner(Bool_t own) { fNumIsCloned = fDenIsCloned = own; };
    /**
     * set normalization range, if scale!=0 then those settings are not used.
     * Algorith try to normalize divied histogra to one at given range
     * @param min lower edge of area
     * @param max upper edge of area
     * @param axis axis id - for x,y,z, respectively 0,1,2
     */
    void SetNorm(Double_t min, Double_t max, Int_t axis = 0);
    /**
     * add label
     * @param label label
     */
    void AddLabel(TString label);
    /**
     * if true then option "B" is used when histogram is obtained
     * @param binomial
     */
    void SetBinomial(Bool_t binomial);
    /**
     * set comment
     * @param comment comment
     */
    void SetComment(TString comment);
    /**
     * fill numerator histogram
     * @param x value to fill
     * @param weight weight
     */
    void FillNum(Double_t x, Double_t weight) { fNum->Fill(x, weight); };
    /**
     *
     * @param x value to fill
     * @param weight weight
     */
    void FillDen(Double_t x, Double_t weight) { fDen->Fill(x, weight); };
    /**
     *
     * @param num numerator
     * @param opt option of adding if equal to "" then histogram is added by
     * copying pointer if "clone" then histogram is cloned into this object
     */
    virtual void AddNum(TH1* num, Option_t* opt = "");
    /**
     *
     * @param den denominator
     * @param opt option of adding if equal to "" then histogram is added by
     * copying pointer if "clone" then histogram is cloned into this object
     */
    virtual void AddDen(TH1* den, Option_t* opt = "");
    /**
     * add numerator and denominator
     * @param num numerator
     * @param den denominator
     * @param opt option of adding if equal to "" then histogram is added by
     * copying pointer if "clone" then histogram is cloned into this object
     */
    virtual void AddNumDen(TH1* num, TH1* den, Option_t* opt = "");
    /**
     * add num/den according to scale
     * @param other
     * @param scale
     */
    virtual void AddScaled(const DividedHisto1D& other, Double_t scale = 1);
    /**
     * makes folding of histogram around given point, point must be in the middle
     * of bin or in the edge of bin, if point is in the middle of bin then content
     * of such bin is multiplied by 2 and error by sqrt(2) in case of 2 and 3D
     * histograms makes folding around given plane
     * @param val value of folding (if value is not in the middle of axis,
     * assumption of periodical histogram is used
     * @param opt option should contain axis label (x,y or z)
     */
    void Fold1D(Double_t val, Option_t* opt = "x");
    /**
     * rebin numerator and/or denominator
     * @param ngroup
     * @param opt if "num" only numerator is rebined, if "den" only denominator is
     * rebined, otherwise both
     */
    virtual void Rebin(Int_t ngroup, Option_t* opt);
    /**
     * add object of the same class
     * @param h another object of the same class
     */
    virtual void Add(const Object* h);
    /**
     * used to draw histograms
     * @param b
     */
    virtual void Browse(TBrowser* b);
    /**
     * fill numerator
     * @param obj
     */
    virtual void FillNumObj(TObject* /*obj*/) {};
    /**
     * fill denominator
     * @param obj
     */
    virtual void FillDenObj(TObject* /*obj*/) {};
    virtual void Print(Option_t* opt) const;
    /**
     *
     * @return number of histogram dimensions
     */
    Int_t GetNDim() const { return fDim; };
    /**
     *
     * @return labels no
     */
    Int_t GetLabelsNo() const;
    /**
     *
     * @return true if division is made with binomial
     */
    Bool_t IsBinomial() const { return fBinomial; };
    /**
     *
     * @return true if owns denominator
     */
    Bool_t IsDenOwner() const { return fDenIsCloned; };
    /**
     *
     * @return true if owner numerator
     */
    Bool_t IsNumOwner() const { return fNumIsCloned; };
    /**
     *
     * @return if own numerator and denominator
     */
    Bool_t IsOwner() const { return (fNumIsCloned && fDenIsCloned); };
    /**
     *
     * @param no axis no
     * @return lower value of normalization range
     */
    Double_t GetNormMin(Int_t no = 0) const;
    /**
     *
     * @param no axis no
     * @return upper value of normalization rage
     */
    Double_t GetNormMax(Int_t no = 0) const;
    /**
     *
     * @return scale used for normalization
     */
    Double_t GetScale() const;
    /**
     *  calucalte denominator/numerator ratio at given range
     * @param min lower norm range
     * @param max upper norm range
     * @return
     */
    Double_t CalculateNorm(Double_t min, Double_t max) const;
    /**
     * used to merge similar objects
     * @param collection
     */
    virtual Long64_t Merge(TCollection* collection);
    /**
     *
     * @return name of axis for divied histogram
     */
    TString GetAxisName() const { return fAxisName; };
    /**
     *
     * @return array with list of labels
     */
    TList* GetLabelList() const { return fLabels; };
    /**
     *
     * @param i label number
     * @return label at i-th position
     */
    TString GetLabel(Int_t i) const;
    /**
     * not used
     * @return
     */
    TString GetExtractType() const;
    /**
     *
     * @return comment
     */
    TString GetComment() const { return fComment; };
    /**
     * draw this object
     * @param opt if "num" then only numerator is drawn, in "den" if "all" draw
     * all histograms m, otherwise draw divided histogram
     */
    virtual void Draw(Option_t* opt = "all");
    /**
     * crates htm repot
     * @param counter counter of this object
     * @param dir place where extract report
     * @return relative address
     */
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~DividedHisto1D();
    ClassDef(DividedHisto1D, 4)
  };
  /**
   * class for storing divided 2-dim histograms
   */
  class DividedHisto2D : public DividedHisto1D {
  protected:
    virtual void Normalize(TH1* h) const;
    virtual void SetHistoName(TH1* h) const;
    /**
     * called by @see Fold2D
     * @param valX value on first axis
     * @param valY value on second axis
     * @param opt option can be "xy" for 2D, for 3D it's say witch plane is used
     * (e.g. can be "yz")
     */
    virtual void Folding2D(Double_t valX, Double_t valY, Option_t* opt);
    virtual void Folding1D(Double_t val, Option_t* axis);
    /**
     * to used only with 3D constructors
     * @param name
     * @param dim
     */
    DividedHisto2D(TString name, const Int_t dim) : DividedHisto1D(name, dim) {};
    /**
     * return projection
     * @param min projection lower range
     * @param max projection upper range
     * @param opt if "num" - then projection of numerator is returned, if "den"
     * projection of denominator is returned, if option don't contain "num" or
     * "den"  then projection of divided function is returned, if "bins" then min
     * and max are treated as number of bins for projection if "scale" is
     * specified, then projection is divided by number of bins in projection
     * range, if "y" is specified the projection is made along y axis, otherwise
     * along x axis.
     * @return projection
     */
    TH1D* Projection2DTo1D(Double_t min, Double_t max, Option_t* opt) const;

  public:
    /**
     * default constructor should not be used
     */
    DividedHisto2D();
    /**
     * constructor without allocation of histograms
     * @param name
     */
    DividedHisto2D(TString name) : DividedHisto1D(name, 2) {};
    /**
     * main constructor
     * @param name name of this object
     * @param binsX number of x-bins
     * @param minX min-x
     * @param maxX max-x
     * @param binsY number of bins on Y
     * @param minY min Y
     * @param maxY max Y
     * @param type type eg. 'D" for TH2D 'I" for TH2I etc.
     */
    DividedHisto2D(TString name,
                   Int_t binsX,
                   Double_t minX,
                   Double_t maxX,
                   Int_t binsY,
                   Double_t minY,
                   Double_t maxY,
                   Char_t type = 'D');
    /**
     * copy constructor
     * @param other
     */
    DividedHisto2D(const DividedHisto2D& other);
    /**
     * for 2D histograms makes folding around point if "xy" options is specified,
     * if options is equal to "z" then Fol1D(valX,"x") and Fold1D(valY,"y") is
     * called (@see Fold1D ) for 3D histogram makes one of following operations -
     * if option has one char then folding around two planes is done like for 2D
     * e.g. Fold2D("z") means Fold1D(valX,"x") + Fold1D(valY,"y")
     * Fold2D("x") means Fold1D(valX,"y") + Fold1D(valY,"z)
     * Fold2D("y") means Fold1D(valX,"x") + Fold(valY,"z")
     *  if options has two chars then folding around axis is made e.g.
     * Fold2D("xy") means folding around z axis
     * @param valX value on first axis
     * @param valY value on second axis
     * @param opt option can be "xy" or "z" for 2D, for 3D it's say witch axis is
     * used (e.g. can be "yz") or how two foldins should be made
     */
    void Fold2D(Double_t valX, Double_t valY, Option_t* opt = "xy");
    /**
     * fill numerator
     * @param x x - value
     * @param y y - value
     * @param weight weight
     */
    void FillNum(Double_t x, Double_t y, Double_t weight) { ((TH2*) fNum)->Fill(x, y, weight); };
    /**
     * fill denominator
     * @param x x - value
     * @param y y - value
     * @param weight weight
     */
    void FillDen(Double_t x, Double_t y, Double_t weight) { ((TH2*) fDen)->Fill(x, y, weight); };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    /**
     * rebin numerator and/or denominator
     * @param ngroup
     * @param opt if "num" only numerator is rebined, if "den" only denominator is
     * rebined, otherwise both, if "x" or "y"  are used then rebin is performed
     * only on given axis
     */
    virtual void Rebin(Int_t ngroup, Option_t* opt);
    /**
     * return projection
     * @param min projection lower range
     * @param max projection upper range
     * @param opt if "num" - then projection of numerator is returned, if "den"
     * projection of denominator is returned, if option don't contain "num" or
     * "den"  then projection of divided function is returned, if "bins" then min
     * and max are treated as number of bins for projection if "scale" is
     * specified, then projection is divided by number of bins in projection
     * range, if "y" is specified the projection is made along y axis, otherwise
     * along x axis.
     * @return projection
     */
    TH1D* GetProjection1D(Double_t min, Double_t max, Option_t* opt) const { return Projection2DTo1D(min, max, opt); }
    /**
     *  calucalte denominator/numerator ratio at given range
     * @param min lower norm range
     * @param max upper norm range
     * 	 @param minY lower norm range
     * @param maxY upper norm range
     * @return
     */
    Double_t CalculateNorm(Double_t minX, Double_t maxX, Double_t minY, Double_t maxY) const;
    virtual void Browse(TBrowser* b);
    /**
     * operator =
     * @param other
     * @return
     */
    DividedHisto2D& operator=(const DividedHisto2D& other);
    ~DividedHisto2D();
    ClassDef(DividedHisto2D, 2)
  };
  /**
   * array for storing divided 3-dim histograms
   */
  class DividedHisto3D : public DividedHisto2D {
  protected:
    void Normalize(TH1* h) const;
    virtual void SetHistoName(TH1* h) const;
    virtual void Folding1D(Double_t val, Option_t* axis);
    virtual void Folding2D(Double_t valX, Double_t valY, Option_t* opt);
    /**
     *
     * @param min1 mimum value for first axis
     * @param max1 maximum value for first axis
     * @param min2 minimum value for second axis
     * @param max2 maximum value for second axis
     * @param opt option, for specify object is allowed to use "num" or "den" (num
     * - for numerator, den for denominator,if none of them specified then result
     * of dividing is returned). If user specifiy "bins" option then projection is
     * made by min an max values but they specify projection by bin number not
     * values on axes. Option "x" "y" "z" can be used for specify axis. If "scale"
     * then histogram is divided by number of histogram in projection range.
     * @return projection
     */
    virtual TH1D* Projection3DTo1D(Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const;

  public:
    /**
     * default constructor  should not be used
     * @param name
     */
    DividedHisto3D();
    /**
     * constructor without alloction of histograms
     * @param name name
     */
    DividedHisto3D(TString name) : DividedHisto2D(name, 3) {};
    /**
     *
     * @param name
     * @param binsX
     * @param minX
     * @param maxX
     * @param binsY
     * @param minY
     * @param maxY
     * @param binsZ
     * @param minZ
     * @param maxZ
     * @param type type of histogram 'D' for TH3D 'I' for TH3I etc
     */
    DividedHisto3D(TString name,
                   Int_t binsX,
                   Double_t minX,
                   Double_t maxX,
                   Int_t binsY,
                   Double_t minY,
                   Double_t maxY,
                   Int_t binsZ,
                   Double_t minZ,
                   Double_t maxZ,
                   Char_t type = 'D');
    /**
     * copy constructor
     * @param other
     */
    DividedHisto3D(const DividedHisto3D& other);
    /**
     * fill numerator
     * @param x x - value
     * @param y y - value
     * @param z z -value
     * @param weight weight
     */
    void FillNum(Double_t x, Double_t y, Double_t z, Double_t weight) { ((TH3*) fNum)->Fill(x, y, z, weight); };
    /**
     * fill denominator
     * @param x x - value
     * @param y y - value
     * @param z z-value
     * @param weight weight
     */
    void FillDen(Double_t x, Double_t y, Double_t z, Double_t weight) { ((TH3*) fDen)->Fill(x, y, z, weight); };
    /**
     *
     * @param min1 mimum value for first axis
     * @param max1 maximum value for first axis
     * @param min2 minimum value for second axis
     * @param max2 maximum value for second axis
     * @param opt option, for specify object is allowed to use "num" or "den" (num
     * - for numerator, den for denominator,if none of them specified then result
     * of dividing is returned). If user specifiy "bins" option then projection is
     * made by min an max values but they specify projection by bin number not
     * values on axes. Option "x" "y" "z" can be used for specify axis. If "scale"
     * then histogram is divided by number of histogram in projection range.
     * @return projection
     */
    TH1D* GetProjection1D(Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const {
      return Projection3DTo1D(min1, max1, min2, max2, opt);
    }
    /**
     * rebin numerator and/or denominator
     * @param ngroup
     * @param opt if "num" only numerator is rebined, if "den" only denominator is
     * rebined, otherwise both, if "x" "y" or "z" are used then rebin is performed
     * only on given axis
     */
    virtual void Rebin(Int_t ngroup, Option_t* opt);
    /**
     * do folding of histograms
     * @param valX value of X to fold around it
     * @param valY value of Y to fold around it
     * @param valZ value of Z to fold around it
     * @param opt option of folding if "xyz" then folding around point is done,
     * otherwise Fold1D(valX,"x") Fold1D(valY,"y") and Fold1D("valZ,"z") are made
     * @see Fold1D
     */
    virtual void Fold3D(Double_t valX, Double_t valY, Double_t valZ, Option_t* opt);
    /**
     * return projection
     * @param min projection lower range
     * @param max projection upper range
     * @param opt if "num" - then projection of numerator is returned, if "den"
     * projection of denominator is returned, if option don't contain "num" or
     * "den"  then projection of divided function is returned, if "bins" then min
     * and max are treated as number of bins for projection. If "scale" is
     * specified, then projection is divided by number of bins in projection
     * range, you can also specify projection direction (@see
     * Hal::Std::GetProjection2D).
     * @return projection
     */
    virtual TH2D* GetProjection2D(Double_t min, Double_t max, Option_t* opt) const;
    /**
     *  calucalte denominator/numerator ratio at given range
     * @param minX lower norm range
     * @param maxX upper norm range
     * @param minY lower norm range
     * @param maxY upper norm range
     * @param minZ lower norm range
     * @param maxZ upper norm range
     * @return
     */
    Double_t CalculateNorm(Double_t minX, Double_t maxX, Double_t minY, Double_t maxY, Double_t minZ, Double_t maxZ) const;
    /**
     * operator =
     * @param other
     * @return
     */
    DividedHisto3D& operator=(const DividedHisto3D& other);
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    ~DividedHisto3D();
    ClassDef(DividedHisto3D, 3)
  };
}  // namespace Hal
#endif /* HALDIVIDEDHISTO1D_H_ */
