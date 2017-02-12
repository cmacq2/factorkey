#include <QTest>
#include <QtDebug>

#include "otp/generator.h"
#include "otp/storage/storage.h"
#include "otp/db/metadata.h"
#include "otp/parameters.h"
#include "otp/dummy/parameters.h"
#include "autotests/mock/db/dummy-metadatastoragehandler.h"
#include "autotests/mock/db/dummy-metadb.h"

#include <QCryptographicHash>

class PollMetadataTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testPolling(void);
    void testPolling_data(void);
};

static void checkString(const otp::storage::db::Metadata& uut, const QString& paramName, const QString& expectedValue, const char * msg)
{
    QVariant paramValue;
    QVERIFY2(uut.readParam(paramName, paramValue), msg);
    QCOMPARE(paramValue.type(), QVariant::String);
    QCOMPARE(paramValue.toString(), expectedValue);
}

static void checkInt(const otp::storage::db::Metadata& uut, const QString& paramName, const int expectedValue, const char * msg)
{
    QVariant paramValue;
    QVERIFY2(uut.readParam(paramName, paramValue), msg);
    QCOMPARE(paramValue.type(), QVariant::Int);
    QCOMPARE(paramValue.toInt(), expectedValue);
}


void PollMetadataTest::testPolling(void)
{
    QFETCH(QString, entryId);
    QFETCH(QString, message);
    QFETCH(int, tokenType);
    QFETCH(int, hash);
    QFETCH(int, keyEncoding);

    const QString conn(QLatin1String("test"));
    const QString dummyTable(QLatin1String("fake"));

    QHash<QString,QVariant> metadata;
    QHash<QString,QString> paramToTables;

    metadata.insert(otp::parameters::hashing::ALGORITHM, QVariant(hash));
    metadata.insert(otp::parameters::key::ENCODING, QVariant(keyEncoding));
    metadata.insert(otp::dummy::parameters::MESSAGE, QVariant(message));
    metadata.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant(tokenType));
    paramToTables.insert(otp::parameters::hashing::ALGORITHM, dummyTable);
    paramToTables.insert(otp::parameters::key::ENCODING, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
    paramToTables.insert(otp::dummy::parameters::MESSAGE, dummyTable);
    paramToTables.insert(otp::storage::Storage::OTP_TOKEN_TYPE, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);

    QSharedPointer<otp::storage::db::MetadataStorageHandler> typeHandler(new stubs::storage::db::FakeSingleEntryMetadataStorageHandler(
        (otp::storage::OTPTokenType) tokenType, entryId, &metadata, paramToTables));
    QSharedPointer<otp::storage::db::MetadataDbManager> db(new stubs::storage::db::FakeSingleItemMetadataDbManager(conn, entryId, typeHandler));

    otp::storage::db::Metadata uut(entryId, db);
    QVERIFY2(uut.exists(), "The entry should be considered to exist.");
    QVERIFY2(uut.poll(), "Fetching metadata with poll should succeed.");

    otp::storage::OTPTokenType typeRead;
    QVERIFY2(uut.readTokenType(typeRead), "Reading token type should succeed");
    QCOMPARE(((int) typeRead), tokenType);

    checkInt(uut, otp::parameters::hashing::ALGORITHM, hash, "Reading the hash algorithm parameter should succeed");
    checkInt(uut, otp::parameters::key::ENCODING, keyEncoding, "Reading the key encoding/type parameter should succeed");
    checkString(uut, otp::dummy::parameters::MESSAGE, message, "Reading the message parameter should succeed");
}

static void result(const QString& caseName,
                   const QString& entryId,
                   const QString& message,
                   const otp::storage::OTPTokenType& tokenType,
                   const QCryptographicHash::Algorithm& hash,
                   const otp::generator::EncodingType& keyEncoding)
{
    QTest::newRow(qPrintable(caseName)) << entryId << message << ((int) tokenType) << ((int) hash) << ((int) keyEncoding);
}

void PollMetadataTest::testPolling_data(void)
{
    QTest::addColumn<QString>("entryId");
    QTest::addColumn<QString>("message");
    QTest::addColumn<int>("tokenType");
    QTest::addColumn<int>("hash");
    QTest::addColumn<int>("keyEncoding");

    result(QLatin1String("Everything defined"),
           QLatin1String("testId"),
           QLatin1String("Hello, world!"),
           otp::storage::OTPTokenType::DummyHMAC,
           QCryptographicHash::Sha1,
           otp::generator::EncodingType::Text);
}

QTEST_APPLESS_MAIN(PollMetadataTest)

#include "poll-metadata.moc"
