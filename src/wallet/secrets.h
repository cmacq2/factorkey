#ifndef FACTORKEY_SECRETS_WALLET_H
#define FACTORKEY_SECRETS_WALLET_H

#include <kwallet.h>
#include "otp/secrets/secrets.h"

namespace otp
{
    namespace secrets
    {
        namespace wallet
        {
            class WalletManager: public otp::secrets::SecretsAPIProvider
            {
            public:
                static const QString WALLET_FOLDER;
                WalletManager(const WId& window, const QString& m_walletName);
                virtual ~WalletManager();
                bool ask(const QString& entryId, const otp::secrets::SecretsAPIProvider::SecretAnswer answer) override;
                bool tell(const QString& entryId, const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation confirm) override;
                bool isOpened(void) const override;
                bool open(void) override;
                bool close(void) override;
                bool readPassword(const QString& entryId, QString& secret);
                bool writePassword(const QString& entryId, const QString& secret);
                bool contains(const QString & entry);
                bool entries(QStringList& entryList);
            private:
                const WId& m_window;
                const QString& m_walletName;
                QScopedPointer<KWallet::Wallet> m_wallet;
            };

            class WalletBuilder: public otp::secrets::SecretsAPIBuilder
            {
            public:
                void setWindow(const WId& m_window);
                void setName(const QString& walletName);
            protected:
                WalletManager * build(void) const override;
            private:
                WId m_window;
                QString m_walletName;
            };
        }
    }
}
#endif
