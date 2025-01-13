/*
 * HalPackageExtractor.h
 *
 *  Created on: 30-04-2022
 *
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALANAFILE_H_
#define HALANAFILE_H_

#include "Std.h"

#include <TObject.h>
#include <TString.h>


class TFile;

/**
 * class for extracting data from reports
 */
namespace Hal {
  class Package;
  class AnaFile : public TObject {
    Int_t fHTMLCounter;
    Int_t fCurrentPackID;
    Int_t fMaxPackID;
    TObjArray* fMainPackageArray;
    TObjArray* fCutContainerArray;
    Package* fMetaDataPack;
    TFile* fFile;
    TList* fPhysKeys;
    TString fFilename;
    Bool_t FindCutContainer(Int_t pack_no);
    void PrintMonitorCutList(TList* list) const;
    void CreateMonitorHTML(Int_t i) const;
    void CreateCutHTML(Int_t i) const;
    void PrintCut(Package* pack) const;
    void PrintCutsInPackage(Package* pack) const;
    Package* GetCutCollection(Hal::ECutUpdate update, Int_t no) const;
    Package* GetMainPackage() const;
    Package* GetCutContainer() const;

  public:
    /**
     * constructor
     * @param filename name of file
     * @param packname name of package (if not specified first will be taken,
     * other packages will not be read - SwitchPackage will not work)
     */
    AnaFile(TString filename, TString packname);

    /**
     * constructor
     * @param filename name of file
     * @param packID - package numer  (if not specified first will be taken, other
     * packages will not be red - SwitchPackage will not work)
     */
    AnaFile(TString filename = " ", Int_t id = -1);
    /**
     *
     * @return kes from HalPhysics director
     */
    TList* GetKeys() const { return fPhysKeys; };
    /**
     * switch package by name
     * @param name name of package
     * @return true if package found, false otherwise
     */
    Bool_t SwitchPackage(TString name);
    /**
     * switch package by number
     * @param no number of package
     * @return true if package found, false otherwise
     */
    Bool_t SwitchPackage(Int_t no);
    /**
     *  return entrire analysis package
     * @param name name of package
     * @return package with report from analysis
     */
    Package* GetPackage(TString name) const;
    /**
     *
     * @return global data (or local if global wasn't found)
     */
    Package* GetMetaData() const;
    /**
     * return object from current physical analysis package
     * in most cases first two objects contain analysis settings
     * @param i object number
     * @return object from package
     */
    TObject* GetMainObject(Int_t i = 0) const;
    /**
     * return object from current physical analysis packages
     * skip first two objects - usually cut and metadata
     * @param i
     * @return
     */
    TObject* GetMainAnaObject(Int_t i = 0) const { return GetMainObject(i + 2); };
    /**
     * return object from current physical analysis package in most cases
     * first two main objects contain information about analysis settings
     * @param name name of object
     * @param index object index (if more objects exist)
     * @return object from package
     */
    TObject* GetMainObject(TString name, Int_t index = 0) const;
    /**
     *
     * @param update cut monitor update ratio/tier
     * @param collection cut monitor collection
     * @param no cut monitor number
     * @return histogram with passed values from cut monitor
     */
    TH1* GetHistogramPassed(Hal::ECutUpdate update, Int_t collection, Int_t no) const;
    /**
     *
     * @param update cut monitor update ratio/tier
     * @param collection cut monitor collection
     * @param no cut monitor number
     * @return histogram with failed values from cut monitor
     */
    TH1* GetHistogramFailed(Hal::ECutUpdate update, Int_t collection, Int_t no) const;
    /**
     *
     * @return number of merged files into this one
     */
    Int_t GetMerged() const;
    /**
     *
     * @param update tier/update ratio
     * @return number of collections at given level
     */
    Int_t GetCollectionsNo(Hal::ECutUpdate update) const;
    /**
     *
     * @return number of objects with current package with analysis
     */
    Int_t GetMainObjectsNo() const;
    /**
     *
     * @param update
     * @param collection
     * @param opt - flag of collections "fast" for only fast collection number,
     * "slow" for only slow collection number, otherwise retur total number of
     * cuts
     * @return number of cuts in given collection
     *
     */
    Int_t GetCutsNo(Hal::ECutUpdate update, Int_t collection, TString opt = "all") const;
    /**
     *
     * @param update
     * @param collection
     * @param opt - flag  - might be 1d, 2d or 3d
     * @return number of cut monitors in given collection
     *
     */
    Int_t GetCuMonitorsNo(Hal::ECutUpdate update, Int_t collection, TString opt = "all") const;
    /**
     *
     * @return number of packages in file
     */
    Int_t GetPackagesNo() const { return fMaxPackID; }
    /**
     * print list of packages with physical analysis in current file
     * @param filename name of file
     */
    static void GetPackageList(TString filename);
    /**
     *
     * @param update update ratio
     * @param collection collection no
     * @param opt option "fast" for getting entries from "fast" cuts
     * @return number of passed objects in given collection
     */
    ULong64_t GetPassedNo(Hal::ECutUpdate update, Int_t collection, Option_t* opt = "slow") const;
    /**
     *
     * @param update update ratio
     * @param collection collection no
     * @param opt option "fast" for getting entries from "fast" cuts
     * @return number of failed objects in given collection
     */
    ULong64_t GetFailedNo(Hal::ECutUpdate update, Int_t collection, Option_t* opt = " slow") const;
    /**
     * export file to HTML
     * @param dirname path to place where report will be created
     * @param task_id if -1 all packages exported, otherwise package with given id
     */
    void ExportHTML(TString dirname = "extr_html", Int_t task_id = -1) const;
    /**
     * print cuts settings
     * @param update - update
     * @param col - collection ID (If -1 print all colllections)
     */
    void PrintCuts(Hal::ECutUpdate update = Hal::ECutUpdate::kNo, Int_t col = -1) const;
    /**
     *
     * @param update
     * @param collection
     * @param cutNo
     * @param type - type of cut (all, slow or fast - for "all" option slow cuts
     * number is shifted by slow cuts number
     * @return package with report from given cut
     */
    Package* GetCutPackage(Hal::ECutUpdate update, Int_t collection, Int_t cutNo, Option_t* type = "all") const;

    /**
     * return name of group list name in file e.g., EventCutCollectionList
     * @param update
     * @return
     */
    static TString GetCollectionListName(Hal::ECutUpdate update);
    /**
     * return name of collection count name in file e.g.f  Event_collections_No
     * @param upd
     * @return
     */
    static TString GetCollectionCountName(Hal::ECutUpdate upd);
    virtual ~AnaFile();
    ClassDef(AnaFile, 1)
  };
}  // namespace Hal

#endif /* HALANAFILE_H_ */
