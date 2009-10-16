#ifndef LISTBOXIMPL_H_INCLUDED
#define LISTBOXIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class ListBoxImpl : public NativeControl,
                        public RowsController
    {
    public:
        typedef NativeControl Super;

        ListBoxImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        virtual bool initAttributeControllers();

        virtual int getRows() const;

        virtual void setRows(int inRows);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded();

    private:
        Fallible<int> mRows;
    };

} // namespace XULWin


#endif // LISTBOXIMPL_H_INCLUDED
