#ifndef FACTORKEY_STEAM_GENERATOR_H
#define FACTORKEY_STEAM_GENERATOR_H

#include "../generator.h"

namespace otp
{
    namespace steam
    {
        namespace generator
        {
            class SteamGuardParameters: public otp::generator::TokenParameters
            {
                Q_OBJECT
            public:
                static SteamGuardParameters * create(otp::storage::Storage * store, QObject * parent = 0);
                static otp::generator::TokenGenerator * generator(SteamGuardParameters * params, QObject * parent = 0);
            public:
                virtual ~SteamGuardParameters();
            protected:
                SteamGuardParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            private:
                static const bool isRegistered;
            };
        }
    }
}

#endif
