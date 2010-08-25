#include "XULWin/DetachedComponent.h"


namespace XULWin
{

    DetachedComponent::DetachedComponent(Component * inParent, const AttributesMapping & inAttr) :
        ConcreteComponent(inParent)
    {
    }
    
    
    DetachedComponent::~DetachedComponent()
    {
    }


    void DetachedComponent::move(int x, int y, int w, int h)
    {
        mRect = Rect(x, y, w, h);
    }


    Rect DetachedComponent::clientRect() const
    {
        return mRect;
    }


    void DetachedComponent::rebuildLayout()
    {
    }


    LRESULT DetachedComponent::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }


    int DetachedComponent::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }


    int DetachedComponent::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }

} // namespace XULWin
