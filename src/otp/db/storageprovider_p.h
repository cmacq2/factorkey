#ifndef FACTORKEY_DB_STORAGEPROVIDER_PRIVATE_H
#define FACTORKEY_DB_STORAGEPROVIDER_PRIVATE_H

#include "../storage/storageprovider.h"
#include "../storage/storage_p.h"
#include "metadb.h"
#include "../secrets/secrets.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class DbStorageProviderPrivate: public otp::storage::StorageProviderPrivate
            {
            public:
                DbStorageProviderPrivate(const QSharedPointer<MetadataDbBuilder> dbInfo, const QSharedPointer<otp::secrets::SecretsAPIBuilder> secretsInfo);
                virtual ~DbStorageProviderPrivate();
                bool contains(const QString& entry);
                bool entries(QStringList& entryList);
            protected:
                bool isOpened(void);
                bool openBackend(void);
                bool closeBackend(void);
                otp::storage::Storage * doCreate(const QString& entry, OTPTokenType type);
                otp::storage::Storage * doLookup(const QString& entry);
            private:
                const QSharedPointer<MetadataDbBuilder> m_dbInfo;
                const QSharedPointer<otp::secrets::SecretsAPIBuilder> m_secretsInfo;
                QSharedPointer<otp::secrets::SecretsAPIProvider> m_secrets;
                QSharedPointer<MetadataDbManager> m_metaDb;
            };
        }
    }
}
#endif
