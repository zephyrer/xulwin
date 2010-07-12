#ifndef WINDOWSLISTBOX_H_INCLUDED
#define WINDOWSLISTBOX_H_INCLUDED


#include "XULWin/Types.h"
#include "XULWin/Windows.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>


namespace XULWin
{

namespace WinAPI
{

    class ListBox
    {
    public:
        ListBox (HMODULE inModuleHandle,
                  HWND inParent,
                  int inChildId);

        ~ListBox();

        HMODULE moduleHandle() const;

        HWND handle() const;

        void add(const std::string & inText);

        int getSelectedIndex() const;

        std::string getByIndex(size_t inIndex) const;

        void removeByIndex(size_t inIndex);

        size_t size() const;

        bool empty() const;

        void clear();

    private:
        HMODULE mModuleHandle;
        HWND mHandle;
        int mChildId;
    };


} // namespace WinAPI

} // namespace XULWin


#endif // WINDOWSLISTBOX_H_INCLUDED
