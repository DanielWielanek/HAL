/*
 * HalCout.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *      bumch of stuff used in many places
 */

#ifndef HALCOUT_H_
#define HALCOUT_H_
#include "Std.h"

#include <TObject.h>
#include <TString.h>


#define HalCoutDebug() Hal::Cout::DebugInfo(__FILE__, __LINE__)
/**
 * class for "friendly printing"
 */
namespace Hal {
  class Cout : public TObject {
  private:
    static Int_t fgLineLength;
    static Cout* fgInstance;
    static Hal::EInfo fVerboseMode;
    static TString GetShift(Int_t i);
    static TString MergeStrings(Int_t no, ...);
    static void InStars(Int_t shift, TString text, Color_t color);
    static void PrintWarning(TString warning);
    static void PrintError(TString error);

  public:
    Cout();
    /**
     * return flag for disabling color in linux
     * @return
     */
    static TString GetDisableColor();
    /**
     *
     * @param Color
     * @return flag to enable given color in linux
     */
    static TString GetColor(Color_t Color);
    /**
     *
     * @return instance of this class
     */
    static Cout* Instance();
    /**
     *
     * @param lenght lenght of screen
     */
    static void SetLineLenght(Int_t lenght) { fgLineLength = lenght; };
    virtual ~Cout();
    /**
     * set verbose mode
     * @param verbose mode
     */
    static void SetVerboseMode(Hal::EInfo verbose);
    /**
     * print strings in database format
     * @param no number of columns
     */
    [[deprecated]] static void Database(Int_t no...);
    /**
     * print strings in database format
     * @param no number of columns
     */
    static void Database(std::initializer_list<TString> list);
    /**
     * print shifted database
     * @param shift number of empty places
     * @param no number of columns
     */
    static void ShiftDatabase(Int_t shift, Int_t no...);
    /**
     * print text in stars
     * @param text text to print
     * @param color color of text (one of : kGray, kOrange, kYellow, kBlue,
     * kCyan,kWhite, kGreen, kViolet)
     */
    static void InStars(TString text, Color_t color = -1);
    /**
     * print shifted text in stars
     * @param shift number of spaces used for shift
     * @param text text
     * @param color color
     */
    static void ShiftInStars(Int_t shift, TString text, Color_t color = -1);
    /**
     * print formatted text
     * @param text text
     * @param option "L" -alight to left "M" - align to center, "R" - align to
     * right
     * @param color color
     */
    static void Text(TString text, TString option = "L", Color_t color = -1);

    /**
     * print stars line
     * @param color color
     */
    static void Stars(Color_t color = -1);
    /**
     * print shifted text
     * @param shift number of spaces
     * @param text text
     * @param option  "L" -alight to left "M" - align to center, "R" - align to
     * right
     * @param color color
     */
    static void ShiftText(Int_t shift, TString text, TString option, Color_t color = -1);
    /**
     * print "empty" line
     * @param color color
     */
    static void Empty(Color_t color = -1);
    /**
     * print percentage value by taking two parameters
     * @param actual actual value
     * @param total total value
     */
    static void Percent(Double_t actual, Double_t total);
    /**
     *print progress bar
     * @param acutal actual progress
     * @param total total progress
     */
    static void ProgressBar(Double_t acutal, Double_t total);
    /**
     * print info, basic method for printing info in code, use verbose value to
     * remove texts with low prioryty
     * @param text text
     * @param status statsu of text
     */
    static void PrintInfo(TString text, Hal::EInfo status);
    /**
     * print line "fail/succeed"
     * @param value desrcription of problem
     * @param flag status of proglem
     * @param color color of status (description is in white)
     */
    static void FailSucced(TString value, TString flag, Color_t color);
    /**
     * print number if violet color
     * @param i value
     */
    static void DebugInfo(Int_t i);
    /**
     * print number if violet color
     * @param file - name of the file (use __FILE__)
     * @param i value (__LINE__)
     */
    static void DebugInfo(TString file, Int_t i);
    /**
     *
     * @return verbolse mode
     */
    static Hal::EInfo GetVerboseMode() { return fVerboseMode; }
    ClassDef(Cout, 1)
  };
}  // namespace Hal
#endif /* HALCOUT_H_ */
