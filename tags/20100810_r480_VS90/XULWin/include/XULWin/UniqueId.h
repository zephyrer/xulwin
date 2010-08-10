#ifndef UNIQUE_ID_H_INCLUDED
#define UNIQUE_ID_H_INCLUDED


#include "XULWin/Types.h"


namespace XULWin
{

#define DECLARE_UNIQUE_ID(ClassName, InitialValue)      \
    class ClassName                                     \
    {                                                   \
    public:                                             \
        ClassName() :                                   \
            mId(ClassName::GetId())                     \
        {                                               \
        }                                               \
        UInt32 value() const { return mId; }            \
    private:                                            \
        UInt32 mId;                                     \
        static UInt32 GetId()                           \
        {                                               \
            static UInt32 fId(InitialValue);            \
            return fId++;                               \
        }                                               \
    };


} // namespace XULWin


#endif // UNIQUE_ID_H_INCLUDED
