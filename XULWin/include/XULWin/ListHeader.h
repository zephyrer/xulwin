#ifndef LISTHEADERIMPL_H_INCLUDED
#define LISTHEADERIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/PhonyComponent.h"


namespace XULWin
{

    class ListHeader : public PhonyComponent,
                       public LabelController
    {
    public:
        typedef PhonyComponent Super;

        ListHeader(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

    private:
        std::string mLabel;
    };

} // namespace XULWin


#endif // LISTHEADERIMPL_H_INCLUDED
