#ifndef FACTORKEY_APP_DEBUGGER_H
#define FACTORKEY_APP_DEBUGGER_H

namespace otp
{
    namespace app
    {
        bool secureAgainstDebugger(void);

        void secureAgainstDebugger(int exitWith);
    }
}
#endif
