#ifndef FACTORKEY_DUMMY_METADATASTORAGEHANDLER_H
#define FACTORKEY_DUMMY_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"
#include "../db/metadb.h"

namespace otp
{
    namespace dummy
    {
        namespace storage
        {
            namespace db
            {
                class DummyMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
                {
                public:
                    static const QSharedPointer<otp::storage::db::MetadataStorageHandler> create(void);
                    static bool registerWith(otp::storage::db::MetadataDbBuilder& builder);
                private:
                    DummyMetadataStorageHandler() = delete;
                    static const QString DUMMY_TABLE_SCHEMA;
                };
            }
        }
    }
}
#endif


