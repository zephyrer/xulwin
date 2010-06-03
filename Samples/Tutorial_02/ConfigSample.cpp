#include "ConfigSample.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/TextBox.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Components.h"
#include "XULWin/Unicode.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <sstream>


namespace XULWin
{

    // Helper function for closing the window
    LRESULT onCancel(Window * inWindow, WPARAM, LPARAM)
    {
        inWindow->close();
        return 0;
    }


    ConfigSample::ConfigSample(HMODULE inModuleHandle, const std::string & inAppDir) :
        mAppDir(inAppDir),
        mXULRunner(inModuleHandle)
    {
    }


    void ConfigSample::run()
    {
        WinAPI::CurrentDirectoryChanger curdir(mAppDir);
        mConfigWindow = mXULRunner.loadApplication("application.ini");
        if (!mConfigWindow)
        {
            ReportError("Failed to load application.ini file for configpanel.");
            return;
        }

        mSetsPopup = mConfigWindow->getElementById("setsMenuList");


        mNewSetButton = mConfigWindow->getElementById("newSetButton");


        //
        // Usage of the ScopedEventListener class is illustrated below.
        // I think the code is self-explanatory. Let me know if it isn't.
        //


        // Connect the button click action with the showNewSetDialog method.
        mEvents.connect(mNewSetButton,
                        boost::bind(&ConfigSample::showNewSetDialog, this));


        // Connect the upload button with the showUpload method.
        mEvents.connect(mConfigWindow->getElementById("uploadButton"),
                        boost::bind(&ConfigSample::showUpload, this));


        if (Window * win = mConfigWindow->component()->downcast<Window>())
        {
            ::DragAcceptFiles(win->handle(), TRUE);

            // Connect the WM_DROPFILES message with the dropFiles method.
            mEvents.connect(mConfigWindow.get(),
                            WM_DROPFILES,
                            boost::bind(&ConfigSample::dropFiles, this, _1, _2));

            // Connect the cancel button with the onCancel function.
            mEvents.connect(mConfigWindow->getElementById("cancelButton"),
                            boost::bind(&onCancel, win, _1, _2));

            win->showModal(WindowPos_CenterInScreen);
        }
    }


    LRESULT ConfigSample::dropFiles(WPARAM wParam, LPARAM lParam)
    {
        // Obtain the number of dropped files.
        int numFiles = ::DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
        for (int idx = 0; idx < numFiles; ++idx)
        {
            TCHAR fileName[MAX_PATH];

            // Obtain the filename for the dropped file.
            ::DragQueryFile((HDROP)wParam, idx, &fileName[0], MAX_PATH);
            showMessage(ToUTF8(&fileName[0]));
        }
        return 0;
    }


    LRESULT ConfigSample::showMessage(const std::string & inMessage)
    {
        ::MessageBoxA(0, inMessage.c_str(), "Message", MB_OK);
        return 0;
    }


    LRESULT ConfigSample::showUpload()
    {
        ::MessageBox(0, TEXT("Upload initiated!"), TEXT("Config panel"), MB_OK);
        return 0;
    }


    void ConfigSample::addNewSet(const std::string & inSetName)
    {
        AttributesMapping attr;
        attr["label"] = inSetName;
        if (!mSetsPopup->children().empty())
        {
            if (XMLMenuPopup * popup = mSetsPopup->children()[0]->downcast<XMLMenuPopup>())
            {
                ElementPtr item = XMLMenuItem::Create(popup, attr);
                item->init();
            }
        }
    }


    LRESULT ConfigSample::closeDialog(Dialog * inDialog, DialogResult inDialogResult)
    {
        return inDialog->endModal(inDialogResult);
    }


    LRESULT ConfigSample::showNewSetDialog()
    {
        mNewSetDlg = mXULRunner.loadXULFromFile("chrome://configpanel/content/newsetdialog.xul");
        mNewSetTextBox = mNewSetDlg->getElementById("settextbox");
        mNewSetOK = mNewSetDlg->getElementById("newSetOKButton");
        mNewSetCancel = mNewSetDlg->getElementById("newSetCancelButton");

        ScopedEventListener localEvents;
        Dialog * nativeDialog = mNewSetDlg->component()->downcast<Dialog>();
        ScopedEventListener::Action onOK = boost::bind(&ConfigSample::closeDialog,
                                                       this,
                                                       nativeDialog,
                                                       XULWin::DialogResult_Ok);
        localEvents.connect(mNewSetOK, onOK);

        ScopedEventListener::Action onCancel = boost::bind(&ConfigSample::closeDialog,
                                                           this,
                                                           nativeDialog,
                                                           XULWin::DialogResult_Cancel);
        localEvents.connect(mNewSetCancel, onCancel);

        if (Dialog * dlg = mNewSetDlg->component()->downcast<Dialog>())
        {
            if (Window * wnd = mConfigWindow->component()->downcast<Window>())
            {
                // Set the focus on the textbox
                XULWin::Element * textEl = mNewSetDlg->getElementById("settextbox");
                if (textEl && textEl->component())
                {
                    XULWin::TextBox * textBox = textEl->component()->downcast<XULWin::TextBox>();
                    if (textBox)
                    {
                        ::SetFocus(textBox->handle());
                    }
                }

                // Show the dialog
                DialogResult dialogResult = dlg->showModal(wnd);
                if (dialogResult == DialogResult_Ok)
                {
                    // Get the "value" attribute of the text box.
                    std::string setName = mNewSetDlg->getElementById("settextbox")->getAttribute("value");
                    addNewSet(setName);
                }
            }
        }
        return 0;
    }


} // namespace XULWin
