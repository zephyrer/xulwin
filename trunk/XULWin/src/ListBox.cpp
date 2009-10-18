#include "XULWin/ListBox.h"
#include "XULWin/ListCols.h"
#include "XULWin/ListBoxImpl.h"
#include "XULWin/ListViewImpl.h"
#include "XULWin/Proxy.h"


namespace XULWin
{

    // At this point we don't know yet whether to make a listbox or a listview.
    // Both have distinct window classnames in the Windows API, but not in XUL.
    // So we need to delay the instantiation. However, we still must have an
    // component, so we use temporarily use a PassiveComonent object.
    ListBox::ListBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListBox::Type(),
                inParent,
                new Proxy(new PassiveComponent(inParent->component(), inAttributesMapping)))
    {
    }

        
    void ListBox::addChild(ElementPtr inChild)
    {
        // The first child should give us the needed information to know
        // whether we need to create a ListBoxImpl or a ListViewImpl.
        setImpl(inChild->type());
        Element::addChild(inChild);
    }


    void ListBox::setImpl(const std::string & inType)
    {
        // The decorator is used as a proxy here.
        if (Proxy * proxy = component()->downcast<Proxy>())
        {
            // Check if component is still of type PassiveComponent.
            if (proxy->downcast<PassiveComponent>())
            {                
                // If the first child is of type "listitem" then can be certain
                // that it is a regular listbox
                if (inType == "listitem")
                {   
                    ListBoxImpl * listBox = new ListBoxImpl(parent()->component(), mAttributes);
                    ElementImplPtr prev = proxy->swap(new MarginDecorator(listBox));
                    listBox->initComponent();

					// Up until this point all the move() calls were directed
                    // to the PassiveComponent object. Now we need to re-apply
                    // this on the native component.
                    proxy->move(prev->clientRect());
                }
                else
                {
                    ListViewImpl * listView = new ListViewImpl(parent()->component(), mAttributes);
                    ElementImplPtr prev = proxy->swap(new MarginDecorator(listView));
                    listView->initComponent();

					// Up until this point all the move() calls were directed
                    // to the PassiveComponent object. Now we need to re-apply
                    // this on the native component.
                    proxy->move(prev->clientRect());
                }
            }
        }
    }


    bool ListBox::init()
    {
        return Element::init();
    }

} // namespace XULWin
