#include "XULWin/ListBox.h"
#include "XULWin/ListCols.h"
#include "XULWin/ListBoxImpl.h"
#include "XULWin/ListViewImpl.h"
#include "XULWin/Proxy.h"


namespace XULWin
{

    // At this point we don't know yet whether to make a listbox or a listview.
    // Both have distinct window classnames in the WinAPI.
    // We'll have to delay creation for the init() method.
    // However, at this point we still need to have an impl, so we use
    // temporarily use a PassiveComonent object.
    ListBox::ListBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListBox::Type(),
                inParent,
                new Proxy(new PassiveComponent(inParent->impl(), inAttributesMapping)))
    {
    }


    void ListBox::ensureNativeImpl()
    {        
        // The decorator is used as a proxy here.
        if (Proxy * proxy = impl()->downcast<Proxy>())
        {
            // If we have a <listcols> element, then we are a ListView
            if (findChildOfType<ListCols>())
            {   
                ListViewImpl * listView = new ListViewImpl(parent()->impl(), mAttributes);
                proxy->swap(new MarginDecorator(listView));
            }
            else
            {
                ListBoxImpl * listBox = new ListBoxImpl(parent()->impl(), mAttributes);
                proxy->swap(new MarginDecorator(listBox));
            }
        }
    }


    bool ListBox::init()
    {
        ensureNativeImpl();
        return Element::init();
    }

} // namespace XULWin
