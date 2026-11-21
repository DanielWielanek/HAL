/*
 * HalOptions.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALOPTIONS_H_
#define HALOPTIONS_H_

#include "Cout.h"

#include <TClonesArray.h>
#include <TObject.h>

// array that recognize labels

namespace Hal {
  /**
   * class for handling options (except cut options)
   */
  class Options : public TObject {
  public:
    struct LabeledArray {
      TString name = {""};
      std::vector<double> values;
    };
    struct Flag {
      TString flag  = {""};
      TString value = {""};
    };

  private:
    std::vector<LabeledArray> fLabeledArray;
    std::vector<Flag> fEqualExpr;
    std::vector<TString> fOptions;
    TString fOriginalOpt;
    void ParseBrackets(TString bracket);

  public:
    /**
     * default constructor
     * convetion:
     * options are separeted by + sign, there are following options:
     * * pure options e.g. "a+b" are two pure options "a" and "b"
     * * flag options e.g. "a=b" is a flag with value "b"
     * * labeled array e.g. "leg={0,0,3,4}" or "{leg=0,0,3,4} are two "leg" arrays with values 0,0,3,4
     * Note: this class parse all options but when use gettero to check option - only first option is set therefore e.g.
     * "a=b+a=c" return flag "a" equal to "b"
     * @param option
     */
    Options(TString option = "");
    /**
     *
     * @param flag
     * @return true if option is present
     */
    Bool_t HasOption(TString flag) const;
    /**
     *
     * @param flag
     * @return true if !option is present
     */
    Bool_t HasNotOption(TString flag) const;
    /**
     * return flag value e.g. for word=val return val
     * @param flag
     * @return "" if flag not found
     */
    TString GetFlagValue(TString flag) const;
    /**
     * return labeled array if found
     * @param name
     * @return emtpy arrray if not found
     */
    LabeledArray GetLabeledArray(TString name) const;
    virtual void Print(Option_t* option = "") const;
    /**
     *
     * @return original option created in constructor
     */
    TString GetOriginalOption() const { return fOriginalOpt; }
    virtual ~Options() {};
    ClassDef(Options, 0)
  };

  /**
   * class for parsing arguments of main
   */
  class MainOption : public TObject {
    std::vector<TString> fArgs;
    std::vector<std::pair<TString, TString>> fParams;

  public:
    /**
     * default ctor, takes arguments from main
     * @param argc
     * @param argv
     */
    MainOption(int argc = 0, char* argv[] = nullptr);
    /**
     * return array of parameters par - first vale is parameter name, second is parameter value
     * e.g. --n=5 means parameter name = "n", parameter value = "5"
     * @return
     */
    std::vector<std::pair<TString, TString>> GetPars() const { return fParams; }
    /**
     *
     * @return array of arguments, argument is everything that doesn't starts with "-"
     */
    std::vector<TString> GetArguments() const { return fArgs; }
    /**
     * return parameter value
     * @param par parameter name
     * @return
     */
    TString GetParameterValue(TString par) const;
    /**
     *
     * @param opt
     * @return true if parameter opt is present
     */
    Bool_t HaveParameter(TString opt) const;
    virtual void Print(Option_t* option = "") const;
    virtual ~MainOption() {};
    ClassDef(MainOption, 0)
  };

}  // namespace Hal

#endif /* HALOPTIONS_H_ */
