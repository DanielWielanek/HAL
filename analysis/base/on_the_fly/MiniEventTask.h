/*
 * MiniEventTask.h
 *
 *  Created on: 19 lut 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMINIEVENTTASK_H_
#define HALMINIEVENTTASK_H_

#include "CompressionMap.h"
#include "TrackAna.h"

/**
 * creates "minievents from given format and thus - makes possible reduction of
 * memory requirements removes unusued tracks - however don;t modify oryginal
 * traks
 */

namespace Hal {
  class MiniEventTask : public TrackAna {
    CompressionMap fMap;

  protected:
    virtual Task::EInitFlag Init();

  public:
    MiniEventTask();
    virtual void Exec(Option_t* opt);
    virtual ~MiniEventTask();
    ClassDef(MiniEventTask, 1)
  };
}  // namespace Hal
#endif /* HALMINIEVENTTASK_H_ */
