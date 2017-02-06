#ifndef FACTORKEY_OATH_TOKEN_H
#define FACTORKEY_OATH_TOKEN_H

#include "../token/token.h"

#include <QDateTime>
#include <QLocale>

namespace otp
{
    namespace oath
    {
        namespace token
        {
            otp::token::Message totpMessage(const QDateTime& epoch, quint64 windowDurationMSec = 0);
            otp::token::Message totpMessage(qint64 epoch = 0, quint64 windowDurationMSec = 0);
            otp::token::Message totpMessage(qint64 currentMSec, qint64 epoch, quint64 windowDurationMSec);

            otp::token::Encoder oathEncoder(uint length, const QLocale& locale);
            otp::token::Encoder oathEncoder(uint length = 0);
        }
    }
}

#endif
