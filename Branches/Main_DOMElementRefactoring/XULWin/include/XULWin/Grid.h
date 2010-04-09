#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Decorator.h"


namespace XULWin
{


    class Grid : public Decorator
    {
    public:
        typedef Decorator Super;

        static const char * TagName() { return "grid"; }

        static ComponentPtr Create(Component * inParentComponent, Poco::XML::Element * inDOMElement);

        Grid(Component * inDecoratedComponent);
    };


    class Rows : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        static const char * TagName() { return "rows"; }

        static ComponentPtr Create(Component * inParentComponent, Poco::XML::Element * inDOMElement)
        { return Component::Create<Rows>(inParentComponent, inDOMElement); }

        Rows(Component * inParent, Poco::XML::Element * inDOMElement);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Row : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        static const char * TagName() { return "row"; }

        static ComponentPtr Create(Component * inParentComponent, Poco::XML::Element * inDOMElement)
        { return Component::Create<Row>(inParentComponent, inDOMElement); }

        Row(Component * inParent, Poco::XML::Element * inDOMElement);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Columns : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        static const char * TagName() { return "columns"; }

        static ComponentPtr Create(Component * inParentComponent, Poco::XML::Element * inDOMElement)
        { return Component::Create<Columns>(inParentComponent, inDOMElement); }

        Columns(Component * inParent, Poco::XML::Element * inDOMElement);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Column : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        static const char * TagName() { return "column"; }

        static ComponentPtr Create(Component * inParentComponent, Poco::XML::Element * inDOMElement)
        { return Component::Create<Column>(inParentComponent, inDOMElement); }

        Column(Component * inParent, Poco::XML::Element * inDOMElement);

        virtual Align getAlign() const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // GRID_H_INCLUDED
