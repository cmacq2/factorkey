#include "metadata-test-utils.h"
#include "autotests/mock/db/dummy-metadatastoragehandler.h"
#include "autotests/mock/db/dummy-metadb.h"

#include <QTest>

namespace test
{
    namespace storage
    {
        namespace db
        {
            QSharedPointer<otp::storage::db::MetadataDbManager> prepareFakeMetadataDB(const QString& entryId,
                                                                                      const otp::storage::OTPTokenType& tokenType,
                                                                                      QHash<QString,QVariant>& metadata,
                                                                                      QHash<QString,QString>& paramToTables)
            {
                const QString conn(QLatin1String("test"));
                QSharedPointer<otp::storage::db::MetadataStorageHandler> typeHandler(new stubs::storage::db::FakeSingleEntryMetadataStorageHandler(
                    tokenType, entryId, &metadata, paramToTables));
                return QSharedPointer<otp::storage::db::MetadataDbManager>(new stubs::storage::db::FakeSingleItemMetadataDbManager(conn, entryId, typeHandler));
            }

            void checkType(const otp::storage::db::Metadata& uut, const otp::storage::OTPTokenType expectedType, const char * msg)
            {
                otp::storage::OTPTokenType typeRead;
                QVERIFY2(uut.readTokenType(typeRead), msg);
                QCOMPARE(typeRead, expectedType);
            }

            void checkDateTime(const otp::storage::db::Metadata& uut, const QString& paramName, const QDateTime& expectedValue, const char * msg)
            {
                QVariant paramValue;
                QVERIFY2(uut.readParam(paramName, paramValue), msg);
                QCOMPARE(paramValue.type(), QVariant::DateTime);
                QCOMPARE(paramValue.toDateTime(), expectedValue);
            }

            void checkString(const otp::storage::db::Metadata& uut, const QString& paramName, const QString& expectedValue, const char * msg)
            {
                QVariant paramValue;
                QVERIFY2(uut.readParam(paramName, paramValue), msg);
                QCOMPARE(paramValue.type(), QVariant::String);
                QCOMPARE(paramValue.toString(), expectedValue);
            }

            void checkInt(const otp::storage::db::Metadata& uut, const QString& paramName, const int expectedValue, const char * msg)
            {
                QVariant paramValue;
                QVERIFY2(uut.readParam(paramName, paramValue), msg);
                QCOMPARE(paramValue.type(), QVariant::Int);
                QCOMPARE(paramValue.toInt(), expectedValue);
            }
        }
    }
}
