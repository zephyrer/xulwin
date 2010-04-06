#ifndef IMAGEVIEWER_H_INCLUDED
#define IMAGEVIEWER_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Windows.h"
#include <boost/scoped_ptr.hpp>
#include <string>


namespace XULWin
{
    class Window;
    class XULRunner;

    class ImageViewerSample
    {
    public:
        ImageViewerSample(const std::string & inPathToXULRunnerSamples);

        void run();

        LRESULT dropFiles(WPARAM wParam, LPARAM lParam);

    private:
        boost::scoped_ptr<XULRunner> mXULRunner;
        Window * mNativeWindow;
        ScopedEventListener mEvents;
        std::string mPathToXULRunnerSamples;
    };


} // namespace XULWin


#endif // IMAGEVIEWER_H_INCLUDED
