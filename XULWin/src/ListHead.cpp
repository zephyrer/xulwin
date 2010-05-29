#include "XULWin/ListHead.h"
#include "XULWin/ListView.h"
#include "XULWin/ListHead.h"
#include "XULWin/ListHeader.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"


namespace XULWin
{

    ListHead::ListHead(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListHead::init()
    {
        return Super::init();
    }


    bool ListHead::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int ListHead::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int ListHead::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    void ListHead::onChildAdded(Component * inChild)
    {
        if (ListView * listViewParent = parent()->downcast<ListView>())
        {
            if (ListHeader * listHeader = inChild->downcast<ListHeader>())
            {
                listViewParent->addListHeader(listHeader);
            }
        }
        else
        {
            ReportError("Invalid container for XMLListHeader!");
        }
    }


} // namespace XULWin
