#include "metadatastoragehandler.h"
#include "parameters.h"
#include "../parameters.h"

#include <QtDebug>

#define DUMMY_TABLE_NAME    (QLatin1String("dummy"))
#define DUMMY_CHARSET_COL   (QLatin1String("charset"))
#define DUMMY_MESSAGE_COL   (QLatin1String("message"))
#define DUMMY_ALGORITHM_COL (QLatin1String("algorithm"))

namespace otp
{
    namespace dummy
    {
        namespace storage
        {
            namespace db
            {
                const QString DummyMetadataStorageHandler::DUMMY_TABLE_SCHEMA = QString(QLatin1String("CREATE TABLE IF NOT EXISTS `%1` (`%2` VARHCAR(36) NOT NULL PRIMARY KEY, `%4` TEXT NOT NULL, `%5` VARCHAR(36), `%6` INTEGER, FOREIGN KEY(`%2`) REFERENCES `%3` (`%2`) ON DELETE CASCADE ON UPDATE CASCADE);\n")).
                    arg(QLatin1String(DUMMY_TABLE_NAME)).
                    arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
                    arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE).
                    arg(DUMMY_MESSAGE_COL).
                    arg(DUMMY_CHARSET_COL).
                    arg(DUMMY_ALGORITHM_COL);

                const QSharedPointer<otp::storage::db::MetadataStorageHandler> DummyMetadataStorageHandler::create(void)
                {
                    static const QString invalid;
                    static const otp::storage::db::MetadataStorageHandler::MappingFunction mapper([](const QString& param, const otp::storage::db::MetadataStorageHandler::RegisterFunction& fn) -> void
                    {
                        const QString t(DUMMY_TABLE_NAME);
                        if(otp::storage::db::MetadataStorageHandler::defaultMappings(param, fn))
                        {
                            return;
                        }
                        if(param == otp::dummy::parameters::MESSAGE)
                        {
                            fn(t, DUMMY_MESSAGE_COL);
                        }
                        if(param == otp::dummy::parameters::CHARSET)
                        {
                            fn(t, DUMMY_CHARSET_COL);
                        }
                        if(param == otp::parameters::hashing::ALGORITHM)
                        {
                            fn(t, DUMMY_ALGORITHM_COL);
                        }
                    });

                    static const otp::storage::db::MetadataStorageHandler::SchemaFunction schema([](const QString& table) -> const QString&
                    {
                        return table == DUMMY_TABLE_NAME ? DUMMY_TABLE_SCHEMA : invalid;
                    });

                    return QSharedPointer<MetadataStorageHandler>(MetadataStorageHandler::build(
                        otp::storage::OTPTokenType::DummyHMAC, otp::dummy::parameters::tokenParameters(), mapper, schema
                    ));
                }

                bool DummyMetadataStorageHandler::registerWith(otp::storage::db::MetadataDbBuilder& builder)
                {
                    const auto hh = create();
                    return builder.registerType(hh);
                }
            }
        }
    }
}


