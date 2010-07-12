#include "XULWin/GdiplusUtils.h"
#include "XULWin/Gdiplus.h"
#include "XULWin/ChromeUrl.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
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

        
        Gdiplus::Image * CreateImage(const std::string & inImagePath)
        {
            std::string curdir = WinAPI::getCurrentDirectory();
            ChromeURL url(inImagePath);
            Poco::Path imagePath(curdir);
            imagePath.append(url.convertToLocalPath());
            std::wstring utf16ImagePath = ToUTF16(imagePath.toString());
            Gdiplus::Bitmap * img(new Gdiplus::Bitmap(utf16ImagePath.c_str()));
            if (!img || img->GetLastStatus() != Gdiplus::Ok)
            {
                delete img;
                return 0;
            }
            return img;
        }

    } // namespace WinAPI

} // namespace XULWin
