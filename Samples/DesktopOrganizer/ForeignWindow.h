#ifndef FOREIGNWINDOW_H_INCLUDED
#define FOREIGNWINDOW_H_INCLUDED


#include "XULWin/AttributeController.h"
#include "XULWin/Element.h"
#include "XULWin/NativeComponent.h"
#include <string>


namespace XULWin
{

    class XMLForeignWindow : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<XMLForeignWindow>(inParent, inAttr); }

        static const char * TagName()
        { return "foreignwindow"; }

    private:
        friend class Element;
        XMLForeignWindow(Element * inParent, const AttributesMapping & inAttr);
    };



    /**
     * Attribute: classname
     * Type: string
     * Purpose: The class name created by a call to the RegisterClass or RegisterClass
     */
    class ClassNameController : public AttributeController
    {
    public:
        static const char * AttributeName()
        { return "classname"; }

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual std::string getClassName() const = 0;

        virtual void setClassName(const std::string & inClassName) = 0;
    };    


    class ForeignWindowInfo
    {
    public:
        ForeignWindowInfo(const std::string & inClassName,
                          const std::string  inTitle,
                          const Rect & inRect,
                          WINDOWPLACEMENT inWINDOWPLACEMENT) :
            mClassName(inClassName),
            mTitle(inTitle),
            mRect(inRect),
            mWINDOWPLACEMENT(inWINDOWPLACEMENT)
        {
        }
            
        ForeignWindowInfo(HWND inHWND);

        inline const std::string & className() const
        { return mClassName; }

        inline const std::string & title() const
        { return mTitle; }

        inline const Rect & rect() const
        { return mRect; }

        inline const WINDOWPLACEMENT & windowPlacement() const
        { return mWINDOWPLACEMENT; }

    private:
        std::string mClassName;
        std::string mTitle;
        Rect mRect;
        WINDOWPLACEMENT mWINDOWPLACEMENT;
    };


    class ForeignWindow : public NativeComponent,                          
                          public virtual ClassNameController,
                          public virtual TitleController
    {
    public:
        typedef NativeComponent Super;
        ForeignWindow(const AttributesMapping & inAttr);

        virtual ~ForeignWindow();

        virtual bool init();

        virtual bool initAttributeControllers();

        // TitleController methods
        virtual std::string getTitle() const;

        virtual void setTitle(const std::string & inTitle);

        // ClassNameController methods
        virtual std::string getClassName() const;

        virtual void setClassName(const std::string & inClassName);

        virtual void rebuildLayout();

        virtual void move(int x, int y, int w, int h);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual Rect clientRect() const;

        virtual Rect windowRect() const;

    private:
        std::string mClassName;
    };


} // namespace XULWin


#endif // FOREIGNWINDOW_H_INCLUDED
