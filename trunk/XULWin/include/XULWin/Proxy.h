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
        ComponentPtr swap(ComponentPtr inNewElement);

        // Takes ownership, and returns previous instance.
        ComponentPtr swap(Component * inNewElement);
    };

} // namespace XULWin


#endif // PROXYIMPL_H_INCLUDED
