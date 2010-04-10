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
     * \brief Base class for XUL elements.
     *
     * The Element class provides an interface to the XML aspects
     * of a XUL element. It enables iteration of the DOM tree by
     * traversal of an element's parent element or child elements.
     * It provides methods for getting and setting the element's
     * XML attributes.
     *
     * Changes made to an XML attribute will propagate to the native backend
     * if the element's corresponding Component object (see Component.h) has
     * registered a matching AttributeController (see AttributeController.h) for the
     * modified attribute.
     *
     * Classes that inherit Element must have an "Element" suffix in their class name.
     * The classes that inherit Component do not need to have any suffix:
     *
     *      Element              Component
     *      -------              ---------
     *      LabelElement         Label
     *      ButtonElement        Button
     *      WindowElement        Window
     *      ...                  ...
     *
     * Setting the "label" attribute on a ButtonElement object will cause invocation
     * of the Button::setLabel method which in turn updates the UI.
     *
     * Example:
     *
     *     myButtonElement->setAttribute("label", "hello");
     *     => calls: LabelController::set("label", "hello");
     *     => calls abstract method: LabelController::setLabel("hello");
     *     => invokes: Button::setLabel("hello");
     *     => calls relevant WinAPI function
     *     => UI update.
     *
     * This requires that the Button class inherits and implements the LabelController class.
     *
     * Not all attributes need to have a corresponding AttributeController class.
     * If the Component object has not registered the AttributeController then
     * any changes to the attribute will simply be stored as a normal XML attribute.
     *
     * For example the "id" attribute does not affect the UI so it doesn't need to be
     * propagated. It is simply stored in a attributes mapping object which is defined
     * as a member of the Element class.
     */
    class Element : private boost::noncopyable
    {
    public:
        /**
         * Factory method for all elements.
         *
         * Initialization order:
         *
         * 1. Parser encounters the opening tag of a new element (e.g. <button>)
         *   - Element constructor
         *   - Construction of any decorators for Component
         *   - Component constructor
         *   - Initialize XUL attributes
         *   - Initialize CSS styles (from the XUL "style" attribute)
         *   - Register the element as a child of the parent element.
         *
         * 2. Creation and initialization of any child elements.
         *
         * 3. Parser encounters closing tag of the element (e.g. </button>)
         *   - Call Element::init().
         *   - Call Component::init().
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

        /**
         * Element destructor
         *
         * Since the factory method returns a smart pointer
         * you never need to call this.
         */
        ~Element();

        /**
         * Element initialization.
         * 
         * Subclasses of Element may override this method to do extra
         * initialization after the default creation steps have been
         * taken care of.
         *
         * This method is invoked after creation by the factory method.
         * It is called when the parser encounters the closing tag, this
         * means that any child elements will be created and initialized
         * before that.
         */
        virtual bool init();

        /**
         * Adds a child element.
         *
         * You never need to call this, it is taken care of by the factory.
         */
        virtual void addChild(ElementPtr inChild);

        /**
         * Removes a child element.
         *
         * If this element maps to a native component then it will
         * also be removed from the native hierarchy of the system.
         * In certain situations this may be result in unexpected
         * behavior. Be careful!
         */
        virtual void removeChild(const Element * inChild);
        
        /**
         * Returns the element tagname.
         *
         * For example button, label, etc...
         */
        const std::string & tagName() const;

        /**
         * Returns the parent element.
         *
         * If the element is the document top-level element then
         * NULL is returned.
         */
        const Element * parent() const { return mParent; }

        /**
         * Returns the non-const parent element.
         */
        Element * parent() { return mParent; }

        /**
         * Gets an attribute.
         *
         * If the attribute is natively mapped then this method is
         * forwarded to the Component object and this may result in 
         * a native system call to query the widget state.
         *
         * If the attribute is not natively mapped then the value
         * will be returned from the Element's attribute mapping.
         *
         * Returns empty string if the attribute was not found.
         */
        std::string getAttribute(const std::string & inName) const;

        /**
         * Sets an attribute.
         * 
         * Sets the attribute by invoking the attribute controller.
         * If no attribute controller found it will insert (or overwrite)
         * the value in the attributes mapping.
         */
        void setAttribute(const std::string & inName, const std::string & inValue);
        
        /**
         * Gets an attribute without querying the component.
         *
         * Gets the attribute immediately from the Element's
         * attribute datastructure without querying the component.
         *
         * Returns empty string if the attribute was not found.
         */
        std::string getDocumentAttribute(const std::string & inName) const;

        /**
         * Gets a style attribute.
         *
         * If the style attribute is natively mapped then this method
         * is forwarded to the Component object and this may result in 
         * a native system call to query the widget state.
         *
         * If the style attributes is not natively mapped then the value
         * will be returned from the Element's attribute mapping.
         *
         * Returns empty string if the attribute was not found.
         *
         * Example: <grid style="overflow:auto">
         * The style attribute "overflow" has value "auto".
         */
        std::string getStyle(const std::string & inName) const;

        /**
         * Sets the text between an Element's opening and closing tags.
         */
        void setInnerText(const std::string & inText);

        /**
         * Returns the text between an Element's opening and closing tags.
         */
        const std::string & innerText() const;
        
        /**
         * Finds an element in the DOM tree with the requested id.
         */
        Element * getElementById(const std::string & inId);

        /**
         * Returns all sub-elements that have the requested tagname.
         */
        void getElementsByTagName(const std::string & inType,
                                  std::vector<Element *> & outElements);

        /**
         * Returns all elements that have the requested type.
         *
         * Typed and faster variant of the getElementsByTagName.
         */
        template<class ElementType>
        void getElementsByType(std::vector<ElementType *> & outElements)
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

        /**
         * Returns the list of child elements.
         */
        const Children & children() const { return mChildren; }

        /**
         * Returns a non-const list of child elements.
         */
        Children & children() { return mChildren; }

        /**
         * Registers an event listener.
         *
         * You may prefer to use a ScopedEventListener object instead.
         * \see ScopedEventListener
         */
        bool addEventListener(EventListener * inEventListener);

        /**
         * Unregisters an event listener.
         */
        bool removeEventListener(EventListener * inEventListener);

        /**
         * Returns the component object.
         */
        Component * component() const;

        /**
         * Casts an element to a subtype.
         *
         * Returns NULL if downcast failed.
         */
        template<class ElementType>
        const ElementType * downcast() const
        {
            if (tagName() == ElementType::TagName())
            {
                return static_cast<ElementType *>(this);
            }
            return 0;
        }

        /**
         * Casts an element to a subtype.
         *
         * Non-const variant of downcast.
         */
        template<class ElementType>
        ElementType * downcast()
        {
            if (tagName() == ElementType::TagName())
            {
                return static_cast<ElementType *>(this);
            }
            return 0;
        }

        /**
         * Searches for a child of given type.
         *
         * Returns the first one found and
         * only searches one level deep.
         */
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

        /**
         * Searches for a child of given type.
         *
         * Non-const variant of findChildOfType.
         */
        template<class ElementType>
        ElementType * findChildOfType()
        {
            return const_cast<ElementType *>(static_cast<const Element *>(this)->findChildOfType<ElementType>());
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


    enum DialogResult
    {
        DialogResult_Error = -1,
        DialogResult_Ok,
        DialogResult_Cancel,
        DialogResult_Custom
    };


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


    class RowElement;
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


#endif // ELEMENT_H_INCLUDED
