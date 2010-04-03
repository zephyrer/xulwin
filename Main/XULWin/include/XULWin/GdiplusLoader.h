#ifndef GDIPLUSLOADER_H_INCLUDED
#define GDIPLUSLOADER_H_INCLUDED


#include "XULWin/Windows.h"
#include <boost/noncopyable.hpp>


namespace XULWin
{


    /**
     * GdiplusLoader
     *
     * This class initializes the GDI+ library in its constructor.
     * If a previous instance of the class already existed a refcount
     * is augmented instead.
     *
     * During destruction the object decrements its refcount. Once
     * this refcount reaches zero, the GDI+ library is finalized.
     *
     * One way of using this class is by simply creating a named object
     * on the stack at the beginning of your application's main function.
     *
     * Another way of using it is by having any classes that depend on GDI+
     * inherit this class.
     */
    class GdiplusLoader : boost::noncopyable
    {
        public:
            GdiplusLoader();

            ~GdiplusLoader();

        private:
            ULONG_PTR mGdiPlusToken;
            static int sRefCount;
    };


} // namespace XULWin


#endif // GDIPLUSLOADER_H_INCLUDED
