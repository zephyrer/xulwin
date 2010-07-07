#include "XULWin/Element.h"
#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/Enums.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

    Element::Element(const std::string & inType, Element * inParent, Component * inNative) :
        mType(inType),
        mParent(inParent),
        mComponent(inNative)
    {
        if (mComponent)
        {
            mComponent->setOwningElement(this);
        }
    }


    Element::~Element()
    {
        // Children require parent access while destructing.
        // So we destruct them while parent still alive.
        mChildren.clear();
    }


    bool Element::init()
    {
        return component()->init();
    }


    const std::string & Element::tagName() const
    {
        return mType;
    }


    void Element::setInnerText(const std::string & inText)
    {
        mInnerText = inText;
    }


    const std::string & Element::innerText() const
    {
        return mInnerText;
    }


    Element * Element::getElementById(const std::string & inId)
    {
        struct Helper
        {
            static Element * findChildById(const Children & inChildren, const std::string & inId)
            {
                Element * result(0);
                for (size_t idx = 0; idx != inChildren.size(); ++idx)
                {
                    ElementPtr child = inChildren[idx];
                    if (child->getAttribute("id") == inId)
                    {
                        result = child.get();
                        break;
                    }
                    else
                    {
                        result = findChildById(child->children(), inId);
                        if (result)
                        {
                            break;
                        }
                    }
                }
                return result;
            }
        };
        Element * result = Helper::findChildById(children(), inId);
        if (!result)
        {
            ReportError("Element with id '" + inId + "' was not found.");
        }
        return result;
    }


    void Element::getElementsByTagName(const std::string & inType, std::vector<Element *> & outElements)
    {
        if (tagName() == inType)
        {
            outElements.push_back(this);
        }
        for (size_t idx = 0; idx != mChildren.size(); ++idx)
        {
            mChildren[idx]->getElementsByTagName(inType, outElements);
        }
    }


    void Element::removeChild(const Element * inChild)
    {
        Children::iterator it = std::find_if(mChildren.begin(), mChildren.end(), boost::bind(&ElementPtr::get, _1) == inChild);
        if (it != mChildren.end())
        {
            ElementPtr keepAlive = *it;
            mChildren.erase(it);
            mComponent->rebuildLayout();
            mComponent->onChildRemoved(keepAlive->component());
            // keepAlive loses scope here and destroys child
        }
        else
        {
            ReportError("Remove child failed because it wasn't found.");
        }
    }


    void Element::setStyles(const AttributesMapping & inAttributes)
    {
        AttributesMapping::const_iterator it = inAttributes.find("style");
        if (it != inAttributes.end())
        {
            Poco::StringTokenizer keyValuePairs(it->second, ";",
                                                Poco::StringTokenizer::TOK_IGNORE_EMPTY
                                                | Poco::StringTokenizer::TOK_TRIM);

            Poco::StringTokenizer::Iterator it = keyValuePairs.begin(), end = keyValuePairs.end();
            for (; it != end; ++it)
            {
                std::string::size_type sep = it->find(":");
                if (sep != std::string::npos && (sep + 1) < it->size())
                {
                    std::string key = it->substr(0, sep);
                    std::string value = it->substr(sep + 1, it->size() - sep - 1);
                    setStyle(key, value);
                }
            }
        }
    }


    void Element::setAttributes(const AttributesMapping & inAttributes)
    {
        mAttributes = inAttributes;

        if (mComponent)
        {
            AttributesMapping::iterator it = mAttributes.begin(), end = mAttributes.end();
            for (; it != end; ++it)
            {
                setAttribute(it->first, it->second);
            }
        }
    }


    void Element::initAttributeControllers()
    {
        if (mComponent)
        {
            mComponent->initAttributeControllers();
        }
    }


    void Element::initStyleControllers()
    {
        if (mComponent)
        {
            mComponent->initStyleControllers();
        }
    }


    std::string Element::getStyle(const std::string & inName) const
    {
        std::string result;
        if (!mComponent || !mComponent->getStyle(inName, result))
        {
            StylesMapping::const_iterator it = mStyles.find(inName);
            if (it != mStyles.end())
            {
                result = it->second;
            }
        }
        return result;

    }


    std::string Element::getAttribute(const std::string & inName) const
    {
        std::string result;
        if (!mComponent || !mComponent->getAttribute(inName, result))
        {
            AttributesMapping::const_iterator it = mAttributes.find(inName);
            if (it != mAttributes.end())
            {
                result = it->second;
            }
        }
        return result;
    }


    std::string Element::getDocumentAttribute(const std::string & inName) const
    {
        std::string result;
        AttributesMapping::const_iterator it = mAttributes.find(inName);
        if (it != mAttributes.end())
        {
            result = it->second;
        }
        return result;
    }


    void Element::setStyle(const std::string & inName, const std::string & inValue)
    {
        std::string type = this->tagName();
        if (!mComponent || !mComponent->setStyle(inName, inValue))
        {
            mStyles[inName] = inValue;
        }
    }


    void Element::setAttribute(const std::string & inName, const std::string & inValue)
    {
        if (!mComponent || !mComponent->setAttribute(inName, inValue))
        {
            mAttributes[inName] = inValue;
        }
    }


    bool Element::addEventListener(EventListener * inEventListener)
    {
        if (!mComponent)
        {
            return false;
        }

        if (NativeComponent * comp = mComponent->downcast<NativeComponent>())
        {
            comp->addEventListener(inEventListener);
            return true;
        }
        return false;
    }


    bool Element::removeEventListener(EventListener * inEventListener)
    {
        if (!mComponent)
        {
            return false;
        }

        if (NativeComponent * comp = mComponent->downcast<NativeComponent>())
        {
            comp->removeEventListener(inEventListener);
            return true;
        }
        return false;
    }


    Component * Element::component() const
    {
        return mComponent.get();
    }


    void Element::addChild(ElementPtr inChild)
    {
        mChildren.push_back(inChild);
        component()->onChildAdded(inChild->component());
    }


    // Create the implemenations
    XULWIN_IMPLEMENT_ELEMENT(Label)
    XULWIN_IMPLEMENT_ELEMENT(Button)
    XULWIN_IMPLEMENT_ELEMENT(CheckBox)
    XULWIN_IMPLEMENT_ELEMENT(MenuList)
    XULWIN_IMPLEMENT_ELEMENT(MenuPopup)
    XULWIN_IMPLEMENT_ELEMENT(MenuItem)
    XULWIN_IMPLEMENT_ELEMENT(TextBox)
    XULWIN_IMPLEMENT_ELEMENT(ListBox)
    XULWIN_IMPLEMENT_ELEMENT(ListItem)
    XULWIN_IMPLEMENT_ELEMENT(Scrollbar)
    XULWIN_IMPLEMENT_ELEMENT(Box)
    XULWIN_IMPLEMENT_ELEMENT(HBox)
    XULWIN_IMPLEMENT_ELEMENT(VBox)
    XULWIN_IMPLEMENT_ELEMENT(Image)
    XULWIN_IMPLEMENT_ELEMENT(ListCell)
    XULWIN_IMPLEMENT_ELEMENT(ListCol)
    XULWIN_IMPLEMENT_ELEMENT(ListCols)
    XULWIN_IMPLEMENT_ELEMENT(ListHead)
    XULWIN_IMPLEMENT_ELEMENT(ListHeader)
    XULWIN_IMPLEMENT_ELEMENT(Menu)
    XULWIN_IMPLEMENT_ELEMENT(MenuBar)
    XULWIN_IMPLEMENT_ELEMENT(MenuSeparator)
    XULWIN_IMPLEMENT_ELEMENT(Toolbar)
    XULWIN_IMPLEMENT_ELEMENT(ToolbarButton)


} // namespace XULWin
