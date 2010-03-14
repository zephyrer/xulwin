#ifndef TESTER_H_INCLUDED
#define TESTER_H_INCLUDED


#include "XULWin/Windows.h"
#include <string>


namespace XULWin
{

    class Tester
    {
    public:
        Tester(HMODULE inModuleHandle);

        void runXULSample(const std::string & inAppName);

        void runNonXULSample();

    private:
        HMODULE mModuleHandle;
        std::string mPathToXULRunnerSamples;
    };


} // namespace XULWin


#endif // TESTER_H_INCLUDED
