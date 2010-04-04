#include "ConfigSample.h"
#include "XULWin/MenuItemElement.h"
#include "XULWin/MenuPopupElement.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <sstream>


namespace XULWin
{


    LRESULT closeWindowHelper(Window * inWindow, WPARAM, LPARAM)
    {
        inWindow->close();
        return 0;
    }


    ConfigSample::ConfigSample(HMODULE inModuleHandle, const std::string & inPathToXULRunnerSamples) : 
        mRunner(inModuleHandle),
        mPathToXULRunnerSamples(inPathToXULRunnerSamples)
    {
    }


    void ConfigSample::run()
    {
        Poco::Path appPath(mPathToXULRunnerSamples, "configpanel");
        Windows::CurrentDirectoryChanger curdir(appPath.toString());
        mConfigWindow = mRunner.loadApplication("application.ini");
        if (!mConfigWindow)
        {
            ReportError("Failed to load application.ini file for configpanel.");
            return;
        }
        
        mSetsPopup = mConfigWindow->getElementById("setsMenuList");


        mNewSetButton = mConfigWindow->getElementById("newSetButton");
        mEvents.connect(mNewSetButton,
                        boost::bind(&ConfigSample::showNewSetDialog, this));

        
        mEvents.connect(mConfigWindow->getElementById("allowRatingsCheckBox"),
                        boost::bind(&ConfigSample::showMessage, this, "Checked"));

        
        mEvents.connect(mConfigWindow->getElementById("tagsTextBox"),
                        WM_KEYUP,
                        boost::bind(&ConfigSample::showMessage, this, "Received WM_KEYUP event."));


        mEvents.connect(mConfigWindow->getElementById("uploadButton"),
                        boost::bind(&ConfigSample::showUpload, this));


        if (Window * win = mConfigWindow->component()->downcast<Window>())
        {
            ::DragAcceptFiles(win->handle(), TRUE);
            mEvents.connect(mConfigWindow.get(),
                            WM_DROPFILES,
                            boost::bind(&ConfigSample::dropFiles, this, _1, _2));

            Element * cancelButton = mConfigWindow->getElementById("cancelButton");
            ScopedEventListener::Action closeAction = boost::bind(&closeWindowHelper, win, _1, _2);
            mEvents.connect(cancelButton, closeAction);
            win->showModal(WindowElement::CenterInScreen);
        }
    }


    LRESULT ConfigSample::dropFiles(WPARAM wParam, LPARAM lParam)
    {
        std::vector<std::string> files;
        int numFiles = ::DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
        for (int idx = 0; idx < numFiles; ++idx)
        {
    	    TCHAR fileName[MAX_PATH];
            ::DragQueryFile((HDROP)wParam, idx, &fileName[0], MAX_PATH);
            files.push_back(ToUTF8(&fileName[0]));
        }
        return 0;
    }


    LRESULT ConfigSample::showMessage(const std::string & inMessage)
    {
        std::stringstream ss;
        ss << inMessage;
        ::MessageBoxA(0, ss.str().c_str(), "Message", MB_OK);
        return 0;
    }


    LRESULT ConfigSample::showUpload()
    {
        ::MessageBox(0, TEXT("Upload initiated!"), TEXT("Config panel"), MB_OK);
        return 0;
    }


    LRESULT ConfigSample::addNewSet(const std::string & inSetName)
    { 
        AttributesMapping attr;
        attr["label"] = inSetName;
        if (!mSetsPopup->children().empty())
        {
            if (MenuPopupElement * popup = mSetsPopup->children()[0]->downcast<MenuPopupElement>())
            {
                ElementPtr item = MenuItemElement::Create(popup, attr);
                item->init();
                return 0;
            }
        }
        return 1;
    }


    LRESULT ConfigSample::showNewSetDialog()
    {
        mNewSetDlg = mRunner.loadXULFromFile("chrome://configpanel/content/newsetdialog.xul");

        mNewSetTextBox = mNewSetDlg->getElementById("settextbox");
        
        mNewSetOK = mNewSetDlg->getElementById("newSetOKButton");
        ScopedEventListener localEvents;
        localEvents.connect(mNewSetOK, boost::bind(&ConfigSample::newSetOK, this));

        mNewSetCancel = mNewSetDlg->getElementById("newSetCancelButton");
        localEvents.connect(mNewSetCancel, boost::bind(&ConfigSample::closeDialog, this, mNewSetDlg.get()));

        if (Dialog * dlg = mNewSetDlg->component()->downcast<Dialog>())
        {
            if (Window * wnd = mConfigWindow->component()->downcast<Window>())
            {
                dlg->showModal(wnd);
            }
        }
        return 0;
    }


    LRESULT ConfigSample::newSetOK()
    {        
        AttributesMapping attr;
        if (TextBox * nativeTextBox = mNewSetTextBox->component()->downcast<TextBox>())
        {
            addNewSet(nativeTextBox->getValue());
        }
        if (Dialog * nativeDialog = mNewSetDlg->component()->downcast<Dialog>())
        {
            nativeDialog->endModal(XULWin::DialogResult_Ok);
            return 0;
        }
        return 1;
    }


    LRESULT ConfigSample::closeDialog(Element * inDialog)
    {
        if (Dialog * nativeDialog = inDialog->component()->downcast<Dialog>())
        {
            nativeDialog->endModal(XULWin::DialogResult_Cancel);
        }
        return 0;
    }


} // namespace XULWin
