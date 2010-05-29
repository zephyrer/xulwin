#ifndef UNIQUE_ID_H_INCLUDED
#define UNIQUE_ID_H_INCLUDED


#include "XULWin/Types.h"


namespace XULWin
{

    /**
     * The DECLARE_UNIQUE_ID macro can be used to declare a class
     * that holds a number that can be used as a unique id.
     *
     * Every usage of DECLARE_UNIQUE_ID must have an equivalent
     * DEFINE_UNIQUE_ID in a source file.
     */
    #define DECLARE_UNIQUE_ID(ClassName)                \
    class ClassName                                     \
    {                                                   \
    public:                                             \
        ClassName()                                     \
            : mId(sId++)                                \
        {                                               \
        }                                               \
        UInt32 value() const { return mId; }            \
    private:                                            \
        UInt32 mId;                                     \
        static UInt32 sId;                              \
    };                                                  


#define DEFINE_UNIQUE_ID(ClassName, InitialValue) \
    UInt32 ClassName::sId(InitialValue);


    /**
     * CommandId is a unique identifier for Components
     */
    DECLARE_UNIQUE_ID(CommandId)

} // namespace XULWin


#endif // UNIQUE_ID_H_INCLUDED
