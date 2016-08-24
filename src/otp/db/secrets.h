#ifndef FACTORKEY_SECRETS_DB_H
#define FACTORKEY_SECRETS_DB_H

#include <kwallet.h>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class WalletManager
            {
            public:
                static const QString WALLET_FOLDER;
                WalletManager(const WId& window, const QString& m_walletName);
                virtual ~WalletManager();
                bool isOpened(void) const;
                bool open(void);
                bool close(void);
                bool contains(const QString & entry);
                bool entries(QStringList& entryList);
            private:
                const WId& m_window;
                const QString& m_walletName; // Wallet::LocalWallet()
                QScopedPointer<KWallet::Wallet> m_wallet;
            };
        }
    }
}
#endif
