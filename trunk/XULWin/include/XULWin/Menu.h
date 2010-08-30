#ifndef MENUCOMPONENT_H_INCLUDED
#define MENUCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/DetachedComponent.h"
#include "XULWin/NativeControl.h"
#include "XULWin/PhonyComponent.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{
    
    namespace WinAPI
    {
        class PopupMenu;
    }


    class Menu : public PhonyComponent,
                 public LabelController
    {
    public:
        typedef PhonyComponent Super;

        Menu(Component * inParent, const AttributesMapping & inAttr);

        virtual ~Menu();

        virtual bool init();

        virtual bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        static Menu * FindById(int inId);

        static WinAPI::MenuNode * FromMenu(const Menu * inMenu);

    private:
        typedef std::map<int, Menu *> MenusById;
        static MenusById sMenusById;
        std::string mLabel;
        HMENU mMenuHandle;
    };


    class MenuBar : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        MenuBar(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class MenuItem : public VirtualComponent,
                     public LabelController
    {
    public:
        typedef VirtualComponent Super;

        MenuItem(Component * inParent, const AttributesMapping & inAttr);

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


    class MenuList : public NativeControl,
                     public SelectedIndexController
    {
    public:
        typedef NativeControl Super;

        MenuList(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        // SelectedIndexController methods
        int getSelectedIndex() const;

        void setSelectedIndex(int inIndex);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void move(int x, int y, int w, int h);

        virtual void onContentChanged();

    private:
        void fillComboBox();
    };


    class MenuPopup : public DetachedComponent
    {
    public:
        typedef DetachedComponent Super;

        MenuPopup(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    protected:
        virtual void onChildAdded(Component * inChild);

        virtual void onChildRemoved(Component * inChild);

    private:
        HWND mParentWindow;
        HMENU mHMENU;
    };


    class MenuSeparator : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        MenuSeparator(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const { return 0; }

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const { return 0; }
    };

} // namespace XULWin


#endif // MENUCOMPONENT_H_INCLUDED
