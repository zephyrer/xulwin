#include "XULWin/Initializer.h"
#include "XULWin/Component.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Label.h"
#include "XULWin/Image.h"
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
        ComponentFactory::Instance().registerComponent<Window>();
        ComponentFactory::Instance().registerComponent<Dialog>();
        ComponentFactory::Instance().registerComponent<Button>();
        ComponentFactory::Instance().registerComponent<CheckBox>();
        ComponentFactory::Instance().registerComponent<Label>();
        ComponentFactory::Instance().registerComponent<TextBox>();
        //ComponentFactory::Instance().registerComponent<Box>();
        //ComponentFactory::Instance().registerComponent<Separator>();
        ComponentFactory::Instance().registerComponent<Grid>();
        ComponentFactory::Instance().registerComponent<Rows>();
        ComponentFactory::Instance().registerComponent<Row>();
        ComponentFactory::Instance().registerComponent<Columns>();
        ComponentFactory::Instance().registerComponent<Column>();
        //ComponentFactory::Instance().registerComponent<Description>();
        //ComponentFactory::Instance().registerComponent<Spacer>();
        //ComponentFactory::Instance().registerComponent<RadioGroup>();
        //ComponentFactory::Instance().registerComponent<Radio>();
        //ComponentFactory::Instance().registerComponent<ProgressMeter>();
        //ComponentFactory::Instance().registerComponent<Deck>();
        //ComponentFactory::Instance().registerComponent<Image>();
        //ComponentFactory::Instance().registerComponent<Scrollbar>();
        //ComponentFactory::Instance().registerComponent<GroupBox>();
        //ComponentFactory::Instance().registerComponent<Caption>();
        //ComponentFactory::Instance().registerComponent<Statusbar>();
    }


    Initializer::~Initializer()
    {
        ErrorReporter::Finalize();
    }

} // namespace XULWin
