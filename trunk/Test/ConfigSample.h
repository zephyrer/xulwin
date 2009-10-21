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
#include <string>


namespace XULWin 
{
    class Element;

    class ConfigSample
    {
    public:
        void run();

        LRESULT dropFiles(WPARAM wParam, LPARAM lParam);

        LRESULT showMessage(const std::string & inMessage);

        LRESULT showUpload();

        LRESULT addNewSet(const std::string & inSetName);

        LRESULT showNewSetDialog();

        LRESULT newSetOK();

        LRESULT closeDialog(Element * inWindow);

    private:
        XULRunner mRunner;   
        ElementPtr mConfigWindow;
        ElementPtr mNewSetDlg;
        XULWin::Fallible<Element*> mNewSetButton;
        XULWin::Fallible<Element*> mSetsPopup;    
        XULWin::Fallible<Element*> mNewSetTextBox;
        XULWin::Fallible<Element*> mNewSetOK;
        XULWin::Fallible<Element*> mNewSetCancel;
        ScopedEventListener mEvents;
    };


} // namespace XULWin


#endif // CONFIGSAMPLE_H_INCLUDED
