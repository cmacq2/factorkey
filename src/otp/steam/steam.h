#ifndef FACTORKEY_STEAM_H
#define FACTORKEY_STEAM_H
#include <QByteArray>
#include <QString>

namespace otp
{
    namespace steam
    {
        QString encodeSteamGuardToken(quint32 value);
        QString encodeSteamGuardToken(const QByteArray& token);
    }
}
#endif
