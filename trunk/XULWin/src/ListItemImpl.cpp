#include "XULWin/ListItemImpl.h"
#include "XULWin/Decorator.h"
#include "XULWin/ListBoxImpl.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    ListItemImpl::ListItemImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping),
        mSelected(false)
    {
    }
        
        
    bool ListItemImpl::initImpl()
    {
        return Super::initImpl();
    }


    std::string ListItemImpl::getLabel() const
    {
        return mLabel;
    }

    
    void ListItemImpl::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


    bool ListItemImpl::isSelected() const
    {
        return mSelected;
    }


    void ListItemImpl::setSelected(bool inSelected)
    {
        mSelected = inSelected;
    }


    bool ListItemImpl::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        setAttributeController("selected", static_cast<SelectedController*>(this));
        return Super::initAttributeControllers();
    }


    int ListItemImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        static int cMargin = 4;
        int result = 0; 
        if (ListBoxImpl * listBox = parent()->downcast<ListBoxImpl>())
        {
            int width = Windows::getTextSize(listBox->handle(), getLabel()).cx;
            int extraWidth = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(listBox->handle()).cx;
            result = width + extraWidth + cMargin;
        }
        return result;
    }


    int ListItemImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (ListBoxImpl * listBox = parent()->downcast<ListBoxImpl>())
        {
            RECT rect;
            Windows::getListBoxItemRect(listBox->handle(), Windows::getListBoxIndexOf(listBox->handle(), getLabel()), rect);
            return rect.bottom - rect.top;
        }
        return 0;
    }
    
} // namespace XULWin
