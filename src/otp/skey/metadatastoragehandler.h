#ifndef FACTORKEY_SKEY_METADATASTORAGEHANDLER_H
#define FACTORKEY_SKEY_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"
#include "../db/metadb.h"

namespace otp
{
    namespace skey
    {
        namespace storage
        {
            namespace db
            {
                class SKeyMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
                {
                public:
                    static const QSharedPointer<otp::storage::db::MetadataStorageHandler> create(void);
                    static bool registerWith(otp::storage::db::MetadataDbBuilder& builder);
                private:
                    SKeyMetadataStorageHandler() = delete;
                    static const QString SKEY_TABLE_SCHEMA;
                };
            }
        }
    }
}
#endif
