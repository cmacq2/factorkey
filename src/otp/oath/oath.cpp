#include "oath.h"

#include <QMessageAuthenticationCode>

namespace otp
{
    namespace oath
    {
        quint64 countTokens(qint64 epoch, quint64 timeStepMSec)
        {
            auto now = QDateTime::currentDateTimeUtc();
            return countTokens(now.toMSecsSinceEpoch(), epoch, timeStepMSec);
        }

        quint64 countTokens(qint64 currentMSec, qint64 epoch, quint64 timeStepMSec)
        {
            /*
             * It's unclear how TOTP should work when the 'epoch' is in the future
             * as compared to the current date/time in UTC.
             * With the current approach, the counter is underflowed deliberately.
             */
            quint64 counter = currentMSec - epoch;
            return counter / (timeStepMSec == 0 ? DEFAULT_TIMESTEP_MSEC : timeStepMSec);
        }

        QByteArray hotpTokenMessage(quint64 counter)
        {
            QByteArray result;
            result.resize(8);
            char * data = result.data();
            for(int i = 0; i < 8; ++i)
            {
                data[7 - i] = counter & 0xFF;
                counter = counter >> 8;
            }
            return result;
        }

        QString encodeOTPToken(const QByteArray& token, const QLocale& locale, uint length)
        {
            return encodeOTPToken(hotpTokenValue(token), locale, length);
        }

        QString encodeOTPToken(quint32 value, const QLocale& locale, uint length)
        {
            auto coded = locale.toString(value);
            auto tokenLength = length == 0 ? DEFAULT_OTP_LENGTH : length;
            return coded.rightJustified(tokenLength, locale.zeroDigit()).right(tokenLength);
        }

        quint32 hotpTokenValue(const QByteArray& token)
        {
            auto length = token.size();
            if(length > 0)
            {
                /*
                 * Truncate the HMAC down to (at most) 4 bytes long sequence.
                 * The last nibble is interpreted as the starting index of the sequence within the HMAC.
                 * This should yield values between 0 and 15 for the offset.
                 */
                auto truncated = token.mid(0x0F & token.at(length - 1), sizeof(quint32));

                /*
                 * Interpret the (at most) 4 byte truncated sequence as a (big endian) unsigned integer.
                 * Mask out the high order bit.
                 * Code adapted from the HOTP spec. Refer to section 5.4 of RFC 4226.
                 * See: https://tools.ietf.org/html/rfc4226#section-5.4
                 */
                const char * tokenResult = truncated.constData();
                switch(truncated.size())
                {
                    case 0:
                        return 0U;
                    case 1:
                        return tokenResult[0] & 0xFF;
                    case 2:
                        return ((tokenResult[0] & 0x7F) << 8) | (tokenResult[1] & 0xFF);
                    case 3:
                        return ((tokenResult[0] & 0x7F) << 16) | ((tokenResult[1] & 0xFF) << 8) | (tokenResult[2] & 0xFF);
                    case 4:
                        return ((tokenResult[0] & 0x7F) << 24) | ((tokenResult[1] & 0xFF) << 16) | ((tokenResult[2] & 0xFF) << 8) | (tokenResult[3] & 0xFF);
                }
            }
            return 0;
        }

        QByteArray hmac(const QByteArray& key, const QByteArray& message, const QCryptographicHash::Algorithm& hash)
        {
            QMessageAuthenticationCode mac(hash);
            mac.setKey(key);
            mac.addData(message);
            QByteArray a = mac.result();
            return a;
        }

    }
}