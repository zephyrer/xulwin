#include "XULWin/XMLMenuPopup.h"
#include "XULWin/MenuPopup.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenuPopup::XMLMenuPopup(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLMenuPopup::TagName(),
                inParent,
                new MenuPopup(inParent->component(), inAttributesMapping))
    {
    }


    XMLMenuPopup::~XMLMenuPopup()
    {
    }

} // namespace XULWin
