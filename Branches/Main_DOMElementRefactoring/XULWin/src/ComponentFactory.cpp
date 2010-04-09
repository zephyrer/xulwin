#include "XULWin/ComponentFactory.h"
#include "XULWin/ErrorReporter.h"


namespace XULWin
{

    bool IsScrollable(Poco::XML::Element * inDOMElement, CSSOverflow & outOverflowX, CSSOverflow & outOverflowY)
    {
        StylesMapping styles;
        GetStyles(inDOMElement, styles);
        outOverflowX = GetOverflow(styles, "overflow-x");
        outOverflowY = GetOverflow(styles, "overflow-y");
        return (outOverflowX != CSSOverflow_Hidden) || (outOverflowY != CSSOverflow_Hidden);
    }


    ComponentFactory & ComponentFactory::Instance()
    {
        static ComponentFactory fComponentFactory;
        return fComponentFactory;
    }


    ComponentPtr ComponentFactory::create(Component * inParent, Poco::XML::Element * inElement)
    {
        ComponentPtr result;
        FactoryMethods::iterator it = mFactoryMethods.find(inElement->tagName());
        if (it != mFactoryMethods.end())
        {
            const FactoryMethod & factoryMethod(it->second);
            result = factoryMethod(inParent, inElement);
            if (result)
            {
                ComponentManager::Instance().addComponent(inElement, result.get());
            }
        }
        return result;
    }


    void ComponentFactory::registerComp(const char * inTagName, const FactoryMethod & inFactoryMethod)
    {            
        FactoryMethods::iterator it = mFactoryMethods.find(inTagName);
        if (it != mFactoryMethods.end())
        {
            throw std::logic_error("Component with tagname '" + std::string(inTagName) + "' has already been registered.");
        }
        mFactoryMethods.insert(std::make_pair(inTagName, inFactoryMethod));
    }


    void GetStyles(Poco::XML::Element * inDOMElement, StylesMapping & styles)
    {
        const Poco::XML::XMLString & styleAttribute = inDOMElement->getAttribute("style");
        if (!styleAttribute.empty())
        {
            Poco::StringTokenizer tok(styleAttribute, ";:", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
            Poco::StringTokenizer::Iterator it = tok.begin(), end = tok.end();
            std::string key, value;
            int counter = 0;
            for (; it != end; ++it)
            {
                if (counter%2 == 0)
                {
                    key = *it;
                }
                else
                {
                    value = *it;
                    styles.insert(std::make_pair(key, value));
                }
                counter++;
            }
        }
    }


    CSSOverflow GetOverflow(const StylesMapping & inStyles, const std::string & inOverflow)
    {
        StylesMapping::const_iterator it = inStyles.find(inOverflow);
        if (it == inStyles.end())
        {
            it = inStyles.find("overflow");
        }
        if (it != inStyles.end())
        {
            return String2CSSOverflow(it->second, CSSOverflow_Visible);
        }
        return CSSOverflow_Hidden;
    }

} // namespace XULWin
