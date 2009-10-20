#include "XULWin/ListBox.h"
#include "XULWin/ListBoxElement.h"
#include "XULWin/ListItem.h"
#include "XULWin/Decorator.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{


    ListBox::ListBox(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, TEXT("LISTBOX"), WS_EX_CLIENTEDGE, 0),
        mRows(0)
    {
        mRows.setInvalid();
    }

    
    bool ListBox::init()
    {
        return Super::init();
    }


    void ListBox::onChildAdded(Component * inChild)
    {
        if (ListBoxElement * listBox = el()->downcast<ListBoxElement>())
        {
            if (ListItem * item = inChild->downcast<ListItem>())
            {
                Windows::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    bool ListBox::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        setAttributeController("rows", static_cast<RowsController*>(this));
        return Super::initAttributeControllers();
    }
    
    
    int ListBox::getRows() const
    {
        return mRows.or(1);
    }

    
    void ListBox::setRows(int inRows)
    {
        mRows = inRows;
    }


    int ListBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            int width = getChild(idx)->calculateWidth(inSizeConstraint);
            if (width > result)
            {
                result = width;
            }
        }
        return result;
    }


    int ListBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        int itemCount = mRows.or(getChildCount());
        for (size_t idx = 0; idx != itemCount; ++idx)
        {
            if (idx < getChildCount())
            {
                result += getChild(idx)->calculateHeight(inSizeConstraint);
            }
            else
            {
                result += getChild(0)->calculateHeight(inSizeConstraint);
            }
        }
        int extraHeight = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;
        return result + extraHeight;
    }
    
} // namespace XULWin
