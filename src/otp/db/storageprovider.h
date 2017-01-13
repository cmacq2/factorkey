#ifndef FACTORKEY_DB_STORAGEPROVIDER_H
#define FACTORKEY_DB_STORAGEPROVIDER_H

#include "../storage/storageprovider.h"
#include "metadb.h"
#include "../secrets/secrets.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class DbStorageProvider: public otp::storage::StorageProvider
            {
                Q_OBJECT
            public:
                DbStorageProvider(const MetadataDbBuilder& dbInfo, const otp::storage::secrets::SecretsAPIBuilder& secretsInfo, QObject * parent = 0);
            };
        }
    }
}
#endif
