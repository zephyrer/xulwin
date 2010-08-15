#include "XULWin/GdiplusLoader.h"
#include "XULWin/Gdiplus.h"
#include <assert.h>



namespace XULWin
{

    int GdiplusLoader::sRefCount(0);


    GdiplusLoader::GdiplusLoader()
    {
        if (sRefCount++ == 0)
        {
            // Init Gdiplus
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            Gdiplus::GdiplusStartup(&mGdiPlusToken, &gdiplusStartupInput, NULL);
        }
    }


    GdiplusLoader::~GdiplusLoader()
    {
        if (--sRefCount == 0)
        {
            Gdiplus::GdiplusShutdown(mGdiPlusToken);
        }
    }

} // namespace XULWin
