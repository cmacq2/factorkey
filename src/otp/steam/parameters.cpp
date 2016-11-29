#include "parameters.h"
#include "../parameters.h"

namespace otp
{
    namespace steam
    {
        namespace parameters
        {
            QStringList tokenParameters(void)
            {
                return otp::parameters::key::sharedParameters();
            }
        }
    }
}
