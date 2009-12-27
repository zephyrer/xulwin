#include "XULWin/DynamicAttribute.h"
#include "XULWin/Conversions.h"
#include "XULWin/Defaults.h"


namespace XULWin
{
    
    Attribute::Attribute(const std::string & inName) :
        mName(inName)
    {
    }


    Attribute::~Attribute()
    {
    }

    
    const std::string & Attribute::name() const
    {
        return mName;
    }


} // namespace XULWin
