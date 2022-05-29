#ifndef HALSTD_H_
#define HALSTD_H_

#include "StdHist.h"
#include "StdMath.h"
#include "StdString.h"

#include <vector>
//#define HAL_DEBUG
class TVirtualPad;

#define Draw_2D_option "colz"
#define Draw_3D_option "box2"
#define HAL_PHYSICALANALYSYS_VER "may2022"

// some enums used almost everywhere

namespace Hal {
  enum class EInfo { kDebugInfo = 0, kInfo = 1, kLowWarning = 2, kWarning = 3, kError = 4, kCriticalError = 5 };
  enum class EFormatType { kSim = 0, kReco = 1, kComplexReco = 2, kComplexSim = 3 };

  enum class ERound { kSeparator, kPrefix };
  enum class ECutUpdate { kNo = -1, kEvent = 0, kTrack = 1, kTwoTrack = 2, kTwoTrackBackground = 3 };

  enum class EFormatDepth { kAll, kBuffered, kNonBuffered };

}  // namespace Hal

namespace Hal {
  namespace Std {
    /**
     * conver enum to "human readable" update ratio description
     * @param upd
     * @return
     */
    TString UpdateEnumToString(Hal::ECutUpdate upd);
    /**
     * copy files
     * @param from path from
     * @param to path to copy
     * @param hidden copy hidden files
     */
    void CopyFiles(TString from, TString to, Bool_t hidden = kFALSE);
    /**
     * get list of files from given directory
     * @param path directory with files
     * @param extension extension
     * @param fullPath set true to add full path to file, othrewise only name of file is returned
     * @param depth - used to look into subdirectories - 0 - don't looki into sudirs, 1 - look in subdirs 2, look in subdirs of
     * subdirs etc. -1 infinity depth
     * @return
     */
    std::vector<TString> GetListOfFiles(TString path, TString extension, Bool_t fullPath = kFALSE, Int_t depth = 0);
    /**
     *
     * @return current date as string
     */
    TString GetDate();
    /**
     *
     * @return current time as string
     */
    TString GetTime();
    /**
     * look in current directory, if object with given name is not present return
     * desired name, otherwise add number that guarantee unique name
     * @param name desired name
     * @return unique name
     */
    TString GetUniqueName(TString name);
    /**
     *
     * @return path to Jsroot (if found)
     */
    TString GetJsRoot();
    /**
     *
     * @return path to halroot plus directory - in case of installation with
     * FAIRROOT its $FAIRROOT/share/fairbase/examples/halroot_plus otherwise
     * $VMCWORKIDR/halroot/features/halroot_plus
     */
    TString GetHalrootPlus();
    /**
     *
     * @param par_name parameter name
     * @return value of parameter from .halroot_config.xml file
     */
    TString GetConfigParameter(TString par_name);
    /**
     * divide current pad in "grid"
     * @param x_pads number of columns
     * @param y_pads number of rows
     * @param x_margin margin on x-axis (if negative then margin is drawn on right
     * side).
     * @param y_margin  maring on y-axis (if negative then margin is drawn on top
     * side)
     * @return arrays of pads
     */
    TVirtualPad*** GetGridPad(Int_t x_pads, Int_t y_pads, Float_t x_margin, Float_t y_margin);
    /**
     *
     * @param name version name eg. mar2016
     * @return numer connected to version, bigger - newer version
     */
    Int_t VersionId(TString name);
    /**
     *
     * @param path
     * @return true if file exists
     */
    Bool_t FileExists(TString path);
    /**
     * calucate boost vector from fixed target to cms
     * @param energy_per_nucleon
     * @param n_proj number of neutrons in projectile
     * @param p_proj number of protons in projectile
     * @param n_tar number of neutrons in target
     * @param p_tar number of protons in  target
     * @return
     */
    TVector3 CalcualteBoostVector(Double_t energy_per_nucleon, Int_t n_proj, Int_t p_proj, Int_t n_tar, Int_t p_tar);
  }  // namespace Std
}  // namespace Hal
#endif
