#include "XULWin/Proxy.h"


namespace XULWin
{

    Proxy::Proxy(ElementImpl * inDecoratedElement) :
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


    ElementImplPtr Proxy::swap(ElementImpl * inNewElementImpl)
    {
        assert(inNewElementImpl);
        ElementImplPtr old = mDecoratedElement;
        mDecoratedElement.reset(inNewElementImpl);
        assert(mDecoratedElement);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }
    
} // namespace XULWin
