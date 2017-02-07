#ifndef FACTORKEY_STEAM_GENERATOR_H
#define FACTORKEY_STEAM_GENERATOR_H

#include "../generator.h"
#include "../storage/storageprovider.h"

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
                static SteamGuardParameters * from(otp::storage::Storage * store, QObject * parent = 0);
                static SteamGuardParameters * create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent = 0);
                otp::generator::TokenGenerator * generator(QObject * parent = nullptr) override;
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
