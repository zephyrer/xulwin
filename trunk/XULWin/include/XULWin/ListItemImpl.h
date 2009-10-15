#ifndef LISTITEMIMPL_H_INCLUDED
#define LISTITEMIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class ListItemImpl : public PassiveComponent,
                         public LabelController,
                         public SelectedController
    {
    public:
        typedef PassiveComponent Super;

        ListItemImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        virtual bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        virtual bool isSelected() const;

        virtual void setSelected(bool inSelected);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        std::string mLabel;
        bool mSelected;
    };

} // namespace XULWin


#endif // LISTITEMIMPL_H_INCLUDED
