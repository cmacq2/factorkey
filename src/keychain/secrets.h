#ifndef FACTORKEY_SECRETS_KEYCHAIN_H
#define FACTORKEY_SECRETS_KEYCHAIN_H

#include <qt5keychain/keychain.h>
#include "otp/secrets/secrets.h"

namespace otp
{
    namespace secrets
    {
        namespace keychain
        {
            class KeychainManager: public otp::secrets::SecretsAPIProvider
            {
            public:
                KeychainManager(const QString& serviceName);
                bool ask(const QString& entryId, const otp::secrets::SecretsAPIProvider::SecretAnswer answer);
                bool tell(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm);
                bool isOpened(void) const;
                bool open(void);
                bool close(void);
            private:
                const QString& m_serviceName;
                bool m_opened;
            };

            class KeychainBuilder: public otp::secrets::SecretsAPIBuilder
            {
            public:
                void setName(const QString& secretsDomain);
            protected:
                KeychainManager * build(void) const;
            private:
                QString m_serviceName;
            };
        }
    }
}
#endif
