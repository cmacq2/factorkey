#ifndef FACTORKEY_DB_STORAGEPROVIDER_H
#define FACTORKEY_DB_STORAGEPROVIDER_H

#include "../storage/storageprovider.h"

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
                DefaultStorageProvider(QObject * parent = 0);
            };
        }
    }
}
#endif
