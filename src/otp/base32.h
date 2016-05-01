#ifndef FACTORKEY_BASE32_H
#define FACTORKEY_BASE32_H

#include <QByteArray>
#include <QString>
#include <QTextCodec>

namespace otp
{
    namespace base32
    {
        QChar encodeByte(int idx);
        int decodeByte(const QChar chr);
        QByteArray decode(const QString& encoded, bool * ok);
        QString encode(const QByteArray& content);
        QString recode(const QString& content, const QTextCodec * codec = nullptr);
    }
}

#endif

