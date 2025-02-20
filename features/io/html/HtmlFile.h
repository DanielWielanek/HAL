/*
 * HalHTMLFile.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_HTML_FILE_H_
#define HAL_FEATURES_IO_HTML_FILE_H_

#include "HtmlObject.h"

#include "HtmlCore.h"

#include <TObject.h>
#include <TString.h>

namespace Hal {
  class HtmlFile : public TObject {
  protected:
    TString fPath;
    TString fRelativePath;
    TString fDir;
    TString fBody;
    Bool_t fSaved;
    const Bool_t fMainFile;

  public:
    /**
     *
     * @param filename name of the file
     * @param mainFile tell if this is the "main file". Main file means that
     * all javascripts and css are copied to the "main file's" directory. If
     * mainFile is false then code will not copy those files but will try to use
     * relative path to "main file"
     */
    HtmlFile(TString filename = "", Bool_t mainFile = kFALSE);
    void AddContent(const HtmlObject& obj) { fBody = fBody + obj.ToString(); };
    void AddStringContent(const TString str) { fBody = fBody + str; };
    virtual void Save();
    virtual ~HtmlFile();
    ClassDef(HtmlFile, 1)
  };
}  // namespace Hal
#endif /* HAL_FEATURES_IO_HTML_FILE_H_ */
