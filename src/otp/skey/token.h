#ifndef FACTORKEY_SKEY_TOKEN_H
#define FACTORKEY_SKEY_TOKEN_H


#include "../token/token.h"
#include <QCryptographicHash>

namespace otp
{
    namespace skey
    {
        class SKeyDictionary;

        namespace token
        {
            otp::token::Algorithm sKeyAlgorithm(quint64 rounds, const QCryptographicHash::Algorithm& hash = QCryptographicHash::Sha1);

            otp::token::Encoder defaultWordsEncoder(const QCryptographicHash::Algorithm& hash);
            otp::token::Encoder wordsEncoder(const QCryptographicHash::Algorithm& hash, const otp::skey::SKeyDictionary * dict);
            otp::token::Encoder hexEncoder(const QCryptographicHash::Algorithm& hash);
        }
    }
}

#endif
