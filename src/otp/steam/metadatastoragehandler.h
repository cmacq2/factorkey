#ifndef FACTORKEY_STEAM_METADATASTORAGEHANDLER_H
#define FACTORKEY_STEAM_METADATASTORAGEHANDLER_H

#include "../db/metadatastoragehandler.h"

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
                    static const otp::storage::db::MetadataStorageHandler * create(void);
                private:
                    SteamGuardMetadataStorageHandler() = delete;
                private:
                    static const bool isRegistered;
                };
            }
        }
    }
}
#endif

