#include "XULWin/Proxy.h"


namespace XULWin
{

    Proxy::Proxy(Component * inDecoratedElement) :
        Super(inDecoratedElement)
    {
    }


    ComponentPtr Proxy::swap(ComponentPtr inNewElement)
    {
        assert(inNewElement);
        ComponentPtr old = mDecoratedElement;
        mDecoratedElement = inNewElement;
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }


    ComponentPtr Proxy::swap(Component * inNewElement)
    {
        assert(inNewElement);
        ComponentPtr old = mDecoratedElement;
        mDecoratedElement.reset(inNewElement);
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }

} // namespace XULWin
