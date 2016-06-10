#include "token.h"

namespace otp
{
    namespace token
    {

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