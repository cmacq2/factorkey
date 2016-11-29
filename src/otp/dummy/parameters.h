#ifndef FACTORKEY_DUMMY_PARAMETERS_H
#define FACTORKEY_DUMMY_PARAMETERS_H

#include <QString>

namespace otp
{
    namespace dummy
    {
        namespace parameters
        {
            const QString MESSAGE = QLatin1String("dummy.message.value");
            const QString CHARSET = QLatin1String("dummy.message.charset");

            QStringList tokenParameters(void);
        }
    }
}
#endif
