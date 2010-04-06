#include "ImageViewerSample.h"
#include "XULWin/ComponentUtilities.h"
#include "XULWin/ElementUtilities.h"
#include "XULWin/XULRunner.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/EventListener.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>
#include <Shellapi.h>


using namespace XULWin;


namespace XULWin
{
    
    ImageViewerSample::ImageViewerSample(const std::string & inPathToXULRunnerSamples) :
        mXULRunner(new XULRunner),
        mNativeWindow(0),
        mPathToXULRunnerSamples(inPathToXULRunnerSamples)
    {
        
    }


    void ImageViewerSample::run()
    {
        Poco::Path appPath(mPathToXULRunnerSamples, "imageviewer");
        Windows::CurrentDirectoryChanger curdir(appPath.toString());

        //system("run.bat");

        
        mXULRunner->loadXULFromFile("chrome://imageviewer/content/imageviewer.xul");
        if (!mXULRunner->rootElement())
        {
            ReportError("Failed to load ImageViewerSample");
            return;
        }

        ScopedEventListener events;
        events.connect(GetComponent<NativeComponent>(mXULRunner->rootElement()),
                       WM_DROPFILES,
                       boost::bind(&ImageViewerSample::dropFiles, this, _1, _2));
       
        if (mNativeWindow = GetComponent<Window>(mXULRunner->rootElement()))
        {
            ::DragAcceptFiles(mNativeWindow->handle(), TRUE);
            mNativeWindow->showModal(WindowPos_CenterInScreen);
        }
    }


    LRESULT ImageViewerSample::dropFiles(WPARAM wParam, LPARAM lParam)
    {
        Poco::XML::Element * imageArea = mXULRunner->rootElement()->getElementById("id", "imagearea");
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
            Poco::XML::Element* image(mXULRunner->document()->createElement("image"));
            image->autoRelease();            
            image->setAttribute("src", ToUTF8(&fileName[0]));
            image->setAttribute("flex", "1");
            image->setAttribute("width", "160");
            image->setAttribute("flex", "0");
            image->setAttribute("keepaspectratio", "1");
            mXULRunner->document()->documentElement()->appendChild(image);
            throw std::runtime_error("Create the corresponding component object!");
        }
        mNativeWindow->rebuildLayout();
        ::RedrawWindow(mNativeWindow->handle(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
        return 0;
    }


} // namespace XULWin
