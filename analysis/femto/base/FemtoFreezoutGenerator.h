/*
 * FemtoFreezoutGenerator.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOFREEZOUTGENERATOR_H_
#define HALFEMTOFREEZOUTGENERATOR_H_

#include "FemtoSourceModel.h"

/**
 * class for creating given coordinates in lab frame
 */
namespace Hal {
  class FemtoFreezoutGenerator : public TObject {
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
    FemtoFreezoutGenerator();
    FemtoFreezoutGenerator(const FemtoSourceModel& source);
    /**
     * copy ctor
     * @param generator
     */
    FemtoFreezoutGenerator(const FemtoFreezoutGenerator& generator);
    /**
     * make copy of this object
     * @return
     */
    virtual FemtoFreezoutGenerator* MakeCopy() const = 0;
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
    void GenerateFreezoutCooordinates(FemtoPair* Pair);
    virtual void Print(Option_t* option = "") const;
    /**
     * create package with report
     * @return package with report
     */
    virtual Package* Report() const;
    virtual ~FemtoFreezoutGenerator();
    ClassDef(FemtoFreezoutGenerator, 1)
  };
  /**
   * used to boost source according to LCMS frame
   */
  class FemtoFreezoutGeneratorLCMS : public FemtoFreezoutGenerator {
  protected:
    void Boost(FemtoPair* pair);

  public:
    FemtoFreezoutGeneratorLCMS();
    FemtoFreezoutGeneratorLCMS(const FemtoSourceModel& model);
    /**
     * copy ctor
     * @param generator
     */
    FemtoFreezoutGeneratorLCMS(const FemtoFreezoutGeneratorLCMS& generator);
    FemtoFreezoutGenerator* MakeCopy() const;
    virtual Package* Report() const;
    virtual ~FemtoFreezoutGeneratorLCMS();
    ClassDef(FemtoFreezoutGeneratorLCMS, 1)
  };

  /**
   * used to boost source according to PRF frame
   */
  class FemtoFreezoutGeneratorPRF : public FemtoFreezoutGenerator {
  protected:
    void Boost(FemtoPair* pair);

  public:
    FemtoFreezoutGeneratorPRF();
    /**
     * copy ctor
     * @param generator
     */
    FemtoFreezoutGeneratorPRF(const FemtoFreezoutGeneratorPRF& generator);
    FemtoFreezoutGeneratorPRF(const FemtoSourceModel& model);
    FemtoFreezoutGenerator* MakeCopy() const;
    virtual Package* Report() const;
    virtual ~FemtoFreezoutGeneratorPRF();
    ClassDef(FemtoFreezoutGeneratorPRF, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOFREEZOUTGENERATOR_H_ */
