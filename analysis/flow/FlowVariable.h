/*
 lo * FlowVariable.h
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFLOWVARIABLE_H_
#define HALFLOWVARIABLE_H_

#include <TObject.h>
#include <TString.h>

namespace Hal {
  class Track;

  /**
   * abstract class for calculating any flow variable
   */
  class FlowVariable : public TObject {
    TString fName;
    TString fUnit;

  protected:
    void SetFieldName(TString name) { fName = name; }
    void SetFieldUnit(TString unit) { fUnit = unit; }

  public:
    /**
     * default constructor
     * @param axisName axis name (e.g. p_{T}
     * @param unitName unit name
     */
    FlowVariable(TString axisName = "dummy", TString unitName = "dummy");
    /**
     *
     * @return axis name
     */
    virtual TString GetAxisName();
    /**
     *
     * @return unit name
     */
    virtual TString GetAxisUnit();
    /**
     *
     * @param p track
     * @return variable with is plotted of flow histogram
     */
    virtual Double_t GetVariable(Track* p);
    virtual Bool_t Init(Int_t taskId) { return kTRUE; }
    virtual FlowVariable* MakeCopy() const = 0;
    virtual ~FlowVariable();
    ClassDef(FlowVariable, 1)
  };

  /**
   * virtual flow variable, always return 0
   */
  class FlowVirtualVariable : public FlowVariable {
  public:
    FlowVirtualVariable();
    Double_t GetVariable(Track* /*p*/) { return 0; };
    FlowVariable* MakeCopy() const { return new FlowVirtualVariable(); };
    virtual ~FlowVirtualVariable();
    ClassDef(FlowVirtualVariable, 1)
  };

  class FlowTrackFieldVariable : public FlowVariable {
    Int_t fFieldId;

  public:
    /**
     * default constructor
     * @param fieldId one of fields Hal::DataFieldID::kTrack
     */
    FlowTrackFieldVariable(Int_t fieldId = 0);
    Double_t GetVariable(Track* p);
    virtual Bool_t Init(Int_t taskId);
    FlowVariable* MakeCopy() const { return new FlowTrackFieldVariable(*this); };
    virtual ~FlowTrackFieldVariable() {};
    ClassDef(FlowTrackFieldVariable, 1)
  };
}  // namespace Hal
#endif /* HALFLOWVARIABLE_H_ */
