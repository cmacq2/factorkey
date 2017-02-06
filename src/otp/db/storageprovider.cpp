#include "storageprovider.h"
#include "storageprovider_p.h"

namespace otp
{
    namespace storage
    {
        namespace db
        {
            DbStorageProvider::DbStorageProvider(const QSharedPointer<MetadataDbBuilder> dbInfo, const QSharedPointer<otp::secrets::SecretsAPIBuilder> secretsInfo, QObject * parent):
                otp::storage::StorageProvider(new otp::storage::db::DbStorageProviderPrivate(dbInfo, secretsInfo), parent) {}
        }
    }
}
