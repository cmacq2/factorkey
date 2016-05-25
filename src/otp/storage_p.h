#ifndef FACTORKEY_STORAGE_PRIVATE_H
#define FACTORKEY_STORAGE_PRIVATE_H

#include <kwallet.h>
#include "storage.h"
#include <QSharedPointer>

namespace otp
{
    class StoragePrivate
    {
    public:
        virtual ~StoragePrivate();
        virtual QString entryId(void) const;
        virtual OTPTokenType type(void) const;
        virtual bool writePassword(const QString&);
        virtual bool readTokenType(OTPTokenType&) const;
        virtual bool writeTokenType(const OTPTokenType&);
        virtual bool readParam(const QString&, QVariant&) const;
        virtual bool readPassword(QString&) const;
        virtual bool writeParam(const QString&, const QVariant&) const;
        virtual bool exists(void) const;
        virtual bool commit(void);
        virtual bool poll(void);
    };

    class StorageProviderPrivate
    {
    public:
        virtual bool open(void);
        virtual bool close(void);
        virtual bool contains(const QString& entry);
        virtual bool entries(QStringList&);
        virtual Storage * lookup(const QString& entry);
        virtual Storage * create(const QString& entry, OTPTokenType type);
        virtual ~StorageProviderPrivate();
    protected:
        virtual Storage * doLookup(const QString&);
        virtual Storage * doCreate(const QString&, OTPTokenType);
        virtual bool isOpened(void);
        virtual bool openBackend(void);
        virtual bool closeBackend(void);
    };

    namespace internal
    {
        class WalletManager;

        Storage * createStorageFrom(QSharedPointer<WalletManager>& backend, const QString& entry);

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

#endif
