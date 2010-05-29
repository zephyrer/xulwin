#ifndef MENUITEMCOMPONENT_H_INCLUDED
#define MENUITEMCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"


namespace XULWin
{

    class MenuItem : public DummyComponent,
                     public LabelController
    {
    public:
        typedef DummyComponent Super;

        MenuItem(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~MenuItem();

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        static MenuItem * FindById(int inId);

    private:
        typedef std::map<int, MenuItem *> MenuItemsById;
        static MenuItemsById sMenuItemsById;

        std::string mLabel;
    };

} // namespace XULWin


#endif // MENUITEMCOMPONENT_H_INCLUDED
