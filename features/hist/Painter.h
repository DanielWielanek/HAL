/*
 * Painter.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_HIST_PAINTER_H_
#define HAL_FEATURES_HIST_PAINTER_H_

#include <TObject.h>
#include <TString.h>

#include <vector>
/**
 * representation of painted objects
 */
class TCanvas;
class TVirtualPad;
// TODO Ownership for HTML
// TODO what to do with Paint - should it call parents paint or repaint?
namespace Hal {
  class Style;
  class PadStyle;
  class HistoStyle;
  /**
   * class for drawing objects, contains pads, and copies of drawn objects
   * user have to overwrite at least those methods:
   * @see ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0) - define flags passed by options
   * @see void MakePadsAndCanvases() - define how many pads/canvas need to be created
   * @see void InnerPaint() - define how to draw object
   */
  class Painter : public TObject {
  private:
    struct commonPointers {
      std::vector<TCanvas*>* fCanvases              = {nullptr};
      std::vector<std::vector<TVirtualPad*>>* fPads = {nullptr};
      Painter* fParent                              = {nullptr};
    };
    commonPointers fCommonData;
    static commonPointers gCommonData;
    TVirtualPad* fTempPad = {nullptr};
    std::vector<Painter*> fSubPainters;
    Hal::PadStyle* fPadStyle = {nullptr};
    Bool_t fPainted          = {kFALSE};
    ULong64_t fDrawFlags     = {0};
    void TryPaint();

  protected:
    static const int kHtmlBit;
    static const int kCanvasBit;
    static const int kPadBit;
    static const int kSameBit;
    static const int kBrowserBit;
    static const int kLastBitPainter;
    /**
     * this should be true if draw flags were changed
     */
    Bool_t fOptionsChanged = {kFALSE};
    /**
     * set bit in drawing flag
     * @param bit
     * @param state
     */
    void SetFlag(Int_t bit, Bool_t state);
    /**
     * overwrite drawing flag
     * @param flag
     */
    void SetFlagForced(ULong64_t flag) { fDrawFlags = flag; };
    /**
     *
     *@param flag flag to set
     * @param bits clear listed bits
     * @param set set this bit if set >0
     */
    void ResetFewBits(ULong64_t& flag, std::initializer_list<Int_t> bits, Int_t set = -1) const;
    /**
     * true if this painter owns main pad
     * @return
     */
    Bool_t OwnGraphic() const { return fCommonData.fParent == this; }
    /**
     *
     * @param opt
     * @return true if opt bit is true in draw flag
     */
    Bool_t CheckOpt(Int_t opt) const { return TESTBIT(fDrawFlags, opt); };
    /**
     *
     * @return draw flags
     */
    ULong64_t GetDrawFlags() const { return fDrawFlags; }
    /**
     * used internally
     * @param opt option of drawing
     *  - "html" - for drawing in HTML mode NOTE - might not be implemented for all objects
     *  - "default" - reset flags to default and add additional flags from option
     *  - "default!" -reset flags to default and ignore rest of the option
     *  - "keep" - keeps old flags, add only new flags
     *  - "skip" - ignore this method
     *  - "grid" - draw grid on all pads there is option "gdrix" and "gridy"
     *  - "logx", "logy", "logz" - draw logs on all pads
     *  - "same" - like in TH1D, note: this will not check if same objects have same number of pads. etc.
     *  proper way to use same:
     *  painter1->Draw();
     *  painter2->Draw("same");
     *  or
     *  painter1->Draw();
     *  ...
     *  painter1->cd();
     *  painter2->Draw("same");
     * @param prev the staring draw flag
     * @return new draw flag
     */
    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0) = 0;
    /**
     * options of drawing were aplied so  reset options
     */
    virtual void OptionsApplied() { fOptionsChanged = kFALSE; }
    /**
     * set default drawing flag
     */
    virtual void SetDefaultFlag() { fDrawFlags = 0; }
    /**
     * @return true if number of subpads is proper for drawing, if return false
     * this painter will not be drawn
     */
    virtual Bool_t CheckPads() const { return kTRUE; };
    /**
     * inner method for repaint - without checking ownership
     * here you redraw object "eg. when draw option was changed"
     */
    virtual void InnerRepaint();
    /**
     * inner method for pain - without checking ownership
     * here you draw object
     */
    virtual void InnerPaint() = 0;
    /**
     * creates pads and canvases, should call generate subpads
     */
    virtual void MakePadsAndCanvases() = 0;
    /**
     * custom method of pad dividing
     * @param c
     */
    virtual void OwnCanvasDivide(TCanvas* c, Int_t x, Int_t y, Int_t canvasNo);
    /**
     *
     * @return true if main pad exist
     */
    Bool_t CanvasExist(Int_t canvasNo = 0) const;
    /**
     * looks for patterns like {flag=val[0],val[1]..} where val[n] is a float, returns true if found
     * @param opt
     * @param flag
     * @param vals
     * @return
     */
    Bool_t GetPatterns(TString opt, TString flag, std::vector<double>& vals) const;
    /**
     * set given flag depending of pattern if "pattern" is found set bit to 1
     * if "!pattern" is found set bit to 0, otherwise do nothing
     * @param option option
     * @param pattern pattern that we want to find
     * @param drawOpt flag
     * @param bit bit to set/clean
     * @param remove if true remowe pattern from string
     * @return
     */
    void ContitionalPattern(TString& option, TString pattern, ULong64_t& drawOpt, Int_t bit, Bool_t remove = kTRUE) const;
    /**
     * keep a current gPad
     */
    void LockPad();
    /**
     * restore LockedPad
     */
    void UnlockPad();
    /**
     *
     * @return number of subpads
     */
    Int_t GetPadsNo(Int_t canvasNo = 0) const { return (*fCommonData.fPads)[canvasNo].size(); }
    /**
     * enter subpad
     * @param no
     * @param canvasNo
     */
    void GotoPad(Int_t no, Int_t canvasNo = 0);
    /**
     * construct a TCanvas with pads
     * @param x
     * @param y
     */
    void MakeCanvasPads(Int_t x = 1, Int_t y = 1, Int_t canvasNo = 0);
    /**
     * redraw all paints if this is owner of them
     */
    void UpdateAllPads();
    /**
     * clears main pad
     */
    void ClearCanvas(Int_t canvas);
    /**
     * clean canvases if own them
     */
    void CleanCommonData();
    /**
     *
     * @return get top ancestor, return itsefl if no parent is for this painter
     */
    Painter* GetAncestor();
    /**
     * apply global pad style to gPad;
     */
    void ApplyGlobalPadStyle() const;
    /**
     * add this as parent subpainter, avoid duplicates
     */
    void AddAsSubPainter();

  public:
    Painter();
    /**
     * true if this painter have parent that own pad
     * @return
     */
    Bool_t HasParent() const;
    Painter(const Painter& other) = delete;
    /**
     * adds painter to the paintes
     * @param painter
     */
    void AddPainter(Painter* painter);
    /**
     * set options for drawing:
     * @param option options:
     * html - for drawing in HTML mode NOTE - might not be implemented for all objects
     * default - reset flags to default and add additional flags from option
     * default! -reset flags to default and ignore rest of the option
     * keep - keeps old flags, add only new flags
     * canvas - reuse canvas do not create pads
     * that requires single pad
     * pad - reuse canvas and pads
     * skip - ignore this method
     * same - draw as "same"
     * @see SetOptionInternal:
     */
    virtual void SetOption(TString option);
    /**
     * called when object needs to be draw (should be called only once) or redraw
     */
    void Paint();
    /**
     *
     * @return parent if present
     */
    Painter* GetParent() const { return fCommonData.fParent; }
    /**
     * assignement operator
     * @param other
     * @return
     */
    Painter& operator=(const Painter& other) = delete;
    /**
     *
     * @return TCanvas assigned to this object (if present)
     */
    TCanvas* GetCanvas(Int_t canvasNo = 0) const { return (*fCommonData.fCanvases)[canvasNo]; }
    /**
     * add subpad
     * @param index (staring with 1) if 0 returns pointer to main pad
     * @return
     */
    TVirtualPad* GetPad(Int_t index, Int_t canvasNo = 0) const;
    /**
     * switch common data to current data, useful when draw "same" is used
     */
    virtual void cd() { gCommonData = fCommonData; }
    void SetGlobalPadStyle(Hal::PadStyle& pad);
    virtual ~Painter();
    static int LastBitPainter() { return 4; }
    ClassDef(Painter, 0)
  };

  /**
   * simple painter for object that is drawn on single pad/canvas
   */
  class SimplePainter : public Painter {
  protected:
    virtual void MakePadsAndCanvases();
    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0) { return prev; };

  public:
    SimplePainter() {};
    virtual ~SimplePainter() {};
    ClassDef(SimplePainter, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_PAINTER_H_ */
