/*
 * SubCut.h
 *
 *  Created on: 10 gru 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSUBCUT_H_
#define HALSUBCUT_H_

#include <TObject.h>
#include <TString.h>


class TH1;

/**
 * small class for storing cut parameters
 */
namespace Hal {
  class Cut;
  class Package;
  class SubCut : public TObject {
    Int_t fSize;
    Double_t* fMin;      //[fSize]
    Double_t* fMax;      //[fSize]
    Double_t* fValue;    //[fSize]
    TString* fUnitName;  //[fSize]
  public:
    /**
     * constructor
     * @param size default sub-cut size
     */
    SubCut(Int_t size = 1);
    /**
     * set name of  cut unit on given parameter
     * @param name unit name
     * @param i parameter numer
     */
    void SetUnitName(TString name, Int_t i = 0) { fUnitName[i] = name; };
    /**
     * set minimum of given sub-cut parameter
     * @param val minimum  (axis)
     * @param i parameter no
     */
    void SetMin(Double_t val, Int_t i = 0);
    /**
     * set maximum  of given sub-cut parameter (axis)
     * @param val maximum
     * @param i parameter no
     */
    void SetMax(Double_t val, Int_t i = 0);
    /**
     * check values
     * @return return true if  values for all axes are between fMin and fMax
     */
    Bool_t Validate();
    /**
     * check values
     * @return return true if  absolute values for all axes are between fMin and
     * fMax
     */
    Bool_t ValidateAbs();
    /**
     *
     * @param i axis (parameter no)
     * @return unit name
     */
    TString GetUnit(Int_t i) const { return fUnitName[i]; };
    /**
     * operator =
     * @param other
     * @return
     */
    SubCut& operator=(const SubCut& other);
    /**
     *
     * @return number of paramters in this subcut
     */
    inline Int_t GetSize() const { return fSize; };
    /**
     * set value of given parameter
     * @param val value
     * @param i parameter no
     */
    inline void SetValue(Double_t val, Int_t i = 0) { fValue[i] = val; };
    /**
     * return lower edge of cut for given parameter
     * @param i parameter no
     * @return lower edge value
     */
    inline Double_t GetMin(Int_t i) const { return fMin[i]; };
    /**
     * return upper edge of cut for given parameter
     * @param i parameter no
     * @return upper edge value
     */
    inline Double_t GetMax(Int_t i) const { return fMax[i]; };
    /**
     * return latest value put by SetValue with given parameter number
     * @param i parameter no
     * @return value
     */
    inline Double_t GetValue(Int_t i) const { return fValue[i]; };
    /**
     * copy constructor
     * @param other
     */
    SubCut(const SubCut& other);
    virtual ~SubCut();
    ClassDef(SubCut, 1)
  };

  /**
   * Similar helper like SubCut but instead of parameters - histogram of
   * acceptance is used
   */
  class SubCutHisto : public TObject {
    Int_t fSize;
    TH1* fAcceptanceHistogram;
    Int_t fParX;
    Int_t fParY;
    Int_t fParZ;

  public:
    /**
     * constructor
     * @param size default sub-cut size
     */
    SubCutHisto(Int_t size = 3);
    /**
     *
     * @return number of checked values
     */
    Int_t GetSize() const { return fSize; };
    /**
     * set acceptance histogram
     * @param h histogram with acceptance, shoud have the same numerf of dimesions
     * like number fSize, values should be between 0 and 1 and describe probablity
     * of accepting/rejecting object
     * @return false if histogram cannot be used here
     */
    Bool_t SetAcceptanceHistogram(const TH1& h);
    /**
     * check values
     * @return return true if  values for all axes are between fMin and fMax
     */
    Bool_t Validate(Double_t x, Double_t y = 0, Double_t z = 0);
    /**
     * check values
     * @return return true if  absolute values for all axes are between fMin and
     * fMax
     */
    Bool_t ValidateAbs(Double_t x, Double_t y = 0, Double_t z = 0);
    /**
     * operator =
     * @param other
     * @return
     */
    SubCutHisto& operator=(const SubCutHisto& other);
    /**
     *
     * @return accpetance histogram
     */
    TH1* GetHisto() const { return fAcceptanceHistogram; };
    /**
     * check if everything is ok
     * @param thisCut reference to this cut
     * @param par1 ID of first checked parameter in thisCut object
     * @param par2 ID of second checked parameter in thisCut object
     * @param par3 ID of third checked parameter in thisCut object
     * @return tru if this is ok
     */
    Bool_t Init(const Cut& thisCut, Int_t par1, Int_t par2 = -1, Int_t par3 = -1);
    /**
     * copy constructor
     * @param other
     */
    SubCutHisto(const SubCutHisto& other);
    /**
     * add this object to cut report
     */
    void AddToReport(Package* report) const;
    virtual ~SubCutHisto();
    ClassDef(SubCutHisto, 1)
  };

  /**
   * class for accepting objects in rectangle-like ranges
   */
  class SubCutRectangle : public TObject {
    std::vector<Double_t> fMinX;
    std::vector<Double_t> fMaxX;
    std::vector<Double_t> fMinY;
    std::vector<Double_t> fMaxY;
    TString fNameX;
    TString fNameY;
    Int_t fParX;
    Int_t fParY;

  public:
    SubCutRectangle();
    void AddSquare(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax);
    /**
     * check if everything is ok
     * @param thisCut reference to this cut
     * @param par1 ID of first checked parameter in thisCut object
     * @param par2 ID of second checked parameter in thisCut object
     * @return tru if this is ok
     */
    Bool_t Init(const Cut& thisCut, Int_t par1, Int_t par2 = 1);
    /**
     * @param x
     * @param y
     * @return number of squares that cover object range
     */
    Int_t Validate(Double_t x, Double_t y);
    /**
     * return number of polygons
     */
    Int_t GetAreasNo() { return fMinX.size(); };
    /**
     * add this object to cut report
     */
    void AddToReport(Package* report) const;
    virtual ~SubCutRectangle();
    ClassDef(SubCutRectangle, 1);
  };
}  // namespace Hal

#endif /* NICASUBCUT_H_ */
