#include "parameters.h"

namespace otp
{
    namespace parameters
    {
        namespace key
        {
            QStringList sharedParameters(void)
            {
                QStringList l;
                l << NAME << CHARSET << ENCODING;
                return l;
            }
        }

        namespace hashing
        {
            QStringList genericParameters(void)
            {
                QStringList l = otp::parameters::key::sharedParameters();
                l << ALGORITHM;
                return l;
            }
        }
    }
}
