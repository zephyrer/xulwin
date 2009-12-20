#include "XULWin/Initializer.h"
#include "XULWin/Component.h"
#include "XULWin/ConditionalState.h"
#include "XULWin/ImageElement.h"
#include "XULWin/ListBoxElement.h"
#include "XULWin/ListCellElement.h"
#include "XULWin/ListColElement.h"
#include "XULWin/ListColsElement.h"
#include "XULWin/ListHeadElement.h"
#include "XULWin/ListHeaderElement.h"
#include "XULWin/ListItemElement.h"
#include "XULWin/MenuElement.h"
#include "XULWin/MenuBarElement.h"
#include "XULWin/MenuItemElement.h"
#include "XULWin/MenuPopupElement.h"
#include "XULWin/MenuSeparatorElement.h"
#include "XULWin/ScriptElement.h"
#include "XULWin/SVGElement.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{
    
    Initializer::Initializer(HINSTANCE inModuleHandle)
    {
        Windows::CommonControlsInitializer mInitCommonControls;
        ErrorReporter::Initialize();
        ConditionalState::Initialize(inModuleHandle);
        Window::Register(inModuleHandle);
        Dialog::Register(inModuleHandle);
        ElementFactory::Instance().registerElement<WindowElement>();
        ElementFactory::Instance().registerElement<DialogElement>();
        ElementFactory::Instance().registerElement<ButtonElement>();
        ElementFactory::Instance().registerElement<CheckBoxElement>();
        ElementFactory::Instance().registerElement<LabelElement>();
        ElementFactory::Instance().registerElement<TextElement>();
        ElementFactory::Instance().registerElement<HBoxElement>();
        ElementFactory::Instance().registerElement<VBoxElement>();
        ElementFactory::Instance().registerElement<BoxElement>();
        ElementFactory::Instance().registerElement<MenuBarElement>();
        ElementFactory::Instance().registerElement<MenuElement>();
        ElementFactory::Instance().registerElement<MenuPopupElement>();
        ElementFactory::Instance().registerElement<MenuItemElement>();
        ElementFactory::Instance().registerElement<MenuListElement>();
        ElementFactory::Instance().registerElement<MenuSeparatorElement>();
        ElementFactory::Instance().registerElement<TextBoxElement>();
        ElementFactory::Instance().registerElement<SeparatorElement>();
        ElementFactory::Instance().registerElement<MenuButtonElement>();
        ElementFactory::Instance().registerElement<GridElement>();
        ElementFactory::Instance().registerElement<RowsElement>();
        ElementFactory::Instance().registerElement<RowElement>();
        ElementFactory::Instance().registerElement<ColumnsElement>();
        ElementFactory::Instance().registerElement<ColumnElement>();
        ElementFactory::Instance().registerElement<DescriptionElement>();
        ElementFactory::Instance().registerElement<SpacerElement>();
        ElementFactory::Instance().registerElement<RadioGroupElement>();
        ElementFactory::Instance().registerElement<RadioElement>();
        ElementFactory::Instance().registerElement<ProgressMeterElement>();
        ElementFactory::Instance().registerElement<DeckElement>();
        ElementFactory::Instance().registerElement<ImageElement>();
        ElementFactory::Instance().registerElement<ScrollbarElement>();
        ElementFactory::Instance().registerElement<ScriptElement>();
        ElementFactory::Instance().registerElement<TabBoxElement>();
        ElementFactory::Instance().registerElement<TabsElement>();
        ElementFactory::Instance().registerElement<TabElement>();
        ElementFactory::Instance().registerElement<TabPanelsElement>();
        ElementFactory::Instance().registerElement<TabPanelElement>();
        ElementFactory::Instance().registerElement<GroupBoxElement>();
        ElementFactory::Instance().registerElement<CaptionElement>();
        ElementFactory::Instance().registerElement<SVGElement>();
        ElementFactory::Instance().registerElement<SVGGroupElement>();
        ElementFactory::Instance().registerElement<SVGPolygonElement>();
        ElementFactory::Instance().registerElement<SVGPathElement>();
        ElementFactory::Instance().registerElement<SVGRectElement>();
        ElementFactory::Instance().registerElement<TreeElement>();
        ElementFactory::Instance().registerElement<TreeChildrenElement>();
        ElementFactory::Instance().registerElement<TreeColsElement>();
        ElementFactory::Instance().registerElement<TreeColElement>();
        ElementFactory::Instance().registerElement<TreeRowElement>();
        ElementFactory::Instance().registerElement<TreeCellElement>();
        ElementFactory::Instance().registerElement<TreeItemElement>();
        ElementFactory::Instance().registerElement<StatusbarElement>();
        ElementFactory::Instance().registerElement<StatusbarPanelElement>();
        ElementFactory::Instance().registerElement<ToolbarElement>();
        ElementFactory::Instance().registerElement<ToolbarButtonElement>();
        ElementFactory::Instance().registerElement<ListBoxElement>();
        ElementFactory::Instance().registerElement<ListItemElement>();
        ElementFactory::Instance().registerElement<ListCellElement>();
        ElementFactory::Instance().registerElement<ListColElement>();
        ElementFactory::Instance().registerElement<ListColsElement>();
        ElementFactory::Instance().registerElement<ListHeadElement>();
        ElementFactory::Instance().registerElement<ListHeaderElement>();
    }


    Initializer::~Initializer()
    {
        ConditionalState::Finalize();
        ErrorReporter::Finalize();
    }

} // namespace XULWin
