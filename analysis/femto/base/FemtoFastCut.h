/*
 * FemtoFastCut.h
 *
 *  Created on: 25-07-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOFASTCUT_H_
#define HALFEMTOFASTCUT_H_

#include <TObject.h>

namespace Hal {
  class FemtoPair;
  class Package;

  /**
   * abstract class for returnin fast cuts
   */
  class FemtoFastCut : public TObject {
  protected:
    /**
     * number of bins
     */
    Int_t fBins;
    /**
     * lower edges of  bins
     */
    Double_t* fMin;  //[fBins]
    /**
     * upper edges of bins
     */
    Double_t* fMax;  //[fBins]
    /**
     * lower edges of bins used for comparisons
     */
    Double_t* fMinTrue;  //[fBins]
    /**
     * upper edges of bins used for comparisons
     */
    Double_t* fMaxTrue;  //[fBins]
    /**
     * pointer to processed pair
     */
    FemtoPair* fPair;

  public:
    /**
     * default constructor
     */
    FemtoFastCut();
    /**
     * default copy constructor
     * @param copy
     */
    FemtoFastCut(const FemtoFastCut& copy);
    /**
     * make copy of this object
     * @return
     */
    virtual FemtoFastCut* MakeCopy() const = 0;
    /**
     * checking if bins don't overlap
     */
    void Allocate();
    /**
     *
     * @param i bin
     * @return lower edge value
     */
    Double_t GetMin(Int_t i) const { return fMin[i]; };
    /**
     *
     * @param i bin number
     * @return upper egde value
     */
    Double_t GetMax(Int_t i) const { return fMax[i]; };
    /**
     * initaliza this class
     * @param pair pointer to processed femto pair (this pointer cannot be changed
     * during run !
     */
    virtual void Init(FemtoPair* pair);
    /**
     *
     * @return number of bins
     */
    virtual Int_t GetNBins() const { return fBins; };
    /**
     * return bin number for pair
     * @return bin number for pair
     */
    virtual Int_t GetBin() const = 0;
    /**
     *
     * @return bin number for rotated pair
     */
    virtual Int_t GetBinRotated() const = 0;
    /**
     *
     * @return bin number for hemisphere pair
     */
    virtual Int_t GetBinHemisphere() const = 0;
    /**
     *
     * @return bin number for mixed pair
     */
    virtual Int_t GetBinMixed() const { return GetBin(); };
    /**
     *
     * @return bin number for perfect pair
     */
    virtual Int_t GetBinPerfect() const { return GetBin(); };
    /**
     *
     * @return bin number for charged pair
     */
    virtual Int_t GetBinCharged() const { return GetBin(); };
    /**
     * add new bin
     * @param min lower endge of new bin
     * @param max upper edge of new bin
     * @param opt not used
     */
    virtual void AddCutBin(Double_t min, Double_t max, Option_t* opt = " ");
    /**
     * create package
     * @return package with report
     */
    virtual Package* Report() const;
    virtual ~FemtoFastCut();
    ClassDef(FemtoFastCut, 1)
  };
  /**
   * class for pass al pairs of track
   */
  class FemtoFastCutVirtual : public FemtoFastCut {
  public:
    FemtoFastCutVirtual() { AddCutBin(-1, 1); };
    /**
     * copy ctor
     * @param fast
     */
    FemtoFastCutVirtual(const FemtoFastCutVirtual& fast);
    Int_t GetBin() const { return 0; };
    Int_t GetBinRotated() const { return 0; };
    Int_t GetBinHemisphere() const { return 0; };
    virtual FemtoFastCut* MakeCopy() const { return new FemtoFastCutVirtual(*this); }
    virtual ~FemtoFastCutVirtual() {};
    ClassDef(FemtoFastCutVirtual, 1)
  };
  /**
   * basic class for calculating kt bins
   */
  class FemtoFastCutKt : public FemtoFastCut {
  public:
    FemtoFastCutKt();
    /**
     * copy ctor
     * @param fast
     */
    FemtoFastCutKt(const FemtoFastCutKt& fast);
    Int_t GetBin() const;
    Int_t GetBinRotated() const;
    Int_t GetBinHemisphere() const;
    virtual FemtoFastCut* MakeCopy() const { return new FemtoFastCutKt(*this); }
    virtual ~FemtoFastCutKt();
    ClassDef(FemtoFastCutKt, 1)
  };
  /**
   * basic class for calculating put sum bins
   */
  class FemtoFastCutPtSum : public FemtoFastCut {
  public:
    FemtoFastCutPtSum();
    /**
     * copy ctor
     * @param fast
     */
    FemtoFastCutPtSum(const FemtoFastCutPtSum& fast);
    Int_t GetBin() const;
    Int_t GetBinRotated() const;
    Int_t GetBinHemisphere() const;
    virtual FemtoFastCut* MakeCopy() const { return new FemtoFastCutPtSum(*this); }
    virtual ~FemtoFastCutPtSum();
    ClassDef(FemtoFastCutPtSum, 1)
  };
  /**
   * basic class for calculating phi bins
   */
  class FemtoFastCutPhi : public FemtoFastCut {
    Double_t fEventPhi;
    Double_t fGlobalMin;
    Double_t fPhiCorrection;
    Double_t RoundPhi(Double_t val) const;
    void TryRoundRange(Int_t i);

  public:
    FemtoFastCutPhi();
    /**
     * copy ctor
     * @param fast
     */
    FemtoFastCutPhi(const FemtoFastCutPhi& fast);
    Int_t GetBin() const;
    Int_t GetBinRotated() const;
    Int_t GetBinHemisphere() const;
    void Init(FemtoPair* pair);
    /**
     * set reaction plane phi
     * @param phi event reaction plane
     */
    void SetEventPhi(const Double_t phi);
    virtual FemtoFastCut* MakeCopy() const { return new FemtoFastCutPhi(*this); }
    virtual ~FemtoFastCutPhi();
    virtual Package* Report() const;
    ClassDef(FemtoFastCutPhi, 1)
  };
  /**
   * basic class for calculating eta bins
   */

  class FemtoFastCutEta : public FemtoFastCut {
  public:
    FemtoFastCutEta();
    /**
     * copy ctor
     * @param fast
     */
    FemtoFastCutEta(const FemtoFastCutEta& fast);
    Int_t GetBin() const;
    Int_t GetBinRotated() const;
    Int_t GetBinHemisphere() const;
    virtual FemtoFastCut* MakeCopy() const { return new FemtoFastCutEta(*this); }
    virtual ~FemtoFastCutEta();
    ClassDef(FemtoFastCutEta, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOFASTCUTS_H_ */
