#include "storageprovider.h"
#include "storage_p.h"

namespace otp
{
    class WalletStorageProvider: public StorageProviderPrivate
    {
    public:
        WalletStorageProvider() : StorageProviderPrivate() {}
        virtual ~WalletStorageProvider() {}
//         static const QString WALLET_FOLDER;
        bool contains(const QString& entry)
        {
            QString id(entry);
            return open() && m_wallet->contains(id);
        }

        bool entries(QStringList& entryList)
        {
            return open() && m_wallet->entries(entryList);
        }

    protected:

        bool isOpened(void)
        {
            return m_wallet && m_wallet->isOpened();
        }

        bool openBackend(void)
        {
//             if(m_wallet)
//             {
//                 // todo emit signal?
//             }
            m_wallet.reset(new internal::WalletManager(m_window, m_walletName));
//             QSharedPointer<otp::internal::WalletManager> w(new internal::WalletManager(m_window, m_walletName));
//             m_wallet = w;
            return m_wallet->open();
        }

        bool closeBackend(void)
        {
            return isOpened() ? m_wallet->close() : true;
        }

        Storage * doCreate(const QString& entry, OTPTokenType type)
        {
            Storage * s = doLookup(entry);
            if(s)
            {
                if(s->writeTokenType(type) && s->commit())
                {
                    return s;
                }
                delete s;
            }
            return nullptr;
        }

        Storage * doLookup(const QString& entry)
        {
            return otp::internal::createStorageFrom(m_wallet, entry);
        }

    private:
        WId m_window;
        const QString m_walletName; // Wallet::LocalWallet()
        QSharedPointer<otp::internal::WalletManager> m_wallet;
    };

    StorageProvider::StorageProvider(StorageProviderPrivate * d, QObject * parent) : QObject(parent), d_ptr(d) {}
    StorageProvider::~StorageProvider() {}

    bool StorageProvider::open(void)
    {
        Q_D(StorageProvider);
        return d->open();
    }

    bool StorageProvider::close(void)
    {
        Q_D(StorageProvider);
        return d->close();
    }

    bool StorageProvider::contains(const QString & id)
    {
        Q_D(StorageProvider);
        return d->contains(id);
    }

    bool StorageProvider::entries(QStringList& entryList)
    {
        Q_D(StorageProvider);
        QStringList l;
        if(d->entries(l))
        {
            entryList = l;
            return true;
        }
        else
        {
            return false;
        }
    }

    Storage * StorageProvider::lookup(const QString& id)
    {
        Q_D(StorageProvider);
        return d->lookup(id);
    }

    Storage * StorageProvider::create(const QString& id, OTPTokenType type)
    {
        Q_D(StorageProvider);
        return d->create(id, type);
    }

}