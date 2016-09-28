#include "storageprovider.h"

#include "secrets.h"
#include "metadata.h"

#include "../storage/storage.h"
#include "../storage/storage_p.h"

#include <QSharedPointer>

namespace otp
{
    namespace storage
    {
        namespace db
        {

            class DefaultStoragePrivate: public otp::storage::StoragePrivate
            {
            public:
                DefaultStoragePrivate(QSharedPointer<WalletManager>& secretBackend, QSharedPointer<MetadataDbManager>& metaBackend, const QString& entry):
                    otp::storage::StoragePrivate(), m_wallet(secretBackend), m_metadata(entry, metaBackend), m_entryId(entry)
                {

                }
                virtual ~DefaultStoragePrivate() {}

                QString entryId(void) const
                {
                    return m_entryId;
                }

                OTPTokenType type(void) const
                {
                    otp::storage::OTPTokenType type;
                    m_metadata.readTokenType(type);
                    return type;
                }

                bool writePassword(const QString&)
                {
                    return false;
                }

                bool readTokenType(OTPTokenType& type) const
                {
                    return m_metadata.readTokenType(type);
                }

                bool writeTokenType(const OTPTokenType& type)
                {
                    return m_metadata.writeTokenType(type);
                }

                bool readParam(const QString& param, QVariant& value) const
                {
                    return m_metadata.readParam(param, value);
                }

                bool readPassword(QString&) const
                {
                    return false;
                }

                bool writeParam(const QString& param, const QVariant& value)
                {
                    return m_metadata.writeParam(param, value);
                }

                bool exists(void) const
                {
                    return m_wallet && m_wallet->contains(entryId());
                }

                bool commit(void)
                {
                    return m_metadata.commit();
                }
                bool poll(void)
                {
                    return m_metadata.poll();
                }
            private:
                QSharedPointer<WalletManager> m_wallet;
                otp::storage::db::Metadata m_metadata;
                const QString m_entryId;
            };

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
                    if(m_metaDb && m_wallet &&isOpened())
                    {
                        bool result = m_wallet->close();
                        return m_metaDb->close() && result;
                    }
                    return true;
                }

                otp::storage::Storage * doCreate(const QString& entry, OTPTokenType type)
                {
                    otp::storage::Storage * s = new otp::storage::Storage(new DefaultStoragePrivate(m_wallet, m_metaDb, entry));
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
                    otp::storage::Storage * s = new otp::storage::Storage(new DefaultStoragePrivate(m_wallet, m_metaDb, entry));
                    if(s->poll())
                    {
                        return s;
                    }
                    else {
                        delete s;
                        return nullptr;
                    }
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
