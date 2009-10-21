#ifndef IMAGEVIEWER_H_INCLUDED
#define IMAGEVIEWER_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Windows.h"
#include <string>


namespace XULWin
{
    class Window;
    class ImageViewerSample
    {
    public:
        ImageViewerSample(const std::string & inPathToXULRunnerSamples);

        void run();

        LRESULT dropFiles(WPARAM wParam, LPARAM lParam);

    private:
        ElementPtr mRootElement;
        Window * mNativeWindow;
        ScopedEventListener mEvents;
        std::string mPathToXULRunnerSamples;
    };


} // namespace XULWin


#endif // IMAGEVIEWER_H_INCLUDED
