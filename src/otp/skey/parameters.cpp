#include "parameters.h"
#include "../parameters.h"

namespace otp
{
    namespace skey
    {
        namespace parameters
        {
            QStringList tokenParameters(void)
            {
                QStringList l = otp::parameters::key::sharedParameters();
                l << DICTIONARY << ENCODING;
                return l;
            }
        }
    }
}
