#ifndef FACTORKEY_SKEY_METADATASTORAGEHANDLER_H
#define FACTORKEY_SKEY_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"

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
                    static const otp::storage::db::MetadataStorageHandler * create(void);
                private:
                    SKeyMetadataStorageHandler() = delete;
                private:
                    static const QString SKEY_TABLE_SCHEMA;
                    static const bool isRegistered;
                };
            }
        }
    }
}
#endif
