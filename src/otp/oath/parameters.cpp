#include "parameters.h"
#include "../parameters.h"

namespace otp
{
    namespace oath
    {
        namespace parameters
        {
            static QStringList genericParameters(void)
            {
                QStringList l = otp::parameters::hashing::genericParameters();
                l << generic::LENGTH << generic::LOCALE;
                return l;
            }

            namespace hotp
            {
                QStringList tokenParameters(void)
                {
                    QStringList l = genericParameters();
                    l << COUNTER;
                    return l;
                }
            }

            namespace totp
            {
                QStringList tokenParameters(void)
                {
                    QStringList l = genericParameters();
                    l << EPOCH << TIMESTEP;
                    return l;
                }
            }
        }
    }
}
