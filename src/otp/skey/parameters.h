#ifndef FACTORKEY_SKEY_PARAMETERS_H
#define FACTORKEY_SKEY_PARAMETERS_H

#include <QString>

namespace otp
{
    namespace skey
    {
        namespace parameters
        {

            //const QString DEFAULT_DICTIONARY_VALUE;
            const QString DICTIONARY = QLatin1String("otp.skey.dictionary");
            const QString ENCODING   = QLatin1String("otp.skey.token.encoding");

            QStringList tokenParameters(void);
        }
    }
}
#endif
