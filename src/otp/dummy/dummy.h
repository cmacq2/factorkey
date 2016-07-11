#ifndef FACTORKEY_DUMMY_TOKEN_H
#define FACTORKEY_DUMMY_TOKEN_H

#include <QByteArray>
#include <QString>

namespace otp
{
    namespace dummy
    {
        QString encodeDummyFormat(const QByteArray& token);
    }
}
#endif
