#include "XULWin/NewElementS.h"


namespace XULWin
{

    NewElementS::NewElementS(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool NewElementS::init()
    {
        return Super::init();
    }

    
    bool NewElementS::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int NewElementS::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int NewElementS::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
