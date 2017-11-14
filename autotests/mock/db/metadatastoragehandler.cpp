#include "metadatastoragehandler.h"

namespace mock
{
    namespace storage
    {
        namespace db
        {
            static const QHash<QString,QString> emptyHash;
            static const QSet<QString> emptySet;

            MockMetadataStorageHandler::~MockMetadataStorageHandler() {}
            MockMetadataStorageHandler::MockMetadataStorageHandler(otp::storage::OTPTokenType type) :
                otp::storage::db::MetadataStorageHandler(type, emptyHash, emptySet, emptyHash, emptyHash) {}
        }
    }
}
