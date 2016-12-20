#ifndef FACTORKEY_AUTOTESTS_STUBBED_MEMORY_METADBH_H
#define FACTORKEY_AUTOTESTS_STUBBED_MEMORY_METADBH_H

#include "otp/db/metadb.h"
#include "otp/db/metadatastoragehandler.h"

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            class MemoryMetadataDbManager: public otp::storage::db::MetadataDbManager
            {
            public:
                MemoryMetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers);
                virtual ~MemoryMetadataDbManager();
                bool drop(void);
            protected:
                virtual bool allowInitDb(void) const;
            protected:
                virtual bool initDb(QSqlDatabase& db);
            };

            class MemoryMetadataDbBuilder: public otp::storage::db::MetadataDbBuilder
            {
            public:
                MemoryMetadataDbManager * build(void) const;
            };
        }
    }
}
#endif
