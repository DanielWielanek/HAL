/*
 * XMLNode.cxx
 *
 *  Created on: 01-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "XMLNode.h"

#include <TDOMParser.h>
#include <TXMLAttr.h>      // for TXMLAttr
#include <TXMLDocument.h>  // for TXMLDocument
#include <TXMLNode.h>
#include <iostream>

namespace Hal {
  XMLNode::XMLNode(TString name, TString value) : TNamed(name, value), fNull(kFALSE) {}

  XMLNode::XMLNode(TString name, TString value, Bool_t null) : TNamed(name, value), fNull(null) {}

  XMLNode::XMLNode(const XMLNode& other) : XMLNode(other.GetName(), other.GetValue(), other.fNull) {
    for (int i = 0; i < (int) other.fChildren.size(); i++) {
      fChildren.push_back(XMLNode(other.GetChild(i)));
    }
    for (int i = 0; i < (int) other.fAttrib.size(); i++) {
      fAttrib.push_back(XMLAttrib(other.GetAttrib(i)));
    }
  }

  XMLNode& XMLNode::operator=(const XMLNode& other) {
    if (&other == this) return *this;
    SetName(other.GetName());
    SetValue(other.GetValue());
    fChildren.clear();
    fAttrib.clear();
    for (int i = 0; i < (int) other.fChildren.size(); i++) {
      fChildren.push_back(XMLNode(other.GetChild(i)));
    }
    for (int i = 0; i < (int) other.fAttrib.size(); i++) {
      fAttrib.push_back(XMLAttrib(other.GetAttrib(i)));
    }
    return *this;
  }

  XMLNode& XMLNode::operator[](int i) {
    if (i < (int) fChildren.size()) return fChildren[i];
    return NullNode();
  }

  XMLNode& XMLNode::operator[](const char* key) {
    TString keys = key;
    for (auto& i : fChildren) {
      if (i.GetStrName() == keys) return i;
    }
    return NullNode();  // just to be compatible
  }

  const XMLNode& XMLNode::operator[](int i) const { return fChildren[i]; }

  const XMLNode& XMLNode::operator[](const char* key) const {
    TString keys = key;
    for (auto& i : fChildren) {
      if (i.GetStrName() == keys) return i;
    }
    return NullNode();  // just to be compatible
  }
  void XMLNode::Copy(TXMLNode* node) {
    fChildren.clear();
    fAttrib.clear();
    SetName(node->GetNodeName());
    SetTitle(node->GetText());
    if (node->HasChildren()) {
      TXMLNode* child = node->GetChildren();
      do {
        if (child == nullptr) break;
        TString name = child->GetNodeName();
        if (name != "text") {  // skip "text" nodes
          XMLNode tempnode;
          tempnode.Copy(child);
          fChildren.push_back(tempnode);
        }
        if (child->HasNextNode()) child = child->GetNextNode();
      } while (child->HasNextNode());
    }
    if (node->HasAttributes()) {
      TList* atr_list = node->GetAttributes();
      for (int i = 0; i < atr_list->GetEntries(); i++) {
        TXMLAttr* atrib = (TXMLAttr*) atr_list->At(i);
        fAttrib.push_back(XMLAttrib(atrib->GetName(), atrib->GetValue()));
      }
    }
  }

  void XMLNode::AddChild(const XMLNode& node) { fChildren.push_back(node); }

  void XMLNode::AddAttrib(const XMLAttrib& attrib) { fAttrib.push_back(attrib); }

  void XMLNode::AddAttrib(TString name, TString value) { fAttrib.push_back(XMLAttrib(name, value)); }

  void XMLNode::AddAttribs(const std::vector<std::vector<TString>> attribs) {
    for (auto x : attribs) {
      if (x.size() == 2) { AddAttrib(x[0], x[1]); }
    }
  }

  Int_t XMLNode::GetNChildren(TString name) const {
    int count = 0;
    for (auto i : fChildren) {
      if (i.GetStrName() == name) count++;
    }
    return count;
  }

  const XMLAttrib& XMLNode::GetAttrib(TString name) const {
    for (auto& i : fAttrib) {
      if (i.GetStrName() == name) return i;
    }
    const XMLAttrib& x = XMLAttrib::NullAttrib();
    return x;
  }

  const XMLAttrib& XMLNode::GetAttrib(Int_t index) const { return fAttrib[index]; }

  const XMLNode& XMLNode::GetChild(TString name, Int_t count) const {
    int counter = -1;
    for (auto& i : fChildren) {
      if (i.GetStrName() == name) counter++;
      if (counter == count) return i;
    }
    return NullNode();
  }

  const XMLNode& XMLNode::GetChild(Int_t index) const { return fChildren[index]; }

  XMLAttrib& XMLNode::GetAttrib(TString name) {
    for (auto& i : fAttrib) {
      if (i.GetStrName() == name) return i;
    }
    return XMLAttrib::NullAttrib();
  }

  XMLAttrib& XMLNode::GetAttrib(Int_t index) { return fAttrib[index]; }

  XMLNode& XMLNode::GetChild(TString name, Int_t count) {
    int counter = -1;
    for (auto& i : fChildren) {
      if (i.GetStrName() == name) counter++;
      if (counter == count) return i;
    }
    return NullNode();
  }

  XMLNode& XMLNode::GetChild(Int_t index) { return fChildren[index]; }

  XMLNode& XMLNode::NullNode() {
    static XMLNode nullnode("", "", true);
    return nullnode;
  }

  XMLNode::~XMLNode() {}

  //---------- HalXMLFile ------------------------------------------------------------------------------------

  XMLFile::XMLFile(TString name, TString mode) : fName(name) {
    if (mode == "read" || mode == "READ") {
      fOverwrite = kFALSE;
      TDOMParser Parser;
      Parser.SetValidate(kFALSE);
      Parser.ParseFile(name);
      TXMLNode* MainNode = Parser.GetXMLDocument()->GetRootNode();
      fRootNode.Copy(MainNode);
    } else {
      fRootNode  = XMLNode::NullNode();
      fOverwrite = kTRUE;
    }
  }

  void XMLFile::CreateRootNode(TString name) { fRootNode = XMLNode(name); }

  void XMLFile::SetRootNode(const XMLNode& node) { fRootNode = node; }

  void XMLFile::Close() {
    if (fOverwrite) {
      if (!fRootNode) {
        std::cout << "HalXMLFile::Close() No root node!" << std::endl;
        return;
      }
      TXMLEngine engine;
      XMLNodePointer_t mainnode = engine.NewChild(0, 0, fRootNode.GetName());
      ExportNode(mainnode, engine, fRootNode);
      XMLDocPointer_t xmldoc = engine.NewDoc();
      engine.DocSetRootElement(xmldoc, mainnode);
      engine.SaveDoc(xmldoc, fName);
      engine.FreeDoc(xmldoc);
    }
  }

  void XMLFile::ExportNode(XMLNodePointer_t& nodePointer, TXMLEngine& engine, const XMLNode& node) const {
    for (int i = 0; i < node.GetNChildren(); i++) {
      XMLNodePointer_t child = engine.NewChild(nodePointer, 0, node.GetChild(i).GetName(), node.GetChild(i).GetValue());
      for (int j = 0; j < node.GetChild(i).GetNAttributes(); j++) {
        engine.NewAttr(child, 0, node.GetChild(i).GetAttrib(j).GetName(), node.GetChild(i).GetAttrib(j).GetValue());
      }
      ExportNode(child, engine, node.GetChild(i));
    }
  }

  XMLFile::~XMLFile() {
    if (!fRootNode.IsNull() && fOverwrite) Close();
  }


}  // namespace Hal
