#include "token.h"
#include "oath.h"

namespace otp
{
    namespace oath
    {
        namespace token
        {
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

            otp::token::Message totpMessage(qint64 currentMSec, qint64 epoch, quint64 windowDurationMSec)
            {
                return otp::token::Message([currentMSec, epoch, windowDurationMSec](void) -> QByteArray
                {
                    return oath::hotpTokenMessage(oath::countTokens(currentMSec, epoch, windowDurationMSec));
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

            otp::token::Algorithm hmacAlgorithm(const QCryptographicHash::Algorithm& hash)
            {
                return otp::token::Algorithm([hash](const QByteArray& k, const QByteArray& m) -> QByteArray
                {
                    return hmac(k, m, hash);
                });
            }
        }
    }
}
