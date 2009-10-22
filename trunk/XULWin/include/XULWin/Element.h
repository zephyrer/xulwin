#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED


#include "XULWin/AttributesMapping.h"
#include "XULWin/Enums.h"
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


    /**
     * Base class XUL elements.
     * Class Element provides a string-based interface for UI components.
     * For a C++ interface you need to get the Component object using the 
     * component() method.
     */
    class Element : private boost::noncopyable
    {
    public:

        /**
         * INITIALIZATION ORDER
         * 
         * PART 1: Parser encounters the OPENING tag of a new element:
         * - Element constructor
         * - Construction of any decorators for Component
         * - Component constructor
         * - Element::initAttributeControllers
         * - Element::setAttributes
         * - Element::initStyleControllers
         * - Element::setStyles
         * - virtual Element::addChild (add element to its parent)
         * - virtual Component::onChildAdded notification
         * 
         * PART 2: Creation and initialization of any child elements.
         *
         * PART 3: Parser finds CLOSING tag of the element:
         * - virtual Element::init
         * - virtual Component::init
         * 
         */  
        template<class ElementType>
        static ElementPtr Create(Element * inParent,
                                 const AttributesMapping & inAttr)
        {
            ElementPtr result(new ElementType(inParent, inAttr));
            result->initAttributeControllers();
            result->setAttributes(inAttr);
            result->initStyleControllers();
            result->setStyles(inAttr);
            if (inParent)
            {
                // The addChild() method also calls Component::onChildAdded
                inParent->addChild(result);                
            }
            return result;
        }

        ~Element();

        // called by parser at end-element event
        virtual bool init();

        // you don't need to call this, Element::Create() does it
        virtual void addChild(ElementPtr inChild);

        // highly volatile, use at your own risk
        virtual void removeChild(const Element * inChild);

        const std::string & type() const;

        Element * parent() const { return mParent; }

        // Gets the attribute value from the attribute controller.
        // If no attribute controller was found then it will search
        // the attribute mapping.
        // Returns empty string if not found.
        std::string getAttribute(const std::string & inName) const;

        // Gets the attribute from the attribute mapping
        // without querying the attribute controllers.
        // Returns empty string if not found.
        std::string getDocumentAttribute(const std::string & inName) const;

        // Gets the CSS property from the style controller.
        // If no style controller was found then it will search
        // the style mapping.
        // Returns empty string if not found.
        std::string getStyle(const std::string & inName) const;

        // Sets the attribute by invoking the attribute controller.
        // If no attribute controller found it will insert (or overwrite)
        // the value in the attributes mapping.
        void setAttribute(const std::string & inName, const std::string & inValue);

        void setInnerText(const std::string & inText);

        const std::string & innerText() const;

        Element * getElementById(const std::string & inId);

        void getElementsByType(const std::string & inType, std::vector<Element*> & outElements);

        template<class ElementType>
        void getElementsByType(std::vector<ElementType*> & outElements)
        {
            if (ElementType * element = downcast<ElementType>())
            {
                outElements.push_back(element);
            }
            for (size_t idx = 0; idx != mChildren.size(); ++idx)
            {
                mChildren[idx]->getElementsByType<ElementType>(outElements);
            }
        }

        const Children & children() const { return mChildren; }

        bool addEventListener(EventListener * inEventListener);

        bool removeEventListener(EventListener * inEventListener);

        Component * component() const;

        template<class ElementType>
        const ElementType * downcast() const
        {
            if (type() == ElementType::Type())
            {
                return static_cast<ElementType*>(this);
            }
            return 0;
        }

        template<class ElementType>
        ElementType * downcast()
        {
            if (type() == ElementType::Type())
            {
                return static_cast<ElementType*>(this);
            }
            return 0;
        }


        // Searches for a child of given type.
        // Returns the first one found.
        // Only searches one level deep.
        template<class ElementType>
        ElementType * findChildOfType()
        {
            return const_cast<ElementType*>(static_cast<const Element*>(this)->findChildOfType<ElementType>());
        }

        template<class ElementType>
        const ElementType * findChildOfType() const
        {
            for (size_t idx = 0; idx != mChildren.size(); ++idx)
            {
                if (ElementType * res = mChildren[idx]->downcast<ElementType>())
                {
                    return res;
                }
            }
            return 0;
        }

    protected:
        Element(const std::string & inType, Element * inParent, Component * inNative);

        Element * mParent;
        Children mChildren;
        AttributesMapping mAttributes;

    private:
        void setAttributes(const AttributesMapping & inAttributes);

        void setStyle(const std::string & inName, const std::string & inValue);

        void setStyles(const AttributesMapping & inAttributes);

        void initAttributeControllers();

        void initStyleControllers();

        friend class ElementFactory;
        std::string mType;
        StylesMapping mStyles;
        std::string mInnerText;
        boost::scoped_ptr<Component> mComponent;
    };


    class WindowElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<WindowElement>(inParent, inAttr); }

        static const char * Type() { return "window"; }


        enum Positioning
        {
            DefaultPosition,
            CenterInScreen
        };

        void show(Positioning inPositioning);

        void showModal(Positioning inPositioning);

        void close();

    private:
        friend class Element;
        WindowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    enum DialogResult
    {
        DialogResult_Error = -1,
        DialogResult_Ok,
        DialogResult_Cancel,
        DialogResult_Custom
    };


    class DialogElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<DialogElement>(inParent, inAttr); }

        static const char * Type() { return "dialog"; }

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
        { return Element::Create<ButtonElement>(inParent, inAttr); }

        static const char * Type() { return "button"; }
    
    private:
        friend class Element;
        ButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class LabelElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<LabelElement>(inParent, inAttr); }

        static const char * Type() { return "label"; }
    
    private:
        friend class Element;
        LabelElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class DescriptionElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<DescriptionElement>(inParent, inAttr); }

        static const char * Type() { return "description"; }

        virtual bool init();
    
    private:
        friend class Element;
        DescriptionElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    // TextElement is the same as label
    class TextElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TextElement>(inParent, inAttr); }

        static const char * Type() { return "text"; }
    
    private:
        friend class Element;
        TextElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TextBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TextBoxElement>(inParent, inAttr); }

        static const char * Type() { return "textbox"; }
    
    private:
        friend class Element;
        TextBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class CheckBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<CheckBoxElement>(inParent, inAttr); }

        static const char * Type() { return "checkbox"; }

    private:
        friend class Element;
        CheckBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class BoxElement : public Element
    {
    public: 
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<BoxElement>(inParent, inAttr); }

        static const char * Type() { return "box"; }
    private:
        friend class Element;
        BoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class HBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<HBoxElement>(inParent, inAttr); }

        static const char * Type() { return "hbox"; }

    private:
        friend class Element;
        HBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class VBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<VBoxElement>(inParent, inAttr); }

        static const char * Type() { return "vbox"; }

    private:
        friend class Element;
        VBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class MenuListElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuListElement>(inParent, inAttr); }

        static const char * Type() { return "menulist"; }

    private:
        friend class Element;
        MenuListElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SeparatorElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<SeparatorElement>(inParent, inAttr); }

        static const char * Type() { return "separator"; }

        virtual ~SeparatorElement();

    private:
        friend class Element;
        SeparatorElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SpacerElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<SpacerElement>(inParent, inAttr); }

        static const char * Type() { return "spacer"; }

        virtual ~SpacerElement();

    private:
        friend class Element;
        SpacerElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class MenuButtonElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuButtonElement>(inParent, inAttr); }

        static const char * Type() { return "menubutton"; }

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
        { return Element::Create<GridElement>(inParent, inAttr); }

        static const char * Type() { return "grid"; }

        virtual ~GridElement();

        //void setRows(const RowsElement & inRows);

        //void setColumns(const ColumnsElement & inColumns);

    private:
        friend class Element;
        GridElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class RowElement;
    class RowsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<RowsElement>(inParent, inAttr); }

        static const char * Type() { return "rows"; }

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
        { return Element::Create<ColumnsElement>(inParent, inAttr); }

        static const char * Type() { return "columns"; }

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
        { return Element::Create<RowElement>(inParent, inAttr); }

        static const char * Type() { return "row"; }

        virtual ~RowElement();

    private:
        friend class Element;
        RowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ColumnElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ColumnElement>(inParent, inAttr); }

        static const char * Type() { return "column"; }

        virtual ~ColumnElement();

    private:
        friend class Element;
        ColumnElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };



    class RadioGroupElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<RadioGroupElement>(inParent, inAttr); }

        static const char * Type() { return "radiogroup"; }

        virtual ~RadioGroupElement();

    private:
        friend class Element;
        RadioGroupElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };



    class RadioElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<RadioElement>(inParent, inAttr); }

        static const char * Type() { return "radio"; }

        virtual ~RadioElement();

    private:
        friend class Element;
        RadioElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ProgressMeterElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ProgressMeterElement>(inParent, inAttr); }

        static const char * Type() { return "progressmeter"; }

        virtual ~ProgressMeterElement();

    private:
        friend class Element;
        ProgressMeterElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class DeckElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<DeckElement>(inParent, inAttr); }

        static const char * Type() { return "deck"; }

        virtual ~DeckElement();

    private:
        friend class Element;
        DeckElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ScrollbarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ScrollbarElement>(inParent, inAttr); }

        static const char * Type() { return "scrollbar"; }

        virtual ~ScrollbarElement();

    private:
        friend class Element;
        ScrollbarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TabBoxElement>(inParent, inAttr); }

        static const char * Type() { return "tabbox"; }

        virtual ~TabBoxElement();

    private:
        friend class Element;
        TabBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TabsElement>(inParent, inAttr); }

        static const char * Type() { return "tabs"; }

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
        { return Element::Create<TabElement>(inParent, inAttr); }

        static const char * Type() { return "tab"; }

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
        { return Element::Create<TabPanelsElement>(inParent, inAttr); }

        static const char * Type() { return "tabpanels"; }

        virtual ~TabPanelsElement();

    private:
        friend class Element;
        TabPanelsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabPanelElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TabPanelElement>(inParent, inAttr); }

        static const char * Type() { return "tabpanel"; }

        virtual ~TabPanelElement();

    private:
        friend class Element;
        TabPanelElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class GroupBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<GroupBoxElement>(inParent, inAttr); }

        static const char * Type() { return "groupbox"; }

    private:
        friend class Element;
        GroupBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class CaptionElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<CaptionElement>(inParent, inAttr); }

        static const char * Type() { return "caption"; }

    private:
        friend class Element;
        CaptionElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeElement>(inParent, inAttr); }

        virtual ~TreeElement();

        static const char * Type() { return "tree"; }
    
    private:
        friend class Element;
        TreeElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeChildrenElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeChildrenElement>(inParent, inAttr); }

        static const char * Type() { return "treechildren"; }

        virtual ~TreeChildrenElement();

    private:
        friend class Element;
        TreeChildrenElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeItemElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeItemElement>(inParent, inAttr); }

        static const char * Type() { return "treeitem"; }

        virtual ~TreeItemElement();

    private:
        friend class Element;
        TreeItemElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeColsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeColsElement>(inParent, inAttr); }

        static const char * Type() { return "treecols"; }

        virtual ~TreeColsElement();

    private:
        friend class Element;
        TreeColsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeColElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeColElement>(inParent, inAttr); }

        static const char * Type() { return "treecol"; }

        virtual ~TreeColElement();

    private:
        friend class Element;
        TreeColElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeRowElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeRowElement>(inParent, inAttr); }

        static const char * Type() { return "treerow"; }

        virtual ~TreeRowElement();

    private:
        friend class Element;
        TreeRowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeCellElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<TreeCellElement>(inParent, inAttr); }

        static const char * Type() { return "treecell"; }

        virtual ~TreeCellElement();

    private:
        friend class Element;
        TreeCellElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class StatusbarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<StatusbarElement>(inParent, inAttr); }

        static const char * Type() { return "statusbar"; }

        virtual ~StatusbarElement();

    private:
        friend class Element;
        StatusbarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class StatusbarPanelElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<StatusbarPanelElement>(inParent, inAttr); }

        static const char * Type() { return "statusbarpanel"; }

        virtual ~StatusbarPanelElement();

    private:
        friend class Element;
        StatusbarPanelElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ToolbarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ToolbarElement>(inParent, inAttr); }

        static const char * Type() { return "toolbar"; }

        virtual ~ToolbarElement();

    private:
        friend class Element;
        ToolbarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class ToolbarButtonElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ToolbarButtonElement>(inParent, inAttr); }

        static const char * Type() { return "toolbarbutton"; }

        virtual ~ToolbarButtonElement();

    private:
        friend class Element;
        ToolbarButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // XULWin


#endif // ELEMENT_H_INCLUDED
