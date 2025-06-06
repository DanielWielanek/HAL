/*
 * HalPackage2HTML.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALPACKAGE2HTML_H_
#define HALPACKAGE2HTML_H_

#include "Std.h"

#include <TObject.h>
#include <TString.h>
#include <initializer_list>
#include <map>
#include <vector>


class TH1;
class TH2;
class TH3;
class TFile;
class TGraph;
class TObjString;
/**
 * make HTML reports
 */

namespace Hal {
  class HtmlFile;
  class HtmlObject;
  class Package;
  class HtmlCell;
  class PicTree;
  class Package2HTML : public TObject {
    friend class AnaFile;
    struct containerInfo {
      std::vector<int> sLinks;
      int sColId;
      double sFraction;
    };

  protected:
    enum class eTableStyle { kStandard, kMetaData };
    Double_t GetDouble(const Package* pack, TString name) const;
    TString GetString(const Package* pack, TString name) const;
    ULong64_t GetULong(const Package* pack, TString name) const;

  private:
    TString AddToUrl(TString add_dir, TString url) const;
    void GetCollectionsNumbers();
    void CreateImagePage(TString filename, TString image_flag, Int_t no, TString path = "") const;
    TString GetLinkCutMon(TH1* h1, TH1* h2, Int_t no, TString path = "") const;
    TString GetLink1D(TH1* h1, TH1* h2, Int_t no, TString path = "") const;
    TString GetLink2D(TH2* h1, TH2* h2, Int_t no, TString path = "") const;
    TString GetLink3D(TH3* h1, TH3* h2, Int_t no, TString path = "") const;
    TString GetLinkToCut(Hal::ECutUpdate update, Int_t collection_no, Int_t cut_no, Bool_t fast) const;
    TString GetLinkToCutMonitor(Hal::ECutUpdate udpate, Int_t collection_no, Int_t cut_mon_no) const;
    void CreatePackageList(HtmlObject& table,
                           Package* pack,
                           eTableStyle style,
                           TString path,
                           Int_t depth,
                           Option_t* opt = "drawcomment+drawclassname");
    void CreateCutAndMonitorList(HtmlObject& table, TString path);
    void ExportCollections(HtmlObject& object, TString path);
    void CreateCutHTML(HtmlObject& table, Hal::ECutUpdate cut_upd, Int_t collection_no);
    void CreateCutLink(HtmlObject& table, Hal::ECutUpdate cut_update, Package* obj, Int_t no, Bool_t fast);
    void CreateCutMonitorLink(HtmlObject& table, Hal::ECutUpdate update, Package* obj, Int_t no, Int_t counter, TString path);
    void CreateCutMonitorHTML(HtmlObject& table, Hal::ECutUpdate cut_upd, Int_t collection_no, Int_t& counter, TString path);
    void CreateListTable(HtmlObject& table, TList* list, Int_t no, TString path_data, TString path_url, TString drawClass);
    containerInfo GetContainerInfo(Package* Pack, Bool_t rev = kFALSE) const;
    HtmlFile* fHTML;
    TFile* fFile;
    TString fDir;
    std::map<TString, Int_t> fTObjectCounter;
    Int_t fDynamicTableCounter, fTaskTableCounter;

    Int_t fCutContainerPosition, fPackageID;
    Int_t fCollectionsNo[4];
    Package* fCurrentCutContainer;
    Int_t fListDeep;
    Int_t fSoftVer;
    void TryExtractAnaResult(HtmlObject& table, Package* pack, TString path);

  protected:
    /**
     * extract run info
     * @return true if run info found
     */
    Bool_t ExtractRunInfo();
    /**
     * default empty ctor
     */
    Package2HTML();
    /**
     * constructor used by package extractor
     * @param ana_pack package with analysis
     * @param global_metadata package with global metadata
     * @param dir place where report will be created
     * @param task_id id of task
     */
    Package2HTML(Package* ana_pack, Package* global_metadata, const TString dir, Int_t task_id);
    /**
     * return true if obj inherits from HalPackage
     * @param obj
     * @return
     */
    Bool_t IsHalPackage(const TObject* obj) const { return obj->InheritsFrom("Hal::Package"); };
    /**
     * return true if obj is exactly Hal::Package
     * @param obj
     * @return
     */
    Bool_t IsExacltyHalPackage(const TObject* obj) const {
      TString className = obj->ClassName();
      return className.EqualTo("Hal::Package");
    }

  public:
    /**
     * extract entire file to HTML report
     * @param filename name of file
     * @param dir place to create report
     */
    Package2HTML(TString filename, const TString dir);
    /**
     * extract only single package into HTML report
     * @param filename name of file
     * @param dir place where report will be extracted
     * @param packname name of package to extraction
     */
    // Package2HTML(TString filename, const TString dir, TString packname);
    /**
     * export single package (even if it is not package with analysis
     * @param pack package for extraction
     * @param dir place where report is created
     * @param main_dir - if true separate coptions of html-scripts is created in
     * dir
     */
    Package2HTML(Package* pack, const TString dir, Bool_t mainDir = kTRUE);
    virtual ~Package2HTML();
    ClassDef(Package2HTML, 1)
  };
}  // namespace Hal

#endif /* HALPACKAGE2HTML_H_ */
