#include "ImageViewerSample.h"
#include "XULWin/XULRunner.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/EventListener.h"
#include "XULWin/ImageElement.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>


using namespace XULWin;


namespace XULWin
{
    
    ImageViewerSample::ImageViewerSample(HMODULE inModuleHandle, const std::string & inPathToXULRunnerSamples) :
        mModuleHandle(inModuleHandle),
        mNativeWindow(0),
        mPathToXULRunnerSamples(inPathToXULRunnerSamples)
    {
    }


    void ImageViewerSample::run()
    {
        Poco::Path appPath(mPathToXULRunnerSamples, "imageviewer");
        Windows::CurrentDirectoryChanger curdir(appPath.toString());

        //system("run.bat");

        XULRunner runner(mModuleHandle);
        mRootElement = runner.loadXUL("chrome://imageviewer/content/imageviewer.xul");
        if (!mRootElement)
        {
            ReportError("Failed to load ImageViewerSample");
            return;
        }

        ScopedEventListener events;
        events.connect(mRootElement.get(), WM_DROPFILES, boost::bind(&ImageViewerSample::dropFiles, this, _1, _2));
       
        if (mNativeWindow = mRootElement->component()->downcast<Window>())
        {
            ::DragAcceptFiles(mNativeWindow->handle(), TRUE);
            mNativeWindow->showModal(WindowElement::CenterInScreen);
        }
    }


    LRESULT ImageViewerSample::dropFiles(WPARAM wParam, LPARAM lParam)
    {
        Element * imageArea = mRootElement->getElementById("imagearea");
        if (!imageArea)
        {
            return 1;
        }

        int numFiles = ::DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
        for (int idx = 0; idx < numFiles; ++idx)
        {
            // Extract file name
        	TCHAR fileName[MAX_PATH];
            ::DragQueryFile((HDROP)wParam, idx, &fileName[0], MAX_PATH);

            // Create the image element
            AttributesMapping attr;
            attr["src"] = ToUTF8(&fileName[0]);
            attr["flex"] = "1";
            attr["width"] = "160";
            attr["flex"] = "0";
            attr["keepaspectratio"] = "1";
            ElementPtr image = ImageElement::Create(imageArea, attr);
            image->init();
        }
        mNativeWindow->rebuildLayout();
        ::RedrawWindow(mNativeWindow->handle(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
        return 0;
    }


} // namespace XULWin
