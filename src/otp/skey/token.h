#ifndef FACTORKEY_TOKEN_H
#define FACTORKEY_TOKEN_H

#include "../token/token.h"
#include <QCryptographicHash>

namespace otp
{
    namespace skey
    {

        otp::token::Algorithm sKeyAlgorithm(quint64 rounds, const QCryptographicHash::Algorithm& hash = QCryptographicHash::Sha1);

        class SKeyDictionary;

        otp::token::Encoder defaultSKeyEncoder(const QCryptographicHash::Algorithm& hash)
        otp::token::Encoder sKeyEncoder(const QSharedPointer<SKeyDictionary> dict, const QCryptographicHash::Algorithm& hash);
    }
}

#endif
