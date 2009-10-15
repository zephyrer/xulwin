#ifndef PROXYIMPL_H_INCLUDED
#define PROXYIMPL_H_INCLUDED


#include "XULWin/Decorator.h"


namespace XULWin
{

    class Proxy : public Decorator
    {
    public:
        typedef Decorator Super;

        Proxy(ElementImpl * inDecoratedElement);

        ElementImplPtr swap(ElementImplPtr inNewElementImpl);

        ElementImplPtr swap(ElementImpl * inNewElementImpl);
    };

} // namespace XULWin


#endif // PROXYIMPL_H_INCLUDED
