#include "metadatastoragehandler.h"
#include "parameters.h"

namespace otp
{
    namespace steam
    {
        namespace storage
        {
            namespace db
            {
                const bool SteamGuardMetadataStorageHandler::isRegistered = otp::storage::db::MetadataStorageHandler::registerType(otp::storage::OTPTokenType::SteamGuard, create);

                const otp::storage::db::MetadataStorageHandler * SteamGuardMetadataStorageHandler::create(void)
                {
                    static const QString invalid;

                    static const otp::storage::db::MetadataStorageHandler::MappingFunction mapper([](const QString& param, const otp::storage::db::MetadataStorageHandler::RegisterFunction& fn) -> void
                    {
                        otp::storage::db::MetadataStorageHandler::defaultMappings(param, fn);
                    });

                    static const otp::storage::db::MetadataStorageHandler::SchemaFunction schema([](const QString&) -> const QString&
                    {
                        return invalid;
                    });

                    static const QScopedPointer<MetadataStorageHandler> h(MetadataStorageHandler::build(
                        otp::storage::OTPTokenType::SteamGuard, otp::steam::parameters::tokenParameters(), mapper, schema
                    ));
                    return h.data();
                }
            }
        }
    }
}

