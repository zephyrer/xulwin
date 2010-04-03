#include "XULWin/ListBoxElement.h"
#include "XULWin/ListColsElement.h"
#include "XULWin/ListItemElement.h"
#include "XULWin/ListBox.h"
#include "XULWin/ListView.h"
#include "XULWin/Proxy.h"


namespace XULWin
{

    // At this point we don't know yet whether to make a listbox or a listview.
    // Both have distinct window classnames in the Windows API, but not in XUL.
    // So we need to delay the instantiation. However, we still must have an
    // component, so we use temporarily use a PassiveComponent object.
    ListBoxElement::ListBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListBoxElement::Type(),
                inParent,
                new Proxy(new PassiveComponent(inParent->component(), inAttributesMapping)))
    {
    }


    void ListBoxElement::addChild(ElementPtr inChild)
    {
        // The first child should give us the needed information to know
        // whether we need to create a ListBox or a ListView.
        set(inChild->type());
        Element::addChild(inChild);
    }


    void ListBoxElement::set(const std::string & inType)
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
                    ListBox * listBox = new ListBox(parent()->component(), mAttributes);
                    ComponentPtr prev = proxy->swap(new MarginDecorator(listBox));
                    listBox->init();

                    // Up until this point all the move() calls were directed
                    // to the PassiveComponent object. Now we need to re-apply
                    // this on the native component.
                    proxy->move(prev->clientRect());
                }
                else
                {
                    ListView * listView = new ListView(parent()->component(), mAttributes);
                    ComponentPtr prev = proxy->swap(new MarginDecorator(listView));
                    listView->init();

                    // Up until this point all the move() calls were directed
                    // to the PassiveComponent object. Now we need to re-apply
                    // this on the native component.
                    proxy->move(prev->clientRect());
                }
            }
        }
    }


    bool ListBoxElement::init()
    {
        return Element::init();
    }

} // namespace XULWin
