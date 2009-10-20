#ifndef MENUCOMPONENT_H_INCLUDED
#define MENUCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"
#include <windows.h>


namespace XULWin
{

    class Menu : public PassiveComponent,
                          public LabelController
    {
    public:
        typedef PassiveComponent Super;

        Menu(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~Menu();

        virtual bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        static Menu * FindById(int inId);

    private:
        typedef std::map<int, Menu*> MenusById;
        static MenusById sMenusById;
        std::string mLabel;
        HMENU mMenuHandle;
    };

} // namespace XULWin


#endif // MENUCOMPONENT_H_INCLUDED
