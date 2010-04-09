#include "XULWin/Component.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/ElementUtilities.h"
#include "XULWin/Algorithms.h"
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
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/String.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <gdiplus.h>


namespace XULWin
{


    void ReportSetterFail(const std::string & inAttributeName,
                          const std::string & inType,
                          const std::string & inId)
    {
        std::stringstream ss;
        ss << "Failed to set attribute '" << inAttributeName << "'"
           << " on the element with type '" << inType << "'";

        if (!inId.empty())
        {
            ss << " and with 'id' " << inId;
        }
        ss << ".";
        ReportError(ss.str());
    }


    int CommandId::sId = 101; // start handleCommand Ids at 101 to avoid conflicts with Windows predefined values

    NativeComponent::ComponentsByHandle NativeComponent::sComponentsByHandle;

    NativeComponent::ComponentsById NativeComponent::sComponentsById;

    ConcreteComponent::ConcreteComponent(Component * inParent, Poco::XML::Element * inElement) :
        mParentComponent(inParent),
        mElement(inElement),
        mCommandId(),
        mExpansive(false),
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
        mStrokeWidth.setInvalid();
        mCSSWidth.setInvalid();
        mCSSHeight.setInvalid();
        mCSSFill.setInvalid();
        mCSSStroke.setInvalid();
        mCSSBackgroundColor.setInvalid();
        mOrient.setInvalid();
        mAlign.setInvalid();
    }


    ConcreteComponent::~ConcreteComponent()
    {
    }

    
    void ConcreteComponent::setTagName(const std::string & inTagName)
    {
        mTagName = inTagName;
    }


    bool ConcreteComponent::init()
    {
        mIsInitialized = true;
        return true;
    }


    int ConcreteComponent::calculateMaxChildWidth(SizeConstraint inSizeConstraint) const
    {
        return max_element_value<int>(mChildren.begin(),
                                      mChildren.end(),
                                      0,
                                      boost::bind(&Component::calculateWidth, _1, inSizeConstraint));
    }


    int ConcreteComponent::calculateMaxChildHeight(SizeConstraint inSizeConstraint) const
    {
        return max_element_value<int>(mChildren.begin(),
                                      mChildren.end(),
                                      0,
                                      boost::bind(&Component::calculateHeight, _1, inSizeConstraint));
    }


    int ConcreteComponent::calculateSumChildWidths(SizeConstraint inSizeConstraint) const
    {
        return sum_element_values<int>(mChildren.begin(),
                                       mChildren.end(),
                                       0,
                                       boost::bind(&Component::calculateWidth, _1, inSizeConstraint));
    }


    int ConcreteComponent::calculateSumChildHeights(SizeConstraint inSizeConstraint) const
    {
        return sum_element_values<int>(mChildren.begin(),
                                       mChildren.end(),
                                       0,
                                       boost::bind(&Component::calculateHeight, _1, inSizeConstraint));
    }


    int ConcreteComponent::getIndex() const
    {
        for (size_t idx = 0; idx != parentComponent()->getChildCount(); ++idx)
        {
            if (this == parentComponent()->getChild(idx))
            {
                return idx;
            }
        }
        return -1;
    }


    size_t ConcreteComponent::getChildCount() const
    {
        return mChildren.size();
    }
    
    
    const std::string & ConcreteComponent::tagName() const
    {
        return mTagName;
    }
    
    
    void ConcreteComponent::addChild(ComponentPtr inComponent)
    {
        if (!inComponent)
        {
            throw std::logic_error("Don't add child components if they are NULL.");
        }
        mChildren.push_back(inComponent);
    }


    const Component * ConcreteComponent::getChild(size_t inIndex) const
    {
        return mChildren[inIndex].get();
    }


    Component * ConcreteComponent::getChild(size_t inIndex)
    {
        return mChildren[inIndex].get();
    }


    HWND ConcreteComponent::getFirstParentHandle()
    {
        if (NativeComponent * comp = NativeControl::GetThisOrParent(parentComponent()))
        {
            return comp->handle();
        }
        return 0;
    }


    void ConcreteComponent::invalidateRect() const
    {
        // Just forward to all childNodes.
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            getChild(idx)->invalidateRect();
        }
    }


    const RGBColor & ConcreteComponent::getCSSFill() const
    {
        if (mCSSFill.isValid())
        {
            return mCSSFill;
        }

        if (parentComponent())
        {
            return parentComponent()->getCSSFill();
        }

        return mCSSFill; // default value
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
            return mCSSStroke.getValue();
        }

        if (parentComponent())
        {
            return parentComponent()->getCSSStroke();
        }

        return mCSSStroke;
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


    void ConcreteComponent::setCSSBackgroundColor(const RGBColor & inColor)
    {
        mCSSBackgroundColor = inColor;
    }


    RGBColor ConcreteComponent::getCSSBackgroundColor() const
    {
        return mCSSBackgroundColor;
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
        return getWidth(Preferred);
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

        return getHeight(Preferred);
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
        if (MarginDecorator * obj = GetComponent<MarginDecorator>(el()))
        {
            obj->setMargin(inTop, inLeft, inRight, inBottom);
        }
        // If no margin decorator found, insert one, and set the value.
        else if (Decorator * dec = GetComponent<Decorator>(el()))
        {
            ComponentPtr newDec(new MarginDecorator(dec->decoratedComponent()));
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
            ErrorCatcher errorCatcher;
            StyleController * controller = it->second;
            controller->set(inValue);
            if (errorCatcher.hasCaught())
            {
                ReportSetterFail(inName, el()->tagName(), el()->getAttribute("id"));
            }
            return true;
        }
        return false;
    }


    bool ConcreteComponent::setAttribute(const std::string & inName, const std::string & inValue)
    {
        AttributeControllers::iterator it = mAttributeControllers.find(inName);
        if (it != mAttributeControllers.end())
        {
            ErrorCatcher errorCatcher;
            it->second->set(inValue);
            if (errorCatcher.hasCaught())
            {
                ReportSetterFail(inName, el()->tagName(), el()->getAttribute("id"));
            }
            return true;
        }
        return false;
    }


    bool ConcreteComponent::initAttributeControllers()
    {
        setAttributeController<WidthController>(this);
        setAttributeController<HeightController>(this);
        setAttributeController<FlexController>(this);
        setAttributeController<HiddenController>(this);
        setAttributeController<AlignController>(this);
        setAttributeController<OrientController>(this);
        return true;
    }


    bool ConcreteComponent::initStyleControllers()
    {
        setStyleController<CSSXController>(this);
        setStyleController<CSSYController>(this);
        setStyleController<CSSWidthController>(this);
        setStyleController<CSSHeightController>(this);
        setStyleController<CSSMarginController>(this);
        setStyleController<CSSBackgroundColorController>(this);
        setStyleController<CSSFillController>(this);
        setStyleController<CSSStrokeController>(this);
        return true;
    }


    void ConcreteComponent::move(const Rect & inRect)
    {
        move(inRect.x(), inRect.y(), inRect.width(), inRect.height());
    }


    void ConcreteComponent::setOwningElement(Poco::XML::Element * inOwningElement)
    {
        mElement = inOwningElement;
    }


    Poco::XML::Element * ConcreteComponent::el() const
    {
        return mElement;
    }


    Component * ConcreteComponent::parentComponent() const
    {
        return mParentComponent;
    }


    void ConcreteComponent::sync()
    {
        AttributeControllers::iterator it = mAttributeControllers.begin(), end = mAttributeControllers.end();
        for (; it != end; ++it)
        {
            const Poco::XML::XMLString & attrValue = mElement->getAttribute(it->first);
            if (!attrValue.empty())
            {
                setAttribute(it->first, attrValue);
            }
        }
        
        for (size_t idx = 0; idx != mChildren.size(); ++idx)
        {
            mChildren[idx]->sync();
        }
    }


    void ConcreteComponent::rebuildChildLayouts()
    {
        if (!mElement)
        {
            return;
        }
        for (size_t idx = 0; idx != mChildren.size(); ++idx)
        {
            if (NativeComponent * nativeComp = mChildren[idx]->downcast<NativeComponent>())
            {
                nativeComp->rebuildLayout();
            }
        }
    }


    HMODULE NativeComponent::sModuleHandle(0);


    NativeComponent::NativeComponent(Component * inParent, Poco::XML::Element * inDOMElement) :
        ConcreteComponent(inParent, inDOMElement),
        mHandle(0),
        mModuleHandle(NativeComponent::GetModuleHandle()),
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
        assert(sComponentsByHandle.find(mHandle) == sComponentsByHandle.end());
        sComponentsByHandle.insert(std::make_pair(mHandle, this));

        assert(sComponentsById.find(mCommandId.intValue()) == sComponentsById.end());
        sComponentsById.insert(std::make_pair(mCommandId.intValue(), this));
    }


    void NativeComponent::unregisterHandle()
    {
        ComponentsById::iterator itById =sComponentsById.find(mCommandId.intValue());
        assert(itById !=sComponentsById.end());
        if (itById != sComponentsById.end())
        {
            sComponentsById.erase(itById);
        }

        ComponentsByHandle::iterator itByHandle = sComponentsByHandle.find(mHandle);
        assert(itByHandle != sComponentsByHandle.end());
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


    NativeComponent * NativeComponent::FindByHandle(HWND inHandle)
    {
        ComponentsByHandle::iterator it = sComponentsByHandle.find(inHandle);
        if (it != sComponentsByHandle.end())
        {
            return it->second;
        }
        return 0;
    }


    NativeComponent * NativeComponent::FindById(int inId)
    {
        ComponentsById::iterator it = sComponentsById.find(inId);
        if (it !=sComponentsById.end())
        {
            return it->second;
        }
        return 0;
    }


    void NativeComponent::invalidateRect() const
    {
        ::RedrawWindow(handle(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
        ::InvalidateRect(handle(), NULL, TRUE);
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


    HMODULE NativeComponent::GetModuleHandle()
    {
        return sModuleHandle ? sModuleHandle : ::GetModuleHandle(0);
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
        setAttributeController<DisabledController>(this);
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    bool NativeComponent::addEventListener(NativeEventListener * inEventListener)
    {
        EventListeners::iterator it = mEventListeners.find(inEventListener);
        if (it == mEventListeners.end())
        {
            mEventListeners.insert(inEventListener);
            return true;
        }
        return false;
    }


    bool NativeComponent::removeEventListener(NativeEventListener * inEventListener)
    {
        EventListeners::iterator it = mEventListeners.find(inEventListener);
        if (it != mEventListeners.end())
        {
            mEventListeners.erase(it);
            return true;
        }
        return false;
    }


    bool NativeComponent::getCustomBrush(HDC inHDC, HWND inHWND, HBRUSH & outHBRUSH)
    {
        if (mCSSBackgroundColor.isValid())
        {
            COLORREF colorRef = RGB(mCSSBackgroundColor.getValue().red(),
                                    mCSSBackgroundColor.getValue().green(),
                                    mCSSBackgroundColor.getValue().blue());
            outHBRUSH = ::CreateSolidBrush(colorRef);
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
            (*it)->handleCommand(this, notificationCode, wParam, lParam);
        }
    }


    //void NativeComponent::handleMenuCommand(WORD inMenuId)
    //{
    //    EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
    //    for (; it != end; ++it)
    //    {
    //        (*it)->handleMenuCommand(this, inMenuId);
    //    }
    //}


    void NativeComponent::handleDialogCommand(WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleDialogCommand(this, inNotificationCode, wParam, lParam);
        }
    }


    LRESULT NativeComponent::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (inMessage)
        {
            case WM_COMMAND:
            {
                if (lParam == 0) // menu or accelerator
                {
                    //int menuId = LOWORD(wParam);
                    //if (Menu * menu = Menu::FindById(menuId))
                    //{
                    //    // TODO: what should we do here
                    //    assert("What is this?");
                    //}
                    //else if (MenuItem * menuItem = MenuItem::FindById(menuId))
                    //{
                    //    handleMenuCommand(menuId);
                    //    return 0;
                    //}
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
                            NativeComponent * focus = FindByHandle(::GetFocus());
                            if (focus)
                            {
                                focus->handleDialogCommand(paramLo, wParam, lParam);
                                return 0;
                            }
                            break;
                        }
                        default:
                        {
                            // NOTE TO SELF: don't use "FindById(LOWORD(wParam))" here
                            //               because that won't work for toolbar buttons.
                            NativeComponent * sender = FindByHandle((HWND)lParam);
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
                HWND handle = (HWND)lParam;
                ComponentsByHandle::iterator it = sComponentsByHandle.find(handle);
                if (it != sComponentsByHandle.end())
                {
                    it->second->handleMessage(inMessage, wParam, lParam);
                    return 0;
                }
                break;
            }
            case WM_CTLCOLORSTATIC:
            {
                HDC hDC = (HDC)wParam;
                HWND hSender = (HWND)lParam;
                if (XULWin::NativeComponent * sender = NativeComponent::FindByHandle(hSender))
                {
                    HBRUSH hBrush;
                    if (sender->getCustomBrush(hDC, hSender, hBrush))
                    {
                        return (BOOL)hBrush;
                    }
                }
                break;
            }
        }

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        bool handled = false;
        for (; it != end; ++it)
        {
            int result = (*it)->handleMessage(this, inMessage, wParam, lParam);
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


    void Dialog::Register(HMODULE inModuleHandle)
    {
        WNDCLASSEX wndClass;
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &Dialog::MessageHandler;
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


    Dialog::Dialog(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeComponent(inParent, inDOMElement),
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


    Dialog::~Dialog()
    {
    }


    bool Dialog::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    bool Dialog::initAttributeControllers()
    {
        setAttributeController<TitleController>(this);
        return Super::initAttributeControllers();
    }


    Rect Dialog::clientRect() const
    {
        RECT rc;
        ::GetClientRect(handle(), &rc);
        return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }


    Rect Dialog::windowRect() const
    {
        RECT rw;
        ::GetWindowRect(handle(), &rw);
        return Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
    }


    int Dialog::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return getOrient() == Horizontal ? calculateSumChildWidths(inSizeConstraint)
               : calculateMaxChildWidth(inSizeConstraint);
    }


    int Dialog::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return getOrient() == Vertical ? calculateSumChildHeights(inSizeConstraint)
               : calculateMaxChildHeight(inSizeConstraint);
    }


    void Dialog::move(int x, int y, int w, int h)
    {
        ::MoveWindow(handle(), x, y, w, h, FALSE);
    }


    void Dialog::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }


    Orient Dialog::getOrient() const
    {
        return Super::getOrient();
    }


    Align Dialog::getAlign() const
    {
        return Super::getAlign();
    }


    std::string Dialog::getTitle() const
    {
        return Windows::getWindowText(handle());
    }


    void Dialog::setTitle(const std::string & inTitle)
    {
        Windows::setWindowText(handle(), inTitle);
    }


    DialogResult Dialog::showModal(Window * inInvoker)
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


    LRESULT Dialog::endModal(DialogResult inDialogResult)
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


    LRESULT Dialog::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (inMessage)
        {
            case WM_SIZE:
            {
                rebuildLayout();
                invalidateRect();
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
                MINMAXINFO * minMaxInfo = (MINMAXINFO *)lParam;
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
                        NativeComponent * sender = FindById(LOWORD(wParam));
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
                NativeComponent * sender = FindByHandle((HWND)lParam);
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
            (*it)->handleMessage(this, inMessage, wParam, lParam);
        }
        return ::DefWindowProc(handle(), inMessage, wParam, lParam);
    }


    LRESULT CALLBACK Dialog::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        NativeComponent * sender = FindByHandle(hWnd);
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


    VirtualComponent::VirtualComponent(Component * inParent, Poco::XML::Element * inDOMElement) :
        ConcreteComponent(inParent, inDOMElement)
    {
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


    PassiveComponent::PassiveComponent(Component * inParent, Poco::XML::Element * inDOMElement) :
        VirtualComponent(inParent, inDOMElement)
    {
    }


    PassiveComponent::~PassiveComponent()
    {

    }


    NativeControl::NativeControl(Component * inParent, Poco::XML::Element * inDOMElement, LPCTSTR inClassName, DWORD inExStyle, DWORD inStyle) :
        NativeComponent(inParent, inDOMElement)
    {
        if (!mParentComponent)
        {
            ReportError("NativeControl constructor failed because parent is NULL.");
            return;
        }

        Rect clientRect = inParent->clientRect();

        NativeComponent * nativeParent = GetThisOrParent(inParent);
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


    NativeControl::NativeControl(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeComponent(inParent, inDOMElement)
    {
        // Don't call registerHandle() or subclass() here.
        // They have to be called in your subclass and after setHandle() has been called.
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
        if (NativeComponent * nativeParent = dynamic_cast<NativeComponent *>(parentComponent()))
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


    const NativeComponent * NativeControl::GetThisOrParent(const Component * inElement)
    {
        if (const NativeComponent * obj = dynamic_cast<const NativeComponent *>(inElement))
        {
            return obj;
        }
        else if (const Decorator * obj = dynamic_cast<const Decorator *>(inElement))
        {
            return GetThisOrParent(obj->decoratedComponent().get());
        }
        else if (const VirtualComponent * obj = dynamic_cast<const VirtualComponent *>(inElement))
        {
            return GetThisOrParent(obj->parentComponent());
        }
        return 0;
    }


    NativeComponent * NativeControl::GetThisOrParent(Component * inElement)
    {
        return const_cast<NativeComponent *>(GetThisOrParent(const_cast<const Component *>(inElement)));
    }


    Button::Button(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeControl(inParent,
                      inDOMElement,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_PUSHBUTTON)
    {
    }


    int Button::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::buttonHeight();
    }


    int Button::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = Windows::getWindowText(handle());
        int minWidth = Windows::getTextSize(handle(), text).cx;
        minWidth += Defaults::textPadding();
        return std::max<int>(minWidth, Defaults::buttonWidth());
    }


    CheckBox::CheckBox(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeControl(inParent, inDOMElement, TEXT("BUTTON"), 0, WS_TABSTOP | BS_AUTOCHECKBOX)
    {
    }


    int CheckBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }


    int CheckBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::checkBoxMinimumWidth() + Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx;
    }


    bool CheckBox::isChecked() const
    {
        return Windows::isCheckBoxChecked(handle());
    }


    void CheckBox::setChecked(bool inChecked)
    {
        Windows::setCheckBoxChecked(handle(), inChecked);
    }


    bool CheckBox::initAttributeControllers()
    {
        setAttributeController<CheckedController>(this);
        return Super::initAttributeControllers();
    }


    TextBox::TextBox(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeControl(inParent,
                      inDOMElement,
                      TEXT("EDIT"),
                      WS_EX_CLIENTEDGE, // exStyle
                      WS_TABSTOP | GetFlags(inDOMElement)),
        mRows(1)
    {
    }


    DWORD TextBox::GetFlags(Poco::XML::Element * inDOMElement)
    {
        DWORD flags = 0;
        const Poco::XML::XMLString & typeAttr = inDOMElement->getAttribute("type");
        if (typeAttr == "password")
        {
            flags |= ES_PASSWORD;
        }
        
        const Poco::XML::XMLString & multilineAttr = inDOMElement->getAttribute("multiline");
        if (multilineAttr == "true")
        {
            flags |= WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
        }
        else
        {
            flags |= ES_AUTOHSCROLL;
        }
        return flags;
    }


    std::string TextBox::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void TextBox::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }


    bool TextBox::isReadOnly() const
    {
        return Windows::isTextBoxReadOnly(handle());
    }


    void TextBox::setReadOnly(bool inReadOnly)
    {
        Windows::setTextBoxReadOnly(handle(), inReadOnly);
    }


    int TextBox::getRows() const
    {
        return mRows;
    }


    void TextBox::setRows(int inRows)
    {
        mRows = inRows;
    }


    bool TextBox::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        setAttributeController<ReadOnlyController>(this);
        setAttributeController<RowsController>(this);
        return Super::initAttributeControllers();
    }


    int TextBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::textBoxWidth();;
    }


    int TextBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight() * getRows();
    }


    LRESULT TextBox::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (inMessage == WM_KEYDOWN)
        {
            if (wParam == VK_TAB)
            {
                //
                // Tabbing doesn't work on multiline textboxes.
                // This code fixes that.
                //
                long style = ::GetWindowLong(handle(), GWL_STYLE);
                if ((style & ES_MULTILINE) && (style & WS_TABSTOP))
                {
                    BOOL shift = 0x8000 & GetKeyState(VK_SHIFT);
                    ::SetFocus(::GetNextDlgTabItem(GetParent(handle()), handle(), shift));
                    return 0;
                }
            }
            else
            {
                //
                // CTRL-a select all
                //
                const int cKeyboard_a = 65;
                if (HIWORD(::GetKeyState(VK_CONTROL)) && wParam == cKeyboard_a)
                {
                    ::SendMessage(handle(), EM_SETSEL, 0, -1);
                }
            }
        }
        return Super::handleMessage(inMessage, wParam, lParam);
    }


    Description::Description(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeControl(inParent,
                      inDOMElement,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_LEFT)
    {
    }


    std::string Description::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void Description::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }


    bool Description::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        return Super::initAttributeControllers();
    }


    int Description::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::textPadding();
    }


    int Description::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Windows::getMultilineTextHeight(handle());
    }


    //VirtualBox::VirtualBox(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualComponent(inParent, inDOMElement),
    //    mBoxLayouter(this)
    //{
    //}


    //Orient VirtualBox::getOrient() const
    //{
    //    return mOrient.or(Horizontal);
    //}


    //Align VirtualBox::getAlign() const
    //{
    //    return mAlign.or(Stretch);
    //}


    //bool VirtualBox::initAttributeControllers()
    //{
    //    return Super::initAttributeControllers();
    //}


    //void VirtualBox::rebuildLayout()
    //{
    //    mBoxLayouter.rebuildLayout();
    //}



    //Box::Box(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent, inDOMElement, TEXT("STATIC"), WS_EX_CONTROLPARENT, WS_TABSTOP),
    //    mBoxLayouter(this)
    //{
    //}


    //Orient Box::getOrient() const
    //{
    //    return mOrient.or(Vertical);
    //}


    //Align Box::getAlign() const
    //{
    //    return mAlign.or(Stretch);
    //}


    //void Box::rebuildLayout()
    //{
    //    mBoxLayouter.rebuildLayout();
    //}


    //int Box::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return mBoxLayouter.calculateWidth(inSizeConstraint);
    //}


    //int Box::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return mBoxLayouter.calculateHeight(inSizeConstraint);
    //}


    //Rect Box::clientRect() const
    //{
    //    return Super::clientRect();
    //}


    //const Component * Box::getChild(size_t idx) const
    //{
    //    return GetComponent<Component>(mElement->childNodes()->item(idx));
    //}


    //Component * Box::getChild(size_t idx)
    //{
    //    return GetComponent<Component>(mElement->childNodes()->item(idx));
    //}


    //MenuList::MenuList(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(
    //        inParent,
    //        inDOMElement,
    //        TEXT("COMBOBOX"),
    //        0, // exStyle
    //        WS_TABSTOP | CBS_DROPDOWNLIST)
    //{
    //}


    //bool MenuList::init()
    //{
    //    fillComboBox();
    //    return Super::init();
    //}


    //void MenuList::fillComboBox()
    //{
    //    if (MenuPopup * popup = findChildOfType<MenuPopup>())
    //    {
    //        for (size_t idx = 0; idx != popup->getChildCount(); ++idx)
    //        {
    //            Poco::XML::Node * child = popup->el()->childNodes()->item(idx);
    //            if (MenuItem * item = GetComponent<MenuItem>(child))
    //            {
    //                std::string label = item->getLabel();
    //                Windows::addStringToComboBox(handle(), label);
    //            }
    //        }
    //        Windows::selectComboBoxItem(handle(), 0);
    //    }
    //}


    //int MenuList::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::menuListMinWidth() + calculateMaxChildWidth(inSizeConstraint);
    //}


    //int MenuList::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::controlHeight();
    //}


    //void MenuList::move(int x, int y, int w, int h)
    //{
    //    // The height of a combobox in Win32 is the height of the dropdown menu
    //    // + the height of the widget itself.

    //    int numItems = Windows::getComboBoxItemCount(handle());
    //    int dropdownHeight = 0;
    //    if (numItems > 0)
    //    {
    //        dropdownHeight = numItems * Windows::getComboBoxItemHeight(handle(), 0); // use index 0
    //    }

    //    // This is usually needed as well, I think :S
    //    int extraHeight = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;

    //    NativeControl::move(x, y, w, h + dropdownHeight + extraHeight);
    //}


    //void MenuList::onContentChanged()
    //{
    //    if (mIsInitialized)
    //    {
    //        Windows::clearComboBox(handle());
    //        fillComboBox();
    //    }
    //    // else: the init will take care of the initial fill
    //}


    //Separator::Separator(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent,
    //                  inDOMElement,
    //                  TEXT("STATIC"),
    //                  0, // exStyle
    //                  SS_GRAYFRAME)
    //{
    //    mExpansive = true;
    //}


    //int Separator::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return 1;
    //}


    //int Separator::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return 1;
    //}


    //Spacer::Spacer(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualComponent(inParent, inDOMElement)
    //{
    //}


    //int Spacer::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return 0;
    //}


    //int Spacer::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return 0;
    //}


    //MenuButton::MenuButton(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent,
    //                  inDOMElement,
    //                  TEXT("BUTTON"),
    //                  0, // exStyle
    //                  WS_TABSTOP | BS_PUSHBUTTON)
    //{
    //}


    //int MenuButton::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx + Defaults::textPadding()*2;
    //}


    //int MenuButton::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::buttonHeight();
    //}


    VirtualGrid::VirtualGrid(Component * inParent,
                             Poco::XML::Element * inDOMElement) :
        VirtualComponent(inParent, inDOMElement)
    {
    }


    int VirtualGrid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Columns * columns = findChildOfType<Columns>())
        {
            result = columns->calculateWidth(inSizeConstraint);
        }
        return result;

    }


    int VirtualGrid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Rows * rows = findChildOfType<Rows>())
        {
            result = rows->calculateHeight(inSizeConstraint);
        }
        return result;
    }


    void VirtualGrid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        Columns * columns = findChildOfType<Columns>();
        if (!columns || columns->getChildCount() == 0)
        {
            return;
        }

        Rows * rows = findChildOfType<Rows>();
        if (!rows || rows->getChildCount() == 0)
        {
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
        {
            if (Column * col = columns->getChild(colIdx)->downcast<Column>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(col->getFlex()),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Preferred))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("Grid has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(row->getFlex()),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Preferred))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("Grid has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(rows->getChildCount(), columns->getChildCount());
        Rect clientRect(clientRect());
        GridLayoutManager::GetOuterRects(clientRect, colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(rows->getChildCount(), columns->getChildCount(), CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                int rowHeight = row->getHeight();
                for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
                {
                    if (Column * column = columns->getChild(colIdx)->downcast<Column>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->getChild(colIdx);
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
        GenericGrid<Rect> innerRects(rows->getChildCount(), columns->getChildCount());
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
            {
                if (rowIdx < rows->getChildCount())
                {
                    if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = columns->getChild(colIdx);
                            const Rect & r = innerRects.get(rowIdx, colIdx);
                            child->move(r.x(), r.y(), r.width(), r.height());
                        }
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }


    Grid::Grid(Component * inParent,
               Poco::XML::Element * inDOMElement) :
        NativeControl(inParent, inDOMElement, TEXT("STATIC"), WS_EX_CONTROLPARENT, WS_TABSTOP)
    {
    }


    int Grid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Columns * columns = findChildOfType<Columns>())
        {
            result = columns->calculateWidth(inSizeConstraint);
        }
        return result;

    }


    int Grid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Rows * rows = findChildOfType<Rows>())
        {
            result = rows->calculateHeight(inSizeConstraint);
        }
        return result;
    }


    void Grid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        Columns * columns = findChildOfType<Columns>();
        if (!columns || columns->getChildCount() == 0)
        {
            return;
        }

        Rows * rows = findChildOfType<Rows>();
        if (!rows || rows->getChildCount() == 0)
        {
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
        {
            if (Column * col = columns->getChild(colIdx)->downcast<Column>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(col->getFlex()),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Preferred))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("Grid has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(row->getFlex()),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Preferred))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("Grid has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(rows->getChildCount(), columns->getChildCount());
        Rect clientRect(clientRect());
        GridLayoutManager::GetOuterRects(clientRect, colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(rows->getChildCount(), columns->getChildCount(), CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                int rowHeight = row->getHeight();
                for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
                {
                    if (Column * column = columns->getChild(colIdx)->downcast<Column>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->getChild(colIdx);
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
        GenericGrid<Rect> innerRects(rows->getChildCount(), columns->getChildCount());
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
            {
                if (rowIdx < rows->getChildCount())
                {
                    if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->getChild(colIdx);
                            const Rect & r = innerRects.get(rowIdx, colIdx);
                            child->move(r.x(), r.y(), r.width(), r.height());
                        }
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }

    

    Rows::Rows(Component * inParent, Poco::XML::Element * inDOMElement) :
        VirtualComponent(inParent, inDOMElement)
    {
    }


    int Rows::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Row *> rows;

        //
        // 1. Obtain the list of rows from the grid element
        //
        GetImmediateChildComponents<Row>(parentComponent(), rows);

        //
        // 2. Get the max row width
        //
        result = max_element_value(rows.begin(),
                                   rows.end(),
                                   0,
                                   boost::bind(&Component::calculateWidth, _1, inSizeConstraint));
        return result;
    }


    int Rows::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Row *> rows;

        //
        // 1. Obtain the list of rows from the grid element
        //
        GetImmediateChildComponents<Row>(parentComponent(), rows);

        //
        // 2. Get the sum of row heights
        //
        result = sum_element_values(rows.begin(),
                                    rows.end(),
                                    0,
                                    boost::bind(&Component::calculateHeight, _1, inSizeConstraint));
        return result;
    }


    Columns::Columns(Component * inParent, Poco::XML::Element * inElement) :
        VirtualComponent(inParent, inElement)
    {
    }


    int Columns::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Column *> columns;

        //
        // 1. Obtain the list of columns from the grid element
        //
        GetImmediateChildComponents<Column>(parentComponent(), columns);

        //
        // 2. Get the max column width
        //
        result = sum_element_values(columns.begin(),
                                    columns.end(),
                                    0,
                                    boost::bind(&Component::calculateWidth, _1, inSizeConstraint));
        return result;
    }


    int Columns::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Column *> columns;

        //
        // 1. Obtain the list of columns from the grid element
        //
        GetImmediateChildComponents<Column>(parentComponent(), columns);

        //
        // 2. Get the sum of column heights
        //
        result = max_element_value(columns.begin(),
                                   columns.end(),
                                   0,
                                   boost::bind(&Component::calculateHeight, _1, inSizeConstraint));
        return result;
    }


    Row::Row(Component * inParent, Poco::XML::Element * inElement) :
        VirtualComponent(inParent, inElement)
    {
    }


    int Row::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildWidths(inSizeConstraint);
    }


    int Row::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildHeight(inSizeConstraint);
    }


    Column::Column(Component * inParent, Poco::XML::Element * inElement) :
        VirtualComponent(inParent, inElement)
    {
    }


    Align Column::getAlign() const
    {
        return mAlign.or(Stretch);
    }


    int Column::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        // This is a special case: we need to get all rows first and calculate
        // the width of each row's corresponding column
        Component * rows(0);
        int ownIndex = -1;
        Component * grid = parentComponent()->parentComponent();
        for (size_t idx = 0; idx != grid->getChildCount(); ++idx)
        {
            Component * child = grid->getChild(idx);
            if (child->tagName() == Rows::TagName())
            {
                rows = child;
            }
            else if (child->tagName() == Columns::TagName())
            {
                for (size_t ownI = 0; ownI != child->getChildCount(); ++ownI)
                {
                    if (child->getChild(ownI)->commandId() == commandId())
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
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            Component * row = rows->getChild(rowIdx);
            if (ownIndex < row->getChildCount())
            {
                int w = row->getChild(ownIndex)->getWidth(inSizeConstraint);
                if (w > res)
                {
                    res = w;
                }
            }
        }
        return res;
    }


    int Column::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildHeights(inSizeConstraint);
    }


    //RadioGroup::RadioGroup(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualBox(inParent, inDOMElement)
    //{
    //}


    //Radio::Radio(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent,
    //                  inDOMElement,
    //                  TEXT("BUTTON"),
    //                  0, // exStyle
    //                  WS_TABSTOP | BS_RADIOBUTTON)
    //{
    //}


    //int Radio::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::radioButtonMinimumWidth() + Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx;
    //}


    //int Radio::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::controlHeight();
    //}


    //ProgressMeter::ProgressMeter(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent,
    //                  inDOMElement,
    //                  PROGRESS_CLASS,
    //                  0, // exStyle
    //                  PBS_SMOOTH)
    //{
    //    Windows::initializeProgressMeter(mHandle, 100);
    //}


    //int ProgressMeter::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::progressMeterWidth();
    //}


    //int ProgressMeter::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::progressMeterHeight();
    //}


    //int ProgressMeter::getValue() const
    //{
    //    return Windows::getProgressMeterProgress(handle());
    //}


    //void ProgressMeter::setValue(int inValue)
    //{
    //    Windows::setProgressMeterProgress(handle(), inValue);
    //}


    //bool ProgressMeter::initAttributeControllers()
    //{
    //    setAttributeController<IntValueController>(this);
    //    return Super::initAttributeControllers();
    //}


    //Deck::Deck(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualComponent(inParent, inDOMElement),
    //    mSelectedIndex(0)
    //{
    //}


    //int Deck::getSelectedIndex() const
    //{
    //    return mSelectedIndex;
    //}


    //void Deck::setSelectedIndex(int inSelectedIndex)
    //{
    //    mSelectedIndex = inSelectedIndex;
    //    rebuildLayout();
    //    invalidateRect();
    //}


    //void Deck::rebuildLayout()
    //{
    //    for (size_t idx = 0; idx != getChildCount(); ++idx)
    //    {
    //        Poco::XML::Element * element = Node2Element(el()->childNodes()->item(idx));
    //        bool visible = idx == mSelectedIndex;
    //        Component * comp = GetComponent<Component>(element);
    //        comp->setHidden(!visible);
    //        if (visible)
    //        {
    //            Rect rect = clientRect();
    //            comp->move(rect.x(), rect.y(), rect.width(), rect.height());
    //        }
    //    }
    //    rebuildChildLayouts();
    //}


    //int Deck::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return calculateMaxChildWidth(inSizeConstraint);
    //}


    //int Deck::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return calculateMaxChildHeight(inSizeConstraint);
    //}


    //bool Deck::initAttributeControllers()
    //{
    //    setAttributeController<SelectedIndexController>(this);
    //    return Super::initAttributeControllers();
    //}


    //DWORD Scrollbar::GetFlags(Poco::XML::Element * inDOMElement)
    //{
    //    DWORD flags = 0;
    //    const Poco::XML::XMLString & orientAttribute = inDOMElement->getAttribute("orient");
    //    if (orientAttribute == "horizontal")
    //    {
    //        flags |= SBS_HORZ | SBS_RIGHTALIGN;
    //    }
    //    else if (orientAttribute == "vertical")
    //    {
    //        flags |= SBS_VERT | SBS_BOTTOMALIGN;
    //    }
    //    else
    //    {
    //        ReportError("Invalid orient found for scrollbar!");
    //    }
    //    return flags;
    //}


    //Scrollbar::Scrollbar(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent, inDOMElement,
    //                  TEXT("SCROLLBAR"),
    //                  0, // exStyle
    //                  WS_TABSTOP | GetFlags(inDOMElement)),
    //    mIncrement(0)
    //{
    //    mExpansive = true;
    //    Windows::setScrollInfo(handle(), 100, 10, 0);
    //}


    //int Scrollbar::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::scrollbarWidth();
    //}


    //int Scrollbar::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::scrollbarWidth();
    //}


    //LRESULT Scrollbar::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    //{
    //    if (WM_VSCROLL == inMessage || WM_HSCROLL == inMessage)
    //    {
    //        int currentScrollPos = Windows::getScrollPos(handle());
    //        int totalHeight = 0;
    //        int pageHeight = 0;
    //        int currentPosition = 0;
    //        Windows::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
    //        switch (LOWORD(wParam))
    //        {
    //            case SB_LINEUP: // user clicked the top arrow
    //            {
    //                currentPosition -= 1;
    //                break;
    //            }
    //            case SB_LINEDOWN: // user clicked the bottom arrow
    //            {
    //                currentPosition += 1;
    //                break;
    //            }
    //            case SB_PAGEUP: // user clicked the scroll bar shaft above the scroll box
    //            {
    //                currentPosition -= pageHeight;
    //                break;
    //            }
    //            case SB_PAGEDOWN: // user clicked the scroll bar shaft below the scroll box
    //            {
    //                currentPosition += pageHeight;
    //                break;
    //            }
    //            case SB_THUMBTRACK: // user dragged the scroll box
    //            {
    //                currentPosition = HIWORD(wParam);
    //                break;
    //            }
    //            default:
    //            {
    //                break;
    //            }
    //        }
    //        if (currentPosition < 0)
    //        {
    //            currentPosition = 0;
    //        }
    //        if (currentPosition > totalHeight)
    //        {
    //            currentPosition = totalHeight;
    //        }

    //        setAttribute("curpos", Int2String(currentPosition));
    //        return 0;
    //    }
    //    else if (WM_MOUSEWHEEL == inMessage)
    //    {
    //        short numDelta = HIWORD(wParam);
    //        short numPages = numDelta / WHEEL_DELTA;
    //        int totalHeight = 0;
    //        int pageHeight = 0;
    //        int currentPosition = 0;
    //        Windows::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
    //        currentPosition = currentPosition - numPages*pageHeight;
    //        if (currentPosition < 0)
    //        {
    //            currentPosition = 0;
    //        }
    //        if (currentPosition > totalHeight)
    //        {
    //            currentPosition = totalHeight;
    //        }
    //        setAttribute("curpos", Int2String(currentPosition));
    //        return 0;
    //    }
    //    return NativeControl::handleMessage(inMessage, wParam, lParam);
    //}


    //int Scrollbar::getCurrentPosition() const
    //{
    //    return Windows::getScrollPos(handle());
    //}


    //void Scrollbar::setCurrentPosition(int inCurrentPosition)
    //{
    //    int totalHeight = 0;
    //    int pageHeight = 0;
    //    int oldCurPos = 0;
    //    Windows::getScrollInfo(handle(), totalHeight, pageHeight, oldCurPos);

    //    // The order in which curpos, maxpos and pageincrement
    //    // will be set (alphabetically by attribute name) can cause
    //    // impossible scrollbar states (i.e. currentpos or pageincrement
    //    // greater than maxpos). And we want to avoid that.
    //    // Our workaround is to detect such states here, and change invalid
    //    // values to valid ones.
    //    if (pageHeight == 0)
    //    {
    //        pageHeight = 1;
    //    }
    //    if (totalHeight < pageHeight)
    //    {
    //        totalHeight = pageHeight + 1;
    //    }
    //    if (totalHeight < inCurrentPosition)
    //    {
    //        totalHeight = inCurrentPosition + 1;
    //    }
    //    Windows::setScrollInfo(handle(), totalHeight, pageHeight, inCurrentPosition);
    //    if ((oldCurPos != inCurrentPosition) && eventHandler())
    //    {
    //        eventHandler()->curposChanged(this, oldCurPos, inCurrentPosition);
    //    }
    //}


    //int Scrollbar::getMaxPosition() const
    //{
    //    int totalHeight = 0;
    //    int pageHeight = 0;
    //    int curPos = 0;
    //    Windows::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
    //    return totalHeight;
    //}


    //void Scrollbar::setMaxPosition(int inMaxPosition)
    //{
    //    int dummy = 0;
    //    int pageHeight = 0;
    //    int curPos = 0;
    //    Windows::getScrollInfo(handle(), dummy, pageHeight, curPos);

    //    // The order in which setCurPos, setMaxPos and setPageIncrement
    //    // will be set (alphabetically by attribute name) can cause
    //    // impossible scrollbar states (i.e. currentpos or pageincrement
    //    // greater than maxpos). And we want to avoid that.
    //    // Our workaround is to detect such states here, and change invalid
    //    // values to valid ones.
    //    if (pageHeight == 0)
    //    {
    //        pageHeight = 1;
    //    }
    //    if (inMaxPosition <= pageHeight)
    //    {
    //        pageHeight = inMaxPosition - 1;
    //    }
    //    Windows::setScrollInfo(handle(), inMaxPosition, pageHeight, curPos);
    //}


    //void Scrollbar::setIncrement(int inIncrement)
    //{
    //    mIncrement = inIncrement;
    //}


    //int Scrollbar::getIncrement() const
    //{
    //    return mIncrement;
    //}


    //void Scrollbar::setPageIncrement(int inPageIncrement)
    //{
    //    int totalHeight = 0;
    //    int dummy = 0;
    //    int curPos = 0;
    //    Windows::getScrollInfo(handle(), totalHeight, dummy, curPos);

    //    // The order in which setCurPos, setMaxPos and setPageIncrement
    //    // will be set (alphabetically by attribute name) can cause
    //    // impossible scrollbar states (i.e. currentpos or pageincrement
    //    // greater than maxpos). And we want to avoid that.
    //    // Our workaround is to detect such states here, and change invalid
    //    // values to valid ones.
    //    if (totalHeight == 0)
    //    {
    //        totalHeight = 1;
    //    }
    //    if (curPos > totalHeight)
    //    {
    //        totalHeight = curPos + 1;
    //    }
    //    if (inPageIncrement >= totalHeight)
    //    {
    //        totalHeight = inPageIncrement + 1;
    //    }
    //    Windows::setScrollInfo(handle(), totalHeight, inPageIncrement, curPos);
    //}


    //int Scrollbar::getPageIncrement() const
    //{
    //    int totalHeight = 0;
    //    int pageHeight = 0;
    //    int curPos = 0;
    //    Windows::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
    //    return pageHeight;
    //}


    //bool Scrollbar::initAttributeControllers()
    //{
    //    setAttributeController<ScrollbarCurrentPositionController>(this);
    //    setAttributeController<ScrollbarMaxPositionController>(this);
    //    setAttributeController<ScrollbarIncrementController>(this);
    //    setAttributeController<ScrollbarPageIncrementController>(this);
    //    return Super::initAttributeControllers();
    //}


    //Tabs::Tabs(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //Tab::Tab(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //TabPanels::Instances TabPanels::sInstances;


    //TabPanels::TabPanels(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualComponent(inParent, inDOMElement),
    //    mParentHandle(0),
    //    mTabBarHandle(0),
    //    mSelectedIndex(0),
    //    mChildCount(0)
    //{
    //    NativeComponent * nativeParent = NativeControl::GetThisOrParent(inParent);
    //    if (!nativeParent)
    //    {
    //        ReportError("TabPanels constructor failed because no native parent was found.");
    //        return;
    //    }

    //    mParentHandle = nativeParent->handle();

    //    mTabBarHandle = ::CreateWindowEx
    //                    (
    //                        0,
    //                        WC_TABCONTROL,
    //                        0,
    //                        WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
    //                        0, 0, 0, 0,
    //                        mParentHandle,
    //                        (HMENU)mCommandId.intValue(),
    //                        NativeComponent::GetModuleHandle(),
    //                        0
    //                    );
    //    ::SendMessage(mTabBarHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
    //    mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mParentHandle, GWL_WNDPROC, (LONG)(LONG_PTR)&TabPanels::MessageHandler);
    //    sInstances.insert(std::make_pair(mParentHandle, this));
    //}


    //TabPanels::~TabPanels()
    //{
    //    Instances::iterator it = sInstances.find(mParentHandle);
    //    if (it != sInstances.end())
    //    {
    //        sInstances.erase(it);
    //    }
    //    ::SetWindowLongPtr(mParentHandle, GWL_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
    //    ::DestroyWindow(mTabBarHandle);
    //}


    //void TabPanels::addTabPanel(TabPanel * inPanel)
    //{
    //    if (Tab * tab = getCorrespondingTab(mChildCount))
    //    {
    //        Windows::appendTabPanel(mTabBarHandle, tab->el()->getAttribute("label"));
    //        mChildCount++;
    //    }
    //    update();
    //}


    //Tab * TabPanels::getCorrespondingTab(size_t inIndex)
    //{
    //    for (size_t idx = 0; idx != el()->parentNode()->childNodes()->length(); ++idx)
    //    {
    //        if ((el()->parentNode()->childNodes()->item(idx))->tagName() == Tabs::TagName())
    //        {
    //            if (Tabs * tabs = GetComponent<Tabs>(el()->parentNode()->childNodes()->item(idx)))
    //            {
    //                return tabs->childNodes()->item(inIndex)->downcast<Tab>();
    //            }
    //        }
    //    }
    //    return 0;
    //}


    //void TabPanels::rebuildLayout()
    //{
    //    Rect rect = clientRect();
    //    for (size_t idx = 0; idx != getChildCount(); ++idx)
    //    {
    //        Component * element = getChild(idx);
    //        element->move(rect.x(),
    //                      rect.y() + Defaults::tabHeight(),
    //                      rect.width(),
    //                      rect.height() - Defaults::tabHeight());
    //    }
    //    ::MoveWindow(mTabBarHandle, rect.x(), rect.y(), rect.width(), Defaults::tabHeight(), FALSE);
    //    rebuildChildLayouts();
    //}


    //int TabPanels::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return calculateMaxChildWidth(inSizeConstraint);
    //}


    //int TabPanels::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::tabHeight() + calculateMaxChildHeight(inSizeConstraint);
    //}


    //void TabPanels::update()
    //{
    //    int selectedIndex = TabCtrl_GetCurSel(mTabBarHandle);
    //    for (size_t idx = 0; idx != mChildCount; ++idx)
    //    {
    //        getChild(idx)->setHidden(idx != selectedIndex);
    //    }
    //}

    //static Window * findParentWindow(Component * inChild)
    //{
    //    Window * result = 0;
    //    if (result = dynamic_cast<Window *>(inChild))
    //    {
    //        return result;
    //    }
    //    else if (inChild->parentNode())
    //    {
    //        return findParentWindow(inChild->parentNode());
    //    }
    //    return 0;
    //}


    //LRESULT TabPanels::MessageHandler(HWND inHandle, UINT inMessage, WPARAM wParam, LPARAM lParam)
    //{

    //    Instances::iterator it = sInstances.find(inHandle);
    //    if (it == sInstances.end())
    //    {
    //        return ::DefWindowProc(inHandle, inMessage, wParam, lParam);
    //    }

    //    TabPanels * pThis = it->second;

    //    switch (inMessage)
    //    {
    //        case WM_NOTIFY:
    //        {
    //            if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
    //            {
    //                pThis->update();

    //                // HACK!
    //                // When changing tabs the min-max sizes of the window can
    //                // change and we need a way to make sure the window
    //                // enlarges if needed. Our workaround consists of moving
    //                // the window to the same location but one pixel higher,
    //                // and then move it back to its original size.
    //                // This seemingly zero-op has two side effects:
    //                // 1. The minmax sizes will be enforced again.
    //                // 2. The content of the tabpanel is refreshed correctly.
    //                Window * wnd = findParentWindow(pThis);
    //                if (wnd)
    //                {
    //                    RECT rw;
    //                    ::GetWindowRect(wnd->handle(), &rw);
    //                    int w = rw.right - rw.left;
    //                    int h = rw.bottom - rw.top;
    //                    ::MoveWindow(wnd->handle(), rw.left, rw.top, w, h+1, FALSE);
    //                    ::MoveWindow(wnd->handle(), rw.left, rw.top, w, h, FALSE);
    //                }
    //                return TRUE;
    //            }
    //        }
    //    }

    //    if (pThis->mOrigProc)
    //    {
    //        return ::CallWindowProc(pThis->mOrigProc, inHandle, inMessage, wParam, lParam);
    //    }
    //    else
    //    {
    //        return ::DefWindowProc(inHandle, inMessage, wParam, lParam);
    //    }
    //}


    //TabPanel::TabPanel(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualBox(inParent, inDOMElement)
    //{
    //}


    //bool TabPanel::init()
    //{
    //    if (TabPanels * parent = el()->parentNode()->downcast<TabPanels>())
    //    {
    //        parent->addTabPanel(this);
    //    }
    //    return Super::init();
    //}


    //GroupBox::GroupBox(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualBox(inParent, inDOMElement),
    //    mGroupBoxHandle(0),
    //    mMarginLeft(2),
    //    mMarginTop(16),
    //    mMarginRight(2),
    //    mMarginBottom(2)
    //{

    //    // HACK!!!
    //    // Group boxes don't render properly if the parent window has the
    //    // WS_CLIPCHILDREN style defined.
    //    // There should be some more decent way to fix this. But for now
    //    // I just remove the flag from the parent. This may result in more
    //    // flickering during manual resize of the .
    //    Windows::removeWindowStyle(NativeControl::GetThisOrParent(inParent)->handle(), WS_CLIPCHILDREN);


    //    mGroupBoxHandle = CreateWindowEx(0,
    //                                     TEXT("BUTTON"),
    //                                     0,
    //                                     WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
    //                                     0, 0, 0, 0,
    //                                     NativeControl::GetThisOrParent(inParent)->handle(),
    //                                     (HMENU)mCommandId.intValue(),
    //                                     NativeComponent::GetModuleHandle(),
    //                                     0);
    //    ::SendMessage(mGroupBoxHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
    //}


    //GroupBox::~GroupBox()
    //{
    //    ::DestroyWindow(mGroupBoxHandle);
    //}


    //Orient GroupBox::getOrient() const
    //{
    //    return mOrient.or(Vertical);
    //}


    //void GroupBox::setCaption(const std::string & inLabel)
    //{
    //    Windows::setWindowText(mGroupBoxHandle, inLabel);
    //}


    //int GroupBox::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    int textWidth = Defaults::textPadding() + Windows::getTextSize(mGroupBoxHandle, Windows::getWindowText(mGroupBoxHandle)).cx;
    //    int contentWidth = mBoxLayouter.calculateWidth(inSizeConstraint);
    //    return mMarginLeft + std::max<int>(textWidth, contentWidth) + mMarginRight;
    //}


    //int GroupBox::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return mMarginTop + mBoxLayouter.calculateHeight(inSizeConstraint) + mMarginBottom;
    //}


    //void GroupBox::rebuildLayout()
    //{
    //    Rect clientRect(Super::clientRect());
    //    ::MoveWindow(mGroupBoxHandle,
    //                 clientRect.x(),
    //                 clientRect.y(),
    //                 clientRect.width(),
    //                 clientRect.height(),
    //                 FALSE);
    //    mBoxLayouter.rebuildLayout();
    //}


    //Rect GroupBox::clientRect() const
    //{
    //    Rect clientRect(Super::clientRect());
    //    Rect rect(clientRect.x() + mMarginLeft,
    //              clientRect.y() + mMarginTop,
    //              clientRect.width() - mMarginRight - mMarginLeft,
    //              clientRect.height() - mMarginTop - mMarginBottom);
    //    return rect;
    //}


    //// The 'caption' is optionally the first child element of the groupbox.
    //// This element is responsible for setting the groupbox title and it has
    //// no other purpose than that.
    //// I think this is a design error in Mozilla XUL. The groupbox title should
    //// have been an attribute, not a child element.
    //// However, we want to be compatible with Mozilla XUL so we have to support it.

    //// In order to keep the layout managers from taking the caption element
    //// into account we never return it and return the element after it instead.
    //// This requires getChildCount to be overridden as well to make it return
    //// the number of childNodes minus one.
    //const Component * GroupBox::getChild(size_t idx) const
    //{
    //    if (Super::getChildCount() > 0)
    //    {
    //        if (Super::getChild(0)->el()->tagName() == ::TagName())
    //        {
    //            assert(idx + 1 < Super::getChildCount());
    //            if (idx + 1 < Super::getChildCount())
    //            {
    //                return Super::getChild(idx + 1);
    //            }
    //        }
    //    }
    //    return Super::getChild(idx);
    //}


    //Component * GroupBox::getChild(size_t idx)
    //{
    //    return const_cast<Component *>(static_cast<const GroupBox *>(this)->getChild(idx));
    //}


    //// Returns number of childNodes minus the caption element.
    //// See documention of GroupBox::getChild method for more information.
    //size_t GroupBox::getChildCount() const
    //{
    //    if (Super::getChildCount() > 0)
    //    {
    //        if (Super::getChild(0)->el()->tagName() == ::TagName())
    //        {
    //            return Super::getChildCount() - 1;
    //        }
    //    }
    //    return Super::getChildCount();
    //}


    //Caption::Caption(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    VirtualComponent(inParent, inDOMElement)
    //{
    //}


    //int Caption::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    if (NativeComponent * comp = NativeControl::GetThisOrParent(mParentComponent))
    //    {
    //        return Windows::getTextSize(comp->handle(), ->getAttribute("label")).cx;
    //    }
    //    return 0;
    //}


    //int Caption::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    if (NativeComponent * comp = NativeControl::GetThisOrParent(mParentComponent))
    //    {
    //        return Windows::getTextSize(comp->handle(), ->getAttribute("label")).cy;
    //    }
    //    return 0;
    //}


    //bool Caption::init()
    //{
    //    if (GroupBox * groupBox = mParentComponent->downcast<GroupBox>())
    //    {
    //        groupBox->setCaption(->getAttribute("label"));
    //    }
    //    return Super::init();
    //}


    //Tree::Tree(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent, inDOMElement, WC_TREEVIEW, 0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS)
    //{
    //}


    //int Tree::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    int result = Defaults::controlWidth();
    //    if (const TreeChildren * childNodes = findChildOfType<TreeChildren>())
    //    {
    //        result = Defaults::treeIndent() + childNodes->calculateWidth(inSizeConstraint);
    //    }
    //    return result;
    //}


    //int Tree::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    if (const TreeChildren * childNodes = findChildOfType<TreeChildren>())
    //    {
    //        result = childNodes->calculateHeight(inSizeConstraint);
    //    }
    //    return result;
    //}


    //void Tree::addInfo(const TreeItemInfo & inInfo)
    //{
    //    addInfo(TVI_ROOT, TVI_FIRST, inInfo);
    //}


    //HTREEITEM Tree::addInfo(HTREEITEM inRoot, HTREEITEM inPrev, const TreeItemInfo & inInfo)
    //{
    //    std::wstring label = ToUTF16(inInfo.data());

    //    TVITEM tvi;
    //    tvi.mask = TVIF_TEXT;
    //    tvi.pszText = const_cast<TCHAR *>(label.c_str());
    //    tvi.cchTextMax = label.size();

    //    TVINSERTSTRUCT tvins;
    //    tvins.item = tvi;
    //    tvins.hInsertAfter = inPrev;
    //    tvins.hParent = inRoot;
    //    inPrev = (HTREEITEM)SendMessage(handle(), TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

    //    HTREEITEM prevChild = TVI_FIRST;
    //    TreeItemInfo::const_iterator it = inInfo.begin(), end = inInfo.end();
    //    for (; it != end; ++it)
    //    {
    //        const TreeItemInfo * itemInfo = *it;
    //        prevChild = addInfo(inPrev, prevChild, *itemInfo);
    //    }

    //    return inPrev;
    //}


    //bool Tree::init()
    //{
    //    if (TreeChildren * childNodes = findChildOfType<TreeChildren>())
    //    {
    //        if (Component * firstChild = childNodes->el()->childNodes()->item(0])
    //        {
    //            if (TreeItem * item = firstChild->downcast<TreeItem>())
    //            {
    //                addInfo(item->itemInfo());
    //            }
    //        }
    //    }
    //    return Super::init();
    //}


    //TreeChildren::TreeChildren(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //int TreeChildren::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    for (size_t idx = 0; idx != getChildCount(); ++idx)
    //    {
    //        const Component * child = getChild(idx);
    //        if (const TreeItem * item = child->downcast<TreeItem>())
    //        {
    //            result += item->calculateHeight(inSizeConstraint);
    //        }
    //    }
    //    return result;
    //}


    //int TreeChildren::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    for (size_t idx = 0; idx != getChildCount(); ++idx)
    //    {
    //        const Component * child = getChild(idx);
    //        if (const TreeItem * item = child->downcast<TreeItem>())
    //        {
    //            int minWidth = item->calculateWidth(inSizeConstraint);
    //            if (result < minWidth)
    //            {
    //                result = minWidth;
    //            }
    //        }
    //    }
    //    return result;
    //}


    //TreeItem::TreeItem(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //int TreeItem::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    if (const TreeRow * row = findChildOfType<TreeRow>())
    //    {
    //        int minHeight = row->calculateHeight(inSizeConstraint);
    //        if (result < minHeight)
    //        {
    //            result = minHeight;
    //        }
    //    }
    //    if (const TreeChildren * treeChildren = findChildOfType<TreeChildren>())
    //    {
    //        result += treeChildren->calculateHeight(inSizeConstraint);
    //    }
    //    return result;
    //}


    //int TreeItem::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    if (const TreeRow * row = findChildOfType<TreeRow>())
    //    {
    //        int minWidth = row->calculateWidth(inSizeConstraint);
    //        if (result < minWidth)
    //        {
    //            result = minWidth;
    //        }
    //    }
    //    if (const TreeChildren * treeChildren = findChildOfType<TreeChildren>())
    //    {
    //        int minWidth = Defaults::treeIndent() + treeChildren->calculateWidth(inSizeConstraint);
    //        if (result < minWidth)
    //        {
    //            result = minWidth;
    //        }
    //    }
    //    return result;
    //}


    //bool TreeItem::isOpened() const
    //{
    //    //if (NativeComponent * comp = NativeControl::GetThisOrParent(const_cast<TreeItem*>(this)))
    //    //{
    //    //    TreeView_GetItemState(comp->handle(), hti, mask);
    //    //}
    //    return false; // TODO: implement
    //}


    //bool TreeItem::init()
    //{
    //    if (TreeRow * row = findChildOfType<TreeRow>())
    //    {
    //        if (TreeCell * cell = row->findChildOfType<TreeCell>())
    //        {
    //            std::string label = cell->getLabel();
    //            mItemInfo.setData(label);
    //        }
    //    }
    //    if (TreeChildren * treeChildren = findChildOfType<TreeChildren>())
    //    {
    //        for (size_t idx = 0; idx != treeChildren->getChildCount(); ++idx)
    //        {
    //            Component * child = treeChildren->getChild(idx);
    //            if (TreeItem * item = child->downcast<TreeItem>())
    //            {
    //                mItemInfo.addChild(&item->itemInfo());
    //            }
    //        }
    //    }
    //    return Super::init();
    //}


    //TreeCols::TreeCols(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //TreeCol::TreeCol(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //TreeRow::TreeRow(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //int TreeRow::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return calculateMaxChildWidth(inSizeConstraint);
    //}


    //int TreeRow::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    for (size_t idx = 0; idx != getChildCount(); ++idx)
    //    {
    //        const Component * child = getChild(idx);
    //        if (const TreeCell * cell = child->downcast<TreeCell>())
    //        {
    //            result += cell->calculateHeight(inSizeConstraint);
    //        }
    //    }
    //    return result;
    //}


    //TreeCell::TreeCell(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement)
    //{
    //}


    //bool TreeCell::initAttributeControllers()
    //{
    //    setAttributeController<LabelController>(this);
    //    return Super::initAttributeControllers();
    //}


    //std::string TreeCell::getLabel() const
    //{
    //    return mLabel;
    //}


    //void TreeCell::setLabel(const std::string & inLabel)
    //{
    //    mLabel = inLabel;
    //}


    //int TreeCell::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    int result = 0;
    //    if (NativeComponent * comp = NativeControl::GetThisOrParent(const_cast<TreeCell *>(this)))
    //    {
    //        result = Windows::getTextSize(comp->handle(), getLabel()).cx + Defaults::textPadding();
    //    }
    //    return result;
    //}


    //int TreeCell::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Defaults::treeItemHeight();
    //}


    //Statusbar::Statusbar(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent, inDOMElement, STATUSCLASSNAME, 0, SBARS_SIZEGRIP),
    //    mBoxLayouter(this)
    //{
    //}


    //bool Statusbar::initAttributeControllers()
    //{
    //    return Super::initAttributeControllers();
    //}


    //int Statusbar::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return calculateSumChildWidths(inSizeConstraint);
    //}


    //int Statusbar::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int ownHeight = Windows::getWindowHeight(handle());
    //    int maxChildHeight = calculateMaxChildHeight(inSizeConstraint);
    //    return std::max<int>(ownHeight, maxChildHeight);
    //}


    //Orient Statusbar::getOrient() const
    //{
    //    return Horizontal;
    //}


    //Align Statusbar::getAlign() const
    //{
    //    return Center;
    //}


    //Rect Statusbar::clientRect() const
    //{
    //    Rect clientRect(Super::clientRect());
    //    // Substract from width one square to make place for the resize gripper widget
    //    return Rect(clientRect.x(), clientRect.y(), clientRect.width() - Windows::getWindowHeight(handle()), clientRect.height());
    //}


    //void Statusbar::rebuildLayout()
    //{
    //    mBoxLayouter.rebuildLayout();
    //}


    //void Statusbar::rebuildChildLayouts()
    //{
    //    Super::rebuildChildLayouts();
    //}


    //StatusbarPanel::StatusbarPanel(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent, inDOMElement, TEXT("STATIC"), 0, 0)
    //{
    //}


    //bool StatusbarPanel::initAttributeControllers()
    //{
    //    return Super::initAttributeControllers();
    //}


    //int StatusbarPanel::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return Windows::getTextSize(handle(), getLabel()).cx;
    //}


    //int StatusbarPanel::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    return Windows::getTextSize(handle(), getLabel()).cy;
    //}


    //Toolbar::Toolbar(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    NativeControl(inParent, inDOMElement)
    //{
    //    if (NativeComponent * native = NativeControl::GetThisOrParent(inParent))
    //    {
    //        mToolbar.reset(new Windows::Toolbar(this, NativeComponent::GetModuleHandle(), native->handle(), mCommandId.intValue()));
    //        setHandle(mToolbar->handle(), false);
    //        registerHandle();
    //        subclass();
    //    }
    //}


    //Toolbar::~Toolbar()
    //{
    //    unsubclass();
    //    unregisterHandle();
    //    mToolbar.reset();
    //}


    //bool Toolbar::init()
    //{
    //    mToolbar->buildToolbar();
    //    mToolbar->rebuildLayout();
    //    ShowWindow(mToolbar->handle(), SW_SHOW);
    //    return Super::init();
    //}


    //int Toolbar::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    return calculateSumChildWidths(inSizeConstraint);
    //}


    //int Toolbar::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int result = calculateMaxChildHeight(inSizeConstraint);
    //    return result;
    //}


    //void Toolbar::rebuildLayout()
    //{
    //    mToolbar->rebuildLayout();
    //}


    //ToolbarButton::ToolbarButton(Component * inParent, Poco::XML::Element * inDOMElement) :
    //    PassiveComponent(inParent, inDOMElement),
    //    mButton(0),
    //    mDisabled(false)
    //{
    //    if (Toolbar * toolbar = parentNode()->downcast<Toolbar>())
    //    {
    //        boost::shared_ptr<Gdiplus::Bitmap> nullImage;
    //        std::string label = getLabel();

    //        std::string buttonType;
    //        AttributesMapping::const_iterator it = inDOMElement.find("type");
    //        if (it != inDOMElement.end())
    //        {
    //            buttonType = it->second;
    //        }
    //        //menu: Set the type attribute to the value menu to create a button with a menu popup. Place a menupopup element inside the button in this case. The user may click anywhere on the button to open and close the menu.
    //        //menu-button: You can also use the value menu-button to create a button with a menu. Unlike the menu type, this type requires the user to press the arrow to open the menu, but a different command may be invoked when the main part of the button is pressed. This type of button would be used for browser's back and forward buttons.
    //        //checkbox: Use this type to create a toggle button which will switch the checked state each time the button is pressed.
    //        //radio: Use this type to create a radio button. You can also create a group of toolbarbutton using this type and the attribute group.
    //        if (buttonType == "menu")
    //        {
    //            mButton = new Windows::ToolbarDropDown(toolbar->nativeToolbar(),
    //                                                   this,
    //                                                   mCommandId.intValue(),
    //                                                   label,
    //                                                   label,
    //                                                   nullImage,
    //                                                   false);
    //        }
    //        else if (buttonType == "menu-button")
    //        {
    //            mButton = new Windows::ToolbarDropDown(toolbar->nativeToolbar(),
    //                                                   this,
    //                                                   mCommandId.intValue(),
    //                                                   label,
    //                                                   label,
    //                                                   nullImage,
    //                                                   true);
    //        }
    //        else // buttonType.empty() or buttonType == "button"
    //        {
    //            mButton = new Windows::(toolbar->nativeToolbar(),
    //                                                        mCommandId.intValue(),
    //                                                        boost::function<void()>(),
    //                                                        label,
    //                                                        label,
    //                                                        nullImage);
    //        }
    //        assert(mButton);
    //        toolbar->nativeToolbar()->add(mButton);
    //        // Now that mButton is constructed we can apply any previously set
    //        // attributes.
    //        setLabel(mLabel);
    //        setDisabled(mDisabled);
    //        setCSSListStyleImage(mCSSListStyleImage);
    //    }
    //}


    //void ToolbarButton::showToolbarMenu(RECT inToolbarButtonRect)
    //{
    //    showPopupMenu(inToolbarButtonRect);
    //}


    //void ToolbarButton::showPopupMenu(RECT inToolbarButtonRect)
    //{
    //    for (size_t idx = 0; idx != getChildCount(); ++idx)
    //    {
    //        Poco::XML::Node * child = el()->childNodes()->item(idx);
    //        if (MenuPopup * popupMenu = child->downcast<MenuPopup>())
    //        {
    //            popupMenu->show(inToolbarButtonRect);
    //        }
    //    }
    //}


    //bool ToolbarButton::initAttributeControllers()
    //{
    //    setAttributeController<LabelController>(this);
    //    setAttributeController<DisabledController>(this);
    //    return Super::initAttributeControllers();
    //}


    //bool ToolbarButton::initStyleControllers()
    //{
    //    setStyleController<CSSListStyleImageController>(this);
    //    return Super::initStyleControllers();
    //}


    //int ToolbarButton::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    if (Toolbar * toolbar = parentNode()->downcast<Toolbar>())
    //    {
    //        int textWidth = Windows::getTextSize(toolbar->handle(), getLabel()).cx;
    //        int imageWidth = 0;
    //        if (mButton && mButton->image())
    //        {
    //            imageWidth = mButton->image()->GetWidth();
    //        }
    //        return std::max<int>(textWidth, imageWidth);

    //    }
    //    return 0;
    //}


    //int ToolbarButton::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    int result = Defaults::toolbarHeight();
    //    if (Toolbar * toolbar = parentNode()->downcast<Toolbar>())
    //    {
    //        int textHeight = Windows::getTextSize(toolbar->handle(), getLabel()).cy;
    //        if (textHeight > result)
    //        {
    //            result = textHeight;
    //        }
    //        if (mButton && mButton->image())
    //        {
    //            int imageHeight = mButton->image()->GetHeight();
    //            if (imageHeight > result)
    //            {
    //                result = imageHeight;
    //            }
    //        }
    //    }
    //    return result;
    //}


    //std::string ToolbarButton::getLabel() const
    //{
    //    if (mButton)
    //    {
    //        return mButton->text();
    //    }
    //    return mLabel;
    //}


    //void ToolbarButton::setLabel(const std::string & inLabel)
    //{
    //    if (mButton)
    //    {
    //        mButton->setText(inLabel);
    //    }
    //    mLabel = inLabel;
    //}


    //bool ToolbarButton::isDisabled() const
    //{
    //    return mDisabled;
    //}


    //void ToolbarButton::setDisabled(bool inDisabled)
    //{
    //    if (mButton)
    //    {
    //        if (Toolbar * toolbar = parentNode()->downcast<Toolbar>())
    //        {
    //            SendMessage(toolbar->handle(), TB_ENABLEBUTTON, (WPARAM)mCommandId.intValue(), (LPARAM)MAKELONG(inDisabled ? FALSE : TRUE, 0));
    //        }
    //    }
    //    else
    //    {
    //        mDisabled = inDisabled;
    //    }
    //}


    //void ToolbarButton::setCSSListStyleImage(const std::string & inURL)
    //{
    //    if (mButton)
    //    {
    //        ChromeURL chromeURL(inURL);
    //        std::wstring utf16URL = ToUTF16(chromeURL.convertToLocalPath());
    //        boost::shared_ptr<Gdiplus::Bitmap> img(new Gdiplus::Bitmap(utf16URL.c_str()));
    //        mButton->setImage(img);
    //    }
    //    mCSSListStyleImage = inURL;
    //}


    //const std::string & ToolbarButton::getCSSListStyleImage() const
    //{
    //    return mCSSListStyleImage;
    //}


} // namespace XULWin
