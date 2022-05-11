/*
 * HalHtmlFile.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HtmlFile.h"

#include <TRegexp.h>
#include <TString.h>
#include <TSystem.h>
#include <fstream>

#include "HalStd.h"
#include "HalStdString.h"
namespace Hal {
  HtmlFile::~HtmlFile() {}

  HtmlFile::HtmlFile(TString path, Bool_t mainFile) : fPath(path), fBody(""), fSaved(kFALSE), fMainFile(mainFile) {
    HtmlCore* core = HtmlCore::Instance();  // to prevent overwriting instance
    core->FixAddress(fPath);
    Int_t last = fPath.Last('/');
    if (last > 0) {
      fDir = fPath(0, last);
      if (fMainFile) HtmlCore::SetMainDir(fDir);
      fRelativePath = HtmlCore::GetRelativePath(fDir);
    } else {
      fDir = "";
      if (fMainFile) HtmlCore::SetMainDir(fDir);
      fRelativePath = "";
    }
  }

  void HtmlFile::Save() {
    if (fSaved) return;
    // Core::Instance();
    if (fMainFile) {
      HtmlCore::CopyCss(fDir);
      HtmlCore::CopyJs(fDir);
    }
    std::ofstream file;
    file.open(fPath);
    file << " <meta charset=\"UTF-8\">  \n";
    file << "<html>\n";
    file << "<head>\n";
    file << "<link rel=\"stylesheet\" href=\"" << fRelativePath << "table.css\" type=\"text/css\"/>\n";
    if (HtmlCore::IsOnline()) {
      file << "<script src=\"http://jsroot.gsi.de/latest/scripts/JSRootCore.js\" "
              "type=\"text/javascript\"></script>\n";
    } else {
      file << "<script src=\"" << fRelativePath
           << "hal_js/scripts/JSRootCore.js\" "
              "type=\"text/javascript\"></script>\n";
    }
    file << "<script src=\"" << fRelativePath << "hal_js.js\" type=\"text/javascript\"></script>\n";
    file << "</head>\n";
    file << "<body>\n";

    file << fBody;

    file << "</body></html>" << std::endl;
    file.close();
    fSaved = kTRUE;
  }
}  // namespace Hal
