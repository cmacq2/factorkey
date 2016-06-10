#include "token.h"
#include "oath.h"

#include "../base32/base32.h"

namespace otp
{
    namespace oath
    {
        otp::token::Algorithm hmacAlgorithm(const QCryptographicHash::Algorithm& hash)
        {
            return otp::token::Algorithm([hash](const QByteArray& k, const QByteArray& m) -> QByteArray
            {
                return oath::hmac(k, m, hash);
            });
        }

        otp::token::Key keyForAuthenticator(void)
        {
            return otp::token::Key([](const QString& key) -> QByteArray
            {
                bool ok = false;
                QByteArray result = otp::base32::decode(key, &ok);
                return ok ? result : QByteArray();
            });
        }

        otp::token::Message totpMessage(const QDateTime& epoch, quint64 windowDurationMSec)
        {
            return totpMessage(epoch.toMSecsSinceEpoch(), windowDurationMSec);
        }

        otp::token::Message totpMessage(qint64 epoch, quint64 windowDurationMSec)
        {
            return otp::token::Message([epoch, windowDurationMSec](void) -> QByteArray
            {
                return oath::hotpTokenMessage(oath::countTokens(epoch, windowDurationMSec));
            });
        }

        otp::token::Encoder oathEncoder(uint length, const QLocale& locale)
        {
            return otp::token::Encoder([length, locale](const QByteArray& token) -> QString
            {
                return oath::encodeOTPToken(token, locale, length);
            });
        }

        otp::token::Encoder oathEncoder(uint length)
        {
            QLocale locale = QLocale::c();
            locale.setNumberOptions(QLocale::OmitGroupSeparator);
            return otp::token::Encoder([length, locale](const QByteArray& token) -> QString
            {
                return oath::encodeOTPToken(token, locale, length);
            });
        }
    }
}
