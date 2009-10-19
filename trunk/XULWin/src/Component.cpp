#include "XULWin/Component.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Layout.h"
#include "XULWin/Menu.h"
#include "XULWin/MenuBar.h"
#include "XULWin/MenuItem.h"
#include "XULWin/MenuPopup.h"
#include "XULWin/PopupMenu.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "Poco/String.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <gdiplus.h>


namespace XULWin
{

    int CommandId::sId = 101; // start handleCommand Ids at 101 to avoid conflicts with Windows predefined values
    
    NativeComponent::ComponentsByHandle NativeComponent::sComponentsByHandle;
    
    NativeComponent::ComponentsById NativeComponent::sComponentsById;    

    ConcreteComponent::ConcreteComponent(Component * inParent) :
        mParent(inParent),
        mCommandId(),
        mExpansive(false),
        mElement(0),
        mFlex(0),
        mHidden(false),
        mOrient(Horizontal),
        mAlign(Start),
        mCSSX(0),
        mCSSY(0),
        mWidth(0),
        mHeight(0),
        mScreenX(0),
        mScreenY(0),
        mFill(RGBColor()),
        mStroke(RGBColor(0, 0, 0, 0)),
        mStrokeWidth(1),
        mCSSWidth(0),
        mCSSHeight(0),
        mCSSFill(RGBColor()),
        mCSSStroke(RGBColor(0, 0, 0, 0)),
        mIsInitialized(false)
    {        
        mCSSX.setInvalid();
        mCSSY.setInvalid();
        mWidth.setInvalid();
        mHeight.setInvalid();
        mScreenX.setInvalid();
        mScreenY.setInvalid();
        mFill.setInvalid();
        mStroke.setInvalid();
        mStrokeWidth.setInvalid();
        mCSSWidth.setInvalid();
        mCSSHeight.setInvalid();
        mCSSFill.setInvalid();
        mCSSStroke.setInvalid();
        mOrient.setInvalid();
        mAlign.setInvalid();
    }


    ConcreteComponent::~ConcreteComponent()
    {
    }


    bool ConcreteComponent::initComponent()
    {
        mIsInitialized = true;
        return true;
    }
    
    
    int ConcreteComponent::getIndex() const
    {
        size_t idx = 0;
        for (; idx != parent()->getChildCount(); ++idx)
        {
            if (this == parent()->getChild(idx))
            {
                return idx;
            }
        }
        return idx;
    }


    size_t ConcreteComponent::getChildCount() const
    {
        return el()->children().size();
    }


    const Component * ConcreteComponent::getChild(size_t inIndex) const
    {
        return el()->children()[inIndex]->component();
    }

    
    Component * ConcreteComponent::getChild(size_t inIndex)
    {
        return el()->children()[inIndex]->component();
    }

    
    HWND ConcreteComponent::getFirstParentHandle()
    {
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(parent()))
        {
            return comp->handle();
        }
        return 0;
    }


    const RGBColor & ConcreteComponent::getCSSFill() const
    {
        if (mCSSFill.isValid())
        {
            return mCSSFill;
        }

        if (parent())
        {
            return parent()->getCSSFill();
        }

        return mCSSFill; // default value
    }        


    void ConcreteComponent::setFill(const RGBColor & inColor)
    {
        mFill = inColor;
    }


    const RGBColor & ConcreteComponent::getFill() const
    {
        return mFill;
    }


    void ConcreteComponent::setStroke(const RGBColor & inColor)
    {
        mStroke = inColor;
    }


    const RGBColor & ConcreteComponent::getStroke() const
    {
        if (mStroke.isValid())
        {
            return mStroke.getValue();
        }

        if (parent())
        {
            return parent()->getStroke();
        }

        return mStroke;
    }


    void ConcreteComponent::setStrokeWidth(int inStrokeWidth)
    {
        mStrokeWidth = inStrokeWidth;
    }


    int ConcreteComponent::getStrokeWidth() const
    {
        if (mStrokeWidth.isValid())
        {
            return mStrokeWidth.getValue();
        }

        if (parent())
        {
            return parent()->getStrokeWidth();
        }

        return mStrokeWidth;
    }
    
    
    void ConcreteComponent::setCSSFill(const RGBColor & inColor)
    {
        mCSSFill = inColor;
    }
    
    
    void ConcreteComponent::setCSSStroke(const RGBColor & inColor)
    {
        mCSSStroke = inColor;
    }


    const RGBColor & ConcreteComponent::getCSSStroke() const
    {
        if (mCSSStroke.isValid())
        {
            return mCSSStroke;
        }

        if (parent())
        {
            return parent()->getCSSStroke();
        }

        return mCSSStroke; // default value
    }


    int ConcreteComponent::getCSSX() const
    {
        return mCSSX;
    }


    void ConcreteComponent::setCSSX(int inX)
    {
        mCSSX = inX;
    }


    int ConcreteComponent::getCSSY() const
    {
        return mCSSY;
    }


    void ConcreteComponent::setCSSY(int inY)
    {
        mCSSY = inY;
    }


    int ConcreteComponent::getCSSWidth() const
    {
        if (mWidth.isValid())
        {
            return mWidth.getValue();
        }
        return mCSSWidth;
    }


    void ConcreteComponent::setCSSWidth(int inWidth)
    {
        mCSSWidth = inWidth;
    }

    
    int ConcreteComponent::getCSSHeight() const
    {
        if (mHeight.isValid())
        {
            return mHeight.getValue();
        }
        return mCSSHeight;
    }


    void ConcreteComponent::setCSSHeight(int inHeight)
    {
        mCSSHeight = inHeight;
    }


    int ConcreteComponent::getWidth() const
    {
        if (mWidth.isValid())
        {
            return mWidth;
        }

        if (mCSSWidth.isValid())
        {
            return std::max<int>(mCSSWidth.getValue(), getWidth(Minimum));
        }

        return getWidth(Optimal);
    }
    
    
    void ConcreteComponent::setWidth(int inWidth)
    {
        mWidth = inWidth;
    }


    int ConcreteComponent::getHeight() const
    {
        if (mHeight.isValid())
        {
            return mHeight;
        }

        if (mCSSHeight.isValid())
        {
            return std::max<int>(mCSSHeight.getValue(), getHeight(Minimum));
        }

        return getHeight(Optimal);
    }
    
    
    void ConcreteComponent::setHeight(int inHeight)
    {
        mHeight = inHeight;
    }


    int ConcreteComponent::getScreenX() const
    {
        if (mScreenX.isValid())
        {
            return mScreenX.getValue();
        }

        if (mCSSX.isValid())
        {
            return mCSSX.getValue();
        }

        return 100;
    }


    void ConcreteComponent::setScreenX(int inX)
    {
        mScreenX = inX;
    }


    int ConcreteComponent::getScreenY() const
    {
        if (mScreenY.isValid())
        {
            return mScreenY.getValue();
        }

        if (mCSSY.isValid())
        {
            return mCSSY.getValue();
        }

        return 100;
    }


    void ConcreteComponent::setScreenY(int inY)
    {
        mScreenY = inY;
    }


    int ConcreteComponent::getFlex() const
    {
        return mFlex;
    }
    
    
    void ConcreteComponent::setFlex(int inFlex)
    {
        mFlex = inFlex;
    }

    
    bool ConcreteComponent::isHidden() const
    {
        return mHidden;
    }


    void ConcreteComponent::setHidden(bool inHidden)
    {
        mHidden = inHidden;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            getChild(idx)->setHidden(inHidden);
        }
    }

    
    Orient ConcreteComponent::getOrient() const
    {
        return mOrient.or(Vertical);
    }


    void ConcreteComponent::setOrient(Orient inOrient)
    {
        mOrient = inOrient;
    }


    Align ConcreteComponent::getAlign() const
    {
        return mAlign.or((getOrient() == Vertical) ? Stretch : Start);
    }


    void ConcreteComponent::setAlign(Align inAlign)
    {
        mAlign = inAlign;
    }

    
    void ConcreteComponent::getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const
    {
        int margin = 0;
        if (const MarginDecorator * marginDecorator = downcast<MarginDecorator>())
        {
            marginDecorator->getMargin(outTop, outLeft, outRight, outBottom);
        }
    }


    void ConcreteComponent::setCSSMargin(int inTop, int inLeft, int inRight, int inBottom)
    {
        if (!el())
        {
            ReportError("ConcreteComponent::setCSSMargin failed because no owning element was present.");
            return;
        }

        // Find a margin decorator, and set the margin value.
        if (MarginDecorator * obj = el()->component()->downcast<MarginDecorator>())
        {
            obj->setMargin(inTop, inLeft, inRight, inBottom);
        }
        // If no margin decorator found, insert one, and set the value.
        else if (Decorator * dec = el()->component()->downcast<Decorator>())
        {
            ElementImplPtr newDec(new MarginDecorator(dec->decoratedElement()));
            dec->setDecoratedElement(newDec);
            if (MarginDecorator * p = newDec->downcast<MarginDecorator>())
            {
                p->setMargin(inTop, inLeft, inRight, inBottom);
            }
        }
        else
        {
            ReportError("No decorator found!");
        }
    }

    
    int ConcreteComponent::getWidth(SizeConstraint inSizeConstraint) const
    {
        if (isHidden())
        {
            return 0;
        }

        if (mWidth.isValid())
        {
            return mWidth.getValue();
        }

        return calculateWidth(inSizeConstraint);
    }

    
    int ConcreteComponent::getHeight(SizeConstraint inSizeConstraint) const
    {
        if (isHidden())
        {
            return 0;
        }

        if (mHeight.isValid())
        {
            return mHeight.getValue();
        }

        return calculateHeight(inSizeConstraint);
    }
    
    
    bool ConcreteComponent::expansive() const
    {
        return mExpansive;
    }
    
    
    bool ConcreteComponent::getStyle(const std::string & inName, std::string & outValue)
    {
        StyleControllers::iterator it = mStyleControllers.find(inName);
        if (it != mStyleControllers.end())
        {
            StyleController * controller = it->second;
            controller->get(outValue);
        }
        return false;
    }
    
    
    bool ConcreteComponent::getAttribute(const std::string & inName, std::string & outValue)
    {
        AttributeControllers::iterator it = mAttributeControllers.find(inName);
        if (it != mAttributeControllers.end())
        {
            it->second->get(outValue);
            return true;
        }
        return false;
    }
    
    
    bool ConcreteComponent::setStyle(const std::string & inName, const std::string & inValue)
    {
        StyleControllers::iterator it = mStyleControllers.find(inName);
        if (it != mStyleControllers.end())
        {
            StyleController * controller = it->second;
            controller->set(inValue);
            return true;
        }
        return false;
    }
    
    
    bool ConcreteComponent::setAttribute(const std::string & inName, const std::string & inValue)
    {
        AttributeControllers::iterator it = mAttributeControllers.find(inName);
        if (it != mAttributeControllers.end())
        {
            it->second->set(inValue);
            return true;
        }
        return false;
    }


    bool ConcreteComponent::initAttributeControllers()
    {
        // STATIC CAST NEEDED HERE OTHERWISE WE GET COMPILER ERROR:
        // error C2594: '=' : ambiguous conversions from 'Element *const ' to 'AttributeController *'
        setAttributeController("width", static_cast<WidthController*>(this));
        setAttributeController("height", static_cast<HeightController*>(this));
        setAttributeController("fill", static_cast<FillController*>(this));
        setAttributeController("stroke", static_cast<StrokeController*>(this));
        setAttributeController("flex", static_cast<FlexController*>(this));
        setAttributeController("hidden", static_cast<HiddenController*>(this));
        setAttributeController("align", static_cast<AlignController*>(this));
        setAttributeController("orient", static_cast<OrientController*>(this));
        return true;
    }


    bool ConcreteComponent::initStyleControllers()
    {
        setStyleController(CSSXController::PropertyName(), static_cast<CSSXController*>(this));
        setStyleController(CSSYController::PropertyName(), static_cast<CSSYController*>(this));
        setStyleController(CSSWidthController::PropertyName(), static_cast<CSSWidthController*>(this));
        setStyleController(CSSHeightController::PropertyName(), static_cast<CSSHeightController*>(this));
        setStyleController(CSSMarginController::PropertyName(), static_cast<CSSMarginController*>(this));
        setStyleController(CSSFillController::PropertyName(), static_cast<CSSFillController*>(this));
        setStyleController(CSSStrokeController::PropertyName(), static_cast<CSSStrokeController*>(this));
        return true;
    }


    void ConcreteComponent::setAttributeController(const std::string & inAttr, AttributeController * inController)
    {
        AttributeControllers::iterator it = mAttributeControllers.find(inAttr);
        assert(it == mAttributeControllers.end());
        if (it == mAttributeControllers.end())
        {
            mAttributeControllers.insert(std::make_pair(inAttr, inController));
        }
    }
    
    
    void ConcreteComponent::setStyleController(const std::string & inAttr, StyleController * inController)
    {
        StyleControllers::iterator it = mStyleControllers.find(inAttr);
        assert(it == mStyleControllers.end());
        if (it == mStyleControllers.end())
        {
            mStyleControllers.insert(std::make_pair(inAttr, inController));
        }
    }

    
    void ConcreteComponent::move(const Rect & inRect)
    {
        move(inRect.x(), inRect.y(), inRect.width(), inRect.height());
    }

    
    void ConcreteComponent::setOwningElement(Element * inElement)
    {
        mElement = inElement;
    }

    
    Element * ConcreteComponent::el() const
    {
        return mElement;
    }


    Component * ConcreteComponent::parent() const
    {
        return mParent;
    }

    
    void ConcreteComponent::rebuildChildLayouts()
    {
        if (!mElement)
        {
            return;
        }

        for (size_t idx = 0; idx != mElement->children().size(); ++idx)
        {
            Component * nativeComp = mElement->children()[idx]->component();
            if (nativeComp)
            {
                nativeComp->rebuildLayout();
            }
        }
    }

    
    HMODULE NativeComponent::sModuleHandle(0);


    NativeComponent::NativeComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        ConcreteComponent(inParent),
        mHandle(0),
        mModuleHandle(sModuleHandle ? sModuleHandle : ::GetModuleHandle(0)),
        mOrigProc(0),
        mOwnsHandle(true)
    {
    }


    NativeComponent::~NativeComponent()
    {   
        if (mHandle && mOwnsHandle)
        {
            unregisterHandle();
            unsubclass();
            ::DestroyWindow(mHandle);
        }
    }


    void NativeComponent::subclass()
    {        
        assert(!mOrigProc);
        mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mHandle, GWL_WNDPROC, (LONG)(LONG_PTR)&NativeComponent::MessageHandler);
    }


    void NativeComponent::unsubclass()
    {
        if (mOrigProc)
        {
            ::SetWindowLongPtr(mHandle, GWL_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
            mOrigProc = 0;
        }
    }


    void NativeComponent::registerHandle()
    {
        assert (sComponentsByHandle.find(mHandle) == sComponentsByHandle.end());
        sComponentsByHandle.insert(std::make_pair(mHandle, this));

        assert (sComponentsById.find(mCommandId.intValue()) == sComponentsById.end());
        sComponentsById.insert(std::make_pair(mCommandId.intValue(), this));
    }


    void NativeComponent::unregisterHandle()
    {
        ComponentsById::iterator itById = sComponentsById.find(mCommandId.intValue());
        assert (itById != sComponentsById.end());
        if (itById != sComponentsById.end())
        {
            sComponentsById.erase(itById);
        }
        
        ComponentsByHandle::iterator itByHandle = sComponentsByHandle.find(mHandle);
        assert (itByHandle != sComponentsByHandle.end());
        if (itByHandle != sComponentsByHandle.end())
        {
            sComponentsByHandle.erase(itByHandle);
        }
    }


    void NativeComponent::setHandle(HWND inHandle, bool inPassOwnership)
    {
        mHandle = inHandle;
        mOwnsHandle = inPassOwnership;
    }

    
    NativeComponent * NativeComponent::FindComponentByHandle(HWND inHandle)
    {
        ComponentsByHandle::iterator it = sComponentsByHandle.find(inHandle);
        if (it != sComponentsByHandle.end())
        {
            return it->second;
        }
        return 0;
    }

    
    NativeComponent * NativeComponent::FindComponentById(int inId)
    {
        ComponentsById::iterator it = sComponentsById.find(inId);
        if (it != sComponentsById.end())
        {
            return it->second;
        }
        return 0;
    }
    
    
    bool NativeComponent::isDisabled() const
    {
        return Windows::isWindowDisabled(handle());
    }


    void NativeComponent::setDisabled(bool inDisabled)
    {
        Windows::disableWindow(handle(), inDisabled);
    }


    std::string NativeComponent::getLabel() const
    {
        return Windows::getWindowText(handle());
    }


    void NativeComponent::setLabel(const std::string & inLabel)
    {
        Windows::setWindowText(handle(), inLabel);
    }
    
    
    void NativeComponent::setHidden(bool inHidden)
    {
        Super::setHidden(inHidden);
        Windows::setWindowVisible(handle(), !inHidden);
    }
        
    
    void NativeComponent::SetModuleHandle(HMODULE inModule)
    {
        sModuleHandle = inModule;
    }


    HWND NativeComponent::handle() const
    {
        return mHandle;
    }


    bool NativeComponent::initStyleControllers()
    {
        return Super::initStyleControllers();
    }
    
    
    bool NativeComponent::initAttributeControllers()
    {
        setAttributeController("disabled", static_cast<DisabledController*>(this));
        setAttributeController("label", static_cast<LabelController*>(this));
        return Super::initAttributeControllers();
    }


    bool NativeComponent::addEventListener(EventListener * inEventListener)
    {       
        EventListeners::iterator it = mEventListeners.find(inEventListener);
        if (it == mEventListeners.end())
        {
            mEventListeners.insert(inEventListener);
            return true;
        }
        return false;
    }


    bool NativeComponent::removeEventListener(EventListener * inEventListener)
    {       
        EventListeners::iterator it = mEventListeners.find(inEventListener);
        if (it != mEventListeners.end())
        {
            mEventListeners.erase(it);
            return true;
        }
        return false;
    }
    
    
    void NativeComponent::handleCommand(WPARAM wParam, LPARAM lParam)
    {
        unsigned short notificationCode = HIWORD(wParam);
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleCommand(el(), notificationCode);
        }
    }
    
    
    void NativeComponent::handleMenuCommand(WORD inMenuId)
    {
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleMenuCommand(el(), inMenuId);
        }
    }

    
    void NativeComponent::handleDialogCommand(WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleDialogCommand(el(), inNotificationCode, wParam, lParam);
        }
    }


    LRESULT NativeComponent::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (inMessage)
        {
            case WM_COMMAND:
            {
                WORD paramHi = HIWORD(wParam);
                WORD paramLo = LOWORD(wParam);
                
                switch (paramLo)
                {
                    case IDOK:
                    case IDCANCEL:
                    case IDABORT:
                    case IDRETRY:
                    case IDIGNORE:
                    case IDYES:
                    case IDNO:
                    case IDHELP:
                    case IDTRYAGAIN:
                    case IDCONTINUE:
                    {
                        ComponentsByHandle::iterator focusIt = sComponentsByHandle.find(::GetFocus());
                        if (focusIt != sComponentsByHandle.end())
                        {
                            focusIt->second->handleDialogCommand(paramLo, wParam, lParam);
                        }
                        break;
                    }
                    default:
                    {                        
                        ComponentsById::iterator it = sComponentsById.find(LOWORD(wParam));
                        if (it != sComponentsById.end())
                        {
                            it->second->handleCommand(wParam, lParam);
                        }
                        break;
                    }
                }
                break;
            }
            // These messages get forwarded to the child elements that produced them.
            case WM_VSCROLL:
            case WM_HSCROLL:
            {
                HWND handle = (HWND)lParam;
                ComponentsByHandle::iterator it = sComponentsByHandle.find(handle);
                if (it != sComponentsByHandle.end())
                {
                    it->second->handleMessage(inMessage, wParam, lParam);
                    return 0;
                }
                break;
            }
        }

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        bool handled = false;
        for (; it != end; ++it)
        {
            int result = (*it)->handleMessage(el(), inMessage, wParam, lParam);
            if (result == 0)
            {
                handled = true;
            }
        }

        if (handled)
        {
            return 0;
        }

        if (mOrigProc)
        {
            return ::CallWindowProc(mOrigProc, mHandle, inMessage, wParam, lParam);
        }
        else
        {
            return ::DefWindowProc(mHandle, inMessage, wParam, lParam);
        }
    }

    
    LRESULT CALLBACK NativeComponent::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        ComponentsByHandle::iterator it = sComponentsByHandle.find(hWnd);
        if (it != sComponentsByHandle.end())
        {
            return it->second->handleMessage(inMessage, wParam, lParam);
        }
        return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
    } 


    void NativeWindow::Register(HMODULE inModuleHandle)
    {
        WNDCLASSEX wndClass;
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &NativeWindow::MessageHandler;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = inModuleHandle;
        wndClass.hIcon = 0;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = TEXT("XULWin::WindowElement");
        wndClass.hIconSm = 0;
        if (! RegisterClassEx(&wndClass))
        {
            ReportError("Could not register XUL::WindowElement class.");
        }
    }


    NativeWindow::NativeWindow(const AttributesMapping & inAttributesMapping) :
        NativeComponent(0, inAttributesMapping),
        mBoxLayouter(this),
        mActiveDialog(0),
        mActiveMenu(0),
        mHasMessageLoop(false)
    {
        mHandle = ::CreateWindowEx
        (
            0, 
            TEXT("XULWin::WindowElement"),
            TEXT(""),
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, Defaults::windowWidth(), Defaults::windowHeight(),
            0,
            (HMENU)0, // must be zero if not menu and not child
            mModuleHandle,
            0
        );

        std::string error = Windows::getLastError(::GetLastError());


        // set default font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));        
        registerHandle();
    }


    NativeWindow::~NativeWindow()
    {
    }

    
    bool NativeWindow::initComponent()
    {
        return Super::initComponent();
    }


    bool NativeWindow::initStyleControllers()
    {  
        return Super::initStyleControllers();
    }


    bool NativeWindow::initAttributeControllers()
    {
        setAttributeController("title", static_cast<TitleController*>(this));
        return Super::initAttributeControllers();
    }
    
    
    Rect NativeWindow::clientRect() const
    {
        RECT rc;
        ::GetClientRect(handle(), &rc);
        return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }
    
    
    Rect NativeWindow::windowRect() const
    {
        RECT rw;
        ::GetWindowRect(handle(), &rw);
        return Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
    }
    
    
    int NativeWindow::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child(el()->children()[idx]);
            int width = child->component()->getWidth(inSizeConstraint);
            if (getOrient() == Horizontal)
            {
                result += width;
            }
            else if (width > result)
            {
                result = width;
            }
        }
        return result;
    }
    
    
    int NativeWindow::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child(el()->children()[idx]);
            int height = child->component()->getHeight(inSizeConstraint);
            if (getOrient() == Vertical)
            {
                result += height;
            }
            else if (height > result)
            {
                result = height;
            }
        }
        return result;
    }
    
    
    void NativeWindow::move(int x, int y, int w, int h)
    {
        ::MoveWindow(handle(), x, y, w, h, FALSE);
    }

    
    void NativeWindow::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }        


    Orient NativeWindow::getOrient() const
    {
        return Super::getOrient();
    }


    Align NativeWindow::getAlign() const
    {
        return Super::getAlign();
    }

    
    std::string NativeWindow::getTitle() const
    {
        return Windows::getWindowText(handle());
    }


    void NativeWindow::setTitle(const std::string & inTitle)
    {
        Windows::setWindowText(handle(), inTitle);
    }


    const Component * NativeWindow::getChild(size_t idx) const
    {
        if (el())
        {
            return el()->children()[idx]->component();
        }
        return 0;
    }


    Component * NativeWindow::getChild(size_t idx)
    {
        if (el())
        {
            return el()->children()[idx]->component();
        }
        return 0;
    }


    void NativeWindow::showModal(WindowElement::Positioning inPositioning)
    {
        show(inPositioning);

        mHasMessageLoop = true;
        MSG message;
        while (GetMessage(&message, NULL, 0, 0))
        {
            HWND hActive = GetActiveWindow();
            if (! IsDialogMessage(hActive, &message))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }


    void NativeWindow::show(WindowElement::Positioning inPositioning)
    {
        rebuildLayout();

        if (inPositioning == WindowElement::CenterInScreen)
        {
            SIZE sz = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
            if (findChildOfType<MenuBarComponent>())
            {
                sz.cy += Defaults::menuBarHeight();
            }
            int w = getWidth() + sz.cx;
            int h = getHeight() + sz.cy;
            int x = (GetSystemMetrics(SM_CXSCREEN) - w)/2;
            int y = (GetSystemMetrics(SM_CYSCREEN) - h)/2;
            move(x, y, w, h);
        }
        else
        {
            SIZE sz = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
            if (findChildOfType<MenuBarComponent>())
            {
                sz.cy += Defaults::menuBarHeight();
            }
            int w = getWidth() + sz.cx;
            int h = getHeight() + sz.cy;
            int x = getCSSX();
            int y = getCSSY();
            move(x, y, w, h);
        }

        ::ShowWindow(handle(), SW_SHOW);
        ::UpdateWindow(handle());
    }
    
    
    void NativeWindow::close()
    {
        setHidden(true);
        if (mHasMessageLoop)
        {
            ::PostQuitMessage(0);
        }
    }


    LRESULT NativeWindow::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch(inMessage)
        {
            case WM_SETFOCUS:
            {
                if (mActiveDialog)
                {
                    ::SetFocus(mActiveDialog->handle());
                    return 0;
                }
                break;
            }
            case WM_SIZE:
            {
                rebuildLayout();
                ::InvalidateRect(handle(), 0, FALSE);
                return 0;
            }
            case WM_CLOSE:
            {
                close();
                return 0;
            }
            case WM_GETMINMAXINFO:
            {
                SIZE sizeDiff = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
                MINMAXINFO * minMaxInfo = (MINMAXINFO*)lParam;
                minMaxInfo->ptMinTrackSize.x = getWidth(Minimum) + sizeDiff.cx;
                minMaxInfo->ptMinTrackSize.y = getHeight(Minimum) + sizeDiff.cy;
                return 0;
            }
            case WM_COMMAND:
            {
                if (lParam == 0) // menu or accelerator
                {
                    int menuId = LOWORD(wParam);
                    if (MenuComponent * menu = MenuComponent::FindById(menuId))
                    {
                        if (mActiveMenu != menu)
                        {
                            mActiveMenu = menu;
                            //showMenu(menu);
                            return 0;
                        }
                        mActiveMenu = 0;
                    }
                    else if (MenuItemComponent * menuItem = MenuItemComponent::FindById(menuId))
                    {
                        handleMenuCommand(menuId);
                        mActiveMenu = 0;
                        return 0;
                    }
                }
                else
                {
                    WORD paramHi = HIWORD(wParam);
                    WORD paramLo = LOWORD(wParam);
                
                    switch (paramLo)
                    {
                        case IDOK:
                        case IDCANCEL:
                        case IDABORT:
                        case IDRETRY:
                        case IDIGNORE:
                        case IDYES:
                        case IDNO:
                        case IDHELP:
                        case IDTRYAGAIN:
                        case IDCONTINUE:
                        {
                            NativeComponent * focus = FindComponentByHandle(::GetFocus());
                            if (focus)
                            {
                                focus->handleDialogCommand(paramLo, wParam, lParam);
                                return 0;
                            }
                            break;
                        }
                        default:
                        {                        
                            NativeComponent * sender = FindComponentById(LOWORD(wParam));
                            if (sender)
                            {
                                sender->handleCommand(wParam, lParam);
                                return 0;
                            }
                            break;
                        }
                    }
                }                
                break;
            }
            // These messages get forwarded to the child elements that produced them.
            case WM_VSCROLL:
            case WM_HSCROLL:
            {
                NativeComponent * sender = FindComponentByHandle((HWND)lParam);
                if (sender)
                {
                    sender->handleMessage(inMessage, wParam, lParam);
                    return 0;
                }
                break;
            }
        }

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleMessage(el(), inMessage, wParam, lParam);
        }
        return ::DefWindowProc(handle(), inMessage, wParam, lParam);
    }

    
    LRESULT CALLBACK NativeWindow::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        NativeComponent * sender = FindComponentByHandle(hWnd);
        if (sender)
        {
            int result = sender->handleMessage(inMessage, wParam, lParam);
            if (result == 0)
            {
                return 0;
            }
        }
        return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
    }


    void NativeWindow::setBlockingDialog(NativeDialog * inDlg)
    {
        mActiveDialog = inDlg;
    }


    void NativeDialog::Register(HMODULE inModuleHandle)
    {
        WNDCLASSEX wndClass;
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &NativeDialog::MessageHandler;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = inModuleHandle;
        wndClass.hIcon = 0;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = TEXT("XULWin::DialogElement");
        wndClass.hIconSm = 0;
        if (! RegisterClassEx(&wndClass))
        {
            ReportError("Could not register XUL::DialogElement class.");
        }
    }
        
        
    namespace
    {
        BOOL CALLBACK DisableAllExcept(HWND inWindow, LPARAM lParam)
        {
            HWND exceptThisOne = (HWND)lParam;
            if (inWindow != exceptThisOne)
            {
                ::EnableWindow(inWindow, false);
            }
            return true;
        }
        
        BOOL CALLBACK EnableAllExcept(HWND inWindow, LPARAM lParam)
        {
            HWND exceptThisOne = (HWND)lParam;
            if (inWindow != exceptThisOne)
            {
                ::EnableWindow(inWindow, true);
            }
            return true;
        }
    }


    NativeDialog::NativeDialog(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeComponent(inParent, inAttributesMapping),
        mBoxLayouter(this),
        mInvoker(0),
        mDialogResult(DialogResult_Cancel)
    {
        if (NativeComponent * comp = inParent->downcast<NativeComponent>())
        {
            mHandle = ::CreateWindowEx
            (
                0, 
                TEXT("XULWin::DialogElement"), 
                TEXT(""),
                WS_POPUPWINDOW | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                CW_USEDEFAULT, CW_USEDEFAULT, Defaults::windowWidth(), Defaults::windowHeight(),
                comp->handle(),
                (HMENU)0, // must be zero if not menu and not child
                mModuleHandle,
                0
            );
        }
        
        std::string error = Windows::getLastError(::GetLastError());


        // set default font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));        
        registerHandle();
    }


    NativeDialog::~NativeDialog()
    {
    }


    bool NativeDialog::initStyleControllers()
    {  
        return Super::initStyleControllers();
    }


    bool NativeDialog::initAttributeControllers()
    {
        setAttributeController("title", static_cast<TitleController*>(this));
        return Super::initAttributeControllers();
    }
    
    
    Rect NativeDialog::clientRect() const
    {
        RECT rc;
        ::GetClientRect(handle(), &rc);
        return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }
    
    
    Rect NativeDialog::windowRect() const
    {
        RECT rw;
        ::GetWindowRect(handle(), &rw);
        return Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
    }
    
    
    int NativeDialog::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child(el()->children()[idx]);
            int width = child->component()->getWidth(inSizeConstraint);
            if (getOrient() == Horizontal)
            {
                result += width;
            }
            else if (width > result)
            {
                result = width;
            }
        }
        return result;
    }
    
    
    int NativeDialog::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child(el()->children()[idx]);
            int height = child->component()->getHeight(inSizeConstraint);
            if (getOrient() == Vertical)
            {
                result += height;
            }
            else if (height > result)
            {
                result = height;
            }
        }
        return result;
    }
    
    
    void NativeDialog::move(int x, int y, int w, int h)
    {
        ::MoveWindow(handle(), x, y, w, h, FALSE);
    }

    
    void NativeDialog::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }        


    Orient NativeDialog::getOrient() const
    {
        return Super::getOrient();
    }


    Align NativeDialog::getAlign() const
    {
        return Super::getAlign();
    }

    
    std::string NativeDialog::getTitle() const
    {
        return Windows::getWindowText(handle());
    }


    void NativeDialog::setTitle(const std::string & inTitle)
    {
        Windows::setWindowText(handle(), inTitle);
    }


    const Component * NativeDialog::getChild(size_t idx) const
    {
        if (el())
        {
            const Children & children = el()->children();
            if (idx < children.size())
            {
                return children[idx]->component();
            }
        }
        return 0;
    }


    Component * NativeDialog::getChild(size_t idx)
    {
        return const_cast<Component*>(static_cast<const NativeDialog*>(this)->getChild(idx));
    }


    DialogResult NativeDialog::showModal(NativeWindow * inInvoker)
    {
        mInvoker = inInvoker;
        if (mInvoker)
        {
            mInvoker->setBlockingDialog(this);
        }

        // Disable all windows except the dialog
        DWORD threadID = GetWindowThreadProcessId(handle(), NULL);
        ::EnumThreadWindows(threadID, &DisableAllExcept, (LPARAM)handle());


        rebuildLayout();
        SIZE sz = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
        int w = getWidth() + sz.cx;
        int h = getHeight() + sz.cy;
        int x = (GetSystemMetrics(SM_CXSCREEN) - w)/2;
        int y = (GetSystemMetrics(SM_CYSCREEN) - h)/2;
        move(x, y, w, h);
        ::ShowWindow(handle(), SW_SHOW);
        ::UpdateWindow(handle());

        MSG message;
        while (GetMessage(&message, NULL, 0, 0))
        {
            HWND hActive = GetActiveWindow();
            if (! IsDialogMessage(hActive, &message))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        return mDialogResult;
    }
    
    
    LRESULT NativeDialog::endModal(DialogResult inDialogResult)
    {
        mDialogResult = inDialogResult;
        // Re-enable all windows
        DWORD threadID = GetWindowThreadProcessId(handle(), NULL);
        ::EnumThreadWindows(threadID, &EnableAllExcept, (LPARAM)handle());
        if (mInvoker)
        {
            mInvoker->setBlockingDialog(0);
            mInvoker = 0;
        }

        setHidden(true);
        ::PostQuitMessage(0);
        return 0;
    }


    LRESULT NativeDialog::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch(inMessage)
        {
            case WM_SIZE:
            {
                rebuildLayout();
                ::InvalidateRect(handle(), 0, FALSE);
                return 0;
            }
            case WM_CLOSE:
            {
                endModal(DialogResult_Cancel);
                return 0;
            }
            case WM_GETMINMAXINFO:
            {
                SIZE sizeDiff = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
                MINMAXINFO * minMaxInfo = (MINMAXINFO*)lParam;
                minMaxInfo->ptMinTrackSize.x = getWidth(Minimum) + sizeDiff.cx;
                minMaxInfo->ptMinTrackSize.y = getHeight(Minimum) + sizeDiff.cy;
                return 0;
            }
            case WM_COMMAND:
            {
                WORD paramHi = HIWORD(wParam);
                WORD paramLo = LOWORD(wParam);
                
                switch (paramLo)
                {
                    case IDYES:
                    case IDOK:
                    {
                        endModal(DialogResult_Ok);
                        return 0;
                    }
                    case IDCANCEL:
                    case IDNO:
                    {
                        endModal(DialogResult_Cancel);
                        return 0;
                    }
                    default:
                    {      
                        NativeComponent * sender = FindComponentById(LOWORD(wParam));
                        if (sender)
                        {
                            sender->handleCommand(wParam, lParam);
                            return 0;
                        }
                        break;
                    }                      
                }
                break;
            }
            // These messages get forwarded to the child elements that produced them.
            case WM_VSCROLL:
            case WM_HSCROLL:
            {
                NativeComponent * sender = FindComponentByHandle((HWND)lParam);
                if (sender)
                {
                    sender->handleMessage(inMessage, wParam, lParam);
                    return 0;
                }
                break;
            }
        }

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleMessage(el(), inMessage, wParam, lParam);
        }
        return ::DefWindowProc(handle(), inMessage, wParam, lParam);
    }

    
    LRESULT CALLBACK NativeDialog::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        NativeComponent * sender = FindComponentByHandle(hWnd);
        if (sender)
        {
            int result = sender->handleMessage(inMessage, wParam, lParam);
            if (result == 0)
            {
                return 0;
            }
        }
        return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
    }


    VirtualComponent::VirtualComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        ConcreteComponent(inParent)
    {
        if (!mParent)
        {
            ReportError("NativeControl constructor failed because parent is NULL.");
            return;
        }
    }
        
        
    VirtualComponent::~VirtualComponent()
    {

    }

     
    bool VirtualComponent::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    bool VirtualComponent::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    void VirtualComponent::move(int x, int y, int w, int h)
    {
        mRect = Rect(x, y, w, h);
    }


    void VirtualComponent::rebuildLayout()
    {
        rebuildChildLayouts();
    }
    
    
    Rect VirtualComponent::clientRect() const
    {
        return mRect;
    }

    
    LRESULT VirtualComponent::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }


    PassiveComponent::PassiveComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }
        
        
    PassiveComponent::~PassiveComponent()
    {

    }


    NativeControl::NativeControl(Component * inParent, const AttributesMapping & inAttributesMapping, LPCTSTR inClassName, DWORD inExStyle, DWORD inStyle) :
        NativeComponent(inParent, inAttributesMapping)
    {
        if (!mParent)
        {
            ReportError("NativeControl constructor failed because parent is NULL.");
            return;
        }

        Rect clientRect = inParent->clientRect();

        NativeComponent * nativeParent = GetNativeThisOrParent(inParent);
        if (!nativeParent)
        {
            ReportError("NativeControl constructor failed because no native parent was found.");
            return;
        }
        
        mHandle = ::CreateWindowEx
        (
            inExStyle, 
            inClassName,
            TEXT(""),
            inStyle | WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
            0, 0, 0, 0,
            nativeParent->handle(),
            (HMENU)mCommandId.intValue(),
            mModuleHandle,
            0
        );

        if (!mHandle)
        {
            ReportError(Windows::getLastError(::GetLastError()));
            return;
        }

        // set default font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

        registerHandle();
        subclass();
    }


    NativeControl::NativeControl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeComponent(inParent, inAttributesMapping)
    {
    }


    NativeControl::~NativeControl()
    {
    }


    bool NativeControl::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    void NativeControl::rebuildLayout()
    {
        rebuildChildLayouts();
    }
    
    
    void NativeControl::move(int x, int y, int w, int h)
    {
        if (NativeComponent * nativeParent = dynamic_cast<NativeComponent*>(parent()))
        {
            // This situation occurs if the scroll decorator created a STATIC window for
            // the scrollable rectangular area. This new context requires that we
            // re-adjust the x and y coords.
            Rect scrollRect = nativeParent->clientRect();
            ::MoveWindow(handle(), x - scrollRect.x(), y - scrollRect.y(), w, h, FALSE);
        }
        else
        {
            // If the parent is a virtual element, then we can position this control normally.
            ::MoveWindow(handle(), x, y, w, h, FALSE);
        }
    }
    
    
    Rect NativeControl::clientRect() const
    {
        HWND hwndParent = ::GetParent(handle());
        if (!hwndParent)
        {
            RECT rc;
            ::GetClientRect(handle(), &rc);
        }

        RECT rc_parent;
        ::GetClientRect(hwndParent, &rc_parent);
        ::MapWindowPoints(hwndParent, HWND_DESKTOP, (LPPOINT)&rc_parent, 2);

        RECT rc_self;
        ::GetClientRect(handle(), &rc_self);
        ::MapWindowPoints(handle(), HWND_DESKTOP, (LPPOINT)&rc_self, 2);

        
        int x = rc_self.left - rc_parent.left;
        int y = rc_self.top - rc_parent.top;
        return Rect(x, y, rc_self.right - rc_self.left, rc_self.bottom - rc_self.top);
    }
    
    
    const NativeComponent * NativeControl::GetNativeThisOrParent(const Component * inElementImpl)
    {
        if (const NativeComponent * obj = dynamic_cast<const NativeComponent*>(inElementImpl))
        {
            return obj;
        }
        else if (const Decorator * obj = dynamic_cast<const Decorator*>(inElementImpl))
        {
            return GetNativeThisOrParent(obj->decoratedElement().get());
        }
        else if (const VirtualComponent * obj = dynamic_cast<const VirtualComponent*>(inElementImpl))
        {
            return GetNativeThisOrParent(obj->parent());
        }
        return 0;
    }
    
    
    NativeComponent * NativeControl::GetNativeThisOrParent(Component * inElementImpl)
    {
        return const_cast<NativeComponent *>(GetNativeThisOrParent(const_cast<const Component*>(inElementImpl)));
    }
    
    
    NativeButton::NativeButton(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_PUSHBUTTON)
    {   
    }
    
    
    int NativeButton::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::buttonHeight();
    }
    
    
    int NativeButton::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = Windows::getWindowText(handle());
        int minWidth = Windows::getTextSize(handle(), text).cx;
        minWidth += Defaults::textPadding();
        return std::max<int>(minWidth, Defaults::buttonWidth());
    }
    
    
    NativeCheckBox::NativeCheckBox(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, TEXT("BUTTON"), 0, WS_TABSTOP | BS_AUTOCHECKBOX)
    {
    }
    
    
    int NativeCheckBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }
    
    
    int NativeCheckBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::checkBoxMinimumWidth() + Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx;
    }

    
    bool NativeCheckBox::isChecked() const
    {
        return Windows::isCheckBoxChecked(handle());
    }


    void NativeCheckBox::setChecked(bool inChecked)
    {
        Windows::setCheckBoxChecked(handle(), inChecked);
    }

    
    bool NativeCheckBox::initAttributeControllers()
    {
        setAttributeController("checked", static_cast<CheckedController *>(this));
        return Super::initAttributeControllers();
    }


    NativeTextBox::NativeTextBox(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("EDIT"),
                      WS_EX_CLIENTEDGE, // exStyle
                      WS_TABSTOP | GetFlags(inAttributesMapping)),
        mReadonly(IsReadOnly(inAttributesMapping)),
        mRows(1)
    {
        
    }


    DWORD NativeTextBox::GetFlags(const AttributesMapping & inAttributesMapping)
    {
        DWORD flags = 0;
        AttributesMapping::const_iterator it = inAttributesMapping.find("type");
        if (it != inAttributesMapping.end() && it->second == "password")
        {
            flags |= ES_PASSWORD;
        }
        it = inAttributesMapping.find("multiline");
        if (it != inAttributesMapping.end() && it->second == "true")
        {
            flags |= WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;            
        }
        else
        {
            flags |= ES_AUTOHSCROLL;            
        }
        return flags;
    } 


    bool NativeTextBox::IsReadOnly(const AttributesMapping & inAttributesMapping)
    {
        AttributesMapping::const_iterator it = inAttributesMapping.find("readonly");
        return it != inAttributesMapping.end() && it->second == "true";
    } 


    std::string NativeTextBox::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void NativeTextBox::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }
    
    
    bool NativeTextBox::isReadOnly() const
    {
        return Windows::isTextBoxReadOnly(handle());
    }

    
    void NativeTextBox::setReadOnly(bool inReadOnly)
    {
        Windows::setTextBoxReadOnly(handle(), inReadOnly);
    }


    int NativeTextBox::getRows() const
    {
        return mRows;
    }


    void NativeTextBox::setRows(int inRows)
    {
        mRows = inRows;
    }

    
    bool NativeTextBox::initAttributeControllers()
    {
        setAttributeController("value", static_cast<StringValueController*>(this));
        setAttributeController("readonly", static_cast<ReadOnlyController*>(this));
        setAttributeController("rows", static_cast<RowsController*>(this));
        return Super::initAttributeControllers();
    }


    int NativeTextBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 20;
    }


    int NativeTextBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight() * getRows();
    }


    void NativeTextBox::handleCommand(WPARAM wParam, LPARAM lParam)
    {
    }


    NativeLabel::NativeLabel(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_LEFT)
    {
    }
    
        
    std::string NativeLabel::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void NativeLabel::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }
    
    
    CSSTextAlign NativeLabel::getCSSTextAlign() const
    {
        LONG styles = Windows::getWindowStyles(handle());
        if (styles & SS_LEFT)
        {
            return CSSTextAlign_Left;
        }
        else if (styles & SS_CENTER)
        {
            return CSSTextAlign_Center;
        }
        else if (styles & SS_RIGHT)
        {
            return CSSTextAlign_Right;
        }
        else
        {
            return CSSTextAlign_Left;
        }
    }


    void NativeLabel::setCSSTextAlign(CSSTextAlign inValue)
    {
        LONG styles = Windows::getWindowStyles(handle());
        styles &= ~SS_LEFT;
        styles &= ~SS_CENTER;
        styles &= ~SS_RIGHT;

        switch (inValue)
        {
            case CSSTextAlign_Left:
            {
                styles |= SS_LEFT;
                break;
            }
            case CSSTextAlign_Center:
            {
                styles |= SS_CENTER;
                break;
            }
            case CSSTextAlign_Right:
            {
                styles |= SS_RIGHT;
                break;
            }
            //case CSSTextAlign_Justify:
            //{
            //    styles |= 0;
            //    break;
            //}
        }
        Windows::setWindowStyle(handle(), styles);
    }

    
    bool NativeLabel::initAttributeControllers()
    {
        setAttributeController("value", static_cast<StringValueController*>(this));
        return Super::initAttributeControllers();
    }
    
    
    bool NativeLabel::initStyleControllers()
    {
        setStyleController(CSSTextAlignController::PropertyName(), static_cast<CSSTextAlignController*>(this));
        return Super::initStyleControllers();
    }


    int NativeLabel::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = Windows::getWindowText(handle());
        int width = Windows::getTextSize(handle(), text).cx;
        return width;
    }

    
    int NativeLabel::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Windows::getTextSize(handle(), Windows::getWindowText(handle())).cy;
    }


    NativeDescription::NativeDescription(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_LEFT)
    {
    }
    
        
    std::string NativeDescription::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void NativeDescription::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }


    bool NativeDescription::initAttributeControllers()
    {
        setAttributeController("value", static_cast<StringValueController*>(this));
        return Super::initAttributeControllers();
    }


    int NativeDescription::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::textPadding();
    }

    
    int NativeDescription::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Windows::getMultilineTextHeight(handle());
    }

    
    VirtualBox::VirtualBox(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping),
        mBoxLayouter(this)
    {
    }
        
        
    Orient VirtualBox::getOrient() const
    {
        return mOrient.or(Horizontal);
    }


    Align VirtualBox::getAlign() const
    {
        return mAlign.or(Stretch);
    }
    
    
    void VirtualBox::setAttributeController(const std::string & inAttr, AttributeController * inController)
    {
        Super::setAttributeController(inAttr, inController);
    }


    bool VirtualBox::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    void VirtualBox::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }

        

    NativeBox::NativeBox(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, TEXT("STATIC"), WS_EX_CONTROLPARENT, WS_TABSTOP),
        mBoxLayouter(this)
    {
    }


    Orient NativeBox::getOrient() const
    {
        return mOrient.or(Vertical);
    }


    Align NativeBox::getAlign() const
    {
        return mAlign.or(Stretch);
    }
    
    
    void NativeBox::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }

    
    int NativeBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return mBoxLayouter.calculateWidth(inSizeConstraint);
    }

    
    int NativeBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return mBoxLayouter.calculateHeight(inSizeConstraint);
    }
    
    
    Rect NativeBox::clientRect() const
    {
        return Super::clientRect();
    }

    
    const Component * NativeBox::getChild(size_t idx) const
    {
        return mElement->children()[idx]->component();
    }

    
    Component * NativeBox::getChild(size_t idx)
    {
        return mElement->children()[idx]->component();
    }

    
    void NativeBox::rebuildChildLayouts()
    {
        return Super::rebuildChildLayouts();
    }


    MenuListImpl::MenuListImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("COMBOBOX"),
                      0, // exStyle
                      WS_TABSTOP | CBS_DROPDOWNLIST)
    {
    }
    
        
    bool MenuListImpl::initComponent()
    {
        fillComboBox();
        return Super::initComponent();
    }


    void MenuListImpl::fillComboBox()
    {        
        if (MenuPopupComponent * popup = findChildOfType<MenuPopupComponent>())
        {
            for (size_t idx = 0; idx != popup->getChildCount(); ++idx)
            {
                ElementPtr child = popup->el()->children()[idx];
                if (MenuItemComponent * item = child->component()->downcast<MenuItemComponent>())
                {
                    std::string label = item->getLabel();
                    Windows::addStringToComboBox(handle(), label);
                }
            }
            Windows::selectComboBoxItem(handle(), 0);
        }
    }

    
    int MenuListImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int itemWidth = 0;
        for (size_t idx = 0; idx != mItems.size(); ++idx)
        {
            int w = Windows::getTextSize(handle(), mItems[idx]).cx;
            if (itemWidth < w)
            {
                itemWidth = w;
            }
        }        
        return Defaults::dropDownListMinimumWidth() + itemWidth;
    }


    int MenuListImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }
    
    
    void MenuListImpl::move(int x, int y, int w, int h)
    {
        // The height of a combobox in Win32 is the height of the dropdown menu
        // + the height of the widget itself.
        
        int numItems = Windows::getComboBoxItemCount(handle());
        int dropdownHeight = 0;
        if (numItems > 0)
        {
            dropdownHeight = numItems * Windows::getComboBoxItemHeight(handle(), 0); // use index 0            
        }

        // This is usually needed as well, I think :S
        int extraHeight = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;
        
        NativeControl::move(x, y, w, h + dropdownHeight + extraHeight);
    }


    void MenuListImpl::showPopupMenu(RECT inToolbarButtonRect)
    {
        // no implementation needed
    }
    
    
    void MenuListImpl::onContentChanged()
    {
        if (mIsInitialized)
        {
            Windows::clearComboBox(handle());
            fillComboBox();
        }
        // else: the initComponent will take care of the initial fill
    }


    NativeSeparator::NativeSeparator(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_GRAYFRAME)
    {
        mExpansive = true;
    }
        
        
    int NativeSeparator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int NativeSeparator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    NativeSpacer::NativeSpacer(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }

        
    int NativeSpacer::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }

     
    int NativeSpacer::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }


    NativeMenuButton::NativeMenuButton(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_PUSHBUTTON)
    {
    }
        
        
    int NativeMenuButton::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx + Defaults::textPadding()*2;
    }

    
    int NativeMenuButton::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::buttonHeight();
    }


    VirtualGrid::VirtualGrid(Component * inParent,
                           const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }
        
        
    int VirtualGrid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (NativeColumns * cols = child->component()->downcast<NativeColumns>())
            {
                for (size_t idx = 0; idx != cols->getChildCount(); ++idx)
                {
                    if (NativeColumn * col = cols->getChild(idx)->downcast<NativeColumn>())
                    {
                        result += col->getWidth(inSizeConstraint);
                    }
                }
            }
        }
        return result;
    }

    
    int VirtualGrid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (NativeRows * rows = child->component()->downcast<NativeRows>())
            {
                for (size_t idx = 0; idx != rows->getChildCount(); ++idx)
                {
                    if (NativeRow * row = rows->getChild(idx)->downcast<NativeRow>())
                    {
                        result += row->getHeight(inSizeConstraint);
                    }
                }
            }
        }
        return result;
    }


    void VirtualGrid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        int numCols = 0;
        int numRows = 0;
        ElementPtr columns;
        ElementPtr rows;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (child->type() == RowsElement::Type())
            {
                rows = child;
                numRows = rows->children().size();
            }
            else if (child->type() == ColumnsElement::Type())
            {
                columns = child;
                numCols = columns->children().size();
            }
            else
            {
                ReportError("GridElement contains incompatible child element: '" + child->type() + "'");
            }
        }
        if (!rows || !columns)
        {
            ReportError("GridElement has no rows or no columns!");
            return;
        }

        if (rows->children().empty())
        {
            ReportError("GridElement has no rows!");
            return;
        }

        if (columns->children().empty())
        {
            ReportError("GridElement has no columns!");
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->children().size(); ++colIdx)
        {
            if (NativeColumn * col = columns->children()[colIdx]->component()->downcast<NativeColumn>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(String2Int(col->el()->getAttribute("flex"), 0)),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Optimal))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("GridElement has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->children().size(); ++rowIdx)
        {
            if (NativeRow * row = rows->children()[rowIdx]->component()->downcast<NativeRow>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(String2Int(row->el()->getAttribute("flex"), 0)),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Optimal))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("GridElement has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(numRows, numCols);
        GridLayoutManager::GetOuterRects(clientRect(), colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(numRows, numCols, CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            if (NativeRow * row = rows->children()[rowIdx]->component()->downcast<NativeRow>())
            {
                int rowHeight = row->getHeight();
                for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
                {
                    if (NativeColumn * column = columns->children()[colIdx]->component()->downcast<NativeColumn>())
                    {
                        if (colIdx < row->getChildCount())
                        {                            
                            Component * child = row->el()->children()[colIdx]->component();
                            widgetInfos.set(rowIdx, colIdx,
                                                     CellInfo(child->getWidth(),
                                                     child->getHeight(),
                                                     String2Align(row->el()->getAttribute("align"), Stretch),
                                                     String2Align(column->el()->getAttribute("align"), Stretch)));
                        }
                    }
                }
            }
        }


        //
        // Get inner rect for each cell
        //
        GenericGrid<Rect> innerRects(numRows, numCols);
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
            {
                if (rowIdx < rows->children().size())
                {
                    ElementPtr rowEl = rows->children()[rowIdx];
                    if (colIdx < rowEl->children().size())
                    {
                        Component * child = rowEl->children()[colIdx]->component();
                        const Rect & r = innerRects.get(rowIdx, colIdx);
                        child->move(r.x(), r.y(), r.width(), r.height());
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }


    NativeGrid::NativeGrid(Component * inParent,
                           const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, TEXT("STATIC"), WS_EX_CONTROLPARENT, WS_TABSTOP)
    {
    }
        
        
    int NativeGrid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (NativeColumns * cols = child->component()->downcast<NativeColumns>())
            {
                for (size_t idx = 0; idx != cols->getChildCount(); ++idx)
                {
                    if (NativeColumn * col = cols->getChild(idx)->downcast<NativeColumn>())
                    {
                        result += col->getWidth(inSizeConstraint);
                    }
                }
            }
        }
        return result;
    }

    
    int NativeGrid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (NativeRows * rows = child->component()->downcast<NativeRows>())
            {
                for (size_t idx = 0; idx != rows->getChildCount(); ++idx)
                {
                    if (NativeRow * row = rows->getChild(idx)->downcast<NativeRow>())
                    {
                        result += row->getHeight(inSizeConstraint);
                    }
                }
            }
        }
        return result;
    }


    void NativeGrid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        int numCols = 0;
        int numRows = 0;
        ElementPtr columns;
        ElementPtr rows;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (child->type() == RowsElement::Type())
            {
                rows = child;
                numRows = rows->children().size();
            }
            else if (child->type() == ColumnsElement::Type())
            {
                columns = child;
                numCols = columns->children().size();
            }
            else
            {
                ReportError("GridElement contains incompatible child element: '" + child->type() + "'");
            }
        }
        if (!rows || !columns)
        {
            ReportError("GridElement has no rows or no columns!");
            return;
        }

        if (rows->children().empty())
        {
            ReportError("GridElement has no rows!");
            return;
        }

        if (columns->children().empty())
        {
            ReportError("GridElement has no columns!");
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->children().size(); ++colIdx)
        {
            if (NativeColumn * col = columns->children()[colIdx]->component()->downcast<NativeColumn>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(col->getFlex()),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Optimal))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("GridElement has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->children().size(); ++rowIdx)
        {
            if (NativeRow * row = rows->children()[rowIdx]->component()->downcast<NativeRow>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(row->getFlex()),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Optimal))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("GridElement has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(numRows, numCols);
        Rect clientRect(clientRect());
        GridLayoutManager::GetOuterRects(clientRect, colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(numRows, numCols, CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            if (NativeRow * row = rows->children()[rowIdx]->component()->downcast<NativeRow>())
            {
                int rowHeight = row->getHeight();
                for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
                {
                    if (NativeColumn * column = columns->children()[colIdx]->component()->downcast<NativeColumn>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->el()->children()[colIdx]->component();
                            widgetInfos.set(rowIdx, colIdx,
                                            CellInfo(child->getWidth(),
                                                     child->getHeight(),
                                                     row->getAlign(),
                                                     column->getAlign()));
                        }
                    }
                }
            }
        }


        //
        // Get inner rect for each cell
        //
        GenericGrid<Rect> innerRects(numRows, numCols);
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
            {
                if (rowIdx < rows->children().size())
                {
                    ElementPtr rowEl = rows->children()[rowIdx];
                    if (colIdx < rowEl->children().size())
                    {
                        Component * child = rowEl->children()[colIdx]->component();
                        const Rect & r = innerRects.get(rowIdx, colIdx);
                        child->move(r.x(), r.y(), r.width(), r.height());
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }


    NativeRows::NativeRows(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }


    NativeColumns::NativeColumns(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }


    NativeRow::NativeRow(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }

    
    int NativeRow::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        const Children & children = el()->children();
        for (size_t idx = 0; idx != children.size(); ++idx)
        {
            ElementPtr child = children[idx];
            res += child->component()->getWidth(inSizeConstraint);
        }
        return res;
    }


    int NativeRow::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        const Children & children = el()->children();
        for (size_t idx = 0; idx != children.size(); ++idx)
        {
            ElementPtr child = children[idx];
            int h = child->component()->getHeight(inSizeConstraint);
            if (h > res)
            {
                res = h;
            }
        }
        return res;
    }


    NativeColumn::NativeColumn(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }
        
        
    Align NativeColumn::getAlign() const
    {
        return mAlign.or(Stretch);
    }

    
    int NativeColumn::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        ElementPtr rows;
        int ownIndex = -1;
        Element * grid = el()->parent()->parent();
        for (size_t idx = 0; idx != grid->children().size(); ++idx)
        {
            ElementPtr child = grid->children()[idx];
            if (child->type() == RowsElement::Type())
            {
                rows = child;
            }
            else if (child->type() == ColumnsElement::Type())
            {
                for (size_t ownI = 0; ownI != child->children().size(); ++ownI)
                {
                    if (child->children()[ownI]->component()->commandId() == commandId())
                    {
                        ownIndex = ownI;
                    }
                }
            }
            if (rows && ownIndex != -1)
            {
                break;
            }
        }
        if (!rows)
        {
            ReportError("Could not find 'rows' element in GridElement.");
            return 0;
        }
        if (ownIndex == -1)
        {
            ReportError("ColumnElement was unable to find itself in its parent container.");
            return 0;
        }

        int res = 0;
        for (size_t rowIdx = 0; rowIdx != rows->children().size(); ++rowIdx)
        {
            ElementPtr row = rows->children()[rowIdx];
            if (ownIndex < row->children().size())
            {
                int w = row->children()[ownIndex]->component()->getWidth(inSizeConstraint);
                if (w > res)
                {
                    res = w;
                }
            }
        }
        return res;
    }


    int NativeColumn::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        const Children & children = el()->children();
        for (size_t idx = 0; idx != children.size(); ++idx)
        {
            ElementPtr child = children[idx];
            res += child->component()->getHeight(inSizeConstraint);
        }
        return res;
    }

    
    NativeRadioGroup::NativeRadioGroup(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualBox(inParent, inAttributesMapping)
    {
    }

    
    NativeRadio::NativeRadio(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_RADIOBUTTON)
    {
    }


    int NativeRadio::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::radioButtonMinimumWidth() + Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx;
    }

    
    int NativeRadio::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }

    
    NativeProgressMeter::NativeProgressMeter(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent,
                      inAttributesMapping,
                      PROGRESS_CLASS,
                      0, // exStyle
                      PBS_SMOOTH)
    {
        Windows::initializeProgressMeter(mHandle, 100);
    }


    int NativeProgressMeter::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::progressMeterWidth();
    }

    
    int NativeProgressMeter::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::progressMeterHeight();
    }
    

    int NativeProgressMeter::getValue() const
    {
        return Windows::getProgressMeterProgress(handle());
    }


    void NativeProgressMeter::setValue(int inValue)
    {
        Windows::setProgressMeterProgress(handle(), inValue);
    }


    bool NativeProgressMeter::initAttributeControllers()
    {
        Super::setAttributeController("value", static_cast<IntValueController*>(this));
        return Super::initAttributeControllers();
    }

    
    NativeDeck::NativeDeck(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping),
        mSelectedIndex(0)
    {
    }
        
    
    int NativeDeck::getSelectedIndex() const
    {
        return mSelectedIndex;
    }


    void NativeDeck::setSelectedIndex(int inSelectedIndex)
    {
        mSelectedIndex = inSelectedIndex;
        rebuildLayout();
    }


    void NativeDeck::rebuildLayout()
    {
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr element = el()->children()[idx];
            bool visible = idx == mSelectedIndex;
            element->component()->setHidden(!visible);
            if (visible)
            {
                Rect rect = clientRect();
                Component * n = element->component();
                n->move(rect.x(), rect.y(), rect.width(), rect.height());
            }
        }
        rebuildChildLayouts();
    }


    int NativeDeck::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            int w = getChild(idx)->getWidth(inSizeConstraint);
            if (w > res)
            {
                res = w;
            }
        }
        return res;
    }

    
    int NativeDeck::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            int h = getChild(idx)->getHeight(inSizeConstraint);
            if (h > res)
            {
                res = h;
            }
        }
        return res;
    }


    bool NativeDeck::initAttributeControllers()
    {
        Super::setAttributeController("selectedIndex", static_cast<SelectedIndexController*>(this));
        return Super::initAttributeControllers();
    }


    DWORD NativeScrollbar::GetFlags(const AttributesMapping & inAttributesMapping)
    {
        DWORD flags = 0;
        AttributesMapping::const_iterator it = inAttributesMapping.find("orient");
        if (it != inAttributesMapping.end())
        {
            const std::string & orient = it->second;
            if (orient == "horizontal")
            {
                flags |= SBS_HORZ | SBS_RIGHTALIGN;
            }
            else if (orient == "vertical")
            {
                flags |= SBS_VERT | SBS_BOTTOMALIGN;
            }
            else
            {
                ReportError("Invalid orient found for scrollbar!");
            }
        }
        return flags;
    }
    
    
    NativeScrollbar::NativeScrollbar(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping,
                      TEXT("SCROLLBAR"),
                      0, // exStyle
                      WS_TABSTOP | GetFlags(inAttributesMapping)),
        mEventListener(0),
        mIncrement(0)
    {
        mExpansive = true;
        Windows::setScrollInfo(handle(), 100, 10, 0);
    }


    int NativeScrollbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::scrollbarWidth();
    }

    
    int NativeScrollbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::scrollbarWidth();
    }
    
    
    LRESULT NativeScrollbar::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_VSCROLL == inMessage || WM_HSCROLL == inMessage)
        {
            int currentScrollPos = Windows::getScrollPos(handle());
            int totalHeight = 0;
            int pageHeight = 0;
            int currentPosition = 0;
            Windows::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
            switch (LOWORD(wParam))
            {
                case SB_LINEUP: // user clicked the top arrow
                {
                    currentPosition -= 1;
                    break;
                }
                case SB_LINEDOWN: // user clicked the bottom arrow
                {
                    currentPosition += 1;
                    break;
                }                
                case SB_PAGEUP: // user clicked the scroll bar shaft above the scroll box
                {
                    currentPosition -= pageHeight;
                    break;
                }                
                case SB_PAGEDOWN: // user clicked the scroll bar shaft below the scroll box
                {
                    currentPosition += pageHeight;
                    break;
                }                
                case SB_THUMBTRACK: // user dragged the scroll box
                {
                    currentPosition = HIWORD(wParam);
                    break;
                }
                default:
                {
                    break; 
                }
            }
            if (currentPosition < 0)
            {
                currentPosition = 0;
            }
            if (currentPosition > totalHeight)
            {
                currentPosition = totalHeight;
            }

            setAttribute("curpos", Int2String(currentPosition));
            return 0;
        }        
        else if (WM_MOUSEWHEEL == inMessage)
		{
			short numDelta = HIWORD(wParam);
			short numPages = numDelta / WHEEL_DELTA;
            int totalHeight = 0;
			int pageHeight = 0;
            int currentPosition = 0;
            Windows::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
            currentPosition = currentPosition - numPages*pageHeight;
            if (currentPosition < 0)
            {
                currentPosition = 0;
            }
            if (currentPosition > totalHeight)
            {
                currentPosition = totalHeight;
            }
            setAttribute("curpos", Int2String(currentPosition));
			return 0;
		}
        return NativeControl::handleMessage(inMessage, wParam, lParam);
    }


    int NativeScrollbar::getCurrentPosition() const
    {
        return Windows::getScrollPos(handle());
    }


    void NativeScrollbar::setCurrentPosition(int inCurrentPosition)
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int oldCurPos = 0;
        Windows::getScrollInfo(handle(), totalHeight, pageHeight, oldCurPos);

        // The order in which curpos, maxpos and pageincrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (i.e. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (pageHeight == 0)
        {
            pageHeight = 1;
        }
        if (totalHeight < pageHeight)
        {
            totalHeight = pageHeight + 1;
        }
        if (totalHeight < inCurrentPosition)
        {
            totalHeight = inCurrentPosition + 1;
        }
        Windows::setScrollInfo(handle(), totalHeight, pageHeight, inCurrentPosition);
        if ((oldCurPos != inCurrentPosition) && eventHandler())
        {
            eventHandler()->curposChanged(this, oldCurPos, inCurrentPosition);
        }
    }


    int NativeScrollbar::getMaxPosition() const
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int curPos = 0;
        Windows::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
        return totalHeight;
    }


    void NativeScrollbar::setMaxPosition(int inMaxPosition)
    {
        int dummy = 0;
        int pageHeight = 0;
        int curPos = 0;
        Windows::getScrollInfo(handle(), dummy, pageHeight, curPos);

        // The order in which setCurPos, setMaxPos and setPageIncrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (i.e. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (pageHeight == 0)
        {
            pageHeight = 1;
        }
        if (inMaxPosition <= pageHeight)
        {
            pageHeight = inMaxPosition - 1;
        }
        Windows::setScrollInfo(handle(), inMaxPosition, pageHeight, curPos);
    }


    void NativeScrollbar::setIncrement(int inIncrement)
    {
        mIncrement = inIncrement;
    }


    int NativeScrollbar::getIncrement() const
    {
        return mIncrement;
    }


    void NativeScrollbar::setPageIncrement(int inPageIncrement)
    {
        int totalHeight = 0;
        int dummy = 0;
        int curPos = 0;
        Windows::getScrollInfo(handle(), totalHeight, dummy, curPos);

        // The order in which setCurPos, setMaxPos and setPageIncrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (i.e. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (totalHeight == 0)
        {
            totalHeight = 1;
        }
        if (curPos > totalHeight)
        {
            totalHeight += 1;
        }
        if (inPageIncrement >= totalHeight)
        {
            totalHeight = inPageIncrement + 1;
        }
        Windows::setScrollInfo(handle(), totalHeight, inPageIncrement, curPos);
    }


    int NativeScrollbar::getPageIncrement() const
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int curPos = 0;
        Windows::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
        return pageHeight;
    }


    bool NativeScrollbar::initAttributeControllers()
    {
        Super::setAttributeController("curpos", static_cast<ScrollbarCurrentPositionController*>(this));
        Super::setAttributeController("maxpos", static_cast<ScrollbarMaxPositionController*>(this));
        Super::setAttributeController("increment", static_cast<ScrollbarIncrementController*>(this));
        Super::setAttributeController("pageincrement", static_cast<ScrollbarPageIncrementController*>(this));
        return Super::initAttributeControllers();
    }


    TabsImpl::TabsImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }


    TabImpl::TabImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }


    TabPanelsImpl::Instances TabPanelsImpl::sInstances;


    TabPanelsImpl::TabPanelsImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping),
        mParentHandle(0),
        mTabBarHandle(0),
        mSelectedIndex(0),
        mChildCount(0)
    {
        NativeComponent * nativeParent = NativeControl::GetNativeThisOrParent(inParent);
        if (!nativeParent)
        {
            ReportError("TabPanelsImpl constructor failed because no native parent was found.");
            return;
        }

        mParentHandle = nativeParent->handle();
        
        mTabBarHandle = ::CreateWindowEx
        (
            0, 
            WC_TABCONTROL,
            0,
            WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
            0, 0, 0, 0,
            mParentHandle,
            (HMENU)mCommandId.intValue(),
            GetModuleHandle(0), // TODO: fix this hack
            0
        );
        ::SendMessage(mTabBarHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
        mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mParentHandle, GWL_WNDPROC, (LONG)(LONG_PTR)&TabPanelsImpl::MessageHandler);
        sInstances.insert(std::make_pair(mParentHandle, this));
    }


    TabPanelsImpl::~TabPanelsImpl()
    {
        Instances::iterator it = sInstances.find(mParentHandle);
        if (it != sInstances.end())
        {
            sInstances.erase(it);
        }
        ::SetWindowLongPtr(mParentHandle, GWL_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
        ::DestroyWindow(mTabBarHandle);
    }
    
    
    void TabPanelsImpl::addTabPanel(TabPanelImpl * inPanel)
    {
        if (TabImpl * tab = getCorrespondingTab(mChildCount))
        {
            Windows::appendTabPanel(mTabBarHandle, tab->el()->getAttribute("label"));
            mChildCount++;
        }
        update();
    }
        
    
    TabImpl * TabPanelsImpl::getCorrespondingTab(size_t inIndex)
    {
        for (size_t idx = 0; idx != el()->parent()->children().size(); ++idx)
        {
            if (el()->parent()->children()[idx]->type() == TabsElement::Type())
            {
                if (TabsElement * tabs = el()->parent()->children()[idx]->downcast<TabsElement>())
                {
                    return tabs->children()[inIndex]->component()->downcast<TabImpl>();
                }
            }
        }
        return 0;
    }


    void TabPanelsImpl::rebuildLayout()
    {
        Rect rect = clientRect();
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            Component * elementImpl = getChild(idx);
            elementImpl->move(rect.x(),
                              rect.y() + Defaults::tabHeight(),
                              rect.width(),
                              rect.height() - Defaults::tabHeight());
        }
        ::MoveWindow(mTabBarHandle, rect.x(), rect.y(), rect.width(), Defaults::tabHeight(), FALSE);
        rebuildChildLayouts();
    }


    int TabPanelsImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            int w = getChild(idx)->getWidth(inSizeConstraint);
            if (w > res)
            {
                res = w;
            }
        }
        return res;
    }

    
    int TabPanelsImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int res = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            int h = getChild(idx)->getHeight(inSizeConstraint);
            if (h > res)
            {
                res = h;
            }
        }
        return res + Defaults::tabHeight();
    }


    void TabPanelsImpl::update()
    {
        int selectedIndex = TabCtrl_GetCurSel(mTabBarHandle);
        for (size_t idx = 0; idx != mChildCount; ++idx)
        {
            getChild(idx)->setHidden(idx != selectedIndex);
        }
    }

    static NativeWindow * findParentWindow(Component * inChild)
    {
        NativeWindow * result = 0;
        if (result = dynamic_cast<NativeWindow*>(inChild))
        {
            return result;
        }
        else if (inChild->parent())
        {
            return findParentWindow(inChild->parent());
        }
        return 0;
    }


    LRESULT TabPanelsImpl::MessageHandler(HWND inHandle, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {

        Instances::iterator it = sInstances.find(inHandle);
        if (it == sInstances.end())
        {
            return ::DefWindowProc(inHandle, inMessage, wParam, lParam);
        }

        TabPanelsImpl * pThis = it->second;

        switch (inMessage)
        {
            case WM_NOTIFY:
            {
                if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
                {
                    pThis->update();

                    // HACK!
                    // When changing tabs the min-max sizes of the window can
                    // change and we need a way to make sure the window
                    // enlarges if needed. Our workaround consists of moving
                    // the window to the same location but one pixel higher,
                    // and then move it back to its original size.
                    // This seemingly zero-op has two side effects:
                    // 1. The minmax sizes will be enforced again. 
                    // 2. The content of the tabpanel is refreshed correctly.
                    NativeWindow * wnd = findParentWindow(pThis);
                    if (wnd)
                    {
                        RECT rw;
                        ::GetWindowRect(wnd->handle(), &rw);
                        int w = rw.right - rw.left;
                        int h = rw.bottom - rw.top;
                        ::MoveWindow(wnd->handle(), rw.left, rw.top, w, h+1, FALSE);
                        ::MoveWindow(wnd->handle(), rw.left, rw.top, w, h, FALSE);
                    }
                    return TRUE;
                }
            }
        }
        
        if (pThis->mOrigProc)
        {
            return ::CallWindowProc(pThis->mOrigProc, inHandle, inMessage, wParam, lParam);
        }
        else
        {
            return ::DefWindowProc(inHandle, inMessage, wParam, lParam);
        }
    }


    TabPanelImpl::TabPanelImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualBox(inParent, inAttributesMapping)
    {
    }
        
    
    bool TabPanelImpl::initComponent()
    {
        if (TabPanelsImpl * parent = el()->parent()->component()->downcast<TabPanelsImpl>())
        {
            parent->addTabPanel(this);
        }
        return Super::initComponent();
    }


    GroupBoxImpl::GroupBoxImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualBox(inParent, inAttributesMapping),
        mGroupBoxHandle(0),
        mMarginLeft(2),
        mMarginTop(16),
        mMarginRight(2),
        mMarginBottom(2)
    {
        
        // HACK!!!
        // Group boxes don't render properly if the parent window has the
        // WS_CLIPCHILDREN style defined.
        // There should be some more decent way to fix this. But for now
        // I just remove the flag from the parent. This may result in more
        // flickering during manual resize of the WindowElement.
        Windows::removeWindowStyle(NativeControl::GetNativeThisOrParent(inParent)->handle(), WS_CLIPCHILDREN);


        mGroupBoxHandle = CreateWindowEx(0,
                                         TEXT("BUTTON"),
                                         0,
                                         WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                                         0, 0, 0, 0,
                                         NativeControl::GetNativeThisOrParent(inParent)->handle(),
                                         (HMENU)mCommandId.intValue(),
                                         GetModuleHandle(0),
                                         0);
        ::SendMessage(mGroupBoxHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
    }


    GroupBoxImpl::~GroupBoxImpl()
    {
        ::DestroyWindow(mGroupBoxHandle);
    }
        
        
    Orient GroupBoxImpl::getOrient() const
    {
        return mOrient.or(Vertical);
    }


    void GroupBoxImpl::setCaption(const std::string & inLabel)
    {
        Windows::setWindowText(mGroupBoxHandle, inLabel);
    }


    int GroupBoxImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int textWidth = Defaults::textPadding() + Windows::getTextSize(mGroupBoxHandle, Windows::getWindowText(mGroupBoxHandle)).cx;
        int contentWidth = mBoxLayouter.calculateWidth(inSizeConstraint);
        return mMarginLeft + std::max<int>(textWidth, contentWidth) + mMarginRight;
    }


    int GroupBoxImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return mMarginTop + mBoxLayouter.calculateHeight(inSizeConstraint) + mMarginBottom;
    }
    
    
    void GroupBoxImpl::rebuildLayout()
    {
        Rect clientRect(Super::clientRect());
        ::MoveWindow(mGroupBoxHandle,
                     clientRect.x(),
                     clientRect.y(),
                     clientRect.width(),
                     clientRect.height(),
                     FALSE);
        mBoxLayouter.rebuildLayout();
    }


    Rect GroupBoxImpl::clientRect() const
    {
        Rect clientRect(Super::clientRect());
        Rect rect(clientRect.x() + mMarginLeft,
                  clientRect.y() + mMarginTop,
                  clientRect.width() - mMarginRight - mMarginLeft,
                  clientRect.height() - mMarginTop - mMarginBottom);
        return rect;
    }

    
    // The 'caption' is optionally the first child element of the groupbox.
    // This element is responsible for setting the groupbox title and it has
    // no other purpose than that.
    // I think this is a design error in Mozilla XUL. The groupbox title should
    // have been an attribute, not a child element.
    // However, we want to be compatible with Mozilla XUL so we have to support it.

    // In order to keep the layout managers from taking the caption element
    // into account we never return it and return the element after it instead.
    // This requires getChildCount to be overridden as well to make it return
    // the number of children minus one.
    const Component * GroupBoxImpl::getChild(size_t idx) const
    {
        if (Super::getChildCount() > 0)
        {
            if (Super::getChild(0)->el()->type() == CaptionElement::Type())
            {
                assert (idx + 1 < Super::getChildCount());
                if (idx + 1 < Super::getChildCount())
                {
                    return Super::getChild(idx + 1);
                }
            }
        }
        return Super::getChild(idx);
    }

    
    Component * GroupBoxImpl::getChild(size_t idx)
    {
        return const_cast<Component *>(static_cast<const GroupBoxImpl*>(this)->getChild(idx));
    }
    
    
    // Returns number of children minus the caption element.
    // See documention of GroupBoxImpl::getChild method for more information.
    size_t GroupBoxImpl::getChildCount() const
    {
        if (Super::getChildCount() > 0)
        {
            if (Super::getChild(0)->el()->type() == CaptionElement::Type())
            {
                return Super::getChildCount() - 1;
            }
        }
        return Super::getChildCount();
    }


    CaptionImpl::CaptionImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }
        
    
    int CaptionImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(mParent))
        {
            return Windows::getTextSize(comp->handle(), mElement->getAttribute("label")).cx;
        }
        return 0;
    }

    
    int CaptionImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(mParent))
        {
            return Windows::getTextSize(comp->handle(), mElement->getAttribute("label")).cy;
        }
        return 0;
    }


    bool CaptionImpl::initComponent()
    {
        if (GroupBoxImpl * groupBox = mParent->downcast<GroupBoxImpl>())
        {
            groupBox->setCaption(mElement->getAttribute("label"));
        }
        return Super::initComponent();
    }


    TreeImpl::TreeImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, WC_TREEVIEW, 0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS)
    {
    }


    int TreeImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = Defaults::controlWidth();
        if (const TreeChildrenImpl * children = findChildOfType<TreeChildrenImpl>())
        {
            result = Defaults::treeIndent() + children->calculateWidth(inSizeConstraint);
        }
        return result;
    }


    int TreeImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const TreeChildrenImpl * children = findChildOfType<TreeChildrenImpl>())
        {
            result = children->calculateHeight(inSizeConstraint);
        }
        return result;
    }

    
    void TreeImpl::addInfo(const TreeItemInfo & inInfo)
    {
        addInfo(TVI_ROOT, TVI_FIRST, inInfo);
    }

    
    HTREEITEM TreeImpl::addInfo(HTREEITEM inRoot, HTREEITEM inPrev, const TreeItemInfo & inInfo)
    {
        std::wstring label = ToUTF16(inInfo.label());

        TVITEM tvi; 
        tvi.mask = TVIF_TEXT;
        tvi.pszText = const_cast<TCHAR*>(label.c_str());
        tvi.cchTextMax = label.size();

        TVINSERTSTRUCT tvins; 
        tvins.item = tvi; 
        tvins.hInsertAfter = inPrev;
        tvins.hParent = inRoot;
        inPrev = (HTREEITEM)SendMessage(handle(), TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

        HTREEITEM prevChild = TVI_FIRST;
        for (size_t idx = 0; idx != inInfo.getChildCount(); ++idx)
        {
            prevChild = addInfo(inPrev, prevChild, *inInfo.getChild(idx));
        }

        return inPrev;
    }
    
    
    bool TreeImpl::initComponent()
    {
        if (TreeChildrenImpl * children = findChildOfType<TreeChildrenImpl>())
        {
            if (Component * firstChild = children->el()->children()[0]->component())
            {
                if (TreeItemImpl * item = firstChild->downcast<TreeItemImpl>())
                {
                    addInfo(item->itemInfo());
                }
            }
        }
        return Super::initComponent();
    }


    TreeChildrenImpl::TreeChildrenImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }
        
    
    int TreeChildrenImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeItemImpl * item = child->downcast<TreeItemImpl>())
            {
                result += item->calculateHeight(inSizeConstraint);
            }
        }
        return result;
    }


    int TreeChildrenImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeItemImpl * item = child->downcast<TreeItemImpl>())
            {
                int minWidth = item->calculateWidth(inSizeConstraint);
                if (result < minWidth)
                {
                    result = minWidth;
                }
            }
        }
        return result;
    }


    TreeItemImpl::TreeItemImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }
        
    
    int TreeItemImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const TreeRowImpl * row = findChildOfType<TreeRowImpl>())
        {
            int minHeight = row->calculateHeight(inSizeConstraint);
            if (result < minHeight)
            {
                result = minHeight;
            }
        }
        if (const TreeChildrenImpl * treeChildren = findChildOfType<TreeChildrenImpl>())
        {
            result += treeChildren->calculateHeight(inSizeConstraint);
        }
        return result;
    }


    int TreeItemImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const TreeRowImpl * row = findChildOfType<TreeRowImpl>())
        {
            int minWidth = row->calculateWidth(inSizeConstraint);
            if (result < minWidth)
            {
                result = minWidth;
            }
        }
        if (const TreeChildrenImpl * treeChildren = findChildOfType<TreeChildrenImpl>())
        {
            int minWidth = Defaults::treeIndent() + treeChildren->calculateWidth(inSizeConstraint);
            if (result < minWidth)
            {
                result = minWidth;
            }            
        }
        return result;
    }
    
    
    bool TreeItemImpl::isOpened() const
    {
        //if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(const_cast<TreeItemImpl*>(this)))
        //{
        //    TreeView_GetItemState(comp->handle(), hti, mask);
        //}
        return false; // TODO: implement
    }


    bool TreeItemImpl::initComponent()
    {
        if (TreeRowImpl * row = findChildOfType<TreeRowImpl>())
        {
            if (TreeCellImpl * cell = row->findChildOfType<TreeCellImpl>())
            {
                std::string label = cell->getLabel();
                mItemInfo.setLabel(label);
            }
        }
        if (TreeChildrenImpl * treeChildren = findChildOfType<TreeChildrenImpl>())
        {
            for (size_t idx = 0; idx != treeChildren->getChildCount(); ++idx)
            {
                Component * child = treeChildren->getChild(idx);
                if (TreeItemImpl * item = child->downcast<TreeItemImpl>())
                {
                    mItemInfo.addChild(&item->itemInfo());
                }
            }
        }
        return Super::initComponent();
    }


    TreeColsImpl::TreeColsImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }


    TreeColImpl::TreeColImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }


    TreeRowImpl::TreeRowImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }


    int TreeRowImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeCellImpl * cell = child->downcast<TreeCellImpl>())
            {
                result += cell->calculateWidth(inSizeConstraint);
            }
        }
        return result;
    }


    int TreeRowImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeCellImpl * cell = child->downcast<TreeCellImpl>())
            {
                result += cell->calculateHeight(inSizeConstraint);
            }
        }
        return result;
    }


    TreeCellImpl::TreeCellImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }


    bool TreeCellImpl::initAttributeControllers()
    {
        Super::setAttributeController("label", static_cast<LabelController*>(this));
        return Super::initAttributeControllers();
    }


    std::string TreeCellImpl::getLabel() const
    {
        return mLabel;
    }


    void TreeCellImpl::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }

    
    int TreeCellImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(const_cast<TreeCellImpl*>(this)))
        {
            result = Windows::getTextSize(comp->handle(), getLabel()).cx + Defaults::textPadding();
        }
        return result;
    }


    int TreeCellImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::treeItemHeight();
    }


    StatusbarImpl::StatusbarImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, STATUSCLASSNAME, 0, SBARS_SIZEGRIP),
        mBoxLayouter(this)
    {
    }


    bool StatusbarImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int StatusbarImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            result += child->calculateWidth(inSizeConstraint);
        }
        return result;
    }


    int StatusbarImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::statusBarHeight();
    }


    Orient StatusbarImpl::getOrient() const
    {
        return Horizontal;
    }


    Align StatusbarImpl::getAlign() const
    {
        return Center;
    }


    Rect StatusbarImpl::clientRect() const
    {
        Rect clientRect(Super::clientRect());
        // Substract from with one square to make place for the resize gripper widget
        return Rect(clientRect.x(), clientRect.y(), clientRect.width() - Defaults::statusBarHeight(), clientRect.height());
    }


    void StatusbarImpl::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }


    void StatusbarImpl::rebuildChildLayouts()
    {
        Super::rebuildChildLayouts();
    }


    StatusbarPanelImpl::StatusbarPanelImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping, TEXT("STATIC"), 0, 0)
    {
    }


    bool StatusbarPanelImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int StatusbarPanelImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Windows::getTextSize(handle(), getLabel()).cx;
    }


    int StatusbarPanelImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Windows::getTextSize(handle(), getLabel()).cy;
    }


    ToolbarImpl::ToolbarImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping)
    {            
        if (NativeComponent * nativeComponent = NativeControl::GetNativeThisOrParent(inParent))
        {
            RECT rect;
            rect.top = 0;
            rect.left = 0;
            rect.bottom = Defaults::toolbarHeight();
            rect.right = 1000;
            mToolbar.reset(new Windows::ToolbarElement(this, ::GetModuleHandle(0), nativeComponent->handle(), rect, mCommandId.intValue()));
            setHandle(mToolbar->handle(), false);
            registerHandle();
        }
    }


    ToolbarImpl::~ToolbarImpl()
    {
        unregisterHandle();
        mToolbar.reset();
    }


    bool ToolbarImpl::initComponent()
    {
        mToolbar->buildToolbar();
        mToolbar->rebuildLayout();
        ShowWindow(mToolbar->handle(), SW_SHOW);
        return Super::initComponent();
    }


    bool ToolbarImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int ToolbarImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            result += child->calculateWidth(inSizeConstraint);
        }
        return result;
    }


    int ToolbarImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            int minHeight = child->calculateHeight(inSizeConstraint);
            if (minHeight > result)
            {
                result = minHeight;
            }
        }
        return result;
    }


    void ToolbarImpl::rebuildLayout()
    {
        mToolbar->rebuildLayout();        
    }


    ToolbarButtonImpl::ToolbarButtonImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping),
        mButton(0),
        mDisabled(false)
    {
        if (ToolbarImpl * toolbar = parent()->downcast<ToolbarImpl>())
        {
            boost::shared_ptr<Gdiplus::Bitmap> nullImage;
            std::string label = getLabel();

            std::string buttonType;
            AttributesMapping::const_iterator it = inAttributesMapping.find("type");
            if (it != inAttributesMapping.end())
            {
                buttonType = it->second;
            }
            //menu: Set the type attribute to the value menu to create a button with a menu popup. Place a menupopup element inside the button in this case. The user may click anywhere on the button to open and close the menu.
            //menu-button: You can also use the value menu-button to create a button with a menu. Unlike the menu type, this type requires the user to press the arrow to open the menu, but a different command may be invoked when the main part of the button is pressed. This type of button would be used for browser's back and forward buttons.
            //checkbox: Use this type to create a toggle button which will switch the checked state each time the button is pressed.
            //radio: Use this type to create a radio button. You can also create a group of toolbarbutton using this type and the attribute group.
            if (buttonType == "menu")
            {
                mButton = new Windows::ToolbarDropDown(toolbar->nativeToolbar(),
                                                       this,
                                                       mCommandId.intValue(), 
                                                       label,
                                                       label,
                                                       nullImage,
                                                       false);
            }
            else if (buttonType == "menu-button")
            {
                mButton = new Windows::ToolbarDropDown(toolbar->nativeToolbar(),
                                                       this,
                                                       mCommandId.intValue(), 
                                                       label,
                                                       label,
                                                       nullImage,
                                                       true);
            }
            else // buttonType.empty() or buttonType == "button"
            {
                mButton = new Windows::ToolbarButtonElement(toolbar->nativeToolbar(),
                                                     mCommandId.intValue(), 
                                                     boost::function<void()>(),
                                                     label,
                                                     label,
                                                     nullImage);
            }
            assert(mButton);
            toolbar->nativeToolbar()->add(mButton);
            // Now that mButton is constructed we can apply any previously set
            // attributes.
            setLabel(mLabel);
            setDisabled(mDisabled);
            setCSSListStyleImage(mCSSListStyleImage);
        }
    }


    bool ToolbarButtonImpl::initComponent()
    {
        return Super::initComponent();
    }
    

    void ToolbarButtonImpl::showToolbarMenu(RECT inToolbarButtonRect)
    {
        showPopupMenu(inToolbarButtonRect);
    }
    

    void ToolbarButtonImpl::showPopupMenu(RECT inToolbarButtonRect)
    {
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (MenuPopupComponent * popupMenu = child->component()->downcast<MenuPopupComponent>())
            {
                popupMenu->show(inToolbarButtonRect);
            }
        }
    }


    bool ToolbarButtonImpl::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        setAttributeController("disabled", static_cast<DisabledController*>(this));
        return Super::initAttributeControllers();
    }


    bool ToolbarButtonImpl::initStyleControllers()
    {
        setStyleController(CSSListStyleImageController::PropertyName(), static_cast<CSSListStyleImageController*>(this));
        return Super::initStyleControllers();
    }


    int ToolbarButtonImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        if (ToolbarImpl * toolbarImpl = parent()->downcast<ToolbarImpl>())
        {
            int textWidth = Windows::getTextSize(toolbarImpl->handle(), getLabel()).cx;
            int imageWidth = 0;
            if (mButton && mButton->image())
            {
                imageWidth = mButton->image()->GetWidth();
            }
            return std::max<int>(textWidth, imageWidth);

        }
        return 0;
    }


    int ToolbarButtonImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = Defaults::toolbarHeight();
        if (ToolbarImpl * toolbarImpl = parent()->downcast<ToolbarImpl>())
        {
            int textHeight = Windows::getTextSize(toolbarImpl->handle(), getLabel()).cy;
            if (textHeight > result)
            {
                result = textHeight;
            }
            if (mButton && mButton->image())
            {
                int imageHeight = mButton->image()->GetHeight();
                if (imageHeight > result)
                {
                    result = imageHeight;
                }
            }
        }
        return result;
    }
    
    
    std::string ToolbarButtonImpl::getLabel() const
    {
        if (mButton)
        {
            return mButton->text();
        }
        return mLabel;
    }

    
    void ToolbarButtonImpl::setLabel(const std::string & inLabel)
    {
        if (mButton)
        {
            mButton->setText(inLabel);
        }
        mLabel = inLabel;
    }


    bool ToolbarButtonImpl::isDisabled() const
    {
        return mDisabled;
    }


    void ToolbarButtonImpl::setDisabled(bool inDisabled)
    {
        if (mButton)
        {
            if (ToolbarImpl * toolbar = parent()->downcast<ToolbarImpl>())
            {
                SendMessage(toolbar->handle(), TB_ENABLEBUTTON, (WPARAM)mCommandId.intValue(), (LPARAM)MAKELONG(inDisabled ? FALSE : TRUE, 0));
            }
        }
        else
        {
            mDisabled = inDisabled;
        }
    }
    
    
    void ToolbarButtonImpl::setCSSListStyleImage(const std::string & inURL)
    {
        if (mButton)
        {
            ChromeURL chromeURL(inURL, Defaults::locale());
            std::wstring utf16URL = ToUTF16(chromeURL.convertToLocalPath());
            boost::shared_ptr<Gdiplus::Bitmap> img(new Gdiplus::Bitmap(utf16URL.c_str()));
            mButton->setImage(img);
        }
        mCSSListStyleImage = inURL;
    }

    
    const std::string & ToolbarButtonImpl::getCSSListStyleImage() const
    {
        return mCSSListStyleImage;
    }


} // namespace XULWin
