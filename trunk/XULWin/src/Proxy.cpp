#include "XULWin/Proxy.h"


namespace XULWin
{

    Proxy::Proxy(Component * inDecoratedElement) :
        Super(inDecoratedElement)
    {
    }


    ElementImplPtr Proxy::swap(ElementImplPtr inNewElementImpl)
    {
        assert(inNewElementImpl);
        ElementImplPtr old = mDecoratedElement;
        mDecoratedElement = inNewElementImpl;
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }


    ElementImplPtr Proxy::swap(Component * inNewElementImpl)
    {
        assert(inNewElementImpl);
        ElementImplPtr old = mDecoratedElement;
        mDecoratedElement.reset(inNewElementImpl);
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }
    
} // namespace XULWin
