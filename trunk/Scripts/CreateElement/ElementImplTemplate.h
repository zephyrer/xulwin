#ifndef {{ELEMENTIMPL_NAME_UPPER}}_H_INCLUDED
#define {{ELEMENTIMPL_NAME_UPPER}}_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class {{ELEMENTIMPL_NAME}} : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        {{ELEMENTIMPL_NAME}}(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initComponent();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // {{ELEMENTIMPL_NAME_UPPER}}_H_INCLUDED
