/*
 * HalPackage.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALPACKAGE_H_
#define HALPACKAGE_H_

#include <TObjString.h>
#include <TString.h>


/**
 * Class for storing package. Package is container for storing data.
 *  In contrast to HalPack its not one large structure but rather bunch of
 * objects. Forr example each cut reate own report in form of HalPackage. Thank
 * to this saving cuts is avoided (what could lead to problems because cuts are
 * frequently changed and framework-dependent.
 */
namespace Hal {

  class Package : public TObject {
    TObjArray* fArrayObjects;
    TObjString fNameClass;
    TObjString fComment;
    Int_t fNo;
    Int_t fMerged;
    void MergeObjects(TObject* first, const TObject* second);
    void SumHalParameter(TObject* first, const TObject* second);
    void SumHistograms(TObject* first, const TObject* second);
    void SumOther(TObject* first, const TObject* second);
    void SumPackages(TObject* first, const TObject* second);
    void SumList(TObject* first, const TObject* second);
    void SumPack(TObject* first, const TObject* second);
    void SumTObjString(TObject* first, const TObject* second);
    // void MergeArrayElements(TObject *first, TObject *second);
    void PrintInfo(Int_t counter, Int_t deep) const;
    void PrintStruct(Int_t i, Int_t index) const;

  public:
    /**
     * basic constructor
     */
    Package();
    /**
     *  create class from object
     * @param object object that give name for this class
     * @param cut_template if true then only first part of name is used as class
     * name (e.g. ClassA_A will be named as ClassA
     */
    Package(const TObject* object, Bool_t cut_template = kFALSE);
    /**
     * add object to array of objects
     * @param object object to add
     */
    void AddObject(TObject* object);
    /**
     * add object of the same structure to this
     * @param pack pacakge to add
     */
    void Add(const Package* pack);
    /**
     * used to merge similar objects
     * @param collection
     */
    virtual Long64_t Merge(TCollection* collection);
    /**
     * overwrite name of class that is creator of this package
     * @param name new name of parent class for this object
     */
    void SetName(TString name);
    /**
     * set comment to this package
     * @param name comment
     */
    void SetComment(TString name);
    /**
     * print info about this object
     * @param max_deep if 0 then objects that have internal structure don print
     * their inner objects
     */
    void PrintInfo(Int_t max_deep = 0) const;
    /**
     * print structure of this object without additional information
     */
    void PrintStruct() const;
    /**
     * add number to name of parent class (e.g. parentClass_1
     * @param counter number to add
     */
    void SetAdditionalCounter(Int_t counter);
    /**
     * remove object by name
     * @param name name of object
     * @param index index (if more than one object with the same name exist)
     * @param delete_obj if true the destructor for removed object is called
     */
    void RemoveObjectByName(TString name, Int_t index = 0, Bool_t delete_obj = kTRUE);
    /**
     * used to by TBrowser
     * @param b
     */
    virtual void Browse(TBrowser* b);
    /**
     *
     * @return number objects inside this object
     */
    Int_t GetEntries() const;
    /**
     *
     * @return number of merging this object if 1 then object has never been
     * merged
     */
    Int_t GetMerged() const;
    /**
     *
     * @param name object name
     * @param index index of object (if few with the same name exists)
     * @param opt if " " search by name and index, if "classname" find by
     * classname not name of object if "index" search only by index
     * @return true if object exist
     */
    Bool_t Exist(TString name, Int_t index = 0, Option_t* opt = " ") const;
    /**
     *
     * @param i object index
     * @return found ojbect
     */
    TObject* GetObject(Int_t i) const;
    /**
     * search for object  by name
     * @param name name of object
     * @param index index of object (if few have the same name)
     * @return found object
     */
    TObject* GetObjectByName(TString name, Int_t index = 0) const;
    /**
     * search for object
     * @param name class name of object
     * @param index used if few object have the same classname
     * @return found object
     */
    TObject* GetObjectByClassName(TString name, Int_t index = 0) const;
    /**
     *
     * @return GetClassName()
     */
    const char* GetName() const;
    /**
     *
     * @return comment
     */
    TString GetComment() const;
    /**
     * create package from package
     * @return created packages
     */
    Package* Report() const;
    /**
     * add objects
     * @param rhs object to add
     * @return this
     */
    Package& operator+(const Package& rhs);
    virtual ~Package();
    ClassDef(Package, 1)
  };
}  // namespace Hal

#endif /* HALPACKAGE_H_ */
