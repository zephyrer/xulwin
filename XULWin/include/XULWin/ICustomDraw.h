#ifndef ICUSTOMDRAW_H_INCLUDED
#define ICUSTOMDRAW_H_INCLUDED


#include "XULWin/Windows.h"
#include <map>


namespace XULWin
{

namespace WinAPI
{

    class ICustomDraw
    {
    public:
        ICustomDraw();

        virtual ~ICustomDraw() = 0;

        void initialize(HWND inHandle);

        void finalize();

        /**
         * Return values:
         *   CDRF_NOTIFYITEMDRAW
         *   CDRF_SKIPDEFAULT
         */
        virtual LRESULT onCustomDraw_ItemPrePaint(WPARAM wParam, LPARAM lPARAM) = 0;

        /**
         * Return values:
         *   CDRF_NOTIFYPOSTPAINT
         *   CDRF_SKIPDEFAULT
         */
        virtual LRESULT onCustomDraw_ItemPostPaint(WPARAM wParam, LPARAM lPARAM) = 0;

    private:
        static LRESULT CALLBACK ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);            

        typedef std::map<ICustomDraw*, HWND> Parents;
        static Parents sParents;
        
        LRESULT parentProc(UINT inMessage, WPARAM wParam, LPARAM lParam);

        bool mInitialized;
        HWND mHandle;
        WNDPROC mParentProc;
    };

} // namespace WinAPI

} // namespace XULWin


#endif // ICUSTOMDRAW_H_INCLUDED
