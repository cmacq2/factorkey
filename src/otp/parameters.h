#ifndef FACTORKEY_SHARED_PARAMETERS_H
#define FACTORKEY_SHARED_PARAMETERS_H

#include <QString>
#include <QStringList>

namespace otp
{
    namespace parameters
    {
        namespace key
        {
            const QString NAME = QLatin1String("otp.token.name");
            const QString CHARSET = QLatin1String("otp.key.charset");
            const QString ENCODING = QLatin1String("otp.key.type");

            QStringList sharedParameters(void);
        }

        namespace hashing
        {
            const QString ALGORITHM = QLatin1String("otp.hmac.hash");

            QStringList genericParameters(void);
        }
    }
}
#endif
