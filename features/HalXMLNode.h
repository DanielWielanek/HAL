/*
 * HalXLMNode.h from HalROOT
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALXLMNODE_H_
#define HALXLMNODE_H_

#include <Rtypes.h>
#include <TList.h>
#include <TNamed.h>
#include <TObject.h>
#include <TString.h>
#include <TXMLEngine.h>
#include <memory>

class TXMLNode;

/**
 * class for representing node attributes
 */
class HalXMLAttrib : public TNamed {
public:
  /**
   * default constructor
   */
  HalXMLAttrib() : TNamed() {}
  /**
   * default constructor
   * @param name name of attribute
   * @param value value of attribute
   */
  HalXMLAttrib(TString name, TString value) {
    SetName(name);
    SetTitle(value);
  }
  /**
   *
   * @return value of node
   */
  TString GetValue() const { return GetTitle(); };
  /**
   *
   * @param val value of node to se
   */
  void SetValue(TString val) { SetTitle(val); };
  virtual ~HalXMLAttrib() {};
  ClassDef(HalXMLAttrib, 1);
};
/**
 * class for representing XML node
 */
class HalXMLNode : public TNamed {
  TList fChildren;
  TList fAttrib;

public:
  /**
   * copy constructor
   * @param other
   */
  HalXMLNode(const HalXMLNode& other);
  /**
   * default constructor
   * @param name name of node
   * @param value value of node
   */
  HalXMLNode(TString name = "", TString value = "");
  /**
   * assignement operator
   * @param node
   * @return
   */
  HalXMLNode& operator=(const HalXMLNode& other);
  /**
   * copy data for node to this
   * @param node
   */
  void Copy(TXMLNode* node);
  /**
   *
   * @param value new value
   */
  void SetValue(TString value) { SetTitle(value); };
  /**
   *  add child node to this node, node is now owned by
   *  parent node
   * @param node node to add
   */
  void AddChild(HalXMLNode* node) { fChildren.AddLast(node); };
  /**
   * add attribute to this class
   * @param attrib
   */
  void AddAttrib(HalXMLAttrib* attrib);
  /**
   *
   * @return number of childen nodes
   */
  Int_t GetNChildren() const { return fChildren.GetEntries(); };
  /**
   *
   * @return number of attributes
   */
  Int_t GetNAttributes() const { return fAttrib.GetEntries(); };
  /**
   *  search for child with given name
   * @param name name of node
   * @return number of node with given name
   */
  Int_t GetNChildren(TString name) const;
  /**
   *
   * @return value of node
   */
  TString GetValue() const { return GetTitle(); };
  /**
   *
   * @param name name of atribute
   * @return
   */
  HalXMLAttrib* GetAttrib(TString name) const;
  /**
   *
   * @param index index of atrribute
   * @return
   */
  HalXMLAttrib* GetAttrib(Int_t index) const;
  /**
   *  search for node with given name
   * @param name name of node
   * @param count number of node (if more than one with given name exist)
   * @return node
   */
  HalXMLNode* GetChild(TString name, Int_t count = 0) const;
  /**
   *
   * @param index child number
   * @return child at given position
   */
  HalXMLNode* GetChild(Int_t index) const;
  virtual ~HalXMLNode();
  ClassDef(HalXMLNode, 1);
};

/**
 * class for opening XML files
 */
class HalXMLFile : public TObject {
  std::unique_ptr<HalXMLNode> fRootNode;
  TString fName;
  Bool_t fOverwrite;
  void ExportNode(XMLNodePointer_t& nodePointer, TXMLEngine& engine, const HalXMLNode& node) const;

public:
  /**
   *
   * @param name name of xml file
   * @param mode if "READ" or "read" - only read file, otherwise create /overwrite file
   */
  HalXMLFile(TString name = "", TString mode = "read");
  /**
   * create new root node
   * @param name name of new node
   */
  void CreateRootNode(TString name);
  /**
   * set new root node, old node will be overwritten
   * @param node root node
   */
  void SetRootNode(HalXMLNode* node);
  /**
   *
   * @return root node
   */
  HalXMLNode* GetRootNode() const { return fRootNode.get(); };
  /**
   * close and write xml (if needed)
   */
  void Close();
  /**
   * destroy object (and save xml file if needed and Close was not called)
   */
  virtual ~HalXMLFile();
  ClassDef(HalXMLFile, 1);
};

#endif /* HalXLMNODE_H_ */
