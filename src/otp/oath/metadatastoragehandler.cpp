#include "metadatastoragehandler.h"
#include "generator.h"
#include "../parameters.h"
#include "parameters.h"

#define TOTP_TABLE_NAME     (QLatin1String("totp_params"))
#define HOTP_TABLE_NAME     (QLatin1String("hotp_params"))

#define OATH_ENC_LENGTH_COL (QLatin1String("enc_length"))
#define OATH_ENC_LOCALE_COL (QLatin1String("enc_locale"))
#define OATH_HASH_ALGO_COL  (QLatin1String("algorithm"))

#define HOTP_COUNTER_COL    (QLatin1String("counter"))

#define TOTP_EPOCH_COL      (QLatin1String("epoch"))
#define TOTP_TIMESTEP_COL   (QLatin1String("step"))

namespace otp
{
    namespace oath
    {
        namespace storage
        {
            namespace db
            {
                static bool oathDefaultMappings(const QString& param, const QString& oathTable, const otp::storage::db::MetadataStorageHandler::RegisterFunction& fn)
                {
                    if(param == otp::parameters::hashing::ALGORITHM)
                    {
                        return fn(oathTable, OATH_HASH_ALGO_COL);
                    }
                    if(param == otp::oath::parameters::generic::LOCALE)
                    {
                        return fn(oathTable, OATH_ENC_LOCALE_COL);
                    }
                    if(param == otp::oath::parameters::generic::LENGTH)
                    {
                        return fn(oathTable, OATH_ENC_LENGTH_COL);
                    }
                    return false;
                }

                const QString HOTPMetadataStorageHandler::HOTP_TABLE_SCHEMA = QString(QLatin1String("CREATE TABLE IF NOT EXISTS `%1` (`%2`, `%3`, `%4`, `%5`, `%6`);\n")).
                    arg(QLatin1String(HOTP_TABLE_NAME)).
                    arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
                    arg(OATH_ENC_LENGTH_COL).
                    arg(OATH_ENC_LOCALE_COL).
                    arg(OATH_HASH_ALGO_COL).
                    arg(HOTP_COUNTER_COL);

                const bool HOTPMetadataStorageHandler::isRegistered = otp::storage::db::MetadataStorageHandler::registerType(otp::storage::OTPTokenType::HOTP, create);

                const otp::storage::db::MetadataStorageHandler * HOTPMetadataStorageHandler::create(void)
                {
                    static const QString invalid;
                    static const otp::storage::db::MetadataStorageHandler::MappingFunction mapper([](const QString& param, const otp::storage::db::MetadataStorageHandler::RegisterFunction& fn) -> void
                    {
                        const QString t(HOTP_TABLE_NAME);
                        if(otp::storage::db::MetadataStorageHandler::defaultMappings(param, fn) || oathDefaultMappings(param, t, fn))
                        {
                            return;
                        }
                        if(param == otp::oath::parameters::hotp::COUNTER)
                        {
                            fn(t, HOTP_COUNTER_COL);
                        }
                    });

                    static const otp::storage::db::MetadataStorageHandler::SchemaFunction schema([](const QString& table) -> const QString&
                    {
                        return table == HOTP_TABLE_NAME ? HOTP_TABLE_SCHEMA : invalid;
                    });

                    static const QScopedPointer<MetadataStorageHandler> h(MetadataStorageHandler::build(
                        otp::storage::OTPTokenType::HOTP, otp::oath::parameters::hotp::tokenParameters(), mapper, schema
                    ));
                    return h.data();
                }

                const QString TOTPMetadataStorageHandler::TOTP_TABLE_SCHEMA = QString(QLatin1String("CREATE TABLE IF NOT EXISTS `%1` (`%2`, `%3`, `%4`, `%5`, `%6`, `%7`);\n")).
                    arg(QLatin1String(TOTP_TABLE_NAME)).
                    arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
                    arg(OATH_ENC_LENGTH_COL).
                    arg(OATH_ENC_LOCALE_COL).
                    arg(OATH_HASH_ALGO_COL).
                    arg(TOTP_EPOCH_COL).
                    arg(TOTP_TIMESTEP_COL);

                const bool TOTPMetadataStorageHandler::isRegistered = otp::storage::db::MetadataStorageHandler::registerType(otp::storage::OTPTokenType::TOTP, create);

                const otp::storage::db::MetadataStorageHandler * TOTPMetadataStorageHandler::create(void)
                {
                    static const QString invalid;
                    static const otp::storage::db::MetadataStorageHandler::MappingFunction mapper([](const QString& param, const otp::storage::db::MetadataStorageHandler::RegisterFunction& fn) -> void
                    {
                        const QString t(TOTP_TABLE_NAME);
                        if(otp::storage::db::MetadataStorageHandler::defaultMappings(param, fn) || oathDefaultMappings(param, t, fn))
                        {
                            return;
                        }
                        if(param == otp::oath::parameters::totp::EPOCH)
                        {
                            fn(t, TOTP_EPOCH_COL);
                        }
                        if(param == otp::oath::parameters::totp::TIMESTEP)
                        {
                            fn(t, TOTP_TIMESTEP_COL);
                        }
                    });

                    static const otp::storage::db::MetadataStorageHandler::SchemaFunction schema([](const QString& table) -> const QString&
                    {
                        return table == TOTP_TABLE_NAME ? TOTP_TABLE_SCHEMA : invalid;
                    });

                    static const QScopedPointer<MetadataStorageHandler> h(MetadataStorageHandler::build(
                        otp::storage::OTPTokenType::TOTP, otp::oath::parameters::totp::tokenParameters(), mapper, schema
                    ));
                    return h.data();
                }
            }
        }
    }
}
