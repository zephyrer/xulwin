#ifndef CHROMEURL_H_INCLUDED
#define CHROMEURL_H_INCLUDED


#include <string>


namespace XULWin
{

    class ChromeURL
    {
    public:
        ChromeURL(const std::string & inURL);

        const std::string & stringValue() const;

        std::string convertToLocalPath() const;

    private:
        std::string mURL;
    };


} // XULWin


#endif // CHROMEURL_H_INCLUDED
