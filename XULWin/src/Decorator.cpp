#include "XULWin/Decorator.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Conversions.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/Elements.h"
#include "XULWin/Types.h"
#include "XULWin/XMLWindow.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Gdiplus.h"
#include "Poco/Path.h"
#include "Poco/UnicodeConverter.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>


namespace XULWin
{
    using namespace WinAPI;

    Decorator::Decorator(Component * inDecoratedComponent) :
        mDecoratedComponent(inDecoratedComponent)
    {
        assert(mDecoratedComponent);
    }


    Decorator::Decorator(ComponentPtr inDecoratedComponent) :
        mDecoratedComponent(inDecoratedComponent)
    {
        assert(mDecoratedComponent);
    }


    Decorator::~Decorator()
    {
    }


    bool Decorator::init()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->init();
        }
        return false;
    }


    ComponentPtr Decorator::swap(ComponentPtr inNewElement)
    {
        assert(inNewElement);
        ComponentPtr old = mDecoratedComponent;
        mDecoratedComponent = inNewElement;
        assert(mDecoratedComponent);
        mDecoratedComponent->setOwningElement(old->el());
        return old;
    }


    ComponentPtr Decorator::swap(Component * inNewElement)
    {
        assert(inNewElement);
        ComponentPtr old = mDecoratedComponent;
        mDecoratedComponent.reset(inNewElement);
        assert(mDecoratedComponent);
        mDecoratedComponent->setOwningElement(old->el());
        return old;
    }

        
    Window * Decorator::findParentWindow()
    {
        return mDecoratedComponent->findParentWindow();
    }

        
    const Window * Decorator::findParentWindow() const
    {
        return mDecoratedComponent->findParentWindow();
    }


    int Decorator::getIndex() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getIndex();
        }
        return 0;
    }


    size_t Decorator::getChildCount() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getChildCount();
        }
        return 0;
    }


    const Component * Decorator::getChild(size_t inIndex) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getChild(inIndex);
        }
        return 0;
    }


    Component * Decorator::getChild(size_t inIndex)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getChild(inIndex);
        }
        return 0;
    }


    void Decorator::invalidateRect() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->invalidateRect();
        }
    }


    int Decorator::getWidth() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getWidth();
        }
        return 0;
    }


    void Decorator::setWidth(int inWidth)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setWidth(inWidth);
        }
    }


    int Decorator::getHeight() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getHeight();
        }
        return 0;
    }


    void Decorator::setHeight(int inHeight)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setHeight(inHeight);
        }
    }


    int Decorator::getScreenX() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getScreenX();
        }
        return 0;
    }


    void Decorator::setScreenX(int inX)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setScreenX(inX);
        }
    }


    int Decorator::getScreenY() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getScreenY();
        }
        return 0;
    }


    void Decorator::setScreenY(int inY)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setScreenY(inY);
        }
    }


    void Decorator::setSVGStroke(const RGBColor & inColor)
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    mDecoratedComponent->setSVGStroke(inColor);
        //}
    }


    const RGBColor & Decorator::getSVGStroke() const
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    return mDecoratedComponent->getSVGStroke();
        //}
        static RGBColor fDefault;
        return fDefault;
    }


    void Decorator::setSVGStrokeWidth(int inStrokeWidth)
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    return mDecoratedComponent->setSVGStrokeWidth(inStrokeWidth);
        //}
    }


    int Decorator::getSVGStrokeWidth() const
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    return mDecoratedComponent->getSVGStrokeWidth();
        //}
        return 0;
    }


    void Decorator::setSVGFill(const RGBColor & inColor)
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    mDecoratedComponent->setSVGFill(inColor);
        //}
    }


    const RGBColor & Decorator::getSVGFill() const
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    mDecoratedComponent->getSVGFill();
        //}
        static RGBColor fDefault;
        return fDefault;
    }


    int Decorator::getFlex() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getFlex();
        }
        return 0;
    }


    void Decorator::setFlex(int inFlex)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setFlex(inFlex);
        }
    }


    int Decorator::getCSSWidth() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSWidth();
        }
        return 0;
    }


    void Decorator::setCSSWidth(int inWidth)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSWidth(inWidth);
        }
    }


    int Decorator::getCSSHeight() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSHeight();
        }
        return 0;
    }


    void Decorator::setCSSHeight(int inHeight)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSHeight(inHeight);
        }
    }


    void Decorator::setCSSBackgroundColor(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSBackgroundColor(inColor);
        }
    }


    RGBColor Decorator::getCSSBackgroundColor() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSBackgroundColor();
        }
        RGBColor fallback;
        (0, 0, 0);
        return fallback;
    }


    void Decorator::setCSSColor(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSColor(inColor);
        }
    }


    RGBColor Decorator::getCSSColor() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSColor();
        }
        RGBColor fallback;
        (0, 0, 0);
        return fallback;
    }


    void Decorator::getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSMargin(outTop, outLeft, outRight, outBottom);
        }
    }


    void Decorator::setCSSMargin(int inTop, int inLeft, int inRight, int inBottom)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSMargin(inTop, inLeft, inRight, inBottom);
        }
    }


    void Decorator::setCSSFill(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSFill(inColor);
        }
    }


    const RGBColor & Decorator::getCSSFill() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSFill();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    void Decorator::setCSSStroke(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSStroke(inColor);
        }
    }


    const RGBColor & Decorator::getCSSStroke() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSStroke();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    int Decorator::getCSSX() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSX();
        }
        return 0;
    }


    void Decorator::setCSSX(int inX)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSX(inX);
        }
    }


    int Decorator::getCSSY() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSY();
        }
        return 0;
    }


    void Decorator::setCSSY(int inY)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSY(inY);
        }
    }


    bool Decorator::isHidden() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->isHidden();
        }
        return false;
    }


    void Decorator::setHidden(bool inHidden)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setHidden(inHidden);
        }
    }


    Orient Decorator::getOrient() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getOrient();
        }
        return Vertical;
    }


    void Decorator::setOrient(Orient inOrient)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setOrient(inOrient);
        }
    }


    Align Decorator::getAlign() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getAlign();
        }
        return Start;
    }


    void Decorator::setAlign(Align inAlign)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setAlign(inAlign);
        }
    }


    UInt32 Decorator::componentId() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->componentId();
        }
        return 0;
    }


    int Decorator::getWidth(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getWidth(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::getHeight(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getHeight(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->calculateWidth(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->calculateHeight(inSizeConstraint);
        }
        return 0;
    }


    bool Decorator::expansive() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->expansive();
        }
        return false;
    }


    void Decorator::move(int x, int y, int w, int h)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->move(x, y, w, h);
        }
    }


    void Decorator::move(const Rect & inRect)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->move(inRect);
        }
    }


    Rect Decorator::clientRect() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->clientRect();
        }
        Rect defaultRect;
        return defaultRect;
    }


    void Decorator::setOwningElement(Element * inElement)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setOwningElement(inElement);
        }
    }


    Element * Decorator::el() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->el();
        }
        return 0;
    }


    Component * Decorator::parent() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->parent();
        }
        return 0;
    }


    void Decorator::rebuildLayout()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->rebuildLayout();
        }
    }


    void Decorator::rebuildChildLayouts()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->rebuildChildLayouts();
        }
    }


    bool Decorator::getAttribute(const std::string & inName, std::string & outValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getAttribute(inName, outValue);
        }
        return false;
    }


    bool Decorator::getStyle(const std::string & inName, std::string & outValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getStyle(inName, outValue);
        }
        return false;
    }


    bool Decorator::setStyle(const std::string & inName, const std::string & inValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setStyle(inName, inValue);
        }
        return false;
    }


    bool Decorator::setAttribute(const std::string & inName, const std::string & inValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setAttribute(inName, inValue);
        }
        return false;
    }


    bool Decorator::initAttributeControllers()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->initAttributeControllers();
        }
        return false;
    }


    bool Decorator::initStyleControllers()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->initStyleControllers();
        }
        return false;
    }


    ComponentPtr Decorator::decoratedElement() const
    {
        assert(mDecoratedComponent);
        return mDecoratedComponent;
    }


    void Decorator::setDecoratedComponent(ComponentPtr inElement)
    {
        mDecoratedComponent = inElement;
        assert(mDecoratedComponent);
    }


    void Decorator::setDecoratedComponent(Component * inElement)
    {
        mDecoratedComponent.reset(inElement);
        assert(mDecoratedComponent);
    }


    void Decorator::onChildAdded(Component * inChild)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->onChildAdded(inChild);
        }
    }


    void Decorator::onChildRemoved(Component * inChild)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->onChildRemoved(inChild);
        }
    }


    void Decorator::onContentChanged()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->onContentChanged();
        }
    }


    LRESULT Decorator::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->handleMessage(inMessage, wParam, lParam);
        }
        return FALSE;
    }

} // namespace XULWin
