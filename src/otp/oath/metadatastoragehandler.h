#ifndef FACTORKEY_OATH_METADATASTORAGEHANDLER_H
#define FACTORKEY_OATH_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"

namespace otp
{
    namespace oath
    {
        namespace storage
        {
            namespace db
            {
                class HOTPMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
                {
                public:
                    static const otp::storage::db::MetadataStorageHandler * create(void);
                private:
                    HOTPMetadataStorageHandler() = delete;
                private:
                    static const QString HOTP_TABLE_SCHEMA;
                    static const bool isRegistered;
                };

                class TOTPMetadataStorageHandler: otp::storage::db::MetadataStorageHandler
                {
                public:
                    static const otp::storage::db::MetadataStorageHandler * create(void);
                private:
                    TOTPMetadataStorageHandler() = delete;
                private:
                    static const QString TOTP_TABLE_SCHEMA;
                    static const bool isRegistered;
                };
            }
        }
    }
}

#endif
