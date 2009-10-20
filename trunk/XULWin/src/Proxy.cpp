#include "XULWin/Proxy.h"


namespace XULWin
{

    Proxy::Proxy(Component * inDecoratedElement) :
        Super(inDecoratedElement)
    {
    }


    ComponentPtr Proxy::swap(ComponentPtr inNewElementImpl)
    {
        assert(inNewElementImpl);
        ComponentPtr old = mDecoratedElement;
        mDecoratedElement = inNewElementImpl;
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }


    ComponentPtr Proxy::swap(Component * inNewElementImpl)
    {
        assert(inNewElementImpl);
        ComponentPtr old = mDecoratedElement;
        mDecoratedElement.reset(inNewElementImpl);
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }
    
} // namespace XULWin
