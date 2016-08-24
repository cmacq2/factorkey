#include "storageprovider.h"

#include "secrets.h"
#include "metadata.h"

#include "../storage/storage_p.h"

#include <QSharedPointer>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            otp::storage::Storage * createStorageFrom(QSharedPointer<WalletManager>& secretBackend, QSharedPointer<MetadataDbManager>& metaBackend, const QString& entry);

            class DefaultStorageProviderPrivate: public otp::storage::StorageProviderPrivate
            {
            public:
                DefaultStorageProviderPrivate() : otp::storage::StorageProviderPrivate() {}
                virtual ~DefaultStorageProviderPrivate() {}
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
                    return m_wallet && m_wallet->isOpened() && m_metaDb && m_metaDb->isOpened();
                }

                bool openBackend(void)
                {
                    //             if(m_wallet)
                    //             {
                    //                 // todo emit signal?
                    //             }
                    m_wallet.reset(new WalletManager(m_window, m_walletName));
                    m_metaDb.reset(new MetadataDbManager());
                    return m_metaDb->open() && m_wallet->open();
                }

                bool closeBackend(void)
                {
                    if(isOpened())
                    {
                        bool result = m_wallet->close();
                        return m_metaDb->close() && result;
                    }
                    return true;
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

                otp::storage::Storage * doLookup(const QString& entry)
                {
                    return createStorageFrom(m_wallet, m_metaDb, entry);
                }

            private:
                WId m_window;
                const QString m_walletName; // Wallet::LocalWallet()
                QSharedPointer<WalletManager> m_wallet;
                QSharedPointer<MetadataDbManager> m_metaDb;
            };

            DefaultStorageProvider::DefaultStorageProvider(QObject * parent): otp::storage::StorageProvider(new DefaultStorageProviderPrivate(), parent) {}
        }
    }
}
