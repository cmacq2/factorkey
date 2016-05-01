#ifndef FACTORKEY_TOKEN_H
#define FACTORKEY_TOKEN_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QLocale>
#include <QString>
#include <QTextCodec>

#include <functional>

namespace otp
{
    typedef std::function<QByteArray(const QByteArray&, const QByteArray&)> Algorithm;
    typedef std::function<QString(const QByteArray&)> Encoder;
    typedef std::function<QByteArray(const QString&)> Key;
    typedef std::function<QByteArray()> Message;

    static const uint DEFAULT_OTP_LENGTH = 6;
    static const quint64 DEFAULT_TIMESTEP_MSEC = 30000U;
    static const qint64 DEFAULT_EPOCH = 0;

    Key keyForCodec(const QTextCodec * codec = nullptr);
    Key keyForAuthenticator(void);

    Algorithm hmacAlgorithm(const QCryptographicHash::Algorithm& hash = QCryptographicHash::Sha1);
    Message totpMessage(const QDateTime& epoch, quint64 windowDurationMSec = 0);
    Message totpMessage(qint64 epoch = 0, quint64 windowDurationMSec = 0);
    Message dummyMessage(const QString& message, const QTextCodec * codec);
    Encoder otpEncoder(uint length, const QLocale& locale);
    Encoder otpEncoder(uint length = 0);

    QString encodeOTPToken(quint32 value, const QLocale& locale = QLocale::c(), uint length = 0);
    QString encodeOTPToken(const QByteArray& token, const QLocale& locale = QLocale::c(), uint length = 0);
    QString encodeSteamGuardToken(quint32 value);
    QString encodeSteamGuardToken(const QByteArray& token);
    QString encodeDummyFormat(const QByteArray& token);

    /**
     * \brief Truncates a raw token byte string/hash.
     * \param token the byte string representing the computed HOTP/TOTP HMAC value.
     * \return 32 bits selected from the given hash.
     */
    quint32 hotpTokenValue(const QByteArray& token);

    QByteArray hmac(const QByteArray& key,
                    const QByteArray& message,
                    const QCryptographicHash::Algorithm& hash);

    QByteArray hotpTokenMessage(quint64 counter);

    quint64 countTokens(qint64 epoch = 0, quint64 timeStepMSec = 0U);
    quint64 countTokens(qint64 currentMSec, qint64 epoch = 0, quint64 timeStepMSec = 0U);

    QString token(const QByteArray& key,
                  const QByteArray& message,
                  const Algorithm& algorithm,
                  const Encoder& encoding);

    QString token(const QString& password,
                  const Message& message,
                  const Key& key,
                  const Algorithm& algorithm,
                  const Encoder& encoding);

}
#endif