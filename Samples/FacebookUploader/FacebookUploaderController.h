#ifndef FACEBOOKUPLOADERCONTROLLER_H_INCLUDED
#define FACEBOOKUPLOADERCONTROLLER_H_INCLUDED


#include "FacebookUploaderModel.h"
#include "XULWin/EventListener.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Windows.h"
#include "XULWin/XULRunner.h"


namespace XULWin
{

    class FacebookUploaderController
    {
    public:
        FacebookUploaderController(HINSTANCE inInstanceHandle, const std::string & inAppDir);

        void runApplication();

        LRESULT handleLoginButton(WPARAM wParam, LPARAM lParam);

        LRESULT handleLogoutButton(WPARAM wParam, LPARAM lParam);

        LRESULT handleAddButton(WPARAM wParam, LPARAM lParam);

        LRESULT handleRemoveButton(WPARAM wParam, LPARAM lParam);

        LRESULT handleRotateLeftButton(WPARAM wParam, LPARAM lParam);

        LRESULT handleRotateRightButton(WPARAM wParam, LPARAM lParam);

        LRESULT handleUploadButton(WPARAM wParam, LPARAM lParam);
        
    private:
        void showMessageBox(const std::string & inMessage);

        std::string mAppDir;
        WinAPI::CurrentDirectoryChanger mCurrentDirectoryChanger;
        XULRunner mXULRunner;

        // This class should be near the bottom of the class variable list
        // so that it will be destroyed first when quitting the program.
        // This prevents that it would try to handle events for widgets
        // that no longer exist.
        ScopedEventListener mEvents;
    };


}


#endif // FACEBOOKUPLOADERCONTROLLER_H_INCLUDED
