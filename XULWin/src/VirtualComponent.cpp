#include "XULWin/VirtualComponent.h"


namespace XULWin
{

    VirtualComponent::VirtualComponent(Component * inParent, const AttributesMapping & inAttr) :
        ConcreteComponent(inParent)
    {
    }


    VirtualComponent::~VirtualComponent()
    {

    }


    bool VirtualComponent::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    bool VirtualComponent::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    void VirtualComponent::move(int x, int y, int w, int h)
    {
        mRect = Rect(x, y, w, h);
    }


    void VirtualComponent::rebuildLayout()
    {
        rebuildChildLayouts();
    }


    Rect VirtualComponent::clientRect() const
    {
        return mRect;
    }


    LRESULT VirtualComponent::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }

} // namespace XULWin
