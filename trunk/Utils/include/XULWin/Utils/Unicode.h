#ifndef UNICODE_H_INCLUDED
#define UNICODE_H_INCLUDED


#include <string>


namespace XULWin
{

    std::string ToUTF8(const std::wstring & inText);

    std::wstring ToUTF16(const std::string & inText);

} // namespace XULWin



#endif // UNICODE_H_INCLUDED
