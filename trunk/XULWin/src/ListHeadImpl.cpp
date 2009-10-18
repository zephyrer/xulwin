#include "XULWin/ListHeadImpl.h"
#include "XULWin/ListViewImpl.h"
#include "XULWin/ListHeadImpl.h"
#include "XULWin/ListHeaderImpl.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"


namespace XULWin
{

    ListHeadImpl::ListHeadImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListHeadImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool ListHeadImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListHeadImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListHeadImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    void ListHeadImpl::onChildAdded(Component * inChild)
    {
        if (ListViewImpl * listViewParent = parent()->downcast<ListViewImpl>())
        {
            if (ListHeaderImpl * listHeader = inChild->downcast<ListHeaderImpl>())
            {
                listViewParent->addListHeader(listHeader);
            }
        }
        else
        {
            ReportError("Invalid container for ListHeader!");
        }
    }

    
} // namespace XULWin
