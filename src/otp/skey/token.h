#ifndef FACTORKEY_SKEY_TOKEN_H
#define FACTORKEY_SKEY_TOKEN_H


#include "../token/token.h"
#include <QCryptographicHash>
#include <QSharedPointer>

namespace otp
{
    namespace skey
    {
        class SKeyDictionary;

        namespace token
        {
            otp::token::Algorithm sKeyAlgorithm(quint64 rounds, const QCryptographicHash::Algorithm& hash = QCryptographicHash::Sha1);

            otp::token::Encoder defaultSKeyEncoder(const QCryptographicHash::Algorithm& hash);
            otp::token::Encoder sKeyEncoder(const QSharedPointer<otp::skey::SKeyDictionary> dict, const QCryptographicHash::Algorithm& hash);
        }
    }
}

#endif
