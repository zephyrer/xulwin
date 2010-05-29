#include "XULWin/XMLListBox.h"
#include "XULWin/ListBox.h"
#include "XULWin/ListView.h"
#include "XULWin/PhonyComponent.h"
#include "XULWin/Proxy.h"


namespace XULWin
{

    // At this point we don't know yet whether to make a listbox or a listview.
    // Both have distinct window classnames in the Windows API, but not in XUL.
    // So we need to delay the instantiation. However, we still must have an
    // component, so we use temporarily use a PhonyComponent object.
    XMLListBox::XMLListBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLListBox::TagName(),
                inParent,
                new Proxy(new PhonyComponent(inParent->component(), inAttributesMapping)))
    {
    }


    void XMLListBox::addChild(ElementPtr inChild)
    {
        // The first child should give us the needed information to know
        // whether we need to create a ListBox or a ListView.
        set(inChild->tagName());
        Element::addChild(inChild);
    }


    void XMLListBox::set(const std::string & inType)
    {
        // The decorator is used as a proxy here.
        if (Proxy * proxy = component()->downcast<Proxy>())
        {
            // Check if component is still of type PhonyComponent.
            if (proxy->downcast<PhonyComponent>())
            {
                // If the first child is of type "listitem" then can be certain
                // that it is a regular listbox
                if (inType == "listitem")
                {
                    ListBox * listBox = new ListBox(parent()->component(), mAttributes);
                    ComponentPtr prev = proxy->swap(new MarginDecorator(listBox));
                    listBox->init();

                    // Up until this point all the move() calls were directed
                    // to the PhonyComponent object. Now we need to re-apply
                    // this on the native component.
                    proxy->move(prev->clientRect());
                }
                else
                {
                    ListView * listView = new ListView(parent()->component(), mAttributes);
                    ComponentPtr prev = proxy->swap(new MarginDecorator(listView));
                    listView->init();

                    // Up until this point all the move() calls were directed
                    // to the PhonyComponent object. Now we need to re-apply
                    // this on the native component.
                    proxy->move(prev->clientRect());
                }
            }
        }
    }


    bool XMLListBox::init()
    {
        return Element::init();
    }

} // namespace XULWin
