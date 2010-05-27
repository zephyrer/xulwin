#ifndef INITIALIZER_H_INCLUDED
#define INITIALIZER_H_INCLUDED


#include "XULWin/ElementFactory.h"
#include "XULWin/Windows.h"
#include <boost/noncopyable.hpp>


namespace XULWin
{

    /**
     * Takes care of all XULWin library initializations and finalizations.
     */
    class Initializer : boost::noncopyable
    {
    public:
        Initializer(HINSTANCE inModuleHandle);

        ~Initializer();

    private:
    };


} // namespace XULWin


#endif // INITIALIZER_H_INCLUDED
