#ifndef TESTER_H_INCLUDED
#define TESTER_H_INCLUDED


#include "XULWin/Windows.h"
#include <string>


namespace XULWin
{

    class Tester
    {
    public:
        enum Features
        {
            Features_EnableJavaScript           = 1 << 0, // Executes JavaScript code in the XUL documents.
            Features_TestWithMozillaXULRunner   = 1 << 1  // Also launches the app with Mozilla XULRunner
        };

        Tester(const std::string & inPathToXULRunnerSamples, Features inFeatures);

        void runXULSample(const std::string & inAppName) const;

    private:
        Features mFeatures;
        std::string mPathToXULRunnerSamples;
    };


} // namespace XULWin


#endif // TESTER_H_INCLUDED
