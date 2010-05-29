#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED


#include "XULWin/NativeControl.h"


namespace XULWin
{

    class TextBox : public NativeControl,
                    public virtual StringValueController,
                    public virtual ReadOnlyController,
                    public virtual RowsController
    {
    public:
        typedef NativeControl Super;

        TextBox(Component * inParent, const AttributesMapping & inAttr);

        // StringValueController methods
        virtual std::string getValue() const;

        virtual void setValue(const std::string & inStringValue);

        // ReadOnlyController methods
        virtual bool isReadOnly() const;

        virtual void setReadOnly(bool inReadOnly);

        // RowsControll methods
        virtual int getRows() const;

        virtual void setRows(int inRows);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        int mRows;
        static DWORD GetFlags(const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // TEXTBOX_H_INCLUDED
