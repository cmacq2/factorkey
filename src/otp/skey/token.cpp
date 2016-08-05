#include "token.h"
#include "skey.h"

#include <QSharedPointer>
#include <QtDebug>

namespace otp
{
    namespace skey
    {
        namespace token
        {
            otp::token::Algorithm sKeyAlgorithm(quint64 rounds, const QCryptographicHash::Algorithm& hash)
            {
                return otp::token::Algorithm([hash, rounds](const QByteArray& k, const QByteArray& s) -> QByteArray
                {
                    return otp::skey::sKey(k, s, hash, rounds);
                });
            }

            otp::token::Encoder hexEncoder(const QCryptographicHash::Algorithm& hash)
            {
                return otp::token::Encoder([hash](const QByteArray& token) -> QString
                {
                    quint64 v = otp::skey::sKeyTokenValue(token, hash);
                    QString format(QStringLiteral("%1"));
                    return format.arg(v, 16, 16).toUpper();
                });
            }

            static QString encodeSKeyWords(const QByteArray& token, const QCryptographicHash::Algorithm& hash, const otp::skey::SKeyDictionary * dict)
            {
                QStringList words = otp::skey::encodeSKeyWords(otp::skey::sKeyTokenValue(token, hash), dict);
                QString format(QStringLiteral("%1 %2 %3 %4 %5 %6"));
                return format.arg(words.at(0)).arg(words.at(1)).arg(words.at(2)).arg(words.at(3)).arg(words.at(4)).arg(words.at(5));
            }

            otp::token::Encoder wordsEncoder(const QCryptographicHash::Algorithm& hash, const otp::skey::SKeyDictionary * dict)
            {
                return otp::token::Encoder([hash, dict](const QByteArray& token) -> QString
                {
                    return encodeSKeyWords(token, hash, dict);
                });
            }

            otp::token::Encoder defaultWordsEncoder(const QCryptographicHash::Algorithm& hash)
            {
                QSharedPointer<otp::skey::DefaultDictionary> dict(otp::skey::defaultDictionary());
                return otp::token::Encoder([hash, dict](const QByteArray& token) -> QString
                {
                    return encodeSKeyWords(token, hash, dict.data());
                });
            }
        }
    }
}
