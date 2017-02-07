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
                DbStoragePrivate(QSharedPointer<otp::secrets::SecretsAPIProvider> secretBackend, QSharedPointer<MetadataDbManager> metaBackend, const QString& entry):
                    otp::storage::StoragePrivate(), m_secrets(secretBackend), m_metadata(entry, metaBackend), m_entryId(entry) {}
                virtual ~DbStoragePrivate() {}

                QString entryId(void) const override
                {
                    return m_entryId;
                }

                OTPTokenType type(void) const override
                {
                    otp::storage::OTPTokenType type;
                    m_metadata.readTokenType(type);
                    return type;
                }

                bool writePassword(const QString& secret, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation) override
                {
                    return m_secrets && m_secrets->tell(m_entryId, secret, confirmation);
                }

                bool readTokenType(OTPTokenType& type) const override
                {
                    return m_metadata.readTokenType(type);
                }

                bool writeTokenType(const OTPTokenType& type) override
                {
                    return m_metadata.writeTokenType(type);
                }

                bool readParam(const QString& param, QVariant& value) const override
                {
                    return m_metadata.readParam(param, value);
                }

                bool readPassword(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret) const override
                {
                    return m_secrets && m_secrets->ask(m_entryId, secret);
                }

                bool writeParam(const QString& param, const QVariant& value) override
                {
                    return m_metadata.writeParam(param, value);
                }

                bool exists(void) const override
                {
                    return m_metadata.exists();
                }

                bool commit(void) override
                {
                    return m_metadata.commit();
                }
                bool poll(void) override
                {
                    return m_metadata.poll();
                }
            private:
                QSharedPointer<otp::secrets::SecretsAPIProvider> m_secrets;
                otp::storage::db::Metadata m_metadata;
                const QString m_entryId;
            };

            DbStorageProviderPrivate::DbStorageProviderPrivate(const QSharedPointer<MetadataDbBuilder> dbInfo, const QSharedPointer<otp::secrets::SecretsAPIBuilder> secretsInfo) :
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
                return m_secrets && m_secrets->isOpened() && m_metaDb && m_metaDb->isOpened();
            }

            bool DbStorageProviderPrivate::openBackend(void)
            {
                if(!m_secrets && m_secretsInfo)
                {
                    m_secrets = m_secretsInfo->create();
                }
                if(!m_metaDb && m_dbInfo)
                {
                    m_metaDb = m_dbInfo->create();
                }
                if(m_metaDb && m_secrets)
                {
                    QSqlDatabase conn = m_metaDb->open();
                    return conn.isValid() && conn.isOpen() && m_secrets->open();
                }
                else
                {
                    if(m_metaDb)
                    {
                        m_metaDb = QSharedPointer<MetadataDbManager>();
                    }

                    if(m_secrets)
                    {
                        m_secrets = QSharedPointer<otp::secrets::SecretsAPIProvider>();
                    }

                    return false;
                }
            }

            bool DbStorageProviderPrivate::closeBackend(void)
            {
                if(m_metaDb && m_secrets &&isOpened())
                {
                    bool result = m_secrets->close();
                    return m_metaDb->close() && result;
                }
                return true;
            }

            otp::storage::Storage * DbStorageProviderPrivate::doCreate(const QString& entry, OTPTokenType type)
            {
                otp::storage::Storage * s = new otp::storage::Storage(new DbStoragePrivate(m_secrets, m_metaDb, entry));
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
                otp::storage::Storage * s = new otp::storage::Storage(new DbStoragePrivate(m_secrets, m_metaDb, entry));
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
