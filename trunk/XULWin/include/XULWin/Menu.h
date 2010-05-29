#ifndef MENUCOMPONENT_H_INCLUDED
#define MENUCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    class Menu : public DummyComponent,
                 public LabelController
    {
    public:
        typedef DummyComponent Super;

        Menu(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~Menu();

        virtual bool init();

        virtual bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        static Menu * FindById(int inId);

        static Windows::MenuNode * FromMenu(const Menu * inMenu);

    private:
        typedef std::map<int, Menu *> MenusById;
        static MenusById sMenusById;
        std::string mLabel;
        HMENU mMenuHandle;
    };

} // namespace XULWin


#endif // MENUCOMPONENT_H_INCLUDED
