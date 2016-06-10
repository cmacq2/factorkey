#ifndef FACTORKEY_OATH_H
#define FACTORKEY_OATH_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QLocale>
#include <QString>

namespace otp
{
    namespace oath
    {

        static const uint DEFAULT_OTP_LENGTH = 6;
        static const quint64 DEFAULT_TIMESTEP_MSEC = 30000U;
        static const qint64 DEFAULT_EPOCH = 0;

        /**
         * \brief Truncates a raw token byte string/hash.
         * \param token the byte string representing the computed HOTP/TOTP HMAC value.
         * \return 32 bits selected from the given hash.
         */
        quint32 hotpTokenValue(const QByteArray& token);

        QString encodeOTPToken(quint32 value, const QLocale& locale = QLocale::c(), uint length = 0);
        QString encodeOTPToken(const QByteArray& token, const QLocale& locale = QLocale::c(), uint length = 0);

        QByteArray hotpTokenMessage(quint64 counter);

        quint64 countTokens(qint64 epoch = 0, quint64 timeStepMSec = 0U);
        quint64 countTokens(qint64 currentMSec, qint64 epoch = 0, quint64 timeStepMSec = 0U);

        QByteArray hmac(const QByteArray& key,
                        const QByteArray& message,
                        const QCryptographicHash::Algorithm& hash);
    }
}
#endif