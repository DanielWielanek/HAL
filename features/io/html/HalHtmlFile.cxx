/*
 * HalHtmlFile.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HalHtmlFile.h"

#include "HalStd.h"
#include "HalStdString.h"

#include <TRegexp.h>
#include <TString.h>
#include <TSystem.h>
#include <fstream>

HalHtmlFile::~HalHtmlFile() {}

HalHtmlFile::HalHtmlFile(TString path, Bool_t mainFile) : fPath(path), fBody(""), fSaved(kFALSE), fMainFile(mainFile) {
  HalHtmlCore* core = HalHtmlCore::Instance();  // to prevent overwriting instance
  core->FixAddress(fPath);
  Int_t last = fPath.Last('/');
  if (last > 0) {
    fDir = fPath(0, last);
    if (fMainFile) HalHtmlCore::SetMainDir(fDir);
    fRelativePath = HalHtmlCore::GetRelativePath(fDir);
  } else {
    fDir = "";
    if (fMainFile) HalHtmlCore::SetMainDir(fDir);
    fRelativePath = "";
  }
}

void HalHtmlFile::Save() {
  if (fSaved) return;
  // Core::Instance();
  if (fMainFile) {
    HalHtmlCore::CopyCss(fDir);
    HalHtmlCore::CopyJs(fDir);
  }
  std::ofstream file;
  file.open(fPath);
  file << " <meta charset=\"UTF-8\">  \n";
  file << "<html>\n";
  file << "<head>\n";
  file << "<link rel=\"stylesheet\" href=\"" << fRelativePath << "table.css\" type=\"text/css\"/>\n";
  if (HalHtmlCore::IsOnline()) {
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
