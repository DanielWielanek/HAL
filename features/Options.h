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
  /** base class for hold options in some arrays **/
  class OptionArray : public TObject {
    std::vector<std::vector<TString>> fParameters;
    std::vector<TString> fLabels;
    TString GetLabel(Int_t i) const;

  public:
    OptionArray();
    /** register label
     * @param label - label name
     */
    void RegisterLabel(TString label);
    /** add option to array
     * @param option - option to add
     */
    Bool_t Add(TString option);
    /** get label for give option e.g for data:format - return data
     * @param label - label for given option
     * @param no - number of option with given label
     */
    TString GetByLabel(TString label, Int_t no = 0) const;
    /** get label by number e.g if registered labes "A" and "B" GetByLabelNo("B")
     * return 1
     * @param label - label
     * */
    Int_t GetByLabelNo(TString label) const;
    /** default destructor */
    void Print(Option_t* /*opt*/ = "") const;
    virtual ~OptionArray();
    ClassDef(OptionArray, 1)
  };
  /**
   * convert option to int
   */
  class OptionConverter : public TObject {
    std::vector<TString> fNames;
    std::vector<Int_t> fValues;

  public:
    OptionConverter();
    /**
     * convert option to int
     * @param val integer for option
     * @param option option
     * @return true if object found
     */
    Bool_t GetOptionInt(Int_t& val, TString option) const;
    /**
     * find string for given int
     * @param val string
     * @param value integer
     * @return true if found
     */
    Bool_t GetString(TString& val, Int_t value) const;
    /**
     * register option
     * @param option option
     */
    void RegisterOption(TString option);
    /**
     * register option with given int
     * @param option option
     * @param no number
     */
    void RegisterOption(TString option, Int_t no);
    virtual ~OptionConverter();
    ClassDef(OptionConverter, 1)
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
    ClassDef(MainOption, 1)
  };

}  // namespace Hal

#endif /* HALOPTIONS_H_ */
