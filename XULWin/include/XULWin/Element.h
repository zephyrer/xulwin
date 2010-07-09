#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED


#include "XULWin/AttributesMapping.h"
#include "XULWin/Enums.h"
#include "XULWin/ForwardDeclarations.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>


namespace XULWin
{

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
     * Not all attributes need to have a corresponding AttributeController class.
     * If the Component object has not registered the AttributeController then
     * any changes to the attribute will simply be stored as a normal XML attribute.
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
         * In certain situations this may result in unexpected
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
        const Element * parent() const
        {
            return mParent;
        }

        /**
         * Returns the non-const parent element.
         */
        Element * parent()
        {
            return mParent;
        }

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
        const Children & children() const
        {
            return mChildren;
        }

        /**
         * Returns a non-const list of child elements.
         */
        Children & children()
        {
            return mChildren;
        }

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


    /**
     * Macro for declaring a XUL element class.
     */
    #define XULWIN_DECLARE_ELEMENT(ELEMENT, TAGNAME) \
        class ELEMENT;                                                          \
        class XML##ELEMENT : public Element                                     \
        {                                                                       \
        public:                                                                 \
            typedef ELEMENT ComponentType;                                      \
            static ElementPtr Create(Element *, const AttributesMapping &);     \
            static const char * TagName() { return TAGNAME; }                   \
        private:                                                                \
            XML##ELEMENT(Element *, const AttributesMapping &);                 \
            friend class Element;                                               \
        };


    /**
     * Macro for generating the class implementations of XMLElement subclasses.
     * Note! A function called Create##ELEMENT will be declared but not defined.
     * For example CreateLabel, CreateTextBox, ...
     *
     * It's up to the implementer to provide definitions for these functions
     * along with the corresponding Component class.
     * 
     */
    #define XULWIN_IMPLEMENT_ELEMENT(ELEMENT) \
        Component * Create##ELEMENT(Component *, const AttributesMapping&);                         \
        ElementPtr XML##ELEMENT::Create(Element * inParent, const AttributesMapping & inAttr)       \
        {                                                                                           \
            return Element::Create<XML##ELEMENT>(inParent, inAttr);                                 \
        }                                                                                           \
                                                                                                    \
        XML##ELEMENT::XML##ELEMENT(Element * inParent, const AttributesMapping & inAttr) :          \
            Element(XML##ELEMENT::TagName(),                                                        \
                    inParent,                                                                       \
                    Create##ELEMENT(inParent->component(), inAttr))                                 \
        {                                                                                           \
        }                                                                                           \


    /**
     * Essential XUL elements
     */
    XULWIN_DECLARE_ELEMENT(Label, "label")
    XULWIN_DECLARE_ELEMENT(Button, "button")
    XULWIN_DECLARE_ELEMENT(CheckBox, "checkbox")
    XULWIN_DECLARE_ELEMENT(MenuList, "menulist")
    XULWIN_DECLARE_ELEMENT(MenuPopup, "menupopup")
    XULWIN_DECLARE_ELEMENT(MenuItem, "menuitem")
    XULWIN_DECLARE_ELEMENT(TextBox, "textbox")
    XULWIN_DECLARE_ELEMENT(ListBox, "listbox")
    XULWIN_DECLARE_ELEMENT(ListItem, "listitem")
    XULWIN_DECLARE_ELEMENT(Scrollbar, "scrollbar")
    XULWIN_DECLARE_ELEMENT(Box, "hbox")
    XULWIN_DECLARE_ELEMENT(HBox, "hbox")
    XULWIN_DECLARE_ELEMENT(VBox, "vbox")
    XULWIN_DECLARE_ELEMENT(Image, "image")
    XULWIN_DECLARE_ELEMENT(ListCell, "listcell")
    XULWIN_DECLARE_ELEMENT(ListCol, "listcol")
    XULWIN_DECLARE_ELEMENT(ListCols, "listcols")
    XULWIN_DECLARE_ELEMENT(ListHead, "listhead")
    XULWIN_DECLARE_ELEMENT(ListHeader, "listheader")
    XULWIN_DECLARE_ELEMENT(Menu, "menu")
    XULWIN_DECLARE_ELEMENT(MenuBar, "menubar")
    XULWIN_DECLARE_ELEMENT(MenuSeparator, "menuseparator")
    XULWIN_DECLARE_ELEMENT(Toolbar, "toolbar")
    XULWIN_DECLARE_ELEMENT(ToolbarButton, "toolbarbutton")
    XULWIN_DECLARE_ELEMENT(ToolbarSpacer, "toolbarspacer")
    

} // XULWin


#endif // ELEMENT_H_INCLUDED
 