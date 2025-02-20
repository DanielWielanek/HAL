/*
 * FemtoFreezeoutGenerator.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOFREEZEOUTGENERATOR_H_
#define HALFEMTOFREEZEOUTGENERATOR_H_

#include "FemtoSourceModel.h"

/**
 * class for creating given coordinates in lab frame
 */
namespace Hal {
  class FemtoFreezeoutGenerator : public TObject {
  protected:
    /**
     * model that generate coordinates in pair frame
     */
    FemtoSourceModel* fModel = {nullptr};
    /**
     * boost coordinates from model to lab and set them to pair
     * @param pair pair than need coordinates
     */
    virtual void Boost(FemtoPair* pair) = 0;

  public:
    FemtoFreezeoutGenerator();
    FemtoFreezeoutGenerator(const FemtoSourceModel& source);
    /**
     * copy ctor
     * @param generator
     */
    FemtoFreezeoutGenerator(const FemtoFreezeoutGenerator& generator);
    /**
     * make copy of this object
     * @return
     */
    virtual FemtoFreezeoutGenerator* MakeCopy() const = 0;
    /**
     * defalut initializer
     */
    Bool_t Init();
    /**
     *
     * @param model used source model
     */
    void SetSourceModel(const FemtoSourceModel& model);
    /**
     *
     * @return source model if present
     */
    FemtoSourceModel* GetSourceModel() const { return fModel; };
    /**
     * generate freezout coordinates and set them to pair
     * @param Pair
     */
    void GenerateFreezeoutCooordinates(FemtoPair* Pair);
    virtual void Print(Option_t* option = "") const;
    /**
     * create package with report
     * @return package with report
     */
    virtual Package* Report() const;
    virtual ~FemtoFreezeoutGenerator();
    ClassDef(FemtoFreezeoutGenerator, 1)
  };
  /**
   * used to boost source according to LCMS frame
   */
  class FemtoFreezeoutGeneratorLCMS : public FemtoFreezeoutGenerator {
  protected:
    void Boost(FemtoPair* pair);

  public:
    FemtoFreezeoutGeneratorLCMS();
    FemtoFreezeoutGeneratorLCMS(const FemtoSourceModel& model);
    /**
     * copy ctor
     * @param generator
     */
    FemtoFreezeoutGeneratorLCMS(const FemtoFreezeoutGeneratorLCMS& generator);
    FemtoFreezeoutGenerator* MakeCopy() const;
    virtual Package* Report() const;
    virtual ~FemtoFreezeoutGeneratorLCMS();
    ClassDef(FemtoFreezeoutGeneratorLCMS, 1)
  };

  /**
   * used to boost source according to PRF frame
   */
  class FemtoFreezeoutGeneratorPRF : public FemtoFreezeoutGenerator {
  protected:
    void Boost(FemtoPair* pair);

  public:
    FemtoFreezeoutGeneratorPRF();
    /**
     * copy ctor
     * @param generator
     */
    FemtoFreezeoutGeneratorPRF(const FemtoFreezeoutGeneratorPRF& generator);
    FemtoFreezeoutGeneratorPRF(const FemtoSourceModel& model);
    FemtoFreezeoutGenerator* MakeCopy() const;
    virtual Package* Report() const;
    virtual ~FemtoFreezeoutGeneratorPRF();
    ClassDef(FemtoFreezeoutGeneratorPRF, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOFREEZOUTGENERATOR_H_ */
