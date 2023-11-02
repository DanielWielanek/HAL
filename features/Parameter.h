/*
 * HalParameter.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 */

#ifndef HALNAMEDVALUE_H_
#define HALNAMEDVALUE_H_

#include "Object.h"

#include <TString.h>

/**
 * abstract for storing parameters, upgraded TParameter known from root
 */

class TBrowser;
namespace Hal {
  class Parameter : public Object {
  protected:
    /**
     * merging option
     */
    enum class EParameterMerge {
      kPlus,      //!< kPlus
      kMultiply,  //!< kMultiply
      kEqual,     //!< kEqual
      kMaximum,   //!< kMaximum
      kMinimum,   //!< kMinimum
      kFirst,     //!< kFirst
      kLast,      //!< kLast
      kAverage    //!< kAverage
    };
    /**
     * mergin mode for this parameter
     */
    EParameterMerge fMergeMode;

  public:
    /**
     * default constructor shouldn't be used
     */
    Parameter();
    /**
     *  main constructor
     * @param name name of parameter
     * @param merge_mode mode of merging
     */
    Parameter(TString name, Char_t merge_mode = '=');
    /**
     * set merge mode
     * @param mergemode one of following modes '+' for adding '=' if merged nodes
     * must have similar values, other describe how value to this node (X) will be
     * set according to value from added node (Y)
     * '*' X=X*Y , 'f' X=X, 'l' X=Y, M X = max(X,Y), m X = min(X,Y), 'a' X =
     * (X+Y)/2
     */
    void SetMergeMode(Char_t mergemode);
    /**
     *
     * @param object other object the same class
     * @return true if both objects have the same name
     */
    Bool_t CheckName(const TObject* object);
    /**
     * used to merge similar objects
     * @param collection
     */
    Long64_t Merge(TCollection* collection);
    /**
     * used in TBrowser
     * @param b
     */
    void Browse(TBrowser* b);
    /**
     * extract this into HTML by return formated value of parameter
     * @param counter - not used
     * @param dir - not used
     * @return formated value
     */
    virtual TString HTMLExtract(Int_t /*counter*/ = 0, TString /*dir*/ = " ") const { return "NULL"; };
    virtual ~Parameter();
    ClassDef(Parameter, 1)
  };
  /**
   * parameter to store Int_t
   */
  class ParameterInt : public Parameter {
    Int_t fValue;

  public:
    ParameterInt();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterInt(TString name, Int_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(Int_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline Int_t GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterInt();
    ClassDef(ParameterInt, 1)
  };
  /**
   * parameter to store Double_t
   */
  class ParameterDouble : public Parameter {
    Double_t fValue;

  public:
    ParameterDouble();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterDouble(TString name, Double_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(Double_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline Double_t GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterDouble();
    ClassDef(ParameterDouble, 1)
  };
  /**
   * parameter to store Float_t
   */
  class ParameterFloat : public Parameter {
    Float_t fValue;

  public:
    ParameterFloat();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterFloat(TString name, Float_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(Float_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline Float_t GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterFloat();
    ClassDef(ParameterFloat, 1)
  };
  /**
   * parameter to store TString
   */
  class ParameterString : public Parameter {
    TString fValue;

  public:
    ParameterString();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterString(TString name, TString value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(TString value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline TString GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterString();
    ClassDef(ParameterString, 1)
  };
  /**
   * parameter to store UInt_t
   */
  class ParameterUInt : public Parameter {
    UInt_t fValue;

  public:
    ParameterUInt();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterUInt(TString name, UInt_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(UInt_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline UInt_t GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterUInt();
    ClassDef(ParameterUInt, 1)
  };
  /**
   * parameter to store ULong_t
   */
  class ParameterULong : public Parameter {
    ULong_t fValue;

  public:
    ParameterULong();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterULong(TString name, ULong_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(ULong_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline ULong_t GetValue() const { return fValue; };
    TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterULong();
    ClassDef(ParameterULong, 1)
  };
  /**
   * parameter to store ULong64_t
   */
  class ParameterULong64 : public Parameter {
    ULong64_t fValue;

  public:
    ParameterULong64();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterULong64(TString name, ULong64_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(ULong64_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline ULong64_t GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterULong64();
    ClassDef(ParameterULong64, 1)
  };
  /**
   * parameter to store Bool_t
   */
  class ParameterBool : public Parameter {
    Bool_t fValue;

  public:
    ParameterBool();
    /**
     *  main constructor
     * @param name name of parameter
     * @param value value of parameter
     * @param merge_mode mode of merging
     */
    ParameterBool(TString name, Bool_t value, Char_t merge_mode = '=');
    void Add(const Object* val);
    /**
     * set value
     * @param value new value
     */
    inline void SetValue(Bool_t value) { fValue = value; };
    /**
     *
     * @return value of parameter
     */
    inline Bool_t GetValue() const { return fValue; };
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~ParameterBool();
    ClassDef(ParameterBool, 1)
  };
}  // namespace Hal
#endif /* HALNAMEDVALUE_H_ */
