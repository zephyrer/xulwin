#include "ForeignWindow.h"
#include "XULWin/Decorator.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Unicode.h"


namespace XULWin
{

    Component * CreateForeignWindow(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ForeignWindow(inAttr));
    }


    ForeignWindowInfo::ForeignWindowInfo(HWND inHWND) :
        mClassName(WinAPI::Window_GetClassName(inHWND)),
        mTitle(WinAPI::Window_GetText(inHWND)),
        mRect(),
        mWINDOWPLACEMENT()
    {
        RECT rw;
        ::GetWindowRect(inHWND, &rw);
        mRect = Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
        
        mWINDOWPLACEMENT.length = 0;
        ::GetWindowPlacement(inHWND, &mWINDOWPLACEMENT);
    }


    XMLForeignWindow::XMLForeignWindow(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLForeignWindow::TagName(), inParent, CreateForeignWindow(inParent->component(), inAttr))
    {
    }
    
        
    void ClassNameController::get(std::string & outValue)
    {
        outValue = getClassName();
    }


    void ClassNameController::set(const std::string & inValue)
    {
        setClassName(inValue);
    }




    ForeignWindow::ForeignWindow(const AttributesMapping & inAttr) :
        NativeComponent(0, inAttr)
    {
    }


    ForeignWindow::~ForeignWindow()
    {
    }


    bool ForeignWindow::init()
    {
        std::wstring className = XULWin::ToUTF16(getClassName());
        std::wstring title = XULWin::ToUTF16(getTitle());
        mHandle = ::FindWindow(className.c_str(), title.empty() ? 0 : title.c_str());
        // registerHandle(); // ???
        return Super::init();
    }


    bool ForeignWindow::initAttributeControllers()
    {
        setAttributeController<ClassNameController>(this);
        setAttributeController<TitleController>(this);
        return Super::initAttributeControllers();
    }


    std::string ForeignWindow::getTitle() const
    {
        return WinAPI::Window_GetText(handle());
    }


    void ForeignWindow::setTitle(const std::string & inTitle)
    {
        WinAPI::Window_SetText(handle(), inTitle);
    }


    std::string ForeignWindow::getClassName() const
    {
        return WinAPI::Window_GetClassName(handle());
    }


    void ForeignWindow::setClassName(const std::string & inClassName)
    {
        mClassName = inClassName;
    }
    
    
    void ForeignWindow::rebuildLayout()
    {
        // No children. So no rebuild layout.
    }


    Rect ForeignWindow::clientRect() const
    {
        RECT rc;
        ::GetClientRect(handle(), &rc);
        return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }


    Rect ForeignWindow::windowRect() const
    {
        RECT rw;
        ::GetWindowRect(handle(), &rw);
        return Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
    }


    int ForeignWindow::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        throw std::runtime_error("ForeignWindow should have a fixed width.");
    }


    int ForeignWindow::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        throw std::runtime_error("ForeignWindow should have a fixed height.");
    }


    void ForeignWindow::move(int x, int y, int w, int h)
    {
        ::MoveWindow(handle(), x, y, w, h, FALSE);
    }


} // namespace XULWin
