#ifndef OVERLAY_H_INCLUDED
#define OVERLAY_H_INCLUDED


#include "XULWin/Window.h"
#include <string>


namespace XULWin
{

    /**
     * Overlay is used for XUL Overlays.
     * This class for internal use only.
     *
     * NOTE: this class inherits Window because otherwise the Parser will complain that it can't
     *       create any native windows without a native parent.
     */
    class Overlay : public Window
    {
    public:
        typedef Window Super;

        Overlay(Component * inParent, const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // OVERLAY_H_INCLUDED
