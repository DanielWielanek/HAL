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
  class Painter : public TObject {
  private:
    TCanvas* fCanvas                    = {nullptr};
    Painter* fParent                    = {nullptr};
    std::vector<TVirtualPad*>* fSubPads = {nullptr};
    TVirtualPad* fMainPad               = {nullptr};
    TVirtualPad* fTempPad               = {nullptr};
    std::vector<Painter*> fSubPainters;
    Hal::PadStyle* fPadStyle = {nullptr};
    Bool_t fOwnPad           = {kTRUE};
    Bool_t fPainted          = {kFALSE};
    ULong64_t fDrawFlags     = {0};
    void TryPaint();

  protected:
    static const int kHtmlBit;
    static const int kGridBit;
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
     * this should be true if draw flags were changed
     */
    Bool_t fOptionsChanged = {kFALSE};
    /**
     * true if this painter owns main pad
     * @return
     */
    Bool_t OwnPad() const { return fOwnPad; }
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
     * make a pad for this painter
     */
    void MakePad();
    /**
     * make subpads for drawing objects
     */
    virtual void MakeSubPads() = 0;
    /**
     *
     * @return true if number of subpads is proper for drawing, if return false
     * this painter will not be drawn
     */
    virtual Bool_t CheckSubPads() const { return kTRUE; };
    /**
     *
     * @return true if main pad exist
     */
    Bool_t MainPadExist() const {
      if (fMainPad) return kTRUE;
      return kFALSE;
    }
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
     * constructor with main pad comes from external place
     * @param mainPad
     */
    Painter(TVirtualPad* mainPad);
    /**
     * keep a current gPad
     */
    void LockPad();
    /**
     * restore LockedPad
     */
    void UnlockPad();
    /**
     * add subpad to this painter
     * @param pad
     */
    void AddSubPad(TVirtualPad* pad) { fSubPads->push_back(pad); }
    /**
     *
     * @return number of subpads
     */
    Int_t GetSubPadsNo() const { return fSubPads->size(); }
    /**
     * enter subpad
     * @param no
     */
    void GotoSubPad(Int_t no);
    /**
     * makes subpads
     * @param x
     * @param y
     */
    void GenerateSubPads(Int_t x, Int_t y);
    /**
     *
     */
    void UpdateAllSubPads();
    /**
     * clears main pad
     */
    void ClearMainPad();
    /**
     * inner method for repaint - without checking ownership
     */
    virtual void InnerRepaint() {};
    /**
     * inner method for pain - without checking ownership
     */
    virtual void InnerPaint() {};

    Painter* GetAncestor() const;

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
     * skip - ignore this method
     * @see SetOptionInternal:
     */
    virtual void SetOption(TString option);
    /**
     * called when object needs to be draw (should be called only once), it try to paint object
     * if object was painted calls repaint it
     */
    void Paint();
    /**
     *
     * @return parent if present
     */
    Painter* GetParent() const { return fParent; }
    Painter& operator=(const Painter& other) = delete;
    /**
     *
     * @return TCanvas assigned to this object (if present)
     */
    TCanvas* GetCanvas() const { return fCanvas; }
    /**
     * add subpad
     * @param index (staring with 1) if 0 returns pointer to main pad
     * @return
     */
    TVirtualPad* GetPad(Int_t index) const;
    void SetGlobalPadStyle(Hal::PadStyle& pad);
    virtual ~Painter();
    ClassDef(Painter, 0)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_PAINTER_H_ */
