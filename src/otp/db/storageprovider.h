#ifndef FACTORKEY_DB_STORAGEPROVIDER_H
#define FACTORKEY_DB_STORAGEPROVIDER_H

#include "../storage/storageprovider.h"
#include "metadb.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class DefaultStorageProviderPrivate;

            class DefaultStorageProvider: public otp::storage::StorageProvider
            {
                Q_OBJECT
            public:
                DefaultStorageProvider(const MetadataDbBuilder& dbInfo, QObject * parent = 0);
            };
        }
    }
}
#endif
