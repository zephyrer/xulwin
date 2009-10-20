#ifndef PROXYIMPL_H_INCLUDED
#define PROXYIMPL_H_INCLUDED


#include "XULWin/Decorator.h"


namespace XULWin
{

    class Proxy : public Decorator
    {
    public:
        typedef Decorator Super;

        // Takes ownership.
        Proxy(Component * inDecoratedElement);

        // Returns previous instance.
        ComponentPtr swap(ComponentPtr inNewElementImpl);

        // Takes ownership, and returns previous instance.
        ComponentPtr swap(Component * inNewElementImpl);
    };

} // namespace XULWin


#endif // PROXYIMPL_H_INCLUDED
