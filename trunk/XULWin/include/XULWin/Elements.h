#ifndef ELEMENTS_H_INCLUDED
#define ELEMENTS_H_INCLUDED


#include "XULWin/AttributesMapping.h"
#include "XULWin/Element.h"
#include "XULWin/Enums.h"
#include "XULWin/ForwardDeclarations.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>


namespace XULWin
{

    class ElementFactory;
    class Component;
    class Element;
    class EventListener;
    typedef boost::shared_ptr<Element> ElementPtr;
    typedef std::vector<ElementPtr> Children;


    class XMLWindow;
    class XMLDialog : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLDialog>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "dialog";
        }

        DialogResult showModal(XMLWindow * inInvoker);

        void endModal(DialogResult inDialogResult);

    private:
        friend class Element;
        XMLDialog(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLDescription : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLDescription>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "description";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLDescription(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "box";
        }
    private:
        friend class Element;
        XMLBox(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLHBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLHBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "hbox";
        }

    private:
        friend class Element;
        XMLHBox(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLVBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLVBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "vbox";
        }

    private:
        friend class Element;
        XMLVBox(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLSeparator : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSeparator>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "separator";
        }

        virtual ~XMLSeparator();

    private:
        friend class Element;
        XMLSeparator(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLSpacer : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSpacer>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "spacer";
        }

        virtual ~XMLSpacer();

    private:
        friend class Element;
        XMLSpacer(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLMenuButton : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenuButton>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menubutton";
        }

        virtual ~XMLMenuButton();

    private:
        friend class Element;
        XMLMenuButton(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLRows;
    class XMLColumns;
    class XMLGrid : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLGrid>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "grid";
        }

        virtual ~XMLGrid();

        //void setRows(const XMLRows & inRows);

        //void setColumns(const XMLColumns & inColumns);

    private:
        friend class Element;
        XMLGrid(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLRows : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLRows>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "rows";
        }

        virtual ~XMLRows();

    private:
        friend class Element;
        XMLRows(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLColumn;
    class XMLColumns : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLColumns>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "columns";
        }

        virtual ~XMLColumns();

        //void addColumn(const XMLColumn & inColumn);

    private:
        friend class Element;
        XMLColumns(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLRow : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLRow>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "row";
        }

        virtual ~XMLRow();

    private:
        friend class Element;
        XMLRow(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLColumn : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLColumn>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "column";
        }

        virtual ~XMLColumn();

    private:
        friend class Element;
        XMLColumn(Element * inParent, const AttributesMapping & inAttr);
    };



    class XMLRadioGroup : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLRadioGroup>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "radiogroup";
        }

        virtual ~XMLRadioGroup();

    private:
        friend class Element;
        XMLRadioGroup(Element * inParent, const AttributesMapping & inAttr);
    };



    class XMLRadio : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLRadio>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "radio";
        }

        virtual ~XMLRadio();

    private:
        friend class Element;
        XMLRadio(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLProgressMeter : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLProgressMeter>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "progressmeter";
        }

        virtual ~XMLProgressMeter();

    private:
        friend class Element;
        XMLProgressMeter(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLDeck : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLDeck>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "deck";
        }

        virtual ~XMLDeck();

    private:
        friend class Element;
        XMLDeck(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTabBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTabBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabbox";
        }

        virtual ~XMLTabBox();

    private:
        friend class Element;
        XMLTabBox(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTabs : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTabs>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabs";
        }

        virtual ~XMLTabs();

        //void addRow(const XMLRow & inRow);

    private:
        friend class Element;
        XMLTabs(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTab : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTab>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tab";
        }

        virtual ~XMLTab();

    private:
        friend class Element;
        XMLTab(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTabPanel;
    class XMLTabPanels : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTabPanels>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabpanels";
        }

        virtual ~XMLTabPanels();

    private:
        friend class Element;
        XMLTabPanels(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTabPanel : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTabPanel>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabpanel";
        }

        virtual ~XMLTabPanel();

    private:
        friend class Element;
        XMLTabPanel(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLGroupBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLGroupBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "groupbox";
        }

    private:
        friend class Element;
        XMLGroupBox(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLCaption : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLCaption>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "caption";
        }

    private:
        friend class Element;
        XMLCaption(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTree : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTree>(inParent, inAttr);
        }

        virtual ~XMLTree();

        static const char * TagName()
        {
            return "tree";
        }

    private:
        friend class Element;
        XMLTree(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTreeChildren : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTreeChildren>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treechildren";
        }

        virtual ~XMLTreeChildren();

    private:
        friend class Element;
        XMLTreeChildren(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTreeItem : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTreeItem>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treeitem";
        }

        virtual ~XMLTreeItem();

    private:
        friend class Element;
        XMLTreeItem(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTreeCols : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTreeCols>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treecols";
        }

        virtual ~XMLTreeCols();

    private:
        friend class Element;
        XMLTreeCols(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTreeCol : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTreeCol>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treecol";
        }

        virtual ~XMLTreeCol();

    private:
        friend class Element;
        XMLTreeCol(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTreeRow : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTreeRow>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treerow";
        }

        virtual ~XMLTreeRow();

    private:
        friend class Element;
        XMLTreeRow(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLTreeCell : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTreeCell>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treecell";
        }

        virtual ~XMLTreeCell();

    private:
        friend class Element;
        XMLTreeCell(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLStatusbar : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLStatusbar>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "statusbar";
        }

        virtual ~XMLStatusbar();

    private:
        friend class Element;
        XMLStatusbar(Element * inParent, const AttributesMapping & inAttr);
    };


    class XMLStatusbarPanel : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLStatusbarPanel>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "statusbarpanel";
        }

        virtual ~XMLStatusbarPanel();

    private:
        friend class Element;
        XMLStatusbarPanel(Element * inParent, const AttributesMapping & inAttr);
    };


} // XULWin


#endif // ELEMENTS_H_INCLUDED
