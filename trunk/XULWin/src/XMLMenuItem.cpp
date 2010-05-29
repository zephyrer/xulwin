#include "XULWin/XMLMenuItem.h"
#include "XULWin/MenuItem.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenuItem::XMLMenuItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLMenuItem::TagName(),
                inParent,
                new MenuItem(inParent->component(), inAttributesMapping))
    {
    }


    XMLMenuItem::~XMLMenuItem()
    {
    }


    std::string XMLMenuItem::label() const
    {
        return getAttribute("label");
    }


    std::string XMLMenuItem::value() const
    {
        return getAttribute("value");
    }

} // namespace XULWin
