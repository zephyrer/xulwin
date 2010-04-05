#ifndef ATTRIBUTESMAPPING_H_INCLUDED
#define ATTRIBUTESMAPPING_H_INCLUDED


#include <map>
#include <string>


namespace XULWin
{

    //
    // !!! HIGHLY EXPERIMENTAL !!!
    //
    // Alternative to typedefs:
    //
    // * Provides type-safety (can't mix up AttributesMappping or StylesMapping objects)
    // * Can be forward declared
    // * Enables template typedefs (the code below is not an example of that however)
    //
    class AttributesMapping : public std::map<std::string, std::string> { };
    class StylesMapping : public std::map<std::string, std::string> { };


} // namespace XULWin


#endif // ATTRIBUTESMAPPING_H_INCLUDED
