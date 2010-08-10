#include "XULWin/ComponentFactory.h"
#include "XULWin/Conversions.h"
#include "XULWin/ErrorReporter.h"


namespace XULWin
{
    ComponentFactory & ComponentFactory::Instance()
    {
        static ComponentFactory fComponentFactory;
        return fComponentFactory;
    }


    void ComponentFactory::GetStyles(const AttributesMapping & inAttr, StylesMapping & styles)
    {
        StylesMapping::const_iterator it = inAttr.find("style");
        if (it != inAttr.end())
        {
            Poco::StringTokenizer tok(it->second, ";:", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
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


    CSSOverflow ComponentFactory::GetOverflow(const StylesMapping & inStyles,
                                              const std::string & inOverflow)
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
