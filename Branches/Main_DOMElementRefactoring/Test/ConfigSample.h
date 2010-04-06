#ifndef CONFIGSAMPLE_H_INCLUDED
#define CONFIGSAMPLE_H_INCLUDED


#include "ImageViewerSample.h"
#include "XULWin/Decorator.h"
#include "XULWin/EventListener.h"
#include "XULWin/Component.h"
#include "XULWin/Initializer.h"
#include "XULWin/XULRunner.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Fallible.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Js/JsXULRunner.h"
#include <string>


namespace XULWin 
{

    class ConfigSample
    {
    public:
        ConfigSample(const std::string & inPathToXULRunnerSamples);

        void run();

        LRESULT dropFiles(WPARAM wParam, LPARAM lParam);

        LRESULT showMessage(const std::string & inMessage);

        LRESULT showUpload();

        //LRESULT addNewSet(const std::string & inSetName);

        //LRESULT showNewSetDialog();

        //LRESULT newSetOK();

        LRESULT closeDialog(Poco::XML::Element * inWindow);

    private:
        Poco::XML::Element * mConfigWindow;
        Poco::XML::Element * mNewSetDlg;
        Js::JsXULRunner mXULRunner;
        Poco::XML::Element* mNewSetButton;
        Poco::XML::Element* mSetsPopup;    
        Poco::XML::Element* mNewSetTextBox;
        Poco::XML::Element* mNewSetOK;
        Poco::XML::Element* mNewSetCancel;
        ScopedEventListener mEvents;
        std::string mPathToXULRunnerSamples;
    };


} // namespace XULWin


#endif // CONFIGSAMPLE_H_INCLUDED
