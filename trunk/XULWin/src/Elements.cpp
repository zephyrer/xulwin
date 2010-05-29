#include "XULWin/Element.h"
#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/Elements.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/ToolbarCustomWindowDecorator.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

    // We want the dialog to have a parent because an owning window is needed
    // in order to prevent our dialog from showing in the Windows taskbar.
    // MSDN Article ID 205158: How To Prevent a WindowElement from Appearing on the Taskbar
    static Window * GetDialogHelper()
    {
        AttributesMapping attr;
        static Window fDialogHelper(attr);
        return &fDialogHelper;
    }


    DialogElement::DialogElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(DialogElement::TagName(),
                inParent,
                new Dialog(inParent ? inParent->component() : GetDialogHelper(), inAttributesMapping))
    {
    }


    DialogResult DialogElement::showModal(WindowElement * inInvoker)
    {
        if (Dialog * nativeDialog = component()->downcast<Dialog>())
        {
            return nativeDialog->showModal(inInvoker->component()->downcast<Window>());
        }
        return DialogResult_Cancel;
    }


    void DialogElement::endModal(DialogResult inDialogResult)
    {
        if (Dialog * nativeDialog = component()->downcast<Dialog>())
        {
            nativeDialog->endModal(inDialogResult);
        }
    }


    ButtonElement::ButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ButtonElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Button>(inParent->component(), inAttributesMapping))
    {
    }


    DescriptionElement::DescriptionElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(DescriptionElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Description>(inParent->component(), inAttributesMapping))
    {
    }


    bool DescriptionElement::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    TextBoxElement::TextBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TextBoxElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, TextBox>(inParent->component(), inAttributesMapping))
    {
    }


    CheckBoxElement::CheckBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(CheckBoxElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, CheckBox>(inParent->component(), inAttributesMapping))
    {
    }


    BoxElement::BoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(BoxElement::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
    }


    HBoxElement::HBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(HBoxElement::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
        component()->setOrient(Horizontal);
    }


    VBoxElement::VBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(VBoxElement::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
        component()->setOrient(Vertical);
    }


    MenuListElement::MenuListElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuListElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, MenuList>(inParent->component(), inAttributesMapping))
    {
    }


    SeparatorElement::SeparatorElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SeparatorElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Separator>(inParent->component(), inAttributesMapping))
    {
    }


    SeparatorElement::~SeparatorElement()
    {
    }


    SpacerElement::SpacerElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SpacerElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Spacer>(inParent->component(), inAttributesMapping))
    {
    }


    SpacerElement::~SpacerElement()
    {
    }


    MenuButtonElement::MenuButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuButtonElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, MenuButton>(inParent->component(), inAttributesMapping))
    {
    }


    MenuButtonElement::~MenuButtonElement()
    {
    }


    GridElement::GridElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(GridElement::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualGrid, Grid>(inParent->component(), inAttributesMapping))
    {
    }


    GridElement::~GridElement()
    {
    }


    RowsElement::RowsElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(RowsElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Rows>(inParent->component(), inAttributesMapping))
    {
    }


    RowsElement::~RowsElement()
    {
    }


    ColumnsElement::ColumnsElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ColumnsElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Columns>(inParent->component(), inAttributesMapping))
    {
    }


    ColumnsElement::~ColumnsElement()
    {
    }


    RowElement::RowElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(RowElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Row>(inParent->component(), inAttributesMapping))
    {
    }


    RowElement::~RowElement()
    {
    }


    ColumnElement::ColumnElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ColumnElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Column>(inParent->component(), inAttributesMapping))
    {
    }


    ColumnElement::~ColumnElement()
    {
    }


    RadioGroupElement::RadioGroupElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(RadioGroupElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, RadioGroup>(inParent->component(), inAttributesMapping))
    {
    }


    RadioGroupElement::~RadioGroupElement()
    {
    }


    RadioElement::RadioElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(RadioElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Radio>(inParent->component(), inAttributesMapping))
    {
    }


    RadioElement::~RadioElement()
    {
    }


    ProgressMeterElement::ProgressMeterElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ProgressMeterElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, ProgressMeter>(inParent->component(), inAttributesMapping))
    {
    }


    ProgressMeterElement::~ProgressMeterElement()
    {
    }


    DeckElement::DeckElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(DeckElement::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, Deck, Deck>(inParent->component(), inAttributesMapping))
    {
    }


    DeckElement::~DeckElement()
    {
    }


    ScrollbarElement::ScrollbarElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ScrollbarElement::TagName(),
                inParent,
                new Decorator(new Scrollbar(inParent->component(), inAttributesMapping)))
    {
    }


    ScrollbarElement::~ScrollbarElement()
    {
    }


    TabBoxElement::TabBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabBoxElement::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
    }


    TabBoxElement::~TabBoxElement()
    {
    }


    TabsElement::TabsElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabsElement::TagName(),
                inParent,
                new Tabs(inParent->component(), inAttributesMapping))
    {
    }


    TabsElement::~TabsElement()
    {
    }


    TabElement::TabElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabElement::TagName(),
                inParent,
                new Tab(inParent->component(), inAttributesMapping))
    {
    }


    TabElement::~TabElement()
    {
    }


    TabPanelsElement::TabPanelsElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabPanelsElement::TagName(),
                inParent,
                new TabPanels(inParent->component(), inAttributesMapping))
    {
    }


    TabPanelsElement::~TabPanelsElement()
    {
    }


    TabPanelElement::TabPanelElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabPanelElement::TagName(),
                inParent,
                new TabPanel(inParent->component(), inAttributesMapping))
    {
    }


    TabPanelElement::~TabPanelElement()
    {
    }


    GroupBoxElement::GroupBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(GroupBoxElement::TagName(),
                inParent,
                new MarginDecorator(new GroupBox(inParent->component(), inAttributesMapping)))
    {
    }


    CaptionElement::CaptionElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(CaptionElement::TagName(),
                inParent,
                new Caption(inParent->component(), inAttributesMapping))
    {
    }


    TreeElement::TreeElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeElement::TagName(),
                inParent,
                new Tree(inParent->component(), inAttributesMapping))
    {
    }


    TreeElement::~TreeElement()
    {
    }


    TreeChildrenElement::TreeChildrenElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeChildrenElement::TagName(),
                inParent,
                new TreeChildren(inParent->component(), inAttributesMapping))
    {
    }


    TreeChildrenElement::~TreeChildrenElement()
    {
    }


    TreeItemElement::TreeItemElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeItemElement::TagName(),
                inParent,
                new TreeItem(inParent->component(), inAttributesMapping))
    {
    }


    TreeItemElement::~TreeItemElement()
    {
    }


    TreeColsElement::TreeColsElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeColsElement::TagName(),
                inParent,
                new TreeCols(inParent->component(), inAttributesMapping))
    {
    }


    TreeColsElement::~TreeColsElement()
    {
    }


    TreeColElement::TreeColElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeColElement::TagName(),
                inParent,
                new TreeCol(inParent->component(), inAttributesMapping))
    {
    }


    TreeColElement::~TreeColElement()
    {
    }


    TreeRowElement::TreeRowElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeRowElement::TagName(),
                inParent,
                new TreeRow(inParent->component(), inAttributesMapping))
    {
    }


    TreeRowElement::~TreeRowElement()
    {
    }


    TreeCellElement::TreeCellElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeCellElement::TagName(),
                inParent,
                new TreeCell(inParent->component(), inAttributesMapping))
    {
    }


    TreeCellElement::~TreeCellElement()
    {
    }


    StatusbarElement::StatusbarElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(StatusbarElement::TagName(),
                inParent,
                new Statusbar(inParent->component(), inAttributesMapping))
    {
    }


    StatusbarElement::~StatusbarElement()
    {
    }


    StatusbarPanelElement::StatusbarPanelElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(StatusbarPanelElement::TagName(),
                inParent,
                new StatusbarPanel(inParent->component(), inAttributesMapping))
    {
    }


    StatusbarPanelElement::~StatusbarPanelElement()
    {
    }


    ToolbarElement::ToolbarElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ToolbarElement::TagName(),
                inParent,
                new Toolbar(inParent->component(), inAttributesMapping))
    {
    }


    ToolbarElement::~ToolbarElement()
    {
    }


    ToolbarButtonElement::ToolbarButtonElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ToolbarButtonElement::TagName(),
                inParent,
                new ToolbarButton(inParent->component(), inAttributesMapping))
    {
    }


    ToolbarButtonElement::~ToolbarButtonElement()
    {
    }


} // namespace XULWin
