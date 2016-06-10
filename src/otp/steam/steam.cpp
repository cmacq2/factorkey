#include "steam.h"
#include "../oath/oath.h"

namespace otp
{
    namespace steam
    {
        QString encodeSteamGuardToken(const QByteArray& token)
        {
            return encodeSteamGuardToken(oath::hotpTokenValue(token));
        }

        QString encodeSteamGuardToken(quint32 value)
        {
            static auto STEAM_TOKEN_LENGTH = 5;
            static const QChar STEAM_ALPHABET[] {
                QLatin1Char('2'),
                QLatin1Char('3'),
                QLatin1Char('4'),
                QLatin1Char('5'),
                QLatin1Char('6'),
                QLatin1Char('7'),
                QLatin1Char('8'),
                QLatin1Char('9'),
                QLatin1Char('B'),
                QLatin1Char('C'),
                QLatin1Char('D'),
                QLatin1Char('F'),
                QLatin1Char('G'),
                QLatin1Char('H'),
                QLatin1Char('J'),
                QLatin1Char('K'),
                QLatin1Char('M'),
                QLatin1Char('N'),
                QLatin1Char('P'),
                QLatin1Char('Q'),
                QLatin1Char('R'),
                QLatin1Char('T'),
                QLatin1Char('V'),
                QLatin1Char('W'),
                QLatin1Char('X'),
                QLatin1Char('Y')
            };
            static auto STEAM_ALPHABET_SIZE = sizeof(STEAM_ALPHABET) / sizeof(const QChar);

            QString result(QLatin1String(""));

            for(int i = 0; i < STEAM_TOKEN_LENGTH; ++i)
            {
                result += STEAM_ALPHABET[value % STEAM_ALPHABET_SIZE];
                value /= STEAM_ALPHABET_SIZE;
            }

            return result;
        }
    }
}
