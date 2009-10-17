#include "XULWin/ListViewImpl.h"
#include "XULWin/ListBox.h"
#include "XULWin/ListHeaderImpl.h"
#include "XULWin/ListItemImpl.h"
#include "XULWin/Decorator.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    ListViewImpl::ListViewImpl(ElementImpl * inParent,
                               const AttributesMapping & inAttributesMapping) :
        Super(inParent,
              inAttributesMapping,
              WC_LISTVIEW,
              WS_EX_CLIENTEDGE,
              LVS_REPORT)
    {
    }


    bool ListViewImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool ListViewImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListViewImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }

    
    int ListViewImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }
    
    
    void ListViewImpl::onChildAdded(ElementImpl * inChild)
    {
        if (ListBox * listBox = el()->downcast<ListBox>())
        {
            if (ListItemImpl * item = inChild->downcast<ListItemImpl>())
            {
                Windows::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    void ListViewImpl::addListHeader(ListHeaderImpl * inListHeader)
    {
        Windows::addColumnToListView(handle(), 0, inListHeader->getLabel());
    }
    
} // namespace XULWin
