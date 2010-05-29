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


    class WindowElement;
    class DialogElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<DialogElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "dialog";
        }

        DialogResult showModal(WindowElement * inInvoker);

        void endModal(DialogResult inDialogResult);

    private:
        friend class Element;
        DialogElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ButtonElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ButtonElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "button";
        }

    private:
        friend class Element;
        ButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class DescriptionElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<DescriptionElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "description";
        }

        virtual bool init();

    private:
        friend class Element;
        DescriptionElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

    class TextBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TextBoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "textbox";
        }

    private:
        friend class Element;
        TextBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class CheckBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<CheckBoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "checkbox";
        }

    private:
        friend class Element;
        CheckBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class BoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<BoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "box";
        }
    private:
        friend class Element;
        BoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class HBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<HBoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "hbox";
        }

    private:
        friend class Element;
        HBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class VBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<VBoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "vbox";
        }

    private:
        friend class Element;
        VBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class MenuListElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<MenuListElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menulist";
        }

    private:
        friend class Element;
        MenuListElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SeparatorElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SeparatorElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "separator";
        }

        virtual ~SeparatorElement();

    private:
        friend class Element;
        SeparatorElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SpacerElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SpacerElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "spacer";
        }

        virtual ~SpacerElement();

    private:
        friend class Element;
        SpacerElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class MenuButtonElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<MenuButtonElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menubutton";
        }

        virtual ~MenuButtonElement();

    private:
        friend class Element;
        MenuButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class RowsElement;
    class ColumnsElement;
    class GridElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<GridElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "grid";
        }

        virtual ~GridElement();

        //void setRows(const RowsElement & inRows);

        //void setColumns(const ColumnsElement & inColumns);

    private:
        friend class Element;
        GridElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class RowsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<RowsElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "rows";
        }

        virtual ~RowsElement();

    private:
        friend class Element;
        RowsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ColumnElement;
    class ColumnsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ColumnsElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "columns";
        }

        virtual ~ColumnsElement();

        //void addColumn(const ColumnElement & inColumn);

    private:
        friend class Element;
        ColumnsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class RowElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<RowElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "row";
        }

        virtual ~RowElement();

    private:
        friend class Element;
        RowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ColumnElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ColumnElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "column";
        }

        virtual ~ColumnElement();

    private:
        friend class Element;
        ColumnElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };



    class RadioGroupElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<RadioGroupElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "radiogroup";
        }

        virtual ~RadioGroupElement();

    private:
        friend class Element;
        RadioGroupElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };



    class RadioElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<RadioElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "radio";
        }

        virtual ~RadioElement();

    private:
        friend class Element;
        RadioElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ProgressMeterElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ProgressMeterElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "progressmeter";
        }

        virtual ~ProgressMeterElement();

    private:
        friend class Element;
        ProgressMeterElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class DeckElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<DeckElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "deck";
        }

        virtual ~DeckElement();

    private:
        friend class Element;
        DeckElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ScrollbarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ScrollbarElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "scrollbar";
        }

        virtual ~ScrollbarElement();

    private:
        friend class Element;
        ScrollbarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TabBoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabbox";
        }

        virtual ~TabBoxElement();

    private:
        friend class Element;
        TabBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TabsElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabs";
        }

        virtual ~TabsElement();

        //void addRow(const RowElement & inRow);

    private:
        friend class Element;
        TabsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TabElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tab";
        }

        virtual ~TabElement();

    private:
        friend class Element;
        TabElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabPanelElement;
    class TabPanelsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TabPanelsElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabpanels";
        }

        virtual ~TabPanelsElement();

    private:
        friend class Element;
        TabPanelsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabPanelElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TabPanelElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "tabpanel";
        }

        virtual ~TabPanelElement();

    private:
        friend class Element;
        TabPanelElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class GroupBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<GroupBoxElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "groupbox";
        }

    private:
        friend class Element;
        GroupBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class CaptionElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<CaptionElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "caption";
        }

    private:
        friend class Element;
        CaptionElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeElement>(inParent, inAttr);
        }

        virtual ~TreeElement();

        static const char * TagName()
        {
            return "tree";
        }

    private:
        friend class Element;
        TreeElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeChildrenElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeChildrenElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treechildren";
        }

        virtual ~TreeChildrenElement();

    private:
        friend class Element;
        TreeChildrenElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeItemElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeItemElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treeitem";
        }

        virtual ~TreeItemElement();

    private:
        friend class Element;
        TreeItemElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeColsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeColsElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treecols";
        }

        virtual ~TreeColsElement();

    private:
        friend class Element;
        TreeColsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeColElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeColElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treecol";
        }

        virtual ~TreeColElement();

    private:
        friend class Element;
        TreeColElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeRowElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeRowElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treerow";
        }

        virtual ~TreeRowElement();

    private:
        friend class Element;
        TreeRowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeCellElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TreeCellElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "treecell";
        }

        virtual ~TreeCellElement();

    private:
        friend class Element;
        TreeCellElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class StatusbarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<StatusbarElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "statusbar";
        }

        virtual ~StatusbarElement();

    private:
        friend class Element;
        StatusbarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class StatusbarPanelElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<StatusbarPanelElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "statusbarpanel";
        }

        virtual ~StatusbarPanelElement();

    private:
        friend class Element;
        StatusbarPanelElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ToolbarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ToolbarElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "toolbar";
        }

        virtual ~ToolbarElement();

    private:
        friend class Element;
        ToolbarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ToolbarButtonElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ToolbarButtonElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "toolbarbutton";
        }

        virtual ~ToolbarButtonElement();

    private:
        friend class Element;
        ToolbarButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // XULWin


#endif // ELEMENTS_H_INCLUDED
