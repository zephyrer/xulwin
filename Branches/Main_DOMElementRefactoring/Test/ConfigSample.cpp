#include "ConfigSample.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/ElementUtilities.h"
#include "XULWin/Decorator.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "XULWin/Windows.h"
#include "Poco/Path.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeList.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <sstream>
#include <Shellapi.h>


namespace XULWin
{


    LRESULT closeWindowHelper(Window * inWindow, WPARAM, LPARAM)
    {
        inWindow->close();
        return 0;
    }


    ConfigSample::ConfigSample(const std::string & inPathToXULRunnerSamples) : 
        mPathToXULRunnerSamples(inPathToXULRunnerSamples),
        mNewSetButton(0),
        mSetsPopup(0),   
        mNewSetTextBox(0),
        mNewSetOK(0),
        mNewSetCancel(0)
    {
    }


    void ConfigSample::run()
    {
        Poco::Path appPath(mPathToXULRunnerSamples, "configpanel");
        Windows::CurrentDirectoryChanger curdir(appPath.toString());
        Poco::XML::Document * document = mXULRunner.loadApplication("application.ini");
        if (!document)
        {
            ReportError("Failed to load application.ini file for configpanel.");
            return;
        }

        mConfigWindow = mXULRunner.rootElement();
        
        mSetsPopup = mXULRunner.document()->getElementById("setsMenuList");


        mNewSetButton = mXULRunner.document()->getElementById("newSetButton");
        //mEvents.connect(GetComponent<NativeComponent>(mNewSetButton),
        //                boost::bind(&ConfigSample::showNewSetDialog, this));

        
        mEvents.connect(GetComponent<NativeComponent>(mXULRunner.document()->getElementById("allowRatingsCheckBox")),
                        boost::bind(&ConfigSample::showMessage, this, "Checked"));

        
        mEvents.connect(GetComponent<NativeComponent>(mXULRunner.document()->getElementById("tagsTextBox")),
                        WM_KEYUP,
                        boost::bind(&ConfigSample::showMessage, this, "Received WM_KEYUP event."));


        mEvents.connect(GetComponent<NativeComponent>(mXULRunner.document()->getElementById("uploadButton")),
                        boost::bind(&ConfigSample::showUpload, this));


        if (Window * win = GetComponent<Window>(mConfigWindow))
        {
            ::DragAcceptFiles(win->handle(), TRUE);
            mEvents.connect(win,
                            WM_DROPFILES,
                            boost::bind(&ConfigSample::dropFiles, this, _1, _2));

            Poco::XML::Element * cancelButton = mXULRunner.document()->getElementById("cancelButton");
            ScopedEventListener::Action closeAction = boost::bind(&closeWindowHelper, win, _1, _2);
            mEvents.connect(GetComponent<NativeComponent>(cancelButton), closeAction);
            win->showModal(WindowPos_CenterInScreen);
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


    //LRESULT ConfigSample::addNewSet(const std::string & inSetName)
    //{ 
    //    AttributesMapping attr;
    //    attr["label"] = inSetName;
    //    if (mSetsPopup->childNodes()->length() == 0)
    //    {
    //        return EventResult_NotHandled;
    //    }

    //    if (MenuPopup * popup = mSetsPopup->children()[0]->downcast<MenuPopup>())
    //    {
    //        Poco::XML::Element * item = MenuItemElement::Create(popup, attr);
    //        item->init();
    //        return 0;
    //    }
    //    return EventResult_Handled;
    //}


    //LRESULT ConfigSample::showNewSetDialog()
    //{
    //    mNewSetDlg = mXULRunner.loadXULFromFile("chrome://configpanel/content/newsetdialog.xul");

    //    mNewSetTextBox = mXULRunner.document()->getElementById("settextbox");
    //    
    //    mNewSetOK = mXULRunner.document()->getElementById("newSetOKButton");
    //    ScopedEventListener localEvents;
    //    localEvents.connect(GetComponent<NativeComponent>(mNewSetOK), boost::bind(&ConfigSample::newSetOK, this));

    //    mNewSetCancel = mXULRunner.document()->getElementById("newSetCancelButton");
    //    localEvents.connect(GetComponent<NativeComponent>(mNewSetCancel), boost::bind(&ConfigSample::closeDialog, this, mNewSetDlg));

    //    if (Dialog * dlg = GetComponent<Dialog>(mNewSetDlg))
    //    {
    //        if (Window * wnd = GetComponent<Window>(mConfigWindow))
    //        {
    //            dlg->showModal(wnd);
    //        }
    //    }
    //    return 0;
    //}


    //LRESULT ConfigSample::newSetOK()
    //{        
    //    AttributesMapping attr;
    //    if (TextBox * nativeTextBox = GetComponent<TextBox>(mNewSetTextBox))
    //    {
    //        addNewSet(nativeTextBox->getValue());
    //    }
    //    if (Dialog * nativeDialog = GetComponent<Dialog>(mNewSetDlg))
    //    {
    //        nativeDialog->endModal(XULWin::DialogResult_Ok);
    //        return 0;
    //    }
    //    return 1;
    //}


    LRESULT ConfigSample::closeDialog(Poco::XML::Element * inDialog)
    {
        if (Dialog * nativeDialog = GetComponent<Dialog>(inDialog))
        {
            nativeDialog->endModal(XULWin::DialogResult_Cancel);
        }
        return 0;
    }


} // namespace XULWin
