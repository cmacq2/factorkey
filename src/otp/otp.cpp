#include "otp.h"
#include "oath/oath.h"

namespace otp
{

    QString encodeDummyFormat(const QByteArray& token)
    {
        return QLatin1String(token.toHex());
    }
}
