#ifndef MENUITEMCOMPONENT_H_INCLUDED
#define MENUITEMCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class MenuItemComponent : public PassiveComponent,
                              public LabelController
    {
    public:
        typedef PassiveComponent Super;

        MenuItemComponent(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~MenuItemComponent();

        virtual bool initAttributeControllers();
            
        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        static MenuItemComponent * FindById(int inId);

    private:
        typedef std::map<int, MenuItemComponent*> MenuItemsById;
        static MenuItemsById sMenuItemsById;

        std::string mLabel;
    };

} // namespace XULWin


#endif // MENUITEMCOMPONENT_H_INCLUDED
