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
                DbStorageProvider(const QSharedPointer<MetadataDbBuilder> dbInfo, const QSharedPointer<otp::secrets::SecretsAPIBuilder> secretsInfo, QObject * parent = 0);
            };
        }
    }
}
#endif
