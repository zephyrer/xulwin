#ifndef LISTCELLIMPL_H_INCLUDED
#define LISTCELLIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/AttributeController.h"


namespace XULWin
{

    class ListCellImpl : public PassiveComponent,
                         public LabelController
    {
    public:
        typedef PassiveComponent Super;

        ListCellImpl(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initComponent();

        bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        std::string mLabel;
    };

} // namespace XULWin


#endif // LISTCELLIMPL_H_INCLUDED
