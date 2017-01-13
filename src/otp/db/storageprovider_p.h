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
                DbStorageProviderPrivate(const MetadataDbBuilder& dbInfo, const otp::storage::secrets::SecretsAPIBuilder& secretsInfo);
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
                const MetadataDbBuilder m_dbInfo;
                const otp::storage::secrets::SecretsAPIBuilder m_secretsInfo;
                QSharedPointer<otp::storage::secrets::SecretsAPIProvider> m_wallet;
                QSharedPointer<MetadataDbManager> m_metaDb;
            };
        }
    }
}
#endif
