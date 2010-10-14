#include "ForeignWindow.h"
#include "DesktopOrganizer.h"
#include "XULWin/Initializer.h"
#include "XULWin/Unicode.h"
#include "Poco/File.h"
#include <sstream>
#include <string>


bool FileExists(const std::string & inFileName)
{
    Poco::File file(inFileName);
    return file.exists();
}


static const std::string cDesktopConfigFile("Desktop.txt");


void run()
{
    XULWin::DesktopOrganizer desktopOrganizer;
    if (!FileExists(cDesktopConfigFile))
    {
        desktopOrganizer.saveDesktop(cDesktopConfigFile);
    }
    else
    {
        desktopOrganizer.loadDesktop(cDesktopConfigFile);
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
    {
        run();
    }
    catch (const std::exception & inException)
    {
        std::stringstream ss;
        ss << "Program has encountered an exception and needs to exit. Reason: " << inException.what();
        ::MessageBox(0, XULWin::ToUTF16(ss.str()).c_str(), L"Desktop Organizer", MB_OK);
    }
}
