/*
 * HalObjectManager.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALOBJECTMANAGER_H_
#define HALOBJECTMANAGER_H_

#include <TNamed.h>
#include <TString.h>


class Package;

/*************************************************************************************************************************************
 *                  1 DIM ARRAYS
 *************************************************************************************************************************************/
/**
 * template that store 1dim array of 1-dim histograms
 */
namespace Hal {
  class ObjectMatrix_1 : public TNamed {
  protected:
    /**
     * size of array
     */
    Int_t fSize;
    /**
     * array of comments
     */
    TString* fComments;  //[fSize]
    /**
     * array of histograms
     */
    TObject** fArray;  //[fSize]
  public:
    ObjectMatrix_1() : TNamed(), fSize(0), fComments(NULL), fArray(NULL) {};
    ObjectMatrix_1(const ObjectMatrix_1& manager);
    ObjectMatrix_1& operator=(const ObjectMatrix_1& manager);
    /**
     * initialize histograms
     * @param size size of array
     * @param temp - template object that will be cloned
     */
    virtual void Init(Int_t size, const TObject* temp);
    /**
     * return histogram
     * @param i histogram position
     * @return histogra
     */
    inline TObject* At(Int_t i) const { return fArray[i]; };
    /**
     *
     * @return array of histogram
     */
    inline TObject** GetArrray() const { return fArray; };
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
    virtual ~ObjectMatrix_1() {
      if (fSize == 0) return;
      delete[] fComments;
      for (int i = 0; i < fSize; i++)
        delete fArray[i];
      delete[] fArray;
    }
    ClassDef(ObjectMatrix_1, 1)
  };
  /*************************************************************************************************************************************
   *                  2 DIM ARRAYS
   *************************************************************************************************************************************/
  /**
   * class for store 2-dim array of 1-dim histograms
   */

  class ObjectMatrix_2 : public TNamed {
  protected:
    /**
     * size of this array
     */
    Int_t fSize;
    /**
     * array of vector
     */
    ObjectMatrix_1** fArray;  //[fSize]
  public:
    ObjectMatrix_2() : TNamed(), fSize(0), fArray(NULL) {};
    ObjectMatrix_2(const ObjectMatrix_2& manager);
    ObjectMatrix_2& operator=(const ObjectMatrix_2& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param temp - template object that will be cloned
     */
    virtual void Init(Int_t sizeX, Int_t sizeY, const TObject* temp);
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
    ObjectMatrix_1* Get(Int_t i) const { return fArray[i]; };
    /**
     *
     * @param i 1st index
     * @param j 2nd index
     * @return histogram
     */
    inline TObject* At(Int_t i, Int_t j) const { return fArray[i]->At(j); };
    virtual ~ObjectMatrix_2();
    ClassDef(ObjectMatrix_2, 1)
  };
  /*************************************************************************************************************************************
   *                  3 DIM ARRAYS
   *************************************************************************************************************************************/
  /**
   * 3-dim array of 1-dim histogram
   */

  class ObjectMatrix_3 : public TNamed {
  protected:
    Int_t fSize;
    /**
     * array of arrays
     */
    ObjectMatrix_2** fArray;  //[fSize]
  public:
    ObjectMatrix_3() : TNamed(), fSize(0), fArray(NULL) {};
    ObjectMatrix_3(const ObjectMatrix_3& manager);
    ObjectMatrix_3& operator=(const ObjectMatrix_3& manager);
    /**
     * initialize histograms
     * @param sizeX size of array
     * @param sizeY size of array
     * @param sizeZ size of array
     * @param temp - template object that will be cloned
     */
    void Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, const TObject* temp);
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
    ObjectMatrix_2* Get(Int_t i) const { return fArray[i]; };
    /**
     *
     * @param i 1st index in array of histograms
     * @param j 2nd index in array of histograms
     * @param k 3rd index in array of histograms
     * @return histogram
     */
    inline TObject* At(Int_t i, Int_t j, Int_t k) const { return fArray[i]->Get(j)->At(k); };
    virtual ~ObjectMatrix_3();
    ClassDef(ObjectMatrix_3, 1)
  };

  //----------------------------- 4 DIM Arrays
  /**
   * 4dim array of 1-dim histograms
   */

  class ObjectMatrix_4 : public TNamed {
  protected:
    Int_t fSize;
    /**
     * array of arrays
     */
    ObjectMatrix_3** fArray;  //[fSize]
  public:
    ObjectMatrix_4() : TNamed(), fSize(0), fArray(NULL) {};
    ObjectMatrix_4(const ObjectMatrix_4& manager);
    ObjectMatrix_4& operator=(const ObjectMatrix_4& manager);
    /**
     * initialize histograms
     * @param sizeX size of array (1st dim)
     * @param sizeY size of array (2nd dim)
     * @param sizeZ size of array (3rd dim)
     * @param sizeD size of array (4th dim)
     * @param temp - template object that will be cloned
     */
    virtual void Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeT, const TObject* temp);
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
    ObjectMatrix_3* Get(const Int_t i) const { return fArray[i]; };
    /**
     * return object with given indexes
     * @param i 1st index
     * @param j 2nd index
     * @param k f3rd index
     * @param l 4th index
     * @return requested object
     */
    inline TObject* At(Int_t i, Int_t j, Int_t k, Int_t l) const { return fArray[i]->Get(j)->Get(k)->At(l); };
    virtual ~ObjectMatrix_4();
    ClassDef(ObjectMatrix_4, 1)
  };
}  // namespace Hal
#endif /* HALOBJECTMANAGER_H_ */
