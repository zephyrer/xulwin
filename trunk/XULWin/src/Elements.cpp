#include "XULWin/Element.h"
#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/Dialog.h"
#include "XULWin/Elements.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Toolbar.h"
#include "XULWin/ToolbarItem.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

    // We want the dialog to have a parent because an owning window is needed
    // in order to prevent our dialog from showing in the Windows taskbar.
    // MSDN Article ID 205158: How To Prevent a XMLWindow from Appearing on the Taskbar
    static Window * GetDialogHelper()
    {
        AttributesMapping attr;
        static Window fDialogHelper(attr);
        return &fDialogHelper;
    }


    XMLDialog::XMLDialog(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLDialog::TagName(),
                inParent,
                new Dialog(inParent ? inParent->component() : GetDialogHelper(), inAttributesMapping))
    {
    }


    DialogResult XMLDialog::showModal(XMLWindow * inInvoker)
    {
        if (Dialog * nativeDialog = component()->downcast<Dialog>())
        {
            return nativeDialog->showModal(inInvoker->component()->downcast<Window>());
        }
        return DialogResult_Cancel;
    }


    void XMLDialog::endModal(DialogResult inDialogResult)
    {
        if (Dialog * nativeDialog = component()->downcast<Dialog>())
        {
            nativeDialog->endModal(inDialogResult);
        }
    }


    XMLButton::XMLButton(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLButton::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Button>(inParent->component(), inAttributesMapping))
    {
    }


    XMLDescription::XMLDescription(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLDescription::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Description>(inParent->component(), inAttributesMapping))
    {
    }


    bool XMLDescription::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    XMLTextBox::XMLTextBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTextBox::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, TextBox>(inParent->component(), inAttributesMapping))
    {
    }


    XMLCheckBox::XMLCheckBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLCheckBox::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, CheckBox>(inParent->component(), inAttributesMapping))
    {
    }


    XMLBox::XMLBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLBox::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
    }


    XMLHBox::XMLHBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLHBox::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
        component()->setOrient(Horizontal);
    }


    XMLVBox::XMLVBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLVBox::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
        component()->setOrient(Vertical);
    }


    XMLMenuList::XMLMenuList(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLMenuList::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, MenuList>(inParent->component(), inAttributesMapping))
    {
    }


    XMLSeparator::XMLSeparator(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSeparator::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Separator>(inParent->component(), inAttributesMapping))
    {
    }


    XMLSeparator::~XMLSeparator()
    {
    }


    XMLSpacer::XMLSpacer(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSpacer::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Spacer>(inParent->component(), inAttributesMapping))
    {
    }


    XMLSpacer::~XMLSpacer()
    {
    }


    XMLMenuButton::XMLMenuButton(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLMenuButton::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, MenuButton>(inParent->component(), inAttributesMapping))
    {
    }


    XMLMenuButton::~XMLMenuButton()
    {
    }


    XMLGrid::XMLGrid(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLGrid::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualGrid, Grid>(inParent->component(), inAttributesMapping))
    {
    }


    XMLGrid::~XMLGrid()
    {
    }


    XMLRows::XMLRows(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLRows::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Rows>(inParent->component(), inAttributesMapping))
    {
    }


    XMLRows::~XMLRows()
    {
    }


    XMLColumns::XMLColumns(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLColumns::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Columns>(inParent->component(), inAttributesMapping))
    {
    }


    XMLColumns::~XMLColumns()
    {
    }


    XMLRow::XMLRow(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLRow::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Row>(inParent->component(), inAttributesMapping))
    {
    }


    XMLRow::~XMLRow()
    {
    }


    XMLColumn::XMLColumn(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLColumn::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Column>(inParent->component(), inAttributesMapping))
    {
    }


    XMLColumn::~XMLColumn()
    {
    }


    XMLRadioGroup::XMLRadioGroup(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLRadioGroup::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, RadioGroup>(inParent->component(), inAttributesMapping))
    {
    }


    XMLRadioGroup::~XMLRadioGroup()
    {
    }


    XMLRadio::XMLRadio(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLRadio::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Radio>(inParent->component(), inAttributesMapping))
    {
    }


    XMLRadio::~XMLRadio()
    {
    }


    XMLProgressMeter::XMLProgressMeter(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLProgressMeter::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, ProgressMeter>(inParent->component(), inAttributesMapping))
    {
    }


    XMLProgressMeter::~XMLProgressMeter()
    {
    }


    XMLDeck::XMLDeck(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLDeck::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, Deck, Deck>(inParent->component(), inAttributesMapping))
    {
    }


    XMLDeck::~XMLDeck()
    {
    }


    XMLScrollbar::XMLScrollbar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLScrollbar::TagName(),
                inParent,
                new Decorator(new Scrollbar(inParent->component(), inAttributesMapping)))
    {
    }


    XMLScrollbar::~XMLScrollbar()
    {
    }


    XMLTabBox::XMLTabBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTabBox::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttributesMapping))
    {
    }


    XMLTabBox::~XMLTabBox()
    {
    }


    XMLTabs::XMLTabs(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTabs::TagName(),
                inParent,
                new Tabs(inParent->component(), inAttributesMapping))
    {
    }


    XMLTabs::~XMLTabs()
    {
    }


    XMLTab::XMLTab(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTab::TagName(),
                inParent,
                new Tab(inParent->component(), inAttributesMapping))
    {
    }


    XMLTab::~XMLTab()
    {
    }


    XMLTabPanels::XMLTabPanels(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTabPanels::TagName(),
                inParent,
                new TabPanels(inParent->component(), inAttributesMapping))
    {
    }


    XMLTabPanels::~XMLTabPanels()
    {
    }


    XMLTabPanel::XMLTabPanel(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTabPanel::TagName(),
                inParent,
                new TabPanel(inParent->component(), inAttributesMapping))
    {
    }


    XMLTabPanel::~XMLTabPanel()
    {
    }


    XMLGroupBox::XMLGroupBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLGroupBox::TagName(),
                inParent,
                new MarginDecorator(new GroupBox(inParent->component(), inAttributesMapping)))
    {
    }


    XMLCaption::XMLCaption(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLCaption::TagName(),
                inParent,
                new Caption(inParent->component(), inAttributesMapping))
    {
    }


    XMLTree::XMLTree(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTree::TagName(),
                inParent,
                new Tree(inParent->component(), inAttributesMapping))
    {
    }


    XMLTree::~XMLTree()
    {
    }


    XMLTreeChildren::XMLTreeChildren(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTreeChildren::TagName(),
                inParent,
                new TreeChildren(inParent->component(), inAttributesMapping))
    {
    }


    XMLTreeChildren::~XMLTreeChildren()
    {
    }


    XMLTreeItem::XMLTreeItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTreeItem::TagName(),
                inParent,
                new TreeItem(inParent->component(), inAttributesMapping))
    {
    }


    XMLTreeItem::~XMLTreeItem()
    {
    }


    XMLTreeCols::XMLTreeCols(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTreeCols::TagName(),
                inParent,
                new TreeCols(inParent->component(), inAttributesMapping))
    {
    }


    XMLTreeCols::~XMLTreeCols()
    {
    }


    XMLTreeCol::XMLTreeCol(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTreeCol::TagName(),
                inParent,
                new TreeCol(inParent->component(), inAttributesMapping))
    {
    }


    XMLTreeCol::~XMLTreeCol()
    {
    }


    XMLTreeRow::XMLTreeRow(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTreeRow::TagName(),
                inParent,
                new TreeRow(inParent->component(), inAttributesMapping))
    {
    }


    XMLTreeRow::~XMLTreeRow()
    {
    }


    XMLTreeCell::XMLTreeCell(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLTreeCell::TagName(),
                inParent,
                new TreeCell(inParent->component(), inAttributesMapping))
    {
    }


    XMLTreeCell::~XMLTreeCell()
    {
    }


    XMLStatusbar::XMLStatusbar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLStatusbar::TagName(),
                inParent,
                new Statusbar(inParent->component(), inAttributesMapping))
    {
    }


    XMLStatusbar::~XMLStatusbar()
    {
    }


    XMLStatusbarPanel::XMLStatusbarPanel(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLStatusbarPanel::TagName(),
                inParent,
                new StatusbarPanel(inParent->component(), inAttributesMapping))
    {
    }


    XMLStatusbarPanel::~XMLStatusbarPanel()
    {
    }


    XMLToolbar::XMLToolbar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLToolbar::TagName(),
                inParent,
                new XULWin::Toolbar(inParent->component(), inAttributesMapping))
    {
    }


    XMLToolbar::~XMLToolbar()
    {
    }


    XMLToolbarButton::XMLToolbarButton(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLToolbarButton::TagName(),
                inParent,
                new ToolbarButton(inParent->component(), inAttributesMapping))
    {
    }


    XMLToolbarButton::~XMLToolbarButton()
    {
    }


} // namespace XULWin
