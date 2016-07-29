#include "token.h"
#include "skey.h"

namespace otp
{
    namespace skey
    {
        namespace token
        {
            otp::token::Algorithm skeyAlgorithm(quint64 rounds, const QCryptographicHash::Algorithm& hash)
            {
                return otp::token::Algorithm([hash, rounds](const QByteArray& k, const QByteArray& s) -> QByteArray
                {
                    return otp::skey::sKey(k, s, hash, rounds);
                });
            }

            otp::token::Encoder sKeyEncoder(const QSharedPointer<otp::skey::SKeyDictionary> dict, const QCryptographicHash::Algorithm& hash)
            {
                return otp::token::Encoder([hash, dict](const QByteArray& token) -> QString
                {
                    QStringList words = otp::skey::encodeSKeyWords(otp::skey::sKeyTokenValue(token, hash), dict.data());
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

            otp::token::Encoder defaultSKeyEncoder(const QCryptographicHash::Algorithm& hash)
            {
                QSharedPointer<otp::skey::DefaultDictionary> dict(otp::skey::defaultDictionary());
                return sKeyEncoder(dict, hash);
            }
        }
    }
}
