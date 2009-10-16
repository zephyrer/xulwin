#include "XULWin/ListBox.h"
#include "XULWin/ListCols.h"
#include "XULWin/ListBoxImpl.h"
#include "XULWin/ListViewImpl.h"
#include "XULWin/Proxy.h"


namespace XULWin
{

    // At this point we don't know yet whether to make a listbox or a listview.
    // Both have distinct window classnames in the WinAPI, but not in XUL.
    // So we need to delay the instantiation. However, we still must have an
    // impl, so we use temporarily use a PassiveComonent object.
    ListBox::ListBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListBox::Type(),
                inParent,
                new Proxy(new PassiveComponent(inParent->impl(), inAttributesMapping)))
    {
    }

        
    void ListBox::addChild(ElementPtr inChild)
    {
		// We need a native instance now or otherwise we
		// can't add the child natively
        setImpl(inChild->type());
        Element::addChild(inChild);
    }


    void ListBox::setImpl(const std::string & inType)
    {
        // The decorator is used as a proxy here.
        if (Proxy * proxy = impl()->downcast<Proxy>())
        {
            if (proxy->downcast<PassiveComponent>())
            {                
                // If the first child is of type "listitem" then can be certain
                // that it is a regular listbox
                if (inType == "listitem")
                {   
                    ListBoxImpl * listBox = new ListBoxImpl(parent()->impl(), mAttributes);
                    ElementImplPtr prev = proxy->swap(new MarginDecorator(listBox));
                    listBox->initImpl();
                }
                else
                {
                    ListViewImpl * listView = new ListViewImpl(parent()->impl(), mAttributes);
                    ElementImplPtr prev = proxy->swap(new MarginDecorator(listView));
                    listView->initImpl();
                }
            }
        }
    }


    bool ListBox::init()
    {
        return Element::init();
    }

} // namespace XULWin
