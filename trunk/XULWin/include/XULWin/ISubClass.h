#ifndef ISUBCLASS_H_INCLUDED
#define ISUBCLASS_H_INCLUDED


#include "XULWin/Windows.h"
#include <map>


namespace XULWin
{

namespace WinAPI
{

    class ISubClass
    {
    public:
        ISubClass();

        virtual ~ISubClass() = 0;

        void initialize(HWND inHandle);

        void finalize();

        HWND handle() const;

        // The caller most either return 0, 1 or CallWindowProc(...)
        virtual LRESULT ISubClass_handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam) = 0;

    private:
        static LRESULT CALLBACK HandleMessage(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);

        typedef std::map<HWND, ISubClass*> Instances;
        static Instances sInstances;

        bool mInitialized;
        HWND mHandle;
        WNDPROC mWindowProc;
    };

} // namespace WinAPI

} // namespace XULWin


#endif // ISUBCLASS_H_INCLUDED
