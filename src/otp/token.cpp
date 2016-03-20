#include "token.h"
#include <QMessageAuthenticationCode>

namespace otp
{
    Algorithm hmacAlgorithm(const QCryptographicHash::Algorithm& hash)
    {
        return Algorithm([&hash](const QByteArray& k, const QByteArray& m) -> QByteArray
        {
            return hmac(k, m, hash);
        });
    }

    Key createKey(const QTextCodec * codec)
    {
        return Key([codec](const QString& key) -> QByteArray
        {
            return codec ? codec->fromUnicode(key) : key.toUtf8();
        });
    }

    Message totpMessage(const QDateTime& epoch, quint64 windowDurationMSec)
    {
        return totpMessage(epoch.toMSecsSinceEpoch(), windowDurationMSec);
    }

    Message totpMessage(qint64 epoch, quint64 windowDurationMSec)
    {
        return Message([epoch, windowDurationMSec](void) -> QByteArray
        {
            return hotpTokenMessage(countTokens(epoch, windowDurationMSec));
        });
    }

    Message dummyMessage(const QString& message, const QTextCodec * codec)
    {
        return Message([message, codec](void) -> QByteArray
        {
            return codec->fromUnicode(message);
        });
    }

    Encoder otpEncoder(uint length, const QLocale& locale)
    {
        return Encoder([length, locale](const QByteArray& token) -> QString
        {
            return encodeOTPToken(token, locale, length);
        });
    }

    Encoder otpEncoder(uint length)
    {
        QLocale locale = QLocale::c();
        locale.setNumberOptions(QLocale::OmitGroupSeparator);
        return Encoder([length, locale](const QByteArray& token) -> QString
        {
            return encodeOTPToken(token, locale, length);
        });
    }

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
        auto length = 16;
        QByteArray result;
        result.reserve(length);
        return result.setNum(counter, 16).rightJustified(length, '0', true);
    }

    QString encodeDummyFormat(const QByteArray& token)
    {
        return QLatin1String(token.toHex());
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

    QString encodeSteamGuardToken(const QByteArray& token)
    {
        return encodeSteamGuardToken(hotpTokenValue(token));
    }

    QString encodeSteamGuardToken(quint32 value)
    {
        static auto STEAM_TOKEN_LENGTH = 5;
        static const QChar STEAM_ALPHABET[] {
            QLatin1Char('2'),
            QLatin1Char('3'),
            QLatin1Char('4'),
            QLatin1Char('5'),
            QLatin1Char('6'),
            QLatin1Char('7'),
            QLatin1Char('8'),
            QLatin1Char('9'),
            QLatin1Char('B'),
            QLatin1Char('C'),
            QLatin1Char('D'),
            QLatin1Char('F'),
            QLatin1Char('G'),
            QLatin1Char('H'),
            QLatin1Char('J'),
            QLatin1Char('K'),
            QLatin1Char('M'),
            QLatin1Char('N'),
            QLatin1Char('P'),
            QLatin1Char('Q'),
            QLatin1Char('R'),
            QLatin1Char('T'),
            QLatin1Char('V'),
            QLatin1Char('W'),
            QLatin1Char('X'),
            QLatin1Char('Y')
        };
        static auto STEAM_ALPHABET_SIZE = sizeof(STEAM_ALPHABET) / sizeof(const QChar);

        QString result(QLatin1String(""));

        for(int i = 0; i < STEAM_TOKEN_LENGTH; ++i)
        {
            result += STEAM_ALPHABET[value % STEAM_ALPHABET_SIZE];
            value /= STEAM_ALPHABET_SIZE;
        }

        return result;
    }

    quint32 hotpTokenValue(const QByteArray& token)
    {
        auto length = token.size();
        if(length > 0)
        {
            /*
             * Truncate the HMAC down to (at most) 8 bytes long sequence.
             * The last nibble is interpreted as the starting index of the sequence within the HMAC.
             * This should yield values between 0 and 15 for the offset.
             */
            auto truncated = token.mid(0x0F & token.at(length - 1), sizeof(quint32));

            /*
             * Interpret the (at most) 4 byte truncated sequence as a (big endian) unsigned integer.
             * To avoid endianness issues, use hex encoding -> hex decoding.
             */
            auto ok = false;
            auto hexed = truncated.toHex();

            /* mask out the high order bit as per HOTP spec */
            quint32 value = hexed.toULong(&ok, 16) &  0x7FFFFFFFU;

            if(ok)
            {
                return value;
            }
        }
        return 0;
    }

    QByteArray hmac(const QByteArray& key, const QByteArray& message, const QCryptographicHash::Algorithm& hash)
    {
        QMessageAuthenticationCode mac(hash);
        mac.setKey(key);
        mac.addData(message);
        return mac.result();
    }

    QString token(const QByteArray& key, const QByteArray& message,
                  const Algorithm& algorithm, const Encoder& encoder)
    {
        auto result = algorithm(key, message);
        return encoder(result);
    }

    QString token(const QString& password,
                  const Message& message, const Key& key, const Algorithm& algorithm, const Encoder& encoder)
    {
        return token(key(password), message(), algorithm, encoder);
    }
}