#include "XULWin/NewElementS.h"
#include "XULWin/NewElementS.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    NewElementS::NewElementS(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(NewElementS::Type(),
                inParent,
                new NewElementSImpl(inParent->impl(), inAttributesMapping))
    {
    }


    bool NewElementS::init()
    {
        return Element::init();
    }

} // namespace XULWin
