#include "token.h"
#include "skey.h"

namespace otp
{
    namespace skey
    {
        otp::token::Algorithm skeyAlgorithm(quint64 rounds, const QCryptographicHash::Algorithm& hash)
        {
            return otp::token::Algorithm([hash, rounds](const QByteArray& k, const QByteArray& s) -> QByteArray
            {
                return skey::sKey(k, s, hash, rounds);
            });
        }

        otp::token::Encoder sKeyEncoder(const QSharedPointer<SKeyDictionary> dict, const QCryptographicHash::Algorithm& hash)
        {
            return otp::token::Encoder([hash, dict](const QByteArray& token) -> QString
            {
                QStringList words = encodeSKeyWords(sKeyTokenValue(token, hash), dict);
                QString format(QStringLiteral("%1 %2 %3 %4 %5 %6"));
                return format.
                    arg(words.at(0)).
                    arg(words.at(1)).
                    arg(words.at(2)).
                    arg(words.at(3)).
                    arg(words.at(4)).
                    arg(words.at(5));
            });
        }

        otp::token::Encoder defaultSKeyEncoder(const QCryptographicHash& hash)
        {
            QSharedPointer<SKeyDictionary> dict(defaultDictionary());
            return sKeyEncoder(dict, hash);
        }
    }
}
