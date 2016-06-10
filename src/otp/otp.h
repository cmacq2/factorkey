#ifndef FACTORKEY_OTP_H
#define FACTORKEY_OTP_H

#include "token/token.h"
#include "oath/token.h"
#include "skey/token.h"

#include <QByteArray>
#include <QString>

namespace otp
{
    QString encodeDummyFormat(const QByteArray& token);
}
#endif
