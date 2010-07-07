#include "XULWin/Element.h"
#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/Decorators.h"
#include "XULWin/Dialog.h"
#include "XULWin/Elements.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

    // We want the dialog to have a parent because an owning window is needed
    // in order to prevent our dialog from showing in the WinAPI taskbar.
    // MSDN Article ID 205158: How To Prevent a XMLWindow from Appearing on the Taskbar
    static Window * GetDialogHelper()
    {
        AttributesMapping attr;
        static Window fDialogHelper(attr);
        return &fDialogHelper;
    }


    XMLDialog::XMLDialog(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLDialog::TagName(),
                inParent,
                new Dialog(inParent ? inParent->component() : GetDialogHelper(), inAttr))
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


    XMLDescription::XMLDescription(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLDescription::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Description>(inParent->component(), inAttr))
    {
    }


    bool XMLDescription::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    XMLSeparator::XMLSeparator(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSeparator::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Separator>(inParent->component(), inAttr))
    {
    }


    XMLSeparator::~XMLSeparator()
    {
    }


    XMLSpacer::XMLSpacer(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSpacer::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Spacer>(inParent->component(), inAttr))
    {
    }


    XMLSpacer::~XMLSpacer()
    {
    }


    XMLMenuButton::XMLMenuButton(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLMenuButton::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, MenuButton>(inParent->component(), inAttr))
    {
    }


    XMLMenuButton::~XMLMenuButton()
    {
    }


    XMLGrid::XMLGrid(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLGrid::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualGrid, Grid>(inParent->component(), inAttr))
    {
    }


    XMLGrid::~XMLGrid()
    {
    }


    XMLRows::XMLRows(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLRows::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Rows>(inParent->component(), inAttr))
    {
    }


    XMLRows::~XMLRows()
    {
    }


    XMLColumns::XMLColumns(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLColumns::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Columns>(inParent->component(), inAttr))
    {
    }


    XMLColumns::~XMLColumns()
    {
    }


    XMLRow::XMLRow(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLRow::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Row>(inParent->component(), inAttr))
    {
    }


    XMLRow::~XMLRow()
    {
    }


    XMLColumn::XMLColumn(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLColumn::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, Column>(inParent->component(), inAttr))
    {
    }


    XMLColumn::~XMLColumn()
    {
    }


    XMLRadioGroup::XMLRadioGroup(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLRadioGroup::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<Decorator, RadioGroup>(inParent->component(), inAttr))
    {
    }


    XMLRadioGroup::~XMLRadioGroup()
    {
    }


    XMLRadio::XMLRadio(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLRadio::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Radio>(inParent->component(), inAttr))
    {
    }


    XMLRadio::~XMLRadio()
    {
    }


    XMLProgressMeter::XMLProgressMeter(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLProgressMeter::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, ProgressMeter>(inParent->component(), inAttr))
    {
    }


    XMLProgressMeter::~XMLProgressMeter()
    {
    }


    XMLDeck::XMLDeck(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLDeck::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, Deck, Deck>(inParent->component(), inAttr))
    {
    }


    XMLDeck::~XMLDeck()
    {
    }


    XMLTabBox::XMLTabBox(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTabBox::TagName(),
                inParent,
                ComponentFactory::Instance().createContainer<Decorator, VirtualBox, Box>(inParent->component(), inAttr))
    {
    }


    XMLTabBox::~XMLTabBox()
    {
    }


    XMLTabs::XMLTabs(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTabs::TagName(),
                inParent,
                new Tabs(inParent->component(), inAttr))
    {
    }


    XMLTabs::~XMLTabs()
    {
    }


    XMLTab::XMLTab(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTab::TagName(),
                inParent,
                new Tab(inParent->component(), inAttr))
    {
    }


    XMLTab::~XMLTab()
    {
    }


    XMLTabPanels::XMLTabPanels(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTabPanels::TagName(),
                inParent,
                new TabPanels(inParent->component(), inAttr))
    {
    }


    XMLTabPanels::~XMLTabPanels()
    {
    }


    XMLTabPanel::XMLTabPanel(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTabPanel::TagName(),
                inParent,
                new TabPanel(inParent->component(), inAttr))
    {
    }


    XMLTabPanel::~XMLTabPanel()
    {
    }


    XMLGroupBox::XMLGroupBox(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLGroupBox::TagName(),
                inParent,
                new MarginDecorator(new GroupBox(inParent->component(), inAttr)))
    {
    }


    XMLCaption::XMLCaption(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLCaption::TagName(),
                inParent,
                new Caption(inParent->component(), inAttr))
    {
    }


    XMLTree::XMLTree(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTree::TagName(),
                inParent,
                new Tree(inParent->component(), inAttr))
    {
    }


    XMLTree::~XMLTree()
    {
    }


    XMLTreeChildren::XMLTreeChildren(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTreeChildren::TagName(),
                inParent,
                new TreeChildren(inParent->component(), inAttr))
    {
    }


    XMLTreeChildren::~XMLTreeChildren()
    {
    }


    XMLTreeItem::XMLTreeItem(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTreeItem::TagName(),
                inParent,
                new TreeItem(inParent->component(), inAttr))
    {
    }


    XMLTreeItem::~XMLTreeItem()
    {
    }


    XMLTreeCols::XMLTreeCols(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTreeCols::TagName(),
                inParent,
                new TreeCols(inParent->component(), inAttr))
    {
    }


    XMLTreeCols::~XMLTreeCols()
    {
    }


    XMLTreeCol::XMLTreeCol(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTreeCol::TagName(),
                inParent,
                new TreeCol(inParent->component(), inAttr))
    {
    }


    XMLTreeCol::~XMLTreeCol()
    {
    }


    XMLTreeRow::XMLTreeRow(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTreeRow::TagName(),
                inParent,
                new TreeRow(inParent->component(), inAttr))
    {
    }


    XMLTreeRow::~XMLTreeRow()
    {
    }


    XMLTreeCell::XMLTreeCell(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLTreeCell::TagName(),
                inParent,
                new TreeCell(inParent->component(), inAttr))
    {
    }


    XMLTreeCell::~XMLTreeCell()
    {
    }


    XMLStatusbar::XMLStatusbar(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLStatusbar::TagName(),
                inParent,
                new Statusbar(inParent->component(), inAttr))
    {
    }


    XMLStatusbar::~XMLStatusbar()
    {
    }


    XMLStatusbarPanel::XMLStatusbarPanel(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLStatusbarPanel::TagName(),
                inParent,
                new StatusbarPanel(inParent->component(), inAttr))
    {
    }


    XMLStatusbarPanel::~XMLStatusbarPanel()
    {
    }


} // namespace XULWin
