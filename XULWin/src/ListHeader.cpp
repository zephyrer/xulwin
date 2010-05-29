#include "XULWin/ListHeader.h"


namespace XULWin
{

    ListHeader::ListHeader(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    bool ListHeader::init()
    {
        return Super::init();
    }


    bool ListHeader::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    std::string ListHeader::getLabel() const
    {
        return mLabel;
    }


    void ListHeader::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


} // namespace XULWin
