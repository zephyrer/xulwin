#ifndef VIRTUALCOMPONENT_H_INCLUDED
#define VIRTUALCOMPONENT_H_INCLUDED


#include "XULWin/ConcreteComponent.h"


namespace XULWin
{

    /**
     * VirtualComponent
     *
     * A virtual component is a widget that does not map to a native HWND.
     * Layout elements (box, grid, deck ..) are implemented as virtual components.
     */
    class VirtualComponent : public ConcreteComponent
    {
    public:
        typedef ConcreteComponent Super;

        VirtualComponent(Component * inParent, const AttributesMapping & inAttr);

        virtual ~VirtualComponent();

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const = 0;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const = 0;

        virtual void rebuildLayout();

        virtual Rect clientRect() const;

        virtual void move(int x, int y, int w, int h);

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    protected:
        Rect mRect;
    };

} // namespace XULWin


#endif // VIRTUALCOMPONENT_H_INCLUDED
