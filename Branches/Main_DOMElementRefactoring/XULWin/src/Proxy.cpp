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
        ComponentPtr old = mDecoratedComponent;
        mDecoratedComponent = inNewElement;
        assert(mDecoratedComponent);
        mDecoratedComponent->setOwningElement(old->el());
        return old;
    }


    ComponentPtr Proxy::swap(Component * inNewElement)
    {
        assert(inNewElement);
        ComponentPtr old = mDecoratedComponent;
        mDecoratedComponent.reset(inNewElement);
        assert(mDecoratedComponent);
        mDecoratedComponent->setOwningElement(old->el());
        return old;
    }

} // namespace XULWin
