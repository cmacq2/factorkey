#ifndef FACTORKEY_AUTOTESTS_TEST_UTILS_METADATA_H
#define FACTORKEY_AUTOTESTS_TEST_UTILS_METADATA_H

#include <functional>
#include <QString>
#include "otp/db/metadata.h"

namespace test
{
    namespace storage
    {
        namespace db
        {
            QSharedPointer<otp::storage::db::MetadataDbManager> prepareFakeMetadataDB(const QString& entryId,
                                                                                      const otp::storage::OTPTokenType& tokenType,
                                                                                      QHash<QString,QVariant>& metadata,
                                                                                      QHash<QString,QString>& paramToTables);

            void checkType(const otp::storage::db::Metadata& uut, const otp::storage::OTPTokenType expectedType, const char * msg);
            void checkString(const otp::storage::db::Metadata& uut, const QString& paramName, const QString& expectedValue, const char * msg);
            void checkInt(const otp::storage::db::Metadata& uut, const QString& paramName, const int expectedValue, const char * msg);
        }
    }
}
#endif
