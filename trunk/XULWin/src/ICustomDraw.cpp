#include "XULWin/ICustomDraw.h"
#include <cassert>


namespace XULWin
{

namespace WinAPI
{


    ICustomDraw::Parents ICustomDraw::sParents;


    ICustomDraw::ICustomDraw() :
        mInitialized(false),
        mHandle(0),
        mParentProc(0)
    {
    }


    ICustomDraw::~ICustomDraw()
    {
    }


    void ICustomDraw::initialize(HWND inHandle)
    {
        if (!mInitialized)
        {
            mHandle = inHandle;
            HWND parentHWND = ::GetParent(mHandle);
            mParentProc = (WNDPROC)(LONG_PTR)SetWindowLongPtr(parentHWND, GWLP_WNDPROC, (LONG)(LONG_PTR)ICustomDraw::ParentProc);
            sParents.insert(std::make_pair(this, parentHWND));
            mInitialized = true;
        }
    }


    void ICustomDraw::finalize()
    {
        if (mInitialized)
        {
            assert(mParentProc);
            HWND parentHWND = ::GetParent(mHandle);
            (WNDPROC)(LONG_PTR)SetWindowLongPtr(parentHWND, GWLP_WNDPROC, (LONG)(LONG_PTR)mParentProc);
            
            Parents::iterator it = sParents.find(this);
            if (it != sParents.end())
            {
                sParents.erase(it);
            }
            mInitialized = false;
        }
    }


    LRESULT CALLBACK ICustomDraw::ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        Parents::iterator it = sParents.begin(), end = sParents.end();
        for (; it != end; ++it)
        {
            if (it->second == hWnd)
            {
                break;
            }
        }

        if (it == sParents.end())
        {
            return DefWindowProc(hWnd, inMessage, wParam, lParam);
        }

        ICustomDraw * pThis = it->first;
        return pThis->parentProc(inMessage, wParam, lParam);
    }


    LRESULT ICustomDraw::parentProc(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (inMessage == WM_NOTIFY)
        {
            LPNMHDR messageHeader = (LPNMHDR)lParam;
            if (messageHeader->hwndFrom == mHandle)
            {
                if (messageHeader->code == NM_CUSTOMDRAW)
                {
                    LPNMCUSTOMDRAW customDrawMsg = (LPNMCUSTOMDRAW)lParam;
                    switch (customDrawMsg->dwDrawStage)
                    {
                        case CDDS_PREPAINT:
                        {
                            return CDRF_NOTIFYITEMDRAW;
                        }
                        case CDDS_ITEMPREPAINT:
                        {
                            return onCustomDraw_ItemPrePaint(wParam, lParam);
                        }
                        case CDDS_ITEMPOSTPAINT:
                        {
                            return onCustomDraw_ItemPostPaint(wParam, lParam);
                        }
                    }
                }
            }
        }
        return CallWindowProc(mParentProc, mHandle, inMessage, wParam, lParam);
    }


} // namespace WinAPI

} // namespace XULWin
