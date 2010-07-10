#ifndef FACEBOOKUPLOADERCONTROLLER_H_INCLUDED
#define FACEBOOKUPLOADERCONTROLLER_H_INCLUDED


#include "FacebookUploaderModel.h"
#include "Item.h"
#include "ItemView.h"
#include "XULWin/EventListener.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Windows.h"
#include "XULWin/XULRunner.h"
#include <boost/scoped_ptr.hpp>


namespace XULWin
{

    class Grid;
    class Rows;
    class TextBox;


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

        void log(const std::string & inMessage);

        void addItems(const std::vector<std::string> & inFiles);

        boost::scoped_ptr<FacebookUploaderModel> mModel;

        // Has ownership
        ItemViews mItemViews;

        std::string mAppDir;
        WinAPI::CurrentDirectoryChanger mCurrentDirectoryChanger;
        XULRunner mXULRunner;
        Grid * mImageArea;
        Rows * mImageAreaRows;
        TextBox * mLogTextBox;

        // This class should be near the bottom of the class variable list
        // so that it will be destroyed first when quitting the program.
        // This prevents that it would try to handle events for widgets
        // that no longer exist.
        ScopedEventListener mEvents;
    };


} // namespace XULWin


#endif // FACEBOOKUPLOADERCONTROLLER_H_INCLUDED
