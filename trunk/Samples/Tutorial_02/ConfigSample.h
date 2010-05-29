#ifndef CONFIGSAMPLE_H_INCLUDED
#define CONFIGSAMPLE_H_INCLUDED


#include "XULWin/Dialog.h"
#include "XULWin/EventListener.h"
#include "XULWin/XULRunner.h"
#include "XULWin/Windows.h"
#include <string>


namespace XULWin
{
    class Element;

    class ConfigSample
    {
    public:
        ConfigSample(HMODULE inModuleHandle, const std::string & inAppDir);

        void run();

    private:
        LRESULT dropFiles(WPARAM wParam, LPARAM lParam);

        LRESULT showMessage(const std::string & inMessage);

        LRESULT showUpload();

        LRESULT showNewSetDialog();

        LRESULT closeDialog(Dialog * inDialog, DialogResult inDialogResult);

        void addNewSet(const std::string & inSetName);

        std::string mAppDir;
        XULRunner mXULRunner;
        ElementPtr mConfigWindow;
        ElementPtr mNewSetDlg;
        XULWin::Fallible<Element *> mNewSetButton;
        XULWin::Fallible<Element *> mSetsPopup;
        XULWin::Fallible<Element *> mNewSetTextBox;
        XULWin::Fallible<Element *> mNewSetOK;
        XULWin::Fallible<Element *> mNewSetCancel;
        ScopedEventListener mEvents;
    };


} // namespace XULWin


#endif // CONFIGSAMPLE_H_INCLUDED
