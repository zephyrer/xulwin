#include "FacebookUploaderController.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/Unicode.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include <sstream>
#include <string>


using namespace XULWin;


std::string unquote(const std::string & inString)
{
    if (inString.size() <= 2)
    {
        return inString;
    }

    if ((inString[0] == '"' && inString[inString.size() - 1] == '"') ||
            (inString[0] == '\'' && inString[inString.size() - 1] == '\''))
    {
        return inString.substr(1, inString.size() - 2);
    }
    return inString;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize the XULWin library
    Initializer initializer(hInstance);
    ErrorCatcher errorCatcher;

    std::string appDir = unquote(lpCmdLine);
    if (appDir.empty())
    {
        Poco::Path path(WinAPI::getCurrentDirectory(), "facebook-uploader");
        appDir = path.toString();
    }

    if (!Poco::File(appDir).exists())
    {
        std::wstring msg = L"Could not find " + ToUTF16(appDir) + L".";
        MessageBox(0, msg.c_str(), L"Facebook Uploader", MB_OK);
        return 1;
    }

    try
    {
        FacebookUploaderController controller(hInstance, appDir);
        controller.runApplication();
    }
    catch (const std::exception & inException)
    {
        std::wstring msg = ToUTF16(inException.what());
        MessageBox(0, msg.c_str(), L"Facebook Uploader", MB_OK);
        return 1;
    }
    return 0;
}
