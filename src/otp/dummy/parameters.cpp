#include "parameters.h"
#include "../parameters.h"

namespace otp
{
    namespace dummy
    {
        namespace parameters
        {
            QStringList tokenParameters(void)
            {
                QStringList l = otp::parameters::hashing::genericParameters();
                l << MESSAGE << CHARSET;
                return l;
            }
        }
    }
}
