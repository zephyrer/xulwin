#include "XULWin/Element.h"
#include "XULWin/ElementCreationSupport.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Component.h"
#include "XULWin/ToolbarCustomWindowDecorator.h"
#include "XULWin/ErrorReporter.h"
#include <boost/bind.hpp>


namespace XULWin
{

    Element::Element(const std::string & inType, Element * inParent, Component * inNativeComponent) :
        mType(inType),
        mParent(inParent),
        mComponent(inNativeComponent)
    {
        if (mComponent)
        {
            mComponent->setOwningElement(this);
        }
    }


    Element::~Element()
    {
        // Children require parent access while destructing.
        // So we destruct them while parent still alive.
        mChildren.clear();
    }

    
    bool Element::init()
    {
        return component()->initComponent();
    }

    
    const std::string & Element::type() const
    {
        return mType;
    }


    void Element::setInnerText(const std::string & inText)
    {
        mInnerText = inText;
    }


    const std::string & Element::innerText() const
    {
        return mInnerText;
    }

    
    Element * Element::getElementById(const std::string & inId)
    {
        struct Helper
        {
            static Element * findChildById(const Children & inChildren, const std::string & inId)
            {
                Element * result(0);
                for (size_t idx = 0; idx != inChildren.size(); ++idx)
                {
                    ElementPtr child = inChildren[idx];
                    if (child->getAttribute("id") == inId)
                    {
                        result = child.get();
                        break;
                    }
                    else
                    {
                        result = findChildById(child->children(), inId);
                        if (result)
                        {
                            break;
                        }
                    }
                }
                return result;
            }
        };
        return Helper::findChildById(children(), inId);
    }
    
    
    void Element::getElementsByType(const std::string & inType, std::vector<Element*> & outElements)
    {
        if (type() == inType)
        {
            outElements.push_back(this);
        }
        for (size_t idx = 0; idx != mChildren.size(); ++idx)
        {
            mChildren[idx]->getElementsByType(inType, outElements);
        }
    }


    void Element::removeChild(const Element * inChild)
    {
        Children::iterator it = std::find_if(mChildren.begin(), mChildren.end(), boost::bind(&ElementPtr::get, _1) == inChild);
        if (it != mChildren.end())
        {
            ElementPtr keepAlive = *it;
            mChildren.erase(it);
            mComponent->rebuildLayout();            
            mComponent->onChildRemoved(keepAlive->component());
            // keepAlive loses scope here and destroys child
        }
        else
        {
            ReportError("Remove child failed because it wasn't found.");
        }
    }


    void Element::setStyles(const AttributesMapping & inAttributes)
    {
        AttributesMapping::const_iterator it = inAttributes.find("style");
        if (it != inAttributes.end())
        {
            Poco::StringTokenizer keyValuePairs(it->second, ";",
                                                Poco::StringTokenizer::TOK_IGNORE_EMPTY
                                                | Poco::StringTokenizer::TOK_TRIM);

            Poco::StringTokenizer::Iterator it = keyValuePairs.begin(), end = keyValuePairs.end();
            for (; it != end; ++it)
            {
                std::string::size_type sep = it->find(":");
                if (sep != std::string::npos && (sep + 1) < it->size())
                {
                    std::string key = it->substr(0, sep);
                    std::string value = it->substr(sep + 1, it->size() - sep - 1);
                    setStyle(key, value);
                }
            }
        }
    }


    void Element::setAttributes(const AttributesMapping & inAttributes)
    {
        mAttributes = inAttributes;
        
        if (mComponent)
        {
            AttributesMapping::iterator it = mAttributes.begin(), end = mAttributes.end();
            for (; it != end; ++it)
            {
                // ignore error reports about failure to apply attributes
                // it's unlikely to be an issue here
                ErrorCatcher errorIgnorer;
                errorIgnorer.disableLogging(true);
                setAttribute(it->first, it->second);
            }
        }
    }


    void Element::initAttributeControllers()
    {
        if (mComponent)
        {
            mComponent->initAttributeControllers();
        }
    }


    void Element::initStyleControllers()
    {
        if (mComponent)
        {
            mComponent->initStyleControllers();
        }
    }

    
    std::string Element::getStyle(const std::string & inName) const
    {
        std::string result;
        if (!mComponent || !mComponent->getStyle(inName, result))
        {
            StylesMapping::const_iterator it = mStyles.find(inName);
            if (it != mStyles.end())
            {
                result = it->second;
            }
        }
        return result;

    }


    std::string Element::getAttribute(const std::string & inName) const
    {
        std::string result;
        if (!mComponent || !mComponent->getAttribute(inName, result))
        {
            AttributesMapping::const_iterator it = mAttributes.find(inName);
            if (it != mAttributes.end())
            {
                result = it->second;
            }
        }
        return result;
    }
    
    
    std::string Element::getDocumentAttribute(const std::string & inName) const
    {
        std::string result;
        AttributesMapping::const_iterator it = mAttributes.find(inName);
        if (it != mAttributes.end())
        {
            result = it->second;
        }
        return result;
    }
    
    
    void Element::setStyle(const std::string & inName, const std::string & inValue)
    {
        std::string type = this->type();
        if (!mComponent || !mComponent->setStyle(inName, inValue))
        {
            mStyles[inName] = inValue;
        }
    }
    
    
    void Element::setAttribute(const std::string & inName, const std::string & inValue)
    {
        if (!mComponent || !mComponent->setAttribute(inName, inValue))
        {
            mAttributes[inName] = inValue;
        }
    }
    
    
    bool Element::addEventListener(EventListener * inEventListener)
    {
        if (!mComponent)
        {
            return false;
        }

        if (NativeComponent * comp = mComponent->downcast<NativeComponent>())
        {
            comp->addEventListener(inEventListener);
            return true;
        }
        return false;
    }


    bool Element::removeEventListener(EventListener * inEventListener)
    {
        if (!mComponent)
        {
            return false;
        }

        if (NativeComponent * comp = mComponent->downcast<NativeComponent>())
        {
            comp->removeEventListener(inEventListener);
            return true;
        }
        return false;
    }
    
    
    Component * Element::component() const
    {
        return mComponent.get();
    }
    
    
    void Element::addChild(ElementPtr inChild)
    {
        mChildren.push_back(inChild);
        component()->onChildAdded(inChild->component());
    }


    Window::Window(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Window::Type(),
                inParent,
                new NativeWindow(inAttributesMapping))
    {
    }


    void Window::show(Positioning inPositioning)
    {
        if (NativeWindow * nativeWindow = component()->downcast<NativeWindow>())
        {
            nativeWindow->show(inPositioning);
        }
    }


    void Window::showModal(Positioning inPositioning)
    {
        if (NativeWindow * nativeWindow = component()->downcast<NativeWindow>())
        {
            nativeWindow->showModal(inPositioning);
        }
    }


    void Window::close()
    {
        if (NativeWindow * nativeWindow = component()->downcast<NativeWindow>())
        {
            nativeWindow->close();
        }
    }



    // We want the dialog to have a parent because an owning window is needed
    // in order to prevent our dialog from showing in the Windows taskbar.
    // MSDN Article ID 205158: How To Prevent a Window from Appearing on the Taskbar
    static NativeWindow * GetDialogHelper()
    {
        AttributesMapping attr;
        static NativeWindow fDialogHelper(attr);
        fDialogHelper.setTitle("XULWin::DialogHelper");
        return &fDialogHelper;
    }


    Dialog::Dialog(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Dialog::Type(),
                inParent,
                new NativeDialog(inParent ? inParent->component() : GetDialogHelper(), inAttributesMapping))
    {
    }


    DialogResult Dialog::showModal(Window * inInvoker)
    {
        if (NativeDialog * nativeDialog = component()->downcast<NativeDialog>())
        {
            return nativeDialog->showModal(inInvoker->component()->downcast<NativeWindow>());
        }
        return DialogResult_Cancel;
    }


    void Dialog::endModal(DialogResult inDialogResult)
    {
        if (NativeDialog * nativeDialog = component()->downcast<NativeDialog>())
        {
            nativeDialog->endModal(inDialogResult);
        }
    }


    Button::Button(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Button::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeButton>(inParent, inAttributesMapping)))
    {
    }


    Label::Label(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Label::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeLabel>(inParent, inAttributesMapping)))
    {
    }


    Description::Description(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Description::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeDescription>(inParent, inAttributesMapping)))
    {
    }


    bool Description::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    Text::Text(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Text::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeLabel>(inParent, inAttributesMapping)))
    {
    }


    TextBox::TextBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TextBox::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeTextBox>(inParent, inAttributesMapping)))
    {
    }


    CheckBox::CheckBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(CheckBox::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeCheckBox>(inParent, inAttributesMapping)))
    {
    }


    Box::Box(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Box::Type(),
                inParent,
                CreateContainer<VirtualBox, NativeBox>(inParent, inAttributesMapping))
    {
    }


    HBox::HBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(HBox::Type(),
                inParent,
                CreateContainer<VirtualBox, NativeBox>(inParent, inAttributesMapping))
    {
        component()->setOrient(Horizontal);
    }


    VBox::VBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(VBox::Type(),
                inParent,CreateContainer<VirtualBox, NativeBox>(inParent, inAttributesMapping))
    {
        component()->setOrient(Vertical);
    }


    Menu::Menu(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Menu::Type(),
                inParent,
                new MenuImpl(inParent->component(), inAttributesMapping))
    {
    }


    MenuBar::MenuBar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuBar::Type(),
                inParent,
                new MenuBarImpl(inParent->component(), inAttributesMapping))
    {
    }


    MenuList::MenuList(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuList::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<MenuListImpl>(inParent, inAttributesMapping)))
    {
    }

    
    MenuPopup::MenuPopup(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuPopup::Type(),
                inParent,
                new MenuPopupImpl(inParent->component(), inAttributesMapping))
    {
    }

    
    MenuPopup::~MenuPopup()
    {
    }

    
    MenuItem::MenuItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuItem::Type(),
                inParent,
                new MenuItemImpl(inParent->component(), inAttributesMapping))
    {
    }
        
    
    MenuItem::~MenuItem()
    {
    }

    
    std::string MenuItem::label() const
    {
        return getAttribute("label");
    }

    
    std::string MenuItem::value() const
    {
        return getAttribute("value");
    }


    Separator::Separator(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Separator::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<NativeSeparator>(inParent, inAttributesMapping)))
    {
    }


    Separator::~Separator()
    {
    }


    Spacer::Spacer(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Spacer::Type(),
                inParent,
                new NativeSpacer(inParent->component(), inAttributesMapping))
    {
    }


    Spacer::~Spacer()
    {
    }


    MenuButton::MenuButton(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuButton::Type(),
                inParent,
                new MarginDecorator(new NativeMenuButton(inParent->component(), inAttributesMapping)))
    {
    }


    MenuButton::~MenuButton()
    {
    }


    Grid::Grid(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Grid::Type(),
                inParent,
                CreateContainer<VirtualGrid, NativeGrid>(inParent, inAttributesMapping))
    {
    }


    Grid::~Grid()
    {
    }


    Rows::Rows(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Rows::Type(),
                inParent,
                new NativeRows(inParent->component(), inAttributesMapping))
    {
    }


    Rows::~Rows()
    {
    }


    Columns::Columns(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Columns::Type(),
                inParent,
                new NativeColumns(inParent->component(), inAttributesMapping))
    {
    }


    Columns::~Columns()
    {
    }


    Row::Row(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Row::Type(),
                inParent,
                new NativeRow(inParent->component(), inAttributesMapping))
    {
    }


    Row::~Row()
    {
    }


    Column::Column(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Column::Type(),
                inParent,
                new NativeColumn(inParent->component(), inAttributesMapping))
    {
    }


    Column::~Column()
    {
    }


    RadioGroup::RadioGroup(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(RadioGroup::Type(),
                inParent,
                new Decorator(new NativeRadioGroup(inParent->component(), inAttributesMapping)))
    { 
    }


    RadioGroup::~RadioGroup()
    {
    }


    Radio::Radio(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Radio::Type(),
                inParent,
                new MarginDecorator(new NativeRadio(inParent->component(), inAttributesMapping)))
    {
    }


    Radio::~Radio()
    {
    }


    ProgressMeter::ProgressMeter(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ProgressMeter::Type(),
                inParent,
                new MarginDecorator(new NativeProgressMeter(inParent->component(), inAttributesMapping)))
    {
    }


    ProgressMeter::~ProgressMeter()
    {
    }


    Deck::Deck(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Deck::Type(),
                inParent,
                new Decorator(new NativeDeck(inParent->component(), inAttributesMapping)))
    {
    }


    Deck::~Deck()
    {
    }


    Scrollbar::Scrollbar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Scrollbar::Type(),
                inParent,
                new Decorator(new NativeScrollbar(inParent->component(), inAttributesMapping)))
    {
    }


    Scrollbar::~Scrollbar()
    {
    }


    TabBox::TabBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabBox::Type(),
                inParent,
                CreateContainer<VirtualBox, NativeBox>(inParent, inAttributesMapping))
    { 
    }


    TabBox::~TabBox()
    {
    }


    Tabs::Tabs(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Tabs::Type(),
                inParent,
                new TabsImpl(inParent->component(), inAttributesMapping))
    {
    }


    Tabs::~Tabs()
    {
    }


    Tab::Tab(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Tab::Type(),
                inParent,
                new TabImpl(inParent->component(), inAttributesMapping))
    {
    }


    Tab::~Tab()
    {
    }


    TabPanels::TabPanels(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabPanels::Type(),
                inParent,
                new TabPanelsImpl(inParent->component(), inAttributesMapping))
    { 
    }


    TabPanels::~TabPanels()
    {
    }


    TabPanel::TabPanel(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TabPanel::Type(),
                inParent,
                new TabPanelImpl(inParent->component(), inAttributesMapping))
    { 
    }


    TabPanel::~TabPanel()
    {
    }


    GroupBox::GroupBox(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(GroupBox::Type(),
                inParent,
                new MarginDecorator(new GroupBoxImpl(inParent->component(), inAttributesMapping)))
    { 
    }


    Caption::Caption(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Caption::Type(),
                inParent,
                new CaptionImpl(inParent->component(), inAttributesMapping))
    {
    }


    Tree::Tree(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Tree::Type(),
                inParent,
                new TreeImpl(inParent->component(), inAttributesMapping))
    {
    }


    Tree::~Tree()
    {
    }


    TreeChildren::TreeChildren(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeChildren::Type(),
                inParent,
                new TreeChildrenImpl(inParent->component(), inAttributesMapping))
    {
    }


    TreeChildren::~TreeChildren()
    {
    }


    TreeItem::TreeItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeItem::Type(),
                inParent,
                new TreeItemImpl(inParent->component(), inAttributesMapping))
    {
    }


    TreeItem::~TreeItem()
    {
    }


    TreeCols::TreeCols(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeCols::Type(),
                inParent,
                new TreeColsImpl(inParent->component(), inAttributesMapping))
    {
    }


    TreeCols::~TreeCols()
    {
    }


    TreeCol::TreeCol(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeCol::Type(),
                inParent,
                new TreeColImpl(inParent->component(), inAttributesMapping))
    {
    }


    TreeCol::~TreeCol()
    {
    }    
    
    
    TreeRow::TreeRow(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeRow::Type(),
                inParent,
                new TreeRowImpl(inParent->component(), inAttributesMapping))
    {
    }


    TreeRow::~TreeRow()
    {
    }

    
    TreeCell::TreeCell(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TreeCell::Type(),
                inParent,
                new TreeCellImpl(inParent->component(), inAttributesMapping))
    {
    }


    TreeCell::~TreeCell()
    {
    }

    
    Statusbar::Statusbar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Statusbar::Type(),
                inParent,
                new StatusbarImpl(inParent->component(), inAttributesMapping))
    {
    }


    Statusbar::~Statusbar()
    {
    }

    
    StatusbarPanel::StatusbarPanel(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(StatusbarPanel::Type(),
                inParent,
                new StatusbarPanelImpl(inParent->component(), inAttributesMapping))
    {
    }


    StatusbarPanel::~StatusbarPanel()
    {
    }

    
    Toolbar::Toolbar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Toolbar::Type(),
                inParent,
                new ToolbarImpl(inParent->component(), inAttributesMapping))
    {
    }


    Toolbar::~Toolbar()
    {
    }

    
    ToolbarButton::ToolbarButton(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ToolbarButton::Type(),
                inParent,
                new ToolbarButtonImpl(inParent->component(), inAttributesMapping))
    {
    }


    ToolbarButton::~ToolbarButton()
    {
    }


} // namespace XULWin
