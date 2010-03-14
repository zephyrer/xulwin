#ifndef {{COMPONENT_NAME_UPPER}}_H_INCLUDED
#define {{COMPONENT_NAME_UPPER}}_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class {{COMPONENT_NAME}} : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        {{COMPONENT_NAME}}(Component * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // {{COMPONENT_NAME_UPPER}}_H_INCLUDED
