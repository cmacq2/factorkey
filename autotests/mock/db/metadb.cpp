#include "metadb.h"

namespace mock
{
    namespace storage
    {
        namespace db
        {
            MockMetadataDbManager::~MockMetadataDbManager() {}
            MockMetadataDbManager::MockMetadataDbManager(const QString& connectionName,
                                                         const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                                         const otp::storage::db::MetadataDbManager::InitDb& configure) :
                otp::storage::db::MetadataDbManager(connectionName, handlers, configure) {}
        }
    }
}
