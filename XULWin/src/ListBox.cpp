#include "XULWin/ListBox.h"
#include "XULWin/Element.h"
#include "XULWin/ListItem.h"
#include "XULWin/MarginDecorator.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{
    
    Component * CreateListBox(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new ListBox(inComponent, inAttr));
    }


    ListBox::ListBox(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr, TEXT("LISTBOX"), WS_EX_CLIENTEDGE, 0),
        mRows(0)
    {
        mRows.setInvalid();
    }


    void ListBox::onChildAdded(Component * inChild)
    {
        if (XMLListBox * listBox = el()->downcast<XMLListBox>())
        {
            if (ListItem * item = inChild->downcast<ListItem>())
            {
                WinAPI::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    bool ListBox::initAttributeControllers()
    {
        // FIXME: does the ListBox implement the "label" attribute?
        //setAttributeController<LabelController>(this);
        setAttributeController<RowsController>(this);
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
        return calculateMaxChildWidth(inSizeConstraint);
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
        int extraHeight = WinAPI::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;
        return result + extraHeight;
    }

} // namespace XULWin
