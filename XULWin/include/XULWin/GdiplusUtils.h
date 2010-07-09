#ifndef GDIPLUSUTILS_H_INCLUDED
#define GDIPLUSUTILS_H_INCLUDED


#include <string>
#include <windows.h>


namespace XULWin
{

    namespace WinAPI
    {

        /**
         * Creates a HICON object from an image file.
         */
        HICON CreateHICON(const std::string & inImagePath);

    } // namespace WinAPI

} // namespace XULWin


#endif // GDIPLUSUTILS_H_INCLUDED
