#include "metadatastoragehandler.h"
#include "parameters.h"

#define SKEY_TABLE_NAME     (QLatin1String("skey_params"))
#define SKEY_DICTIONARY_COL (QLatin1String("dictionary"))
#define SKEY_ENCODING_COL   (QLatin1String("encoding"))

namespace otp
{
    namespace skey
    {
        namespace storage
        {
            namespace db
            {
                const bool SKeyMetadataStorageHandler::isRegistered = otp::storage::db::MetadataStorageHandler::registerType(otp::storage::OTPTokenType::SKey, create);

                const QString SKeyMetadataStorageHandler::SKEY_TABLE_SCHEMA = QString(QLatin1String("CREATE TABLE IF NOT EXISTS `%1` (`%2`, `%3`, `%4`);\n")).
                    arg(QLatin1String(SKEY_TABLE_NAME)).
                    arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
                    arg(SKEY_DICTIONARY_COL).
                    arg(SKEY_ENCODING_COL);

                const otp::storage::db::MetadataStorageHandler * SKeyMetadataStorageHandler::create(void)
                {
                    static const QString invalid;
                    static const otp::storage::db::MetadataStorageHandler::MappingFunction mapper([](const QString& param, const otp::storage::db::MetadataStorageHandler::RegisterFunction& fn) -> void
                    {
                        const QString t(SKEY_TABLE_NAME);
                        if(otp::storage::db::MetadataStorageHandler::defaultMappings(param, fn))
                        {
                            return;
                        }
                        if(param == otp::skey::parameters::DICTIONARY)
                        {
                            fn(t, SKEY_DICTIONARY_COL);
                        }
                        if(param == otp::skey::parameters::ENCODING)
                        {
                            fn(t, SKEY_ENCODING_COL);
                        }
                    });
                    static const otp::storage::db::MetadataStorageHandler::SchemaFunction schema([](const QString& table) -> const QString&
                    {
                        return table == SKEY_TABLE_NAME ? SKEY_TABLE_SCHEMA : invalid;
                    });
                    static const QScopedPointer<MetadataStorageHandler> h(MetadataStorageHandler::build(
                        otp::storage::OTPTokenType::DummyHMAC, otp::skey::parameters::tokenParameters(), mapper, schema
                    ));
                    return h.data();
                }
            }
        }
    }
}
