/*
 * EventCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 */

#ifndef HALEVENTCUT_H_
#define HALEVENTCUT_H_

#include "Cut.h"
namespace Hal {
  class Event;

  /** abstract class for event cuts **/
  class EventCut : public Cut {
  public:
    /** default constructor **/
    EventCut(const Int_t i = 1);
    /** create package **/
    virtual Package* Report() const;
    /** return true if event pass cut, false otherwise, should be overwittern in
     * subclass
     * *@param event - event to check**/
    virtual Bool_t Pass(Event* event) = 0;
    /** default destructor **/
    virtual ~EventCut();
    ClassDef(EventCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTCUT_H_ */
