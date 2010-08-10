#ifndef WINDOWSLISTVIEW_H_INCLUDED
#define WINDOWSLISTVIEW_H_INCLUDED

#include "XULWin/Types.h"
#include "XULWin/Windows.h"
#include "XULWin/GdiplusLoader.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>


namespace Gdiplus
{
    class Image;
}


namespace XULWin
{

namespace WinAPI
{

    class ListItem;
    typedef boost::shared_ptr<ListItem> ListItemPtr;

    class ListView : public GdiplusLoader
    {
    public:
        typedef std::vector<ListItemPtr> ListItems;

        ListView (HMODULE inModuleHandle,
                  HWND inParent,
                  int inChildId);

        ~ListView();

        void setNumColumns(size_t inNumColumns);

        // Takes ownership
        void add(ListItem * inListItem);

        size_t size() const;

        bool empty() const;

        void removeByIndex(size_t inIndex);

        void clear();

        const ListItem * getByIndex(size_t inIndex) const;

        ListItem * getByIndex(size_t inIndex);

        HMODULE moduleHandle() const;

        HWND handle() const;

    private:
        static LRESULT CALLBACK ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);

        typedef std::map<ListView *, HWND> Parents;
        static Parents sParents;

        HMODULE mModuleHandle;
        HWND mParent;
        HWND mHandle;
        int mChildId;
        WNDPROC mParentProc;
        ListItems mListItems;
    };


    class ListItem
    {
    public:
        ListItem(ListView * inListView);

        virtual void draw(LPNMLVCUSTOMDRAW inMsg, const RECT & inRect) = 0;

    private:
        ListView * mListView;
    };


    class ListItem_Text : public ListItem
    {
    public:
        ListItem_Text(ListView * inListView, const std::string & inText);

        virtual void draw(LPNMLVCUSTOMDRAW inMsg, const RECT & inRect);

    private:
        std::wstring mText;
    };


    class ListItem_Image : public ListItem,
                           public GdiplusLoader
    {
    public:
        // Takes ownership
        ListItem_Image(ListView * inListView, Gdiplus::Image * inImage);

        virtual void draw(LPNMLVCUSTOMDRAW inMsg, const RECT & inRect);

    private:
        boost::scoped_ptr<Gdiplus::Image> mImage;
    };


} // namespace WinAPI

} // namespace XULWin


#endif // WINDOWSLISTVIEW_H_INCLUDED
