#ifndef FACTORKEY_OATH_PARAMETERS_H
#define FACTORKEY_OATH_PARAMETERS_H

#include <QString>

namespace otp
{
    namespace oath
    {
        namespace parameters
        {
            namespace generic
            {
                const QString LOCALE = QLatin1String("otp.encoder.locale");
                const QString LENGTH = QLatin1String("otp.encoder.length");
            }

            namespace hotp
            {
                const QString COUNTER = QLatin1String("hotp.counter");

                QStringList tokenParameters(void);
            }

            namespace totp
            {
                const QString EPOCH = QLatin1String("totp.epoch");
                const QString TIMESTEP = QLatin1String("totp.timestep");

                QStringList tokenParameters(void);
            }
        }
    }
}

#endif
