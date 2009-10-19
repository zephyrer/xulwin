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
        ElementPtr window = WindowElement::Create(0, attr);
        ElementPtr vbox = VBoxElement::Create(window.get(), attr);

        ElementPtr hbox1 = HBoxElement::Create(vbox.get(), attr);

        attr["value"] = "Username:";
        ElementPtr label = LabelElement::Create(hbox1.get(), attr);
        attr.clear();

        attr["flex"] = "1";
        ElementPtr text = TextBoxElement::Create(hbox1.get(), attr);
        attr.clear();

        ElementPtr hbox2 = HBoxElement::Create(vbox.get(), attr);

        attr["value"] = "Password:";
        ElementPtr passLabel = LabelElement::Create(hbox2.get(), attr);
        attr.clear();

        attr["flex"] = "1";
        ElementPtr passText = TextBoxElement::Create(hbox2.get(), attr);
        attr.clear();

        window->downcast<WindowElement>()->showModal(WindowElement::CenterInScreen);
    }


} // namespace XULWin
