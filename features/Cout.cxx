/*
 * HalCout.cpp
 *
 *  Created on: 20-06-2013
 *      Author: Daniel Wielanek
 */

#include "Cout.h"
#include <iostream>
//#ifndef fgLineLength
//#define fgLineLength			100
//#define (fgLineLength-4)		(fgLineLength-4)
//#endif
#if !defined(R__ALPHA) && !defined(R__SOLARIS) && !defined(R__ACC) && !defined(R__FBSD)

#endif
namespace Hal {
  Int_t Cout::fgLineLength      = 100;
  Cout* Cout::fgInstance        = NULL;
  Hal::EInfo Cout::fVerboseMode = Hal::EInfo::kDebugInfo;
  void Cout::Database(Int_t no, ...) {
    va_list ap;
    TString begin;
    TString capt;
    TString line;
    va_start(ap, no);
    Int_t wide = (fgLineLength - 2 - no) / no;
    Int_t act_wide;
    Int_t total = 0;
    for (Int_t i = 0; i < no; i++) {
      capt = va_arg(ap, char*);
      // sl = capt.Length();
      line     = " ";
      line     = line + capt;
      act_wide = wide - line.Length();
      for (Int_t j = 0; j < act_wide - 1; j++) {
        line = " " + line;
      }
      total += act_wide;
      if (i < no - 1) line = line + "|";
      begin = begin + line;
    }

    va_end(ap);
    Text(begin, "R");
  }

  void Cout::InStars(TString text, Color_t color) {
    TString Header = "******";
    Header         = Header + text;
    Int_t dlugosc  = Header.Length();
    Int_t spacji   = (fgLineLength - dlugosc);
    for (Int_t i = 0; i < spacji; i++) {
      Header = Header + "*";
    }
    if ((Int_t) color == -1) {
      std::cout << Header << std::endl;
    } else {
      std::cout << EnableColor(color) << Header << DisableColor() << std::endl;
    }
  }

  void Cout::Text(TString text, TString option, Color_t color) {
    if (text.Length() <= fgLineLength - 4) {
      if (option == "M") {
        Int_t dlugosc = text.Length();
        Int_t spacji  = (fgLineLength - 2 - dlugosc) / 2;
        for (Int_t i = 0; i < spacji; i++) {
          text = " " + text;
        }
        for (Int_t i = 0; i < spacji; i++) {
          text = text + " ";
        }
        if (dlugosc % 2 == 1) {
          text = text + " ";
          text = text + "*";
        } else {
          text = text + "*";
        }
        text = "*" + text;

      } else if (option == "L") {
        Int_t dlugosc = text.Length();
        text          = "* " + text;
        for (Int_t i = 0; i < fgLineLength - 3 - dlugosc; i++) {
          text = text + " ";
        }
        text = text + "*";
      }
      if (option == "R") {
        Int_t dlugosc = text.Length();
        text          = text + " *";
        for (Int_t i = 0; i < fgLineLength - 3 - dlugosc; i++) {
          text = " " + text;
        }
        text = "*" + text;
      }

      if ((Int_t) color == -1)
        std::cout << text << std::endl;
      else {
        std::cout << EnableColor(color) << text << DisableColor() << std::endl;
      }
    } else {
      Int_t zz = (Int_t) text.Length() / (fgLineLength - 4);
      for (Int_t i = 0; i < zz; i++) {
        Int_t beg    = i * (fgLineLength - 4);
        Int_t end    = (fgLineLength - 4);
        TString temp = TString(text(beg, end));
        Text(temp, option, color);
      }
      if (zz * (fgLineLength - 4) < text.Length()) {
        Int_t beg    = zz * (fgLineLength - 4);
        Int_t end    = text.Length() - zz * (fgLineLength - 4);
        TString temp = TString(text(beg, end));
        Text(temp, option, color);
      }
    }
  }

  void Cout::Stars(Color_t color) {
    TString starline;
    for (Int_t i = 0; i < fgLineLength; i++) {
      starline = starline + "*";
    }
    if ((Int_t) color == -1) {
      std::cout << starline << std::endl;
    } else {
      std::cout << EnableColor(color) << starline << DisableColor() << std::endl;
    }
  }

  void Cout::PrintError(TString error) {
    InStars("ERROR", kRed);
    if (error.Length() < (fgLineLength - 4)) {
      Text(error, "L", kRed);
    } else {
      Int_t zz = (Int_t) error.Length() / ((fgLineLength - 4));
      for (Int_t i = 0; i < zz; i++) {
        Int_t beg    = i * (fgLineLength - 4);
        Int_t end    = (fgLineLength - 4);
        TString temp = TString(error(beg, end));
        Text(temp, "L", kRed);
      }
      if (zz * (fgLineLength - 4) < error.Length()) {
        Int_t beg    = zz * (fgLineLength - 4);
        Int_t end    = error.Length() - zz * (fgLineLength - 4);
        TString temp = TString(error(beg, end));
        Text(temp, "L", kRed);
      }
    }
    Stars(kRed);
    exit(0);
  }

  void Cout::PrintWarning(TString warning) {
    InStars("WARNING", kOrange);
    if (warning.Length() < fgLineLength - 10) {
      Text(warning, "L", kOrange);
    } else {
      Int_t zz = (Int_t) warning.Length() / ((fgLineLength - 4));
      for (Int_t i = 0; i < zz; i++) {
        Int_t beg    = i * (fgLineLength - 4);
        Int_t end    = (fgLineLength - 4);
        TString temp = TString(warning(beg, end));
        Text(temp, "L", kOrange);
      }
      if (zz * (fgLineLength - 4) < warning.Length()) {
        Int_t beg    = zz * (fgLineLength - 4);
        Int_t end    = warning.Length() - zz * (fgLineLength - 4);
        TString temp = TString(warning(beg, end));
        Text(temp, "L", kOrange);
      }
    }
    Stars(kOrange);
  }

  void Cout::Empty(Color_t color) {
    // TString empline;
    // empline="*";
    TString empline(' ', fgLineLength - 2);
    empline = "*" + empline;
    if ((Int_t) color == -1) {
      std::cout << empline << std::endl;
    } else {
      std::cout << EnableColor(color) << empline << std::endl;
    }
  }

  void Cout::Percent(Double_t actual, Double_t total) {
    // EnableColor(kViolet);
    double ratio = actual / total;
    // printf("%4d%% ", (int)(ratio*100) );
    // printf("\n\033[F\033[J");
    std::cout << EnableColor(kViolet) << Form("%4d%% ", (int) (ratio * 100)) << Form("\n\033[F\033[J") << DisableColor()
              << std::endl;
    // DisableColor();
  }

  void Cout::ProgressBar(Double_t actual, Double_t total) {
    // Only update r times.
    //   if ( 100 % ((Int_t)total/93) != 0 ) return;
    // Calculuate the ratio of complete-to-incomplete.
    double ratio = actual / total;
    int c        = ratio * (fgLineLength - 7);
    // Show the percentage complete.
    EnableColor(kBlue);
    printf("%3d%% [", (int) (ratio * 100));
    // Show the load bar.
    for (int x = 0; x < c; x++)
      printf("=");
    for (int x = c; x < fgLineLength - 7; x++)
      printf(" ");
    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\n\033[F\033[J");
    DisableColor();
  }

  void Cout::DebugInfo(Int_t i) {
    Cout::Stars(kViolet);
    Cout::Text(Hal::Std::RoundToString(i), "L", kViolet);
    Cout::Stars(kViolet);
  }

  TString Cout::DisableColor() {
#ifdef __linux__
    return ("\e[0;0m");
#endif
    return " ";
  }

  TString Cout::EnableColor(Color_t Color) {
#ifdef __linux__
    if (Color == kGray) {
      return ("\e[0;30m");
    } else if (Color == kRed) {
      return ("\e[0;31m");
    } else if (Color == kOrange || Color == kYellow) {
      return ("\e[0;33m");
    } else if (Color == kBlue) {
      return ("\e[0;34m");
    } else if (Color == kCyan) {
      return ("\e[0;36m");
    } else if (Color == kWhite) {
      return ("\e[0;37m");
    } else if (Color == kGreen) {
      return ("\e[0;32m");
    } else if (Color == kViolet) {
      return ("\e[0;35m");
    }
#endif
    return " ";
  }

  TString Cout::MergeStrings(Int_t no, ...) {
    va_list ap;
    TString begin;
    TString capt;
    va_start(ap, no);
    for (Int_t i = 0; i < no; i++) {
      capt  = va_arg(ap, char*);
      begin = begin + capt;
    }
    va_end(ap);
    return begin;
  }

  void Cout::ShiftDatabase(Int_t shift, Int_t no, ...) {
    va_list ap;
    TString begin;
    TString capt;
    TString line;
    va_start(ap, no);
    Int_t wide = (fgLineLength - shift - 1 - 2 - no) / no;
    Int_t sl, act_wide;
    Int_t total = 0;
    line        = Cout::GetShift(shift);
    for (Int_t i = 0; i < no; i++) {
      capt     = va_arg(ap, char*);
      sl       = capt.Length();
      line     = " ";
      line     = line + capt;
      act_wide = wide - sl;
      for (Int_t j = 0; j < act_wide - 1; j++) {
        line = " " + line;
      }
      total += act_wide;
      if (i < no - 1) line = line + "|";
      begin = begin + line;
    }

    va_end(ap);
    Cout::ShiftText(shift, begin, "R", -1);
  }

  void Cout::InStars(Int_t shift, TString text, Color_t color) {
    text = Cout::GetShift(shift) + text;
    Cout::InStars(text, color);
  }

  void Cout::ShiftText(Int_t shift, TString text, TString option, Color_t color) {
    text = GetShift(shift) + text;
    Cout::Text(text, option, color);
  }

  void Cout::ShiftInStars(Int_t shift, TString text, Color_t color) {
    text = text + GetShift(shift);
    InStars(text, color);
  }

  TString Cout::GetShift(Int_t i) {
    TString val = "";
    for (int j = 0; j < i; j++) {
      val += "-";
    }
    return val;
  }

  void Cout::SetVerboseMode(Hal::EInfo verbose) { fVerboseMode = verbose; }

  void Cout::PrintInfo(TString text, Hal::EInfo status) {
    if (status < fVerboseMode) { return; }
    switch (status) {
      case Hal::EInfo::kDebugInfo: FailSucced(text, "INFO    ", kCyan); break;
      case Hal::EInfo::kInfo: FailSucced(text, "INFO!   ", kCyan); break;
      case Hal::EInfo::kLowWarning: FailSucced(text, "WARNING ", kOrange); break;
      case Hal::EInfo::kWarning: PrintWarning(text); break;
      case Hal::EInfo::kError: FailSucced(text, "ERROR   ", kRed); break;
      case Hal::EInfo::kCriticalError: PrintError(text); break;
    }
  }

  void Cout::FailSucced(TString value, TString flag, Color_t color) {
    Int_t total     = value.Length() + flag.Length();
    Int_t nr_spaces = (fgLineLength - 4) - total - 2;
    if (nr_spaces < 0) nr_spaces = 0;
    TString spaces(' ', nr_spaces);
    spaces = value + spaces;
    std::cout << EnableColor(kWhite) << "* " << spaces << "[" << EnableColor(color) << flag << EnableColor(kWhite) << "] *"
              << DisableColor() << std::endl;
  }

  Cout::Cout() {
    if (fgInstance) {
      PrintWarning("Instance of HalCout exist");
    } else {
      fgInstance = this;
    }
  }

  Cout* Cout::Instance() {
    if (fgInstance == NULL) { fgInstance = new Cout(); }
    return fgInstance;
  }

  Cout::~Cout() {}
}  // namespace Hal
