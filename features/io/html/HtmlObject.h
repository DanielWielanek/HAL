/*
 * HalHTMLObject.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_HTML_OBJECT_H_
#define HAL_FEATURES_IO_HTML_OBJECT_H_
#include <TObject.h>
#include <TString.h>
namespace Hal {
  class HtmlObject : public TObject {
    TString fClass;
    TString fID;
    TString fStyle;
    TString fContent;
    TString fOtherAttribs;

  protected:
    TString GetClass() const { return fClass; };
    TString GetId() const { return fID; };
    TString GetStyle() const { return fStyle; };
    TString GetProperties() const;
    /**
     * returns default string like <tag properties="properties">content</tag>
     * @param tag
     * @return
     */
    TString GetDefaultString(TString tag) const;

  public:
    HtmlObject();
    HtmlObject(TString id, TString className, TString style);
    /**
     * set object of this file
     * @param className
     */
    void SetClass(TString className) { fClass = className; };
    /**
     * set default class for given type of objects (if exists)
     */
    virtual void SetDefaultClass() {};
    /**
     * set id of this object
     * @param id
     */
    void SetId(TString id) { fID = id; };
    /**
     * set style of this object
     * @param style
     */
    void SetStyle(TString style) { fStyle = style; };
    /**
     * add content to this object (like content of HTML cell
     * @param obj
     */
    virtual void AddContent(const HtmlObject& obj);
    void SetStringContent(TString content) { fContent = content; };
    TString GetContent() const { return fContent; };
    virtual void AddStringContent(TString text) { fContent = fContent + text; };
    void AddAtrib(TString name, TString value);
    /** convert this object to HTML **/
    virtual TString ToString() const = 0;
    virtual ~HtmlObject();
    HtmlObject(const HtmlObject& other);
    HtmlObject& operator=(const HtmlObject& other);
    ClassDef(HtmlObject, 1)
  };
}  // namespace Hal
#endif /* HAL_FEATURES_IO_HTML_OBJECT_H_ */
