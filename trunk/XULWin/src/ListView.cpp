#include "XULWin/ListView.h"
#include "XULWin/ListViewImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListView::ListView(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListView::Type(),
                inParent,
                new ListViewImpl(inParent->impl(), inAttributesMapping))
    {
    }


    bool ListView::init()
    {
        return Element::init();
    }

} // namespace XULWin
