#include "XULWin/Contrib/Toolbar.h"


namespace XULWin
{

    Toolbar::Toolbar(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr)
    {
        if (NativeComponent * native = NativeControl::GetThisOrParent(inParent))
        {
            mToolbar.reset(new Windows::Toolbar(this, NativeComponent::GetModuleHandle(), native->handle(), mComponentId.value()));
            setHandle(mToolbar->handle(), false);
            registerHandle();
            subclass();
        }
    }


    Toolbar::~Toolbar()
    {
        unsubclass();
        unregisterHandle();
        mToolbar.reset();
    }


    bool Toolbar::init()
    {
        mToolbar->buildToolbar();
        mToolbar->rebuildLayout();
        ShowWindow(mToolbar->handle(), SW_SHOW);
        return Super::init();
    }


    int Toolbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildWidths(inSizeConstraint);
    }


    int Toolbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = calculateMaxChildHeight(inSizeConstraint);
        return result;
    }


    void Toolbar::rebuildLayout()
    {
        mToolbar->rebuildLayout();
    }

} // namespace XULWin
