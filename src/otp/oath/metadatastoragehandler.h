#ifndef FACTORKEY_OATH_METADATASTORAGEHANDLER_H
#define FACTORKEY_OATH_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"
#include "../db/metadb.h"

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
                    static const QSharedPointer<otp::storage::db::MetadataStorageHandler> create(void);
                    static bool registerWith(otp::storage::db::MetadataDbBuilder& builder);
                private:
                    HOTPMetadataStorageHandler() = delete;
                    static const QString HOTP_TABLE_SCHEMA;
                };

                class TOTPMetadataStorageHandler: otp::storage::db::MetadataStorageHandler
                {
                public:
                    static const QSharedPointer<otp::storage::db::MetadataStorageHandler> create(void);
                    static bool registerWith(otp::storage::db::MetadataDbBuilder& builder);
                private:
                    TOTPMetadataStorageHandler() = delete;
                    static const QString TOTP_TABLE_SCHEMA;
                };
            }
        }
    }
}

#endif
