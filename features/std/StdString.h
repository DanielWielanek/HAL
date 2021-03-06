/*
 * Hal::StdString.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSTDSTRING_H_
#define HALSTDSTRING_H_

#include <TString.h>

namespace Hal {
  namespace Std {
    /**
     * remove text in [] brackes
     * @param string input string
     * @return string without units
     */
    TString RemoveUnits(TString string);
    /**
     * split input string into array of strings delimited by given char
     * @param string string to explode
     * @param delimiter delimiter
     * @return vector of strings
     */
    std::vector<TString> ExplodeString(TString string, Char_t delimiter);
    /**
     * extract parameters
     * @param argc first argument of main - number of strings
     * @param argv string
     * @return pairs of string e.g. myProgram arg1 arg2
     * returns [ ["","arg1],["",arg2] ] myProgram arg1 --flag=arg2
     * returns [ ["","arg1],["flag",arg2] ]
     */
    std::vector<std::pair<TString, TString>> ConvertMainArgs(int argc, char* argv[]);
    /**
     * round value to string
     * @param value value to round
     * @param precission precission ,if smaller then zero and "prefix" is used then
     * this precission describe total number of digits for example 22 milions with
     * precission 3 and option "prefix" will be represented as 22.000M with
     * precission -3 and option "prefix" it will be 22.0 M
     * @param opt, if "" then standard round with given precission is made, if
     * "separators" then separators are created if "prefix" then SI prefix is used
     * and precission define minimal number of significant digits
     * @return formated value
     */
    TString RoundToString(Double_t value, Int_t precission = 0, Option_t* opt = "");
    /**
     *
     * @param value value to round
     * @param precission precission
     * @param opt, if "" then standard round with given precission is made, if
     * "separators" then separators are created if "prefix" then SI prefix is used
     * and precission define minimal number of significant digits
     * @return formateed value
     */
    TString RoundToString(Int_t value, Int_t precission = 0, Option_t* opt = "");
    /**
     *
     * @param value value to round
     * @param precission precission
     * @param opt, if "" then standard round with given precission is made, if
     * "separators" then separators are created if "prefix" then SI prefix is used
     * and precission define minimal number of significant digits
     * @return formateed value
     */
    TString RoundToString(UInt_t value, Int_t precission = 0, Option_t* opt = "");
    /**
     *
     * @param value value to round
     * @param precission precission
     * @param opt, if "" then standard round with given precission is made, if
     * "separators" then separators are created if "prefix" then SI prefix is used
     * and precission define minimal number of significant digits
     * @return formateed value
     */
    TString RoundToString(ULong_t value, Int_t precission = 0, Option_t* opt = "");
    /**
     *
     * @param value value to round
     * @param precission precission
     * @param opt, if "" then standard round with given precission is made, if
     * "separators" then separators are created if "prefix" then SI prefix is used
     * and precission define minimal number of significant digits
     * @return formateed value
     */
    TString RoundToString(ULong64_t value, Int_t precission = 0, Option_t* opt = "");
    /**
     * return string with removed pattern
     * @param text string to process
     * @param pattern pattern
     * @return strint without pattern
     */
    TString RemoveString(TString text, TString pattern);
    /**
     * find given pattern and ignore upper/lowercase letter
     * patter must be separate world or word separated by '+' signs
     * e.g. FindParam("someapple+orange", "oragne" returns true
     * but FindParam("someapple+orange", "apple" returns false
     * @param option
     * @param pattern
     * @param remove remove matched pattern, also removes all "++" patterns and
     * spaces
     * @return true if match pattern
     */
    Bool_t FindParam(TString& option, TString pattern, Bool_t remove = kFALSE);
    /**
     * find number in exression like *{A}*
     * @param expression
     * @param val value found
     * @param remove remove from expression
     * @return true if expression found
     */
    Bool_t FindExpressionSingleValue(TString& expression, Int_t& val, Bool_t remove);
    /**
     * find pair of numbers in expression like *{AxB}*
     * @param expression
     * @param val1 first found value
     * @param va2 second found value
     * @param remove remove from expression
     * @return true if expression found
     */
    Bool_t FindExpressionTwoValues(TString& expression, Int_t& val1, Int_t& val2, Bool_t remove);
  }  // namespace Std
}  // namespace Hal
#endif /* HALSTDSTRING_H_ */
