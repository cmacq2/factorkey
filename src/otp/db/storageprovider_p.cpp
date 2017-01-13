#include "storageprovider_p.h"

#include <QSharedPointer>

#include "../storage/storage.h"
#include "../storage/storage_p.h"
#include "metadata.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class DbStoragePrivate: public otp::storage::StoragePrivate
            {
            public:
                DbStoragePrivate(QSharedPointer<otp::storage::secrets::SecretsAPIProvider>& secretBackend, QSharedPointer<MetadataDbManager>& metaBackend, const QString& entry):
                otp::storage::StoragePrivate(), m_wallet(secretBackend), m_metadata(entry, metaBackend), m_entryId(entry)
                {

                }
                virtual ~DbStoragePrivate() {}

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

                bool writePassword(const QString& secret, const otp::storage::secrets::SecretsAPIProvider::SecretConfirmation& confirmation)
                {
                    return m_wallet && m_wallet->tell(m_entryId, secret, confirmation);
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

                bool readPassword(const otp::storage::secrets::SecretsAPIProvider::SecretAnswer& secret) const
                {
                    return m_wallet && m_wallet->ask(m_entryId, secret);
                }

                bool writeParam(const QString& param, const QVariant& value)
                {
                    return m_metadata.writeParam(param, value);
                }

                bool exists(void) const
                {
                    return m_metadata.exists();
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
                QSharedPointer<otp::storage::secrets::SecretsAPIProvider> m_wallet;
                otp::storage::db::Metadata m_metadata;
                const QString m_entryId;
            };

            DbStorageProviderPrivate::DbStorageProviderPrivate(const MetadataDbBuilder& dbInfo, const otp::storage::secrets::SecretsAPIBuilder& secretsInfo) :
                otp::storage::StorageProviderPrivate(), m_dbInfo(dbInfo), m_secretsInfo(secretsInfo) {}
            DbStorageProviderPrivate::~DbStorageProviderPrivate() {}

            bool DbStorageProviderPrivate::contains(const QString& entry)
            {
                QString id(entry);
                return open() && m_metaDb->contains(id);
            }

            bool DbStorageProviderPrivate::entries(QStringList& entryList)
            {
                return open() && m_metaDb->entries(entryList);
            }

            bool DbStorageProviderPrivate::isOpened(void)
            {
                return m_wallet && m_wallet->isOpened() && m_metaDb && m_metaDb->isOpened();
            }

            bool DbStorageProviderPrivate::openBackend(void)
            {
                m_wallet = m_secretsInfo.create();
                m_metaDb = m_dbInfo.create();
                QSqlDatabase conn = m_metaDb->open();
                return conn.isValid() && conn.isOpen() && m_wallet->open();
            }

            bool DbStorageProviderPrivate::closeBackend(void)
            {
                if(m_metaDb && m_wallet &&isOpened())
                {
                    bool result = m_wallet->close();
                    return m_metaDb->close() && result;
                }
                return true;
            }

            otp::storage::Storage * DbStorageProviderPrivate::doCreate(const QString& entry, OTPTokenType type)
            {
                otp::storage::Storage * s = new otp::storage::Storage(new DbStoragePrivate(m_wallet, m_metaDb, entry));
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

            otp::storage::Storage * DbStorageProviderPrivate::doLookup(const QString& entry)
            {
                otp::storage::Storage * s = new otp::storage::Storage(new DbStoragePrivate(m_wallet, m_metaDb, entry));
                if(s->poll())
                {
                    return s;
                }
                else {
                    delete s;
                    return nullptr;
                }
            }
        }
    }
}
