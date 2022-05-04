/*
 * HalXMLNode.cxx
 *
 *  Created on: 01-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalXMLNode.h"

#include <TDOMParser.h>
#include <TXMLAttr.h>      // for TXMLAttr
#include <TXMLDocument.h>  // for TXMLDocument
#include <TXMLNode.h>
#include <iostream>

HalXMLNode::HalXMLNode(TString name, TString value) : TNamed(name, value) {
  fChildren.SetOwner(kTRUE);
  fAttrib.SetOwner(kTRUE);
}

HalXMLNode::HalXMLNode(const HalXMLNode& other) : HalXMLNode(other.GetName(), other.GetValue()) {
  for (int i = 0; i < other.fChildren.GetEntries(); i++) {
    fChildren.Add(new HalXMLNode(*other.GetChild(i)));
  }
  for (int i = 0; i < other.fAttrib.GetEntries(); i++) {
    fAttrib.Add(new HalXMLAttrib(*other.GetAttrib(i)));
  }
}

HalXMLNode& HalXMLNode::operator=(const HalXMLNode& other) {
  if (&other == this) return *this;
  SetName(other.GetName());
  SetValue(other.GetValue());
  fChildren.Clear();
  fAttrib.Clear();
  for (int i = 0; i < other.fChildren.GetEntries(); i++) {
    fChildren.Add(new HalXMLNode(*other.GetChild(i)));
  }
  for (int i = 0; i < other.fAttrib.GetEntries(); i++) {
    fAttrib.Add(new HalXMLAttrib(*other.GetAttrib(i)));
  }
  return *this;
}

void HalXMLNode::Copy(TXMLNode* node) {
  fChildren.Clear();
  fAttrib.Clear();
  SetName(node->GetNodeName());
  SetTitle(node->GetText());
  if (node->HasChildren()) {
    TXMLNode* child = node->GetChildren();
    do {
      if (child == nullptr) break;
      TString name = child->GetNodeName();
      if (name != "text") {  // skip "text" nodes
        HalXMLNode* tempnode = new HalXMLNode();
        tempnode->Copy(child);
        fChildren.Add(tempnode);
      }
      if (child->HasNextNode()) child = child->GetNextNode();
    } while (child->HasNextNode());
  }
  if (node->HasAttributes()) {
    TList* atr_list = node->GetAttributes();
    for (int i = 0; i < atr_list->GetEntries(); i++) {
      TXMLAttr* atrib = (TXMLAttr*) atr_list->At(i);
      fAttrib.Add(new HalXMLAttrib(atrib->GetName(), atrib->GetValue()));
    }
  }
}

void HalXMLNode::AddAttrib(HalXMLAttrib* attrib) {
  TString new_atr = attrib->GetName();
  if (GetAttrib(new_atr) != nullptr) {
    std::cout << "HalXMLNode::AddAttrib Can't have two attributes with the same name!" << std::endl;
    return;
  }
  fAttrib.AddLast(attrib);
}

Int_t HalXMLNode::GetNChildren(TString name) const {
  Int_t counter = 0;
  for (int i = 0; i < GetNChildren(); i++) {
    TString name_temp = GetChild(i)->GetName();
    if (name_temp == name) { counter++; }
  }
  return counter;
}

HalXMLNode* HalXMLNode::GetChild(TString name, Int_t count) const {
  Int_t control_index = 0;
  for (int i = 0; i < fChildren.GetEntries(); i++) {
    HalXMLNode* node = GetChild(i);
    TString temp     = node->GetName();
    if (temp == name) { control_index++; }
    if (control_index > count) return node;
  }
  return nullptr;
}

HalXMLAttrib* HalXMLNode::GetAttrib(TString name) const { return static_cast<HalXMLAttrib*>(fAttrib.FindObject(name)); }

HalXMLNode* HalXMLNode::GetChild(Int_t index) const { return static_cast<HalXMLNode*>(fChildren.At(index)); }

HalXMLAttrib* HalXMLNode::GetAttrib(Int_t index) const { return static_cast<HalXMLAttrib*>(fAttrib.At(index)); }

HalXMLNode::~HalXMLNode() {}

//---------- HalXMLFile ------------------------------------------------------------------------------------

HalXMLFile::HalXMLFile(TString name, TString mode) : fName(name) {
  if (mode == "read" || mode == "READ") {
    fOverwrite = kFALSE;
    TDOMParser Parser;
    Parser.SetValidate(kFALSE);
    Parser.ParseFile(name);
    TXMLNode* MainNode = Parser.GetXMLDocument()->GetRootNode();
    fRootNode.reset(new HalXMLNode());
    fRootNode->Copy(MainNode);
  } else {
    fOverwrite = kTRUE;
  }
}

void HalXMLFile::CreateRootNode(TString name) { fRootNode.reset(new HalXMLNode(name)); }

void HalXMLFile::SetRootNode(HalXMLNode* node) { fRootNode.reset(node); }

void HalXMLFile::Close() {
  if (fOverwrite) {
    if (!fRootNode) {
      std::cout << "HalXMLFile::Close() No root node!" << std::endl;
      return;
    }
    TXMLEngine engine;
    XMLNodePointer_t mainnode = engine.NewChild(0, 0, fRootNode->GetName());
    ExportNode(mainnode, engine, *fRootNode.get());
    XMLDocPointer_t xmldoc = engine.NewDoc();
    engine.DocSetRootElement(xmldoc, mainnode);
    engine.SaveDoc(xmldoc, fName);
    engine.FreeDoc(xmldoc);
    fRootNode.reset(nullptr);
  }
}

void HalXMLFile::ExportNode(XMLNodePointer_t& nodePointer, TXMLEngine& engine, const HalXMLNode& node) const {
  for (int i = 0; i < node.GetNChildren(); i++) {
    XMLNodePointer_t child = engine.NewChild(nodePointer, 0, node.GetChild(i)->GetName(), node.GetChild(i)->GetValue());
    for (int j = 0; j < node.GetChild(i)->GetNAttributes(); j++) {
      engine.NewAttr(child, 0, node.GetChild(i)->GetAttrib(j)->GetName(), node.GetChild(i)->GetAttrib(j)->GetValue());
    }
    ExportNode(child, engine, *node.GetChild(i));
  }
}

HalXMLFile::~HalXMLFile() {
  if (fRootNode && fOverwrite) Close();
}
