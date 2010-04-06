#include "XULWin/ElementFactory.h"
#include "XULWin/ErrorReporter.h"


namespace XULWin
{

    ElementFactory & ElementFactory::Instance()
    {
        static ElementFactory fInstance;
        return fInstance;
    }


    ElementFactory::ElementFactory()
    {
    }


    ElementPtr ElementFactory::createElement(const std::string & inType, Poco::XML::Element * inParent, const AttributesMapping & inAttr)
    {
        ElementPtr result;
        FactoryMethods::iterator it = mFactoryMethods.find(inType);
        if (it != mFactoryMethods.end())
        {
            result = it->second(inParent, inAttr);
            // NOTE TO SELF: don't add any custom code here, use Poco::XML::Element::Create or override Poco::XML::Element::init.
        }
        else
        {
            ReportError("No mapping found for XUL type '" + std::string(inType) + "'");
        }
        return result;
    }

} // namespace XULWin
