#include "XULWin/ListBox.h"
#include "XULWin/ListBoxImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListBox::ListBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListBox::Type(),
                inParent,
                new MarginDecorator(new ListBoxImpl(inParent->impl(), inAttributesMapping)))
    {
    }


    bool ListBox::init()
    {
        return Element::init();
    }

} // namespace XULWin
