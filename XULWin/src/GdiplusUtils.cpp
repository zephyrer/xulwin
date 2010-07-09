#include "XULWin/GdiplusUtils.h"
#include "XULWin/Gdiplus.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "Poco/Path.h"


namespace XULWin
{

    namespace WinAPI
    {

        HICON CreateHICON(const std::string & inImagePath)
        {
            HICON result(0);
            Poco::Path imagePath(inImagePath);

            if (imagePath.getExtension() == "ico")
            {
                std::wstring utf16Path = ToUTF16(imagePath.toString());
                result = ::ExtractIcon(::GetModuleHandle(0), utf16Path.c_str(), 0);
            }
            else
            {
                // Load a HICON by converting a non-ico file (bmp, jpeg, png, ...)
                Gdiplus::Bitmap img(ToUTF16(inImagePath).c_str());
                if (img.GetLastStatus() != Gdiplus::Ok)
                {
                    return 0;
                }
                img.GetHICON(&result);
            }
            return result;
        }

    } // namespace WinAPI

} // namespace XULWin
