#ifndef LISTCELLIMPL_H_INCLUDED
#define LISTCELLIMPL_H_INCLUDED


#include "XULWin/VirtualComponent.h"


namespace XULWin
{

    class ListCell : public VirtualComponent,
                     public virtual LabelController
    {
    public:
        typedef VirtualComponent Super;

        ListCell(Component * inParent, const AttributesMapping & inAttr);

        virtual bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual std::string getLabel() const
        {
            return mLabel;
        }

        virtual void setLabel(const std::string & inLabel)
        {
            mLabel = inLabel;
        }

    private:
        std::string mLabel;
    };

} // namespace XULWin


#endif // LISTCELLIMPL_H_INCLUDED
