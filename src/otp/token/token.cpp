#include "token.h"
#include "../base32/base32.h"

#include <QMessageAuthenticationCode>

namespace otp
{
    namespace token
    {
        Key keyForBase32(void)
        {
            return Key([](const QString& key) -> QByteArray
            {
                bool ok = false;
                QByteArray result = otp::base32::decode(key, &ok);
                return ok ? result : QByteArray();
            });
        }

        Key keyForCodec(const QTextCodec * codec)
        {
            return Key([codec](const QString& key) -> QByteArray
            {
                return codec ? codec->fromUnicode(key) : key.toUtf8();
            });
        }

        Message textMessage(const QString& message, const QTextCodec * codec)
        {
            return Message([message, codec](void) -> QByteArray
            {
                return codec ? codec->fromUnicode(message) : message.toUtf8();
            });
        }

        Algorithm hmacAlgorithm(const QCryptographicHash::Algorithm& hash)
        {
            return Algorithm([hash](const QByteArray& k, const QByteArray& m) -> QByteArray
            {
                return hmac(k, m, hash);
            });
        }

        QByteArray hmac(const QByteArray& key, const QByteArray& message, const QCryptographicHash::Algorithm& hash)
        {
            QMessageAuthenticationCode mac(hash);
            mac.setKey(key);
            mac.addData(message);
            QByteArray a = mac.result();
            return a;
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
            QByteArray k = key(password);
            QByteArray m = message();
            return token(k, m, algorithm, encoder);
        }

    }
}
