#ifndef UNIQUE_ID_H_INCLUDED
#define UNIQUE_ID_H_INCLUDED


#include "XULWin/Types.h"


namespace XULWin
{

#define DECLARE_UNIQUE_ID(ClassName)                    \
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


    DECLARE_UNIQUE_ID(CommandId)

} // namespace XULWin


#endif // UNIQUE_ID_H_INCLUDED
