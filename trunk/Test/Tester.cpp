#include "Tester.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/XULRunner.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    void Tester::runXULSample(const std::string & inAppName)
    {
        std::string chdir = "../xulrunnersamples/" + inAppName + "/";
        Windows::CurrentDirectoryChanger curdir(chdir);

#if TEST_WITH_MOZILLA_XULRUNNER
        system("run.bat");
#endif

        XULRunner runner;
        runner.run("application.ini");
    }


    void Tester::runNonXULSample()
    {
        AttributesMapping attr;
        ElementPtr window = Window::Create(0, attr);
        ElementPtr vbox = VBox::Create(window.get(), attr);

        ElementPtr hbox1 = HBox::Create(vbox.get(), attr);

        attr["value"] = "Username:";
        ElementPtr label = Label::Create(hbox1.get(), attr);
        attr.clear();

        attr["flex"] = "1";
        ElementPtr text = TextBox::Create(hbox1.get(), attr);
        attr.clear();

        ElementPtr hbox2 = HBox::Create(vbox.get(), attr);

        attr["value"] = "Password:";
        ElementPtr passLabel = Label::Create(hbox2.get(), attr);
        attr.clear();

        attr["flex"] = "1";
        ElementPtr passText = TextBox::Create(hbox2.get(), attr);
        attr.clear();

        window->downcast<Window>()->showModal();
    }


} // namespace XULWin
