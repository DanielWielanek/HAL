/*
 * HalHistogramManager.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALHISTOGRAMMANAGER_H_
#define HALHISTOGRAMMANAGER_H_

#include <TNamed.h>
#include <TString.h>


/**
 * class for storing information about axis configuration
 */
namespace Hal {
  class Package;
  class HistogramAxisConf : public TNamed {
    Int_t fBins;
    Double_t fMin, fMax;

  protected:
    void SetConf(Int_t bins, Double_t min, Double_t max) {
      fBins = bins;
      fMin  = min;
      fMax  = max;
    }

  public:
    HistogramAxisConf();
    /**
     *
     * @param bins number of bins
     * @param min lower edge of axis
     * @param max upper edge of axis
     */
    HistogramAxisConf(Int_t bins, Double_t min, Double_t max);
    /**
     *
     * @param title axis title
     * @param bins number of bins
     * @param min lower edge of axis
     * @param max upper edge of axis
     */
    HistogramAxisConf(TString title, Int_t bins = 100, Double_t min = 0, Double_t max = 100);
    /**
     *
     * @return number of bins
     */
    Int_t GetNBins() const { return fBins; };
    /**
     *
     * @return minimum of axis
     */
    Double_t GetMin() const { return fMin; };
    /**
     *
     * @return maximum of axis
     */
    Double_t GetMax() const { return fMax; };
    virtual ~HistogramAxisConf() {};
    ClassDef(HistogramAxisConf, 1)

      virtual void Print(Option_t* option = "") const;
  };

  /*************************************************************************************************************************************
   *                  1 DIM ARRAYS
   *************************************************************************************************************************************/
  /**
   * template that store 1dim array of 1-dim histograms
   */
  template<class T>

  class HistogramManager_1_1D : public TObject {
  protected:
    /**
     * size of array
     */
    Int_t fSize;
    /**
     * number of axis
     */
    Int_t fAxisNo;
    /**
     * array of comments
     */
    TString* fComments;  //[fSize]
  public:
    /**
     * array of histograms
     */
    T** fArray;  //[fSize]
    HistogramManager_1_1D() : fSize(0), fAxisNo(0), fComments(NULL), fArray(NULL) {};
    HistogramManager_1_1D(const HistogramManager_1_1D& manager);
    HistogramManager_1_1D<T>& operator=(const HistogramManager_1_1D& manager);
    /**
     * initialize histograms
     * @param size size of array
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t size, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param size size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t size, Int_t binsX, Double_t minX, Double_t maxX, Bool_t sumw = kFALSE);
    /**
     * overwite this histogram, old histogram will be deleted
     * @param
     * @param pos
     */
    void OverwriteAt(T* t, Int_t pos);
    /**
     * return histogram
     * @param i histogram position
     * @return histogra
     */
    inline T* At(Int_t i) const { return fArray[i]; };
    /**
     *
     * @return array of histogram
     */
    inline T** GetArrray() const { return fArray; };
    /**
     * fill histogram
     * @param i histogram index
     * @param val value to fill
     */
    inline void Fill(Int_t i, Double_t val) { fArray[i]->Fill(val); };
    /**
     * fill histogram
     * @param i histogram position
     * @param val value to fill
     * @param weight weight of value
     */
    inline void Fill(Int_t i, Double_t val, Double_t weight) { fArray[i]->Fill(val, weight); };
    /**
     *
     * @return size of this array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @param i histogram index
     * @return comment for histogram
     */
    TString GetComment(Int_t i) { return fComments[i]; };
    /**
     * set comment to histogram
     * @param comment comment
     * @param i histogram position
     */
    void SetComment(TString comment, Int_t i) { fComments[i] = comment; };
    /**
     *
     * @return histogram array as TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    virtual ~HistogramManager_1_1D() {
      delete[] fComments;
      for (int i = 0; i < fSize; i++)
        delete fArray[i];
      delete[] fArray;
    }
    ClassDef(HistogramManager_1_1D, 1)
  };
  /**
   * template that store 1dim array of 2-dim histograms
   */
  template<class T>
  class HistogramManager_1_2D : public TObject {
  protected:
    /**
     * length of vector
     */
    Int_t fSize;
    /**
     * number of axes
     */
    Int_t fAxisNo;
    /**
     * array of comments
     */
    TString* fComments;  //[fSize]
  public:
    /**
     * array with histograms
     */
    T** fArray;  //[fSize]
    HistogramManager_1_2D() : fSize(0), fAxisNo(0), fComments(NULL), fArray(NULL) {};
    HistogramManager_1_2D(const HistogramManager_1_2D& manager);
    HistogramManager_1_2D<T>& operator=(const HistogramManager_1_2D& manager);
    /**
     * initialize histograms
     * @param size size of array
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t size, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * 	@param binsY number of bins
     * @param minY y-min
     * @param maxY y-max
     * @param sumw if true call Sumw
     */
    void
    Init(Int_t sizeX, Int_t binsX, Double_t minX, Double_t maxX, Int_t binsY, Double_t minY, Double_t maxY, Bool_t sumw = kFALSE);
    /**
     * return histogram
     * @param i histogram position
     * @return histogra
     */
    inline T* At(Int_t i) const { return fArray[i]; };
    /**
     *
     * @return array of histogram
     */
    inline T** GetArrray() const { return fArray; };
    /**
     * fill histogram
     * @param i index of histogram
     * @param X value on x
     * @param Y value on y
     */
    inline void Fill(Int_t i, Double_t X, Double_t Y) { fArray[i]->Fill(X, Y); };
    /**
     *  fill histogram with weight
     * @param i index of histogram
     * @param X value on x
     * @param Y value on y
     * @param weight weight of filling
     */
    inline void Fill(Int_t i, Double_t X, Double_t Y, Double_t weight) { fArray[i]->Fill(X, Y, weight); };
    /**
     *
     * @return size of this array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @param i histogram index
     * @return comment for histogram
     */
    TString GetComment(Int_t i) { return fComments[i]; };
    /**
     * overwite this histogram, old histogram will be deleted
     * @param
     * @param pos
     */
    void OverwriteAt(T* t, Int_t pos);
    /**
     * set comment to histogram
     * @param comment comment
     * @param i histogram position
     */
    void SetComment(TString comment, Int_t i) { fComments[i] = comment; };
    /**
     *
     * @return histogram array as TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    virtual ~HistogramManager_1_2D() {
      for (int i = 0; i < fSize; i++) {
        delete fArray[i];
      }
      delete[] fArray;
      delete[] fComments;
    }
    ClassDef(HistogramManager_1_2D, 1)
  };

  /**
   * template that store 1dim array of 3-dim histograms
   */
  template<class T>
  class HistogramManager_1_3D : public TObject {
  protected:
    /**
     * length or array
     */
    Int_t fSize;
    /**
     * axes number
     */
    Int_t fAxisNo;
    /**
     * array of comments
     */
    TString* fComments;  //[fSize]
  public:
    /**
     * array of histograms
     */
    T** fArray;  //[fSize]
    HistogramManager_1_3D() : fSize(0), fAxisNo(0), fComments(NULL), fArray(NULL) {};
    HistogramManager_1_3D(const HistogramManager_1_3D& manager);
    HistogramManager_1_3D<T>& operator=(const HistogramManager_1_3D& manager);
    /**
     * initialize histograms
     * @param size size of array
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t size, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param binsY
     * @param minY  y-min
     * @param maxY y-min
     * @param binsZ
     * @param minZ z-min
     * @param maxZ z-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Double_t minY,
              Double_t maxY,
              Int_t binsZ,
              Double_t minZ,
              Double_t maxZ,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return size of this array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @param i histogram index
     * @return comment for histogram
     */
    TString GetComment(Int_t i) { return fComments[i]; };
    /**
     * set comment to histogram
     * @param comment comment
     * @param i histogram position
     */
    void SetComment(TString comment, Int_t i) { fComments[i] = comment; };
    /**
     *
     * @return histogram array as TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     * return histogram
     * @param i histogram position
     * @return histogram
     */
    inline T* At(const Int_t i) const { return fArray[i]; };
    /**
     *
     * @return array of histogram
     */
    inline T** GetArrray() const { return fArray; };
    /**
     * overwite this histogram, old histogram will be deleted
     * @param
     * @param pos
     */
    void OverwriteAt(T* t, Int_t pos);
    /**
     * fill histogram
     * @param i histogram position
     * @param X x-value
     * @param Y y-value
     * @param Z z-value
     */
    inline void Fill(Int_t i, Double_t X, Double_t Y, Double_t Z) { fArray[i]->Fill(X, Y, Z); };
    /**
     * fill histogram
     * @param i histogram position
     * @param X x-value
     * @param Y y-value
     * @param Z z-value
     * @param weight weight used for filling
     */
    inline void Fill(Int_t i, Double_t X, Double_t Y, Double_t Z, Double_t weight) { fArray[i]->Fill(X, Y, Z, weight); };
    virtual ~HistogramManager_1_3D() {
      for (int i = 0; i < fSize; i++)
        delete fArray[i];
      delete[] fArray;
      delete[] fComments;
    };
    ClassDef(HistogramManager_1_3D, 1)
  };
  /*************************************************************************************************************************************
   *                  2 DIM ARRAYS
   *************************************************************************************************************************************/
  /**
   * class for store 2-dim array of 1-dim histograms
   */
  template<class T>
  class HistogramManager_2_1D : public TObject {
    /**
     * size of this array
     */
    Int_t fSize;
    /**
     * number of axes
     */
    Int_t fAxisNo;

  public:
    /**
     * array of vector
     */
    HistogramManager_1_1D<T>* fArray;  //[fSize]
    HistogramManager_2_1D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_2_1D(const HistogramManager_2_1D& manager);
    HistogramManager_2_1D<T>& operator=(const HistogramManager_2_1D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t sizeX, Int_t sizeY, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX, Int_t sizeY, Int_t binsX, Double_t minX, Double_t maxX, Bool_t sumw = kFALSE);
    /**
     *
     * @return 2nd dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return histograms in flat TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i index
     * @return array of histograms in histogram manager
     */
    HistogramManager_1_1D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     *
     * @param i 1st index
     * @param j 2nd index
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j) const { return fArray[i].fArray[j]; };
    /**
     * fill histogram
     * @param i  1st index of histogram
     * @param j 2nd index of histogram
     * @param X value to fill
     * @param X value to fill
     */
    inline void Fill(Int_t i, Int_t j, Double_t X) { fArray[i].fArray[j]->Fill(X); };
    /**
     * fill given histogram
     * @param i  1st index of histogram
     * @param j 2nd index of histogram
     * @param X value to fill
     * @param weight weight for fill
     */
    inline void Fill(Int_t i, Int_t j, Double_t X, Double_t weight) { fArray[i].fArray[j]->Fill(X, weight); };
    virtual ~HistogramManager_2_1D() { delete[] fArray; };
    ClassDef(HistogramManager_2_1D, 1)
  };
  /**
   * class for store 2-dim array of 2-dim histograms
   */

  template<class T>
  class HistogramManager_2_2D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_1_2D<T>* fArray;  //[fSize]
    HistogramManager_2_2D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_2_2D(const HistogramManager_2_2D& manager);
    HistogramManager_2_2D<T>& operator=(const HistogramManager_2_2D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t sizeX, Int_t sizeY, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms\
     * @param sizeX size of array
     * @param sizeY size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * 	@param binsY number of bins
     * @param minY y-min
     * @param maxY y-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t sizeY,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Int_t minY,
              Int_t maxY,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return 2nd dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return flat list of histograms in TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i index
     * @return array of histograms in histogram manager
     */
    HistogramManager_1_2D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     *
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j) const { return fArray[i].fArray[j]; };
    /**
     * fill histogram
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param X x-value to fill
     * @param Y y-value to fill
     */
    inline void Fill(Int_t i, Int_t j, Double_t X, Double_t Y) { fArray[i].fArray[j]->Fill(X, Y); };
    /**
     * fill histogram
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param X x-value to fill
     * @param Y y-value to fill
     * @param weight weight of filling
     */
    inline void Fill(Int_t i, Int_t j, Double_t X, Double_t Y, Double_t weight) { fArray[i].fArray[j]->Fill(X, Y, weight); };
    virtual ~HistogramManager_2_2D() { delete[] fArray; };
    ClassDef(HistogramManager_2_2D, 1)
  };
  /**
   * class for store 2-dim array of 3-dim histograms
   */
  template<class T>
  class HistogramManager_2_3D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_1_3D<T>* fArray;  //[fSize]
    HistogramManager_2_3D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_2_3D(const HistogramManager_2_3D& manager);
    HistogramManager_2_3D<T>& operator=(const HistogramManager_2_3D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t sizeX, Int_t sizeY, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param binsY
     * @param minY  y-min
     * @param maxY y-min
     * @param binsZ
     * @param minZ z-min
     * @param maxZ z-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t sizeY,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Double_t minY,
              Double_t maxY,
              Int_t binsZ,
              Double_t minZ,
              Double_t maxZ,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return 2nd dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i index
     * @return array of histograms in hitstogram manager
     */
    HistogramManager_1_3D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     *
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @return histogram
     */
    inline T* At(const Int_t i, Int_t j) const { return fArray[i].fArray[j]; };
    /**
     * fill histogram
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param X x-value of fill
     * @param Y y-value of fill
     * @param Z z-value of fill
     */
    inline void Fill(Int_t i, Int_t j, Double_t X, Double_t Y, Double_t Z) { fArray[i].fArray[j]->Fill(X, Y, Z); };
    /**
     * fill histogram
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param X x-value of fill
     * @param Y y-value of fill
     * @param Z z-value of fill
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Double_t X, Double_t Y, Double_t Z, Double_t weight) {
      fArray[i].fArray[j]->Fill(X, Y, Z, weight);
    };
    virtual ~HistogramManager_2_3D() { delete[] fArray; };
    ClassDef(HistogramManager_2_3D, 1)
  };
  /*************************************************************************************************************************************
   *                  3 DIM ARRAYS
   *************************************************************************************************************************************/
  /**
   * 3-dim array of 1-dim histogram
   */
  template<class T>
  class HistogramManager_3_1D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_2_1D<T>* fArray;  //[fSize]
    HistogramManager_3_1D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_3_1D(const HistogramManager_3_1D& manager);
    HistogramManager_3_1D<T>& operator=(const HistogramManager_3_1D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t binsX, Double_t minX, Double_t maxX, Bool_t sumw = kFALSE);
    /**
     *
     * @return 3rd dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     * get sub-barray
     * @param i index
     * @return sub-array
     */
    HistogramManager_2_1D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     *
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param k 3rd index in array of histograms
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j, Int_t k) const { return fArray[i].fArray[j].fArray[k]; };
    /**
     * fill given histogram
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param k f3rd index in array of histograms
     * @param X value
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Double_t X) { fArray[i].fArray[j].fArray[k]->Fill(X); };
    /**
     * fill given histogram
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param k f3rd index in array of histograms
     * @param X value
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Double_t X, Double_t weight) { fArray[i].fArray[j].fArray[k]->Fill(X, weight); };
    virtual ~HistogramManager_3_1D() { delete[] fArray; };
    ClassDef(HistogramManager_3_1D, 1)
  };
  /**
   * 3-dim array of 2-dim histograms
   */
  template<class T>
  class HistogramManager_3_2D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_2_2D<T>* fArray;  //[fSize]
    HistogramManager_3_2D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_3_2D(const HistogramManager_3_2D& manager);
    HistogramManager_3_2D<T>& operator=(const HistogramManager_3_2D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * 	@param binsY number of bins
     * @param minY y-min
     * @param maxY y-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t sizeY,
              Int_t sizeZ,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Int_t minY,
              Int_t maxY,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return 3rd dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i indexs
     * @return sub-array
     */
    HistogramManager_2_2D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     *
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j, Int_t k) const { return fArray[i].fArray[j].fArray[k]; };
    /**
     * fill given histogram
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @param X x-value
     * @param Y y-value
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Double_t X, Double_t Y) { fArray[i].fArray[j].fArray[k]->Fill(X, Y); };
    /**
     * fill given histogram
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @param X x-value
     * @param Y y-value
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Double_t X, Double_t Y, Double_t weight) {
      fArray[i].fArray[j].fArray[k]->Fill(X, Y, weight);
    };
    virtual ~HistogramManager_3_2D() { delete[] fArray; };
    ClassDef(HistogramManager_3_2D, 1)
  };
  /**
   * 3-dim array of 3-dim histograms
   */
  template<class T>
  class HistogramManager_3_3D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_2_3D<T>* fArray;  //[fSize]
    HistogramManager_3_3D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_3_3D(const HistogramManager_3_3D& manager);
    HistogramManager_3_3D<T>& operator=(const HistogramManager_3_3D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histogtrams
     */
    void Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param binsY
     * @param minY  y-min
     * @param maxY y-min
     * @param binsZ
     * @param minZ z-min
     * @param maxZ z-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t sizeY,
              Int_t sizeZ,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Double_t minY,
              Double_t maxY,
              Int_t binsZ,
              Double_t minZ,
              Double_t maxZ,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return 3rd dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i 1st index
     * @return sub-array
     */
    HistogramManager_2_3D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     * return histogram with given indexes
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j, Int_t k) const { return fArray[i].fArray[j].fArray[k]; };
    /**
     * fill histogram with given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param X x-value
     * @param Y y-value
     * @param Z z-value
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Double_t X, Double_t Y, Double_t Z) {
      fArray[i].fArray[j].fArray[k]->Fill(X, Y, Z);
    };
    /**
     * fill histogram with given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param X x-value
     * @param Y y-value
     * @param Z z-value
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Double_t X, Double_t Y, Double_t Z, Double_t weight) {
      fArray[i].fArray[j].fArray[k]->Fill(X, Y, Z, weight);
    };
    virtual ~HistogramManager_3_3D() { delete[] fArray; };
    ClassDef(HistogramManager_3_3D, 1)
  };

  //----------------------------- 4 DIM Arrays
  /**
   * 4dim array of 1-dim histograms
   */
  template<class T>
  class HistogramManager_4_1D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_3_1D<T>* fArray;  //[fSize]
    HistogramManager_4_1D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_4_1D(const HistogramManager_4_1D& manager);
    HistogramManager_4_1D<T>& operator=(const HistogramManager_4_1D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param sizeD size of array (4th dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histogtrams
     */
    void
    Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeD, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * @param sizeT size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param sumw if true call Sumw
     */
    void
    Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeT, Int_t binsX, Double_t minX, Double_t maxX, Bool_t sumw = kFALSE);
    /**
     *
     * @return 4th dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i index
     * @return sub-array
     */
    HistogramManager_3_1D<T>* Get(const Int_t i) const { return &fArray[i]; };
    /**
     * return histogram with given indexes
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @param l 4th index
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j, Int_t k, Int_t l) const { return fArray[i].fArray[j].fArray[k].fArray[l]; };
    /**
     * fill histogram that have given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param l 4th index
     * @param X value to fill
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Int_t l, Double_t X) { fArray[i].fArray[j].fArray[k].fArray[l]->Fill(X); };
    /**
     * fill histogram that have given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param l 4th index
     * @param X value to fill
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Int_t l, Double_t X, Double_t weight) {
      fArray[i].fArray[j].fArray[k].fArray[l]->Fill(X, weight);
    };
    virtual ~HistogramManager_4_1D() { delete[] fArray; };
    ClassDef(HistogramManager_4_1D, 1)
  };
  /**
   * 4-dim array of 2-dim histograms
   */
  template<class T>
  class HistogramManager_4_2D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_3_2D<T>* fArray;  //[fSize]
    HistogramManager_4_2D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_4_2D(const HistogramManager_4_2D& manager);
    HistogramManager_4_2D<T>& operator=(const HistogramManager_4_2D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param sizeD size of array (4th dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histogtrams
     */
    void
    Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeD, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * 	 @param sizeT size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * 	@param binsY number of bins
     * @param minY y-min
     * @param maxY y-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t sizeY,
              Int_t sizeZ,
              Int_t sizeT,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Int_t minY,
              Int_t maxY,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return 4th dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i index
     * @return 3dim array at given index
     */
    HistogramManager_3_2D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     * return histogram with given indexes
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @param l 4th index
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j, Int_t k, Int_t l) const { return fArray[i].fArray[j].fArray[k].fArray[l]; };
    /**
     * fill histogram that have given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param l 4th index
     * @param X x-value to fill
     * @param Y z-value to fill
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Int_t l, Double_t X, Double_t Y) {
      fArray[i].fArray[j].fArray[k].fArray[l]->Fill(X, Y);
    };
    /**
     *
     * fill histogram that have given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param l 4th index
     * @param X x-value to fill
     * @param Y y-value to fill
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Int_t l, Double_t X, Double_t Y, Double_t weight) {
      fArray[i].fArray[j].fArray[k].fArray[l]->Fill(X, Y, weight);
    };
    virtual ~HistogramManager_4_2D() { delete[] fArray; };
    ClassDef(HistogramManager_4_2D, 1)
  };
  /**
   * 4-dim array of 3-dim histograms
   */
  template<class T>
  class HistogramManager_4_3D : public TObject {
    Int_t fSize;
    Int_t fAxisNo;

  public:
    /**
     * array of arrays
     */
    HistogramManager_3_3D<T>* fArray;  //[fSize]
    HistogramManager_4_3D() : fSize(0), fAxisNo(0), fArray(NULL) {};
    HistogramManager_4_3D(const HistogramManager_4_3D& manager);
    HistogramManager_4_3D<T>& operator=(const HistogramManager_4_3D& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param sizeD size of array (4th dim)
     * @param axisconf array of axis configurations
     * @param title title of histograms
     * @param Sumw if true Suwm is called for histograms
     */
    void
    Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeD, HistogramAxisConf** axisconf, TString title, Bool_t Sumw = kFALSE);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * 	@param sizeT size of array
     * @param binsX number of bins
     * @param minX x-min
     * @param maxX x-max
     * @param binsY
     * @param minY  y-min
     * @param maxY y-min
     * @param binsZ
     * @param minZ z-min
     * @param maxZ z-max
     * @param sumw if true call Sumw
     */
    void Init(Int_t sizeX,
              Int_t sizeY,
              Int_t sizeZ,
              Int_t sizeT,
              Int_t binsX,
              Double_t minX,
              Double_t maxX,
              Int_t binsY,
              Double_t minY,
              Double_t maxY,
              Int_t binsZ,
              Double_t minZ,
              Double_t maxZ,
              Bool_t sumw = kFALSE);
    /**
     *
     * @return 4th dimension of array
     */
    Int_t GetSize() const { return fSize; };
    /**
     *
     * @return all histograms in "flat" TList
     */
    TList* GetFlatList() const;
    /**
     *
     * @return package mad from this
     */
    Package* Report() const;
    /**
     *
     * @param i index
     * @return 3dim array at given index
     */
    HistogramManager_3_3D<T>* Get(Int_t i) const { return &fArray[i]; };
    /**
     * return histogram with given indexes
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @param l 4th index
     * @return histogram
     */
    inline T* At(Int_t i, Int_t j, Int_t k, Int_t l) const { return fArray[i].fArray[j].fArray[k].fArray[l]; };
    /**
     *
     * fill histogram that have given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param l 4th index
     * @param X x-value to fill
     * @param Y y-value to fill
     * @param Z z-value to fill
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Int_t l, Double_t X, Double_t Y, Double_t Z) {
      fArray[i].fArray[j].fArray[k].fArray[l]->Fill(X, Y, Z);
    };
    /**
     * fill histogram that have given index
     * @param i 1st index
     * @param j 2nd index
     * @param k 3rd index
     * @param l 4th index
     * @param X x-value to fill
     * @param Y y-value to fill
     * @param Z z-value to fill
     * @param weight weight
     */
    inline void Fill(Int_t i, Int_t j, Int_t k, Int_t l, Double_t X, Double_t Y, Double_t Z, Double_t weight) {
      fArray[i].fArray[j].fArray[k].fArray[l]->Fill(X, Y, Z, weight);
    };
    virtual ~HistogramManager_4_3D() { delete[] fArray; };
    ClassDef(HistogramManager_4_3D, 1)
  };
}  // namespace Hal
#endif /* HALHISTOGRAMMANAGER_H_ */
