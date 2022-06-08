/*
 * HalHTML.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALHTML_H_
#define HALHTML_H_

#include <TObject.h>
#include <TString.h>

/**
 * class that helps creation of HTML files
 */
class TH1;
class TVector2;
class TGraph;
namespace Hal {
  class HtmlTable;
  class HtmlCore : public TObject {
  private:
    static HtmlCore* fgInstance;
    static TString fMainDir;
    static Bool_t fgOnline;
    static TString GetLinkVector(TVector2* vect);
    static TString GetLinkTH1(TH1* h, Int_t no, TString path);
    static TString GetLinkGraph(TGraph* h, Int_t no, TString path);
    static TString GetLinkList(TList* h, Int_t no, TString path);

  public:
    /**
     * default constructor, that is not used already
     */
    HtmlCore();
    /**
     *
     * @return instance of this class
     */
    static HtmlCore* Instance();
    /**
     *
     * @param root_file path to root file
     * @param object_name  name of displayed object (take care about ;1!)
     * @param draw_opt drawing option
     * @param draw_div_name name of div where plot will be painted (should be
     * unique)
     */
    static TString GetJsDiv(TString root_file, TString object_name, TString draw_opt = "colz", TString draw_div_name = "drawing");
    /**
     * return URL to file
     * @param adress adress of file
     * @param text name of URL
     * @return
     */
    static TString GetUrl(TString adress, TString text);
    /**
     * Create clickable pictures, after each clicking one picture
     * wiil be replaced by next picture.
     * @param id_name name of ocject (shoul be unique on each page)
     * @param strings - array with names of the pictures in TObjString objects,
     * first object will be displayed by default
     * @param width width of picture, if <=0 not used
     * @param height height of picture, if <=0 not used
     * @return line of code that should be included into HTML file
     */
    static TString ClickablePic(TString id_name, TObjArray* strings, Int_t width = 796, Int_t height = 572);
    /**
     *
     * @param h histogram to show
     * @param no number of histogram (avoid overwriting histogram)
     * @param path path to histogram
     * @return
     */
    static TString GetLinkToHistogram(TH1* h, Int_t no, TString path);
    /**
     *
     * @param path path from "core directory" to "this directory"
     * @return relative path to "core directory"
     */
    static TString GetRelativePath(TString path);
    /**
     * returns button tag for hiding table
     * @param tableName id of table to hide
     * @param text text on button
     * @return
     */
    static TString GetHideButtonTable(TString tableName, TString text);
    /**
     * returns button tag for hidding list
     * @param listName id of list
     * @param text text on button
     * @return
     */
    static TString GetHideButtonRow(TString listName, TString text);
    /**
     * fix HTML adress by removing double slashes and add shlash at the end of
     * adress if not present
     * @param address
     */
    static void FixAddress(TString& address);
    /**
     *
     * @return true if online mode is present
     */
    static Bool_t IsOnline() { return fgOnline; }
    static TString GetLastDir(TString name, Int_t cut = 1);
    /**
     *
     * @param online if true - use url from JSroot page, don;t use own JS files
     */
    static void SetOnline(Bool_t online) { fgOnline = online; };
    /**
     * set main diretory. It's place where first CSS file and javascritp files are
     * created. So it should be set only once per one exporting of
     * file/object/taks into HTML. Other objects are inside of main dir - so
     * setting value for them is not necessary
     *
     *
     *  only once - and after extraction of data
     * this value shold be resteed.
     * @param name
     */
    static void SetMainDir(TString name);
    /**
     * set this directory as a main and extract there Jss and css files, should be used together with HTMLEXtract for single
     * objects
     * @param name
     */
    static void SetMainDirAndExtract(TString name);
    static void ResetMainDir() { fMainDir = ""; };
    /**
     * copy css file into specified place
     * @param dir target place
     * @param css_name name of table with template, currently default value should
     * be used
     */
    static void CopyCss(TString dir, TString css_name = "table.css");
    /**
     * copy Javascript files to given directory
     * @param dir
     */
    static void CopyJs(TString dir);
    /**
     * extract this object into HTML
     * @param obj object to export
     * @param no (used to numbering directories to avoid name conflicts)
     * @param dir directory where object should be extracted
     * @return value or Url to html with report
     */
    static TString HTMLExtract(TObject* obj, Int_t no, TString dir = "");
    /**
     * extract this object into html table table, currently this function is not used
     * @param obj object to extract
     * @param no object number
     * @param table table to extract this object
     * @param dir directory to extract the strucuture of object
     * @param rel_dir relative path from this to dir
     */
    static void HTMLExtractIntoTable(TObject* obj, Int_t no, HtmlTable& table, TString dir = "", TString rel_dir = "");
    /**
     * TODO
     * @param obj
     * @return true of object can be table element
     *
     */
    virtual Bool_t CanBeTableElement(TObject* obj);
    virtual ~HtmlCore() {};
    ClassDef(HtmlCore, 1)
  };
}  // namespace Hal
#endif
