/*
 * HalParameter.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 */

#ifndef HALNAMEDVALUE_H_
#define HALNAMEDVALUE_H_

#include "io/HalObject.h"

#include <TString.h>

/**
 * abstract for storing parameters, upgraded TParameter known from root
 */

class TBrowser;

class HalParameter : public HalObject {
protected:
  /**
   * merging option
   */
  enum class EHalParameterMerge {
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
  EHalParameterMerge fMergeMode;

public:
  /**
   * default constructor shouldn't be used
   */
  HalParameter();
  /**
   *  main constructor
   * @param name name of parameter
   * @param merge_mode mode of merging
   */
  HalParameter(TString name, Char_t merge_mode = '=');
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
  virtual ~HalParameter();
  ClassDef(HalParameter, 1)
};
/**
 * parameter to store Int_t
 */
class HalParameterInt : public HalParameter {
  Int_t fValue;

public:
  HalParameterInt();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterInt(TString name, Int_t value, Char_t merge_mode = '=');
  void Add(const HalObject* val);
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
  virtual ~HalParameterInt();
  ClassDef(HalParameterInt, 1)
};
/**
 * parameter to store Double_t
 */
class HalParameterDouble : public HalParameter {
  Double_t fValue;

public:
  HalParameterDouble();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterDouble(TString name, Double_t value, Char_t merge_mode = '=');
  void Add(const HalObject* val);
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
  virtual ~HalParameterDouble();
  ClassDef(HalParameterDouble, 1)
};
/**
 * parameter to store Float_t
 */
class HalParameterFloat : public HalParameter {
  Float_t fValue;

public:
  HalParameterFloat();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterFloat(TString name, Float_t value, Char_t merge_mode = '=');
  ;
  void Add(const HalObject* val);
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
  virtual ~HalParameterFloat();
  ClassDef(HalParameterFloat, 1)
};
/**
 * parameter to store TString
 */
class HalParameterString : public HalParameter {
  TString fValue;

public:
  HalParameterString();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterString(TString name, TString value, Char_t merge_mode = '=');
  ;
  void Add(const HalObject* val);
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
  virtual ~HalParameterString();
  ClassDef(HalParameterString, 1)
};
/**
 * parameter to store UInt_t
 */
class HalParameterUInt : public HalParameter {
  UInt_t fValue;

public:
  HalParameterUInt();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterUInt(TString name, UInt_t value, Char_t merge_mode = '=');
  ;
  void Add(const HalObject* val);
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
  virtual ~HalParameterUInt();
  ClassDef(HalParameterUInt, 1)
};
/**
 * parameter to store ULong_t
 */
class HalParameterULong : public HalParameter {
  ULong_t fValue;

public:
  HalParameterULong();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterULong(TString name, ULong_t value, Char_t merge_mode = '=');
  ;
  void Add(const HalObject* val);
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
  virtual ~HalParameterULong();
  ClassDef(HalParameterULong, 1)
};
/**
 * parameter to store ULong64_t
 */
class HalParameterULong64 : public HalParameter {
  ULong64_t fValue;

public:
  HalParameterULong64();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterULong64(TString name, ULong64_t value, Char_t merge_mode = '=');
  ;
  void Add(const HalObject* val);
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
  virtual ~HalParameterULong64();
  ClassDef(HalParameterULong64, 1)
};
/**
 * parameter to store Bool_t
 */
class HalParameterBool : public HalParameter {
  Bool_t fValue;

public:
  HalParameterBool();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  HalParameterBool(TString name, Bool_t value, Char_t merge_mode = '=');
  ;
  void Add(const HalObject* val);
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
  virtual ~HalParameterBool();
  ClassDef(HalParameterBool, 1)
};

#endif /* HALNAMEDVALUE_H_ */
