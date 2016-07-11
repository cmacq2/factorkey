#ifndef FACTORKEY_TOKEN_H
#define FACTORKEY_TOKEN_H

#include <QByteArray>
#include <QDateTime>
#include <QLocale>
#include <QString>
#include <QTextCodec>

#include <functional>

namespace otp
{
    namespace token
    {
        typedef std::function<QByteArray(const QByteArray&, const QByteArray&)> Algorithm;
        typedef std::function<QString(const QByteArray&)> Encoder;
        typedef std::function<QByteArray(const QString&)> Key;
        typedef std::function<QByteArray()> Message;

        Key keyForCodec(const QTextCodec * codec = nullptr);
        Key keyForBase32(void);

        Message textMessage(const QString& message, const QTextCodec * codec = nullptr);

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
}

#endif
