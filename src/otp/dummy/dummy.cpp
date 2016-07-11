#include "dummy.h"

namespace otp
{
    namespace dummy
    {
        QString encodeDummyFormat(const QByteArray& token)
        {
            return QLatin1String(token.toHex());
        }
    }
}
