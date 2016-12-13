#ifndef FACTORKEY_STEAM_METADATASTORAGEHANDLER_H
#define FACTORKEY_STEAM_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"
#include "../db/metadb.h"

namespace otp
{
    namespace steam
    {
        namespace storage
        {
            namespace db
            {
                class SteamGuardMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
                {
                public:
                    static const QSharedPointer<otp::storage::db::MetadataStorageHandler> create(void);
                    static bool registerWith(otp::storage::db::MetadataDbBuilder& builder);
                private:
                    SteamGuardMetadataStorageHandler() = delete;
                };
            }
        }
    }
}
#endif

