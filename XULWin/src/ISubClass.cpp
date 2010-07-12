#include "XULWin/ISubClass.h"
#include <cassert>


namespace XULWin
{

namespace WinAPI
{


    ISubClass::Instances ISubClass::sInstances;


    ISubClass::ISubClass() :
        mInitialized(false),
        mHandle(0),
        mWindowProc(0)
    {
    }


    ISubClass::~ISubClass()
    {
        finalize();
    }


    void ISubClass::initialize(HWND inHandle)
    {
        if (!mInitialized)
        {
            mHandle = inHandle;
            sInstances.insert(std::make_pair(mHandle, this));
            mWindowProc = (WNDPROC)(LONG_PTR)SetWindowLongPtr(mHandle, GWLP_WNDPROC, (LONG)(LONG_PTR)ISubClass::HandleMessage);
            mInitialized = true;
        }
    }


    void ISubClass::finalize()
    {
        if (mInitialized)
        {
            (WNDPROC)(LONG_PTR)SetWindowLongPtr(mHandle, GWLP_WNDPROC, (LONG)(LONG_PTR)mWindowProc);
            Instances::iterator it = sInstances.find(mHandle);
            if (it != sInstances.end())
            {
                sInstances.erase(it);
            }            
            mInitialized = false;
        }
    }


    HWND ISubClass::handle() const
    {
        return mHandle;
    }


    LRESULT CALLBACK ISubClass::HandleMessage(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        Instances::iterator it = sInstances.find(hWnd);
        if (it == sInstances.end())
        {
            return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
        }

        ISubClass * pThis = it->second;
        return pThis->ISubClass_handleMessage(inMessage, wParam, lParam);
    }


} // namespace WinAPI

} // namespace XULWin
