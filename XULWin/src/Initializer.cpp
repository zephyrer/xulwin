#include "XULWin/Initializer.h"
#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/ConditionalState.h"
#include "XULWin/Dialog.h"
#include "XULWin/Element.h"
#include "XULWin/Elements.h"
#include "XULWin/PopupMenu.h"
#include "XULWin/XMLImage.h"
#include "XULWin/XMLListCell.h"
#include "XULWin/XMLListCol.h"
#include "XULWin/XMLListCols.h"
#include "XULWin/XMLListHead.h"
#include "XULWin/XMLListHeader.h"
#include "XULWin/XMLMenu.h"
#include "XULWin/XMLMenuBar.h"
#include "XULWin/XMLMenuSeparator.h"
#include "XULWin/XMLOverlay.h"
#include "XULWin/XMLScript.h"
#include "XULWin/XMLSVG.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"


namespace XULWin
{

    Initializer::Initializer(HINSTANCE inModuleHandle)
    {
        XULRunner::SetModuleHandle(inModuleHandle);
        Windows::CommonControlsInitializer mInitCommonControls;
        ErrorReporter::Initialize();
        Window::Register(inModuleHandle);
        Dialog::Register(inModuleHandle);
        Windows::PopupMenu::Register(inModuleHandle);
        ElementFactory::Instance().registerElement<XMLWindow>();
        ElementFactory::Instance().registerElement<XMLDialog>();
        ElementFactory::Instance().registerElement<XMLButton>();
        ElementFactory::Instance().registerElement<XMLCheckBox>();
        ElementFactory::Instance().registerElement<XMLLabel>();
        ElementFactory::Instance().registerElement<XMLHBox>();
        ElementFactory::Instance().registerElement<XMLVBox>();
        ElementFactory::Instance().registerElement<XMLBox>();
        ElementFactory::Instance().registerElement<XMLMenuBar>();
        ElementFactory::Instance().registerElement<XMLMenu>();
        ElementFactory::Instance().registerElement<XMLMenuPopup>();
        ElementFactory::Instance().registerElement<XMLMenuItem>();
        ElementFactory::Instance().registerElement<XMLMenuList>();
        ElementFactory::Instance().registerElement<XMLMenuSeparator>();
        ElementFactory::Instance().registerElement<XMLTextBox>();
        ElementFactory::Instance().registerElement<XMLSeparator>();
        ElementFactory::Instance().registerElement<XMLMenuButton>();
        ElementFactory::Instance().registerElement<XMLGrid>();
        ElementFactory::Instance().registerElement<XMLRows>();
        ElementFactory::Instance().registerElement<XMLRow>();
        ElementFactory::Instance().registerElement<XMLColumns>();
        ElementFactory::Instance().registerElement<XMLColumn>();
        ElementFactory::Instance().registerElement<XMLDescription>();
        ElementFactory::Instance().registerElement<XMLSpacer>();
        ElementFactory::Instance().registerElement<XMLRadioGroup>();
        ElementFactory::Instance().registerElement<XMLRadio>();
        ElementFactory::Instance().registerElement<XMLProgressMeter>();
        ElementFactory::Instance().registerElement<XMLDeck>();
        ElementFactory::Instance().registerElement<XMLImage>();
        ElementFactory::Instance().registerElement<XMLScrollBar>();
        ElementFactory::Instance().registerElement<XMLScript>();
        ElementFactory::Instance().registerElement<XMLTabBox>();
        ElementFactory::Instance().registerElement<XMLTabs>();
        ElementFactory::Instance().registerElement<XMLTab>();
        ElementFactory::Instance().registerElement<XMLTabPanels>();
        ElementFactory::Instance().registerElement<XMLTabPanel>();
        ElementFactory::Instance().registerElement<XMLGroupBox>();
        ElementFactory::Instance().registerElement<XMLCaption>();
        ElementFactory::Instance().registerElement<XMLSVG>();
        ElementFactory::Instance().registerElement<XMLSVGGroup>();
        ElementFactory::Instance().registerElement<XMLSVGPolygon>();
        ElementFactory::Instance().registerElement<XMLSVGPath>();
        ElementFactory::Instance().registerElement<XMLSVGRect>();
        ElementFactory::Instance().registerElement<XMLTree>();
        ElementFactory::Instance().registerElement<XMLTreeChildren>();
        ElementFactory::Instance().registerElement<XMLTreeCols>();
        ElementFactory::Instance().registerElement<XMLTreeCol>();
        ElementFactory::Instance().registerElement<XMLTreeRow>();
        ElementFactory::Instance().registerElement<XMLTreeCell>();
        ElementFactory::Instance().registerElement<XMLTreeItem>();
        ElementFactory::Instance().registerElement<XMLStatusbar>();
        ElementFactory::Instance().registerElement<XMLStatusbarPanel>();
        ElementFactory::Instance().registerElement<XMLListBox>();
        ElementFactory::Instance().registerElement<XMLListItem>();
        ElementFactory::Instance().registerElement<XMLListCell>();
        ElementFactory::Instance().registerElement<XMLListCol>();
        ElementFactory::Instance().registerElement<XMLListCols>();
        ElementFactory::Instance().registerElement<XMLListHead>();
        ElementFactory::Instance().registerElement<XMLListHeader>();
        ElementFactory::Instance().registerElement<XMLOverlay>();
        ConditionalState::Initialize(inModuleHandle);
    }


    Initializer::~Initializer()
    {
        ConditionalState::Finalize();
        ErrorReporter::Finalize();
    }

} // namespace XULWin
