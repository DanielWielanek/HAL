/*
 * Hal::StdString.cxx
 *
 *  Created on: 25 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "StdString.h"

#include "Cout.h"

#include <TMath.h>
#include <TRegexp.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>

NamespaceImp(Hal::Std);
namespace Hal {
  namespace Std {

    TString RemoveUnits(TString tString) {
      TString res;
      Bool_t copy = kTRUE;
      for (int i = 0; i < tString.Length(); i++) {
        if (tString[i] == '[') copy = kFALSE;
        if (copy) res = res + tString[i];
        if (tString[i] == ']') copy = kTRUE;
      }
      // if (res[res.Length() - 1] == "") { res = res(0, res.Length() - 1); }
      return res;
    }

    std::vector<TString> ExplodeString(TString string, Char_t delimiter) {
      std::vector<TString> array;
      if (string.Length() == 0) return array;
      if (string[0] == delimiter) string = string(1, string.Length() - 1);
      if (string.Length() == 0) return array;
      if (string[string.Length() - 1] != delimiter) string = string + delimiter;
      Int_t no = string.CountChar(delimiter);
      for (int count = 0; count < no; count++) {
        Size_t pos      = string.First(delimiter);
        TString element = string(0, pos);
        if (element.Length() != 0) array.push_back(element);
        string = string(pos + 1, string.Length() - pos - 1);
      }
      return array;
    }

    std::vector<std::pair<TString, TString>> ConvertMainArgs(int argc, char* argv[]) {
      std::vector<std::pair<TString, TString>> res;
      for (int i = 1; i < argc; i++) {
        TString fullArg = argv[i];
        if (fullArg.BeginsWith("--")) {
          std::vector<TString> vec = ExplodeString(fullArg, '=');
          if (vec.size() < 1) {
            Hal::Cout::PrintInfo(Form("Problem with extraction of argument %s", fullArg.Data()), Hal::EInfo::kWarning);
          } else if (vec.size() != 2) {
            vec[0] = vec[0](2, vec[0].Length() - 2);
            res.push_back(std::pair<TString, TString>(vec[0], ""));
          } else {
            vec[0] = vec[0](2, vec[0].Length() - 2);
            res.push_back(std::pair<TString, TString>(vec[0], vec[1]));
          }
        } else {
          res.push_back(std::pair<TString, TString>("", fullArg));
        }
      }
      return res;
    }

    TString RoundToString(Double_t value, Int_t precission, Option_t* opt) {
      TString option = opt;
      if (option == "prefix") {
        Double_t lognum   = TMath::Log10(TMath::Abs(value));
        Int_t full_lognum = lognum / 3;
        if (lognum < 0) { full_lognum--; }
        TString answer;
        Double_t number = value / TMath::Power(10, full_lognum * 3);
        if (precission >= 0) {
          answer = RoundToString(number, precission, "");
        } else {
          precission         = TMath::Abs(precission);
          Int_t digits       = 0;
          Double_t abs_value = TMath::Abs(number);
          if (abs_value < 10) {
            digits = precission - 1;
          } else if (abs_value < 100) {
            digits = precission - 2;
          } else {
            digits = precission - 3;
          }
          if (digits < 0) digits = 0;
          answer = RoundToString(number, digits, "");
        }
        TString prefix = "x";
        switch (full_lognum * 3) {
          case -24: prefix = "y"; break;
          case -21: prefix = "z"; break;
          case -18: prefix = "a"; break;
          case -15: prefix = "f"; break;
          case -12: prefix = "p"; break;
          case -9: prefix = "n"; break;
          case -6: prefix = "#mu"; break;
          case -3: prefix = "m"; break;
          case 0: prefix = ""; break;
          case 3: prefix = "k"; break;
          case 6: prefix = "M"; break;
          case 9: prefix = "G"; break;
          case 12: prefix = "T"; break;
          case 15: prefix = "P"; break;
          case 18: prefix = "E"; break;
          case 21: prefix = "Z"; break;
          case 24: prefix = "Y"; break;
          default: prefix = Form("x10^%i", full_lognum * 3); break;
        }
        if (value == 0) prefix = "";
        return answer + prefix;
      } else if (option == "separators") {
        ULong64_t integer   = TMath::Abs(value);
        Double_t fractional = TMath::Abs(value) - ((Double_t) integer);
        fractional          = TMath::Abs(fractional);
        TString first_cout  = RoundToString(integer, 0, "separators");
        TString sec_cout    = RoundToString(fractional, precission, "");
        TString other       = "";
        Int_t pos           = sec_cout.First('.');
        for (int i = 1; i < sec_cout.Length() - 1; i++) {
          if (i % 3 != 0)
            other = other + sec_cout[pos + i];
          else
            other = other + sec_cout[pos + i] + " ";
        }
        if (value >= 0)
          return Form("%s.%s", first_cout.Data(), other.Data());
        else
          return Form("-%s.%s", first_cout.Data(), other.Data());
      } else {
        TString flag = Form("%%4.%if", precission);
        return Form(flag, value);
      }
    }

    TString RoundToString(Int_t value, Int_t precission, Option_t* opt) {
      TString option = opt;
      if (option == "separators") {
        TString val = Form("%i", TMath::Abs(value));
        if (val.Length() < 4) {
          if (value < 0) {
            return "-" + val;
          } else {
            return val;
          }
        }
        TString newstr = "";
        Int_t mod      = val.Length() % 3;
        for (int i = 0; i < val.Length(); i++) {
          if (mod == 0) {
            if (i != 0) newstr = newstr + " ";
            newstr = newstr + val[i];
            mod    = 2;
          } else {
            mod--;
            newstr = newstr + val[i];
          }
        }
        if (value < 0) {
          return ("-" + newstr);
        } else {
          return newstr;
        }
      } else if (option == "prefix") {
        return RoundToString((Double_t) value, precission, opt);
      } else {
        return Form("%i", value);
      }
    }

    TString RoundToString(ULong_t value, Int_t precission, Option_t* opt) {
      // FIXME large numbers and round issue
      TString option = opt;
      if (option == "separators") {
        TString val = Form("%lu", TMath::Abs((long int) value));
        if (val.Length() < 4) { return val; }
        TString newstr = "";
        Int_t mod      = val.Length() % 3;
        for (int i = 0; i < val.Length(); i++) {
          if (mod == 0) {
            if (i != 0) newstr = newstr + " ";
            newstr = newstr + val[i];
            mod    = 2;
          } else {
            mod--;
            newstr = newstr + val[i];
          }
        }
        return newstr;
      } else if (option == "prefix") {
        Double_t newval = (Double_t) value;
        return RoundToString(newval, precission, "prefix");
      } else {
        return Form("%lu", value);
      }
    }

    TString RoundToString(ULong64_t value, Int_t precission, Option_t* opt) {
      // FIXME large numbers and round issue
      TString option = opt;
      if (option == "separators") {
        TString val = Form("%llu", TMath::Abs((long long int) value));
        if (val.Length() < 4) { return val; }
        TString newstr = "";
        Int_t mod      = val.Length() % 3;
        for (int i = 0; i < val.Length(); i++) {
          if (mod == 0) {
            if (i != 0) newstr = newstr + " ";
            newstr = newstr + val[i];
            mod    = 2;
          } else {
            mod--;
            newstr = newstr + val[i];
          }
        }
        return newstr;
      } else if (option == "prefix") {
        Double_t newval = (Double_t) value;
        return RoundToString(newval, precission, "prefix");
      } else {
        return Form("%llu", value);
      }
    }

    TString RoundToString(UInt_t value, Int_t precission, Option_t* opt) {
      // FIXME large numbers and round issue
      TString option = opt;
      if (option == "separators") {
        TString val = Form("%u", value);
        if (val.Length() < 4) { return val; }
        TString newstr = "";
        Int_t mod      = val.Length() % 3;
        for (int i = 0; i < val.Length(); i++) {
          if (mod == 0) {
            if (i != 0) newstr = newstr + " ";
            newstr = newstr + val[i];
            mod    = 2;
          } else {
            mod--;
            newstr = newstr + val[i];
          }
        }
        return newstr;
      } else if (option == "prefix") {
        Double_t newval = (Double_t) value;
        return RoundToString(newval, precission, "prefix");
      } else {
        return Form("%u", value);
      }
    }

    TString RemoveString(TString text, TString pattern) {
      TRegexp match(pattern);
      TString newword   = text;
      TString regstring = text(match);
      if (regstring > 0) newword.Remove(text.Index(match), regstring.Length());
      return newword;
    };

    TString RemoveNChars(TString str, Int_t n, Char_t opt) {
      Int_t lenght = str.Length();
      if (opt == 'b') {
        return TString(str(n, lenght - n));
      } else
        return TString(str(0, lenght - n));
    }

    void ReplaceInFile(TString path, TString newPath, TString oldPattern, TString newPattern) {
      if (!FileExists(path)) return;
      if (path.EqualTo(".temp.txt")) {
        Cout::PrintInfo("File .temp.txt cannot be modified by HAL", EInfo::kLowWarning);
        return;
      }
      if (path == newPath) {                                       // replace in file
        ReplaceInFile(path, ".temp.txt", oldPattern, newPattern);  // replace file -> .temp.txt
        gSystem->Exec(Form("mv .temp.txt %s", path.Data()));       // overwrite old file
      } else {
        std::ifstream inFile;
        inFile.open(path);
        std::ofstream outFile;
        outFile.open(newPath);
        for (std::string line; std::getline(inFile, line);) {
          TString temp = line;
          temp.ReplaceAll(oldPattern, newPattern);
          outFile << temp << std::endl;
        }
        outFile.close();
        inFile.close();
      }
    }

    void ReplaceInFile(TString path,
                       TString newPath,
                       std::initializer_list<TString> oldPattern,
                       std::initializer_list<TString> newPattern) {
      if (!FileExists(path)) return;
      std::vector<TString> list1 = Std::GetVector(oldPattern);
      std::vector<TString> list2 = Std::GetVector(newPattern);
      if (list1.size() != list2.size()) {
        Cout::PrintInfo(Form("Not compatible lists %s %i", __FILE__, __LINE__), EInfo::kLowWarning);
        return;
      }
      if (list1.size() == 0) return;
      ReplaceInFile(path, newPath, list1[0], list2[0]);
      for (unsigned int i = 1; i < list1.size(); i++) {
        ReplaceInFile(newPath, newPath, list1[i], list2[i]);
      }
    }

    Bool_t FindParam(TString& option, TString pattern, Bool_t remove) {
      if (remove) {
        option.ReplaceAll(" ", "");
        option.ReplaceAll("++", "");
      }
      TString lower_opt     = option;
      TString lower_pattern = pattern;
      const Int_t lenght    = lower_pattern.Length();
      lower_opt.ToLower();
      lower_pattern.ToLower();
      /**
       * exact match
       */
      if (lower_opt == lower_pattern) {
        if (remove) option = "";
        return kTRUE;
      } else if (lower_opt.Length() == lower_pattern.Length()) {
        return kFALSE;
      }
      lower_pattern.ReplaceAll("+", "\\+");
      /**
       * match like somethig+
       */
      Int_t index = lower_opt.Index(TRegexp(Form("^%s\\+", lower_pattern.Data())));
      if (index >= 0) {
        if (remove) option.Remove(index, lenght + 1);
        return kTRUE;
      }
      /**
       * match like +something
       */
      index = lower_opt.Index(TRegexp(Form("\\+%s", lower_pattern.Data())));
      if (index >= 0 && index + lenght + 1 == lower_opt.Length()) {
        if (remove) option.Remove(index, lenght + 1);
        return kTRUE;
      }
      /**
       * match like +something+
       */
      index = lower_opt.Index(TRegexp(Form("\\+%s\\+", lower_pattern.Data())));
      if (index >= 0) {
        if (remove) option.Remove(index, lenght + 1);
        return kTRUE;
      }
      return kFALSE;
    }

    Bool_t FindExpressionSingleValue(TString& expression, Int_t& val, Bool_t remove) {
      TString option = expression;
      TRegexp regexp("{[0-9]+}");
      TString expr = option(regexp);
      if (expr.Length() <= 0) { return kFALSE; }
      // found regular exprestion like {number}
      option.Remove(option.Index(regexp),
                    expr.Length());  // remove exprestion from string
      TRegexp number_expr("{[0-9]+}");
      TString first      = expr(number_expr);
      TString number_str = first(TRegexp("[0-9]+"));
      val                = number_str.Atoi();
      if (remove) { expression = option; }
      return kTRUE;
    }

    Bool_t FindExpressionTwoValues(TString& expression, Int_t& val1, Int_t& val2, Bool_t remove) {
      TString option = expression;
      TRegexp regexp("{[0-9]+x[0-9]+}");
      TString expr = option(regexp);
      if (expr.Length() <= 0) { return kFALSE; }
      // found regular exprestion like {number x number}
      option.Remove(option.Index(regexp),
                    expr.Length());  // remove exprestion from string
      TRegexp number_expr("{[0-9]+x");
      TRegexp jump_expr("x[0-9]+}");
      TString first      = expr(number_expr);
      TString sec        = expr(jump_expr);
      TString number_str = first(TRegexp("[0-9]+"));
      TString jump_str   = sec(TRegexp("[0-9]+"));
      val1               = number_str.Atoi();
      val2               = jump_str.Atoi();
      if (remove) { expression = option; }
      return kTRUE;
    }

  }  // namespace Std
}  // namespace Hal
