#include "debugger.h"

#include "config.h"

#if !defined(FACTORKEY_WITH_DUMPING) || !FACTORKEY_WITH_DUMPING
#include <sys/prctl.h>
#endif

#include <QCoreApplication>

namespace otp
{
    namespace app
    {
        bool secureAgainstDebugger(void)
        {
            #if defined(FACTORKEY_WITH_DUMPING) && FACTORKEY_WITH_DUMPING
            return true;
            #elif defined(FACTORKEY_USE_LINUX_PRCTL) && FACTORKEY_USE_LINUX_PRCTL
            return prctl(PR_SET_DUMPABLE, 0, 0, 0, 0) ? false : true;
            #else
            return false;
            #endif
        }

        void secureAgainstDebugger(int exitWith)
        {
            if(!secureAgainstDebugger())
            {
                QCoreApplication::exit(exitWith);
            }
        }
    }
}
