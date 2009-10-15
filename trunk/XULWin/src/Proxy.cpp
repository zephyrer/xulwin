#include "XULWin/Proxy.h"


namespace XULWin
{

    Proxy::Proxy(ElementImpl * inDecoratedElement) :
        Super(inDecoratedElement)
    {
    }


    ElementImplPtr Proxy::swap(ElementImplPtr inNewElementImpl)
    {
        ElementImplPtr old = mDecoratedElement;
        mDecoratedElement = inNewElementImpl;
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }


    ElementImplPtr Proxy::swap(ElementImpl * inNewElementImpl)
    {
        ElementImplPtr old = mDecoratedElement;
        mDecoratedElement.reset(inNewElementImpl);
        mDecoratedElement->setOwningElement(old->el());
        return old;
    }
    
} // namespace XULWin
