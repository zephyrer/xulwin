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
        Proxy(ElementImpl * inDecoratedElement);

        // Returns previous instance.
        ElementImplPtr swap(ElementImplPtr inNewElementImpl);

        // Takes ownership, and returns previous instance.
        ElementImplPtr swap(ElementImpl * inNewElementImpl);
    };

} // namespace XULWin


#endif // PROXYIMPL_H_INCLUDED
