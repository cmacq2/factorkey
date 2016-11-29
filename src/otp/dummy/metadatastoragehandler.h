#ifndef FACTORKEY_DUMMY_METADATASTORAGEHANDLER_H
#define FACTORKEY_DUMMY_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"

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
                    static const otp::storage::db::MetadataStorageHandler * create(void);
                private:
                    DummyMetadataStorageHandler() = delete;
                private:
                    static const QString DUMMY_TABLE_SCHEMA;
                    static const bool isRegistered;
                };
            }
        }
    }
}
#endif


