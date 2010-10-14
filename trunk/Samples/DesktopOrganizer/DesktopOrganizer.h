#ifndef DESKTOPORGANIZER_H_INCLUDED
#define DESKTOPORGANIZER_H_INCLUDED


#include <string>
#include <vector>
#include "XULWin/Windows.h"


namespace XULWin
{

    class WindowEnumerator
    {
    public:
        WindowEnumerator();

        std::vector<HWND> getDesktopWindows() const;

    private:
        static BOOL CALLBACK OnFoundWindow(HWND inHWND, LPARAM lParam);
        void onFoundWindow(HWND inHWND);
        mutable std::vector<HWND> mFoundWindows;
    };


    class DesktopOrganizer
    {
    public:
        DesktopOrganizer();

        // Save the current desktop configuration as a XUL file.
        void saveDesktop(const std::string & inXULFile);

        // Load a desktop configuration file and apply it to the current desktop.
        void loadDesktop(const std::string & inConfigFile);
    };

} // namespace XULWin


#endif // DESKTOPORGANIZER_H_INCLUDED
