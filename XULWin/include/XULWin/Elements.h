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
        XMLDialog(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLButton : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLButton>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "button";
        }

    private:
        friend class Element;
        XMLButton(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLDescription(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

    class XMLTextBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLTextBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "textbox";
        }

    private:
        friend class Element;
        XMLTextBox(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLCheckBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLCheckBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "checkbox";
        }

    private:
        friend class Element;
        XMLCheckBox(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLBox(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLHBox(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLVBox(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLMenuList : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenuList>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menulist";
        }

    private:
        friend class Element;
        XMLMenuList(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLSeparator(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLSpacer(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLMenuButton(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLGrid(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLRows(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLColumns(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLRow(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLColumn(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLRadioGroup(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLRadio(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLProgressMeter(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLDeck(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLScrollbar : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLScrollbar>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "scrollbar";
        }

        virtual ~XMLScrollbar();

    private:
        friend class Element;
        XMLScrollbar(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTabBox(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTabs(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTab(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTabPanels(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTabPanel(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLGroupBox(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLCaption(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTree(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTreeChildren(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTreeItem(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTreeCols(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTreeCol(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTreeRow(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLTreeCell(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLStatusbar(Element * inParent, const AttributesMapping & inAttributesMapping);
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
        XMLStatusbarPanel(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLToolbar : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLToolbar>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "toolbar";
        }

        virtual ~XMLToolbar();

    private:
        friend class Element;
        XMLToolbar(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLToolbarButton : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLToolbarButton>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "toolbarbutton";
        }

        virtual ~XMLToolbarButton();

    private:
        friend class Element;
        XMLToolbarButton(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // XULWin


#endif // ELEMENTS_H_INCLUDED
