#ifndef GDIPLUSUTILS_H_INCLUDED
#define GDIPLUSUTILS_H_INCLUDED


#include "XULWin/Windows.h"
#include <string>


namespace Gdiplus
{
    class Image;
}


namespace XULWin
{

    namespace WinAPI
    {

        /**
         * Creates a HICON object from an image file.
         */
        HICON CreateHICON(const std::string & inImagePath);

        /**
         * Creates a Gdiplus::Image object from an image file.
         */
        Gdiplus::Image * CreateImage(const std::string & inImagePath);

    } // namespace WinAPI

} // namespace XULWin


#endif // GDIPLUSUTILS_H_INCLUDED
