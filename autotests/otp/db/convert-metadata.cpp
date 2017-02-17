#include <QTest>
#include <QtDebug>

#include "otp/generator.h"
#include "otp/storage/storage.h"
#include "otp/db/metadata.h"
#include "otp/parameters.h"
#include "otp/dummy/parameters.h"
#include "otp/oath/parameters.h"
#include "autotests/util/db/metadata-test-utils.h"
#include "autotests/mock/db/dummy-metadatastoragehandler.h"
#include "autotests/mock/db/dummy-metadb.h"

#include <QCryptographicHash>
#include <QDateTime>

class ConvertMetadataTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testConverting(void);
    void testConverting_data(void);
};

void ConvertMetadataTest::testConverting(void)
{
    QFETCH(QString, entryId);
    QFETCH(QString, message);
    QFETCH(int, tokenType);
    QFETCH(int, hash);
    QFETCH(int, keyEncoding);
    QFETCH(int, newTokenType);
    QFETCH(QDateTime, epoch);

    const QString sharedTable(QLatin1String("fake"));
    const QString initialTable(QLatin1String("initial-fake"));
    const QString newTable(QLatin1String("new-fake"));

    QHash<QString,QVariant> initialMetadata, newMetadata;
    QHash<QString,QString> initialParamToTables, newParamToTables;

    initialMetadata.insert(otp::parameters::hashing::ALGORITHM, QVariant(hash));
    initialMetadata.insert(otp::parameters::key::ENCODING, QVariant(keyEncoding));
    initialMetadata.insert(otp::dummy::parameters::MESSAGE, QVariant(message));
    initialMetadata.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant(tokenType));

    newParamToTables.insert(otp::parameters::hashing::ALGORITHM, newTable);
    newParamToTables.insert(otp::parameters::key::ENCODING, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
    newParamToTables.insert(otp::storage::Storage::OTP_TOKEN_TYPE, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
    newParamToTables.insert(otp::oath::parameters::totp::EPOCH, newTable);

    initialParamToTables.insert(otp::parameters::hashing::ALGORITHM, initialTable);
    initialParamToTables.insert(otp::parameters::key::ENCODING, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
    initialParamToTables.insert(otp::storage::Storage::OTP_TOKEN_TYPE, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
    initialParamToTables.insert(otp::dummy::parameters::MESSAGE, initialTable);


    const QString conn(QLatin1String("test"));
    QSharedPointer<otp::storage::db::MetadataStorageHandler> initialTypeHandler(new stubs::storage::db::FakeSingleEntryMetadataStorageHandler(
        (otp::storage::OTPTokenType) tokenType, entryId, &initialMetadata, initialParamToTables));
    QSharedPointer<otp::storage::db::MetadataStorageHandler> newTypeHandler(new stubs::storage::db::FakeSingleEntryMetadataStorageHandler(
        (otp::storage::OTPTokenType) newTokenType, entryId, &newMetadata, newParamToTables));
    QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>> typeHandlers;
    typeHandlers.insert(tokenType, initialTypeHandler);
    typeHandlers.insert(newTokenType, newTypeHandler);

    QVERIFY2(initialTypeHandler->isParamNameValid(otp::storage::Storage::OTP_TOKEN_TYPE), "token type param name should be valid");
    QVERIFY2(newTypeHandler->isParamNameValid(otp::storage::Storage::OTP_TOKEN_TYPE), "token type param name should be valid");

    QSharedPointer<otp::storage::db::MetadataDbManager> db(new stubs::storage::db::FakeSingleItemMetadataDbManager(conn, entryId, typeHandlers, (otp::storage::OTPTokenType) tokenType));

    otp::storage::db::Metadata uut(entryId, db);
    QVERIFY2(uut.exists(), "The entry should be considered to exist.");
    QVERIFY2(uut.poll(), "Fetching metadata with poll should succeed.");

    test::storage::db::checkType(uut, (otp::storage::OTPTokenType) tokenType, "Reading token type should succeed before type conversion");
    test::storage::db::checkInt(uut, otp::parameters::hashing::ALGORITHM, hash, "Reading the hash algorithm parameter should succeed before type conversion");
    test::storage::db::checkInt(uut, otp::parameters::key::ENCODING, keyEncoding, "Reading the key encoding/type parameter should succeed before type conversion");
    test::storage::db::checkString(uut, otp::dummy::parameters::MESSAGE, message, "Reading the message parameter should succeed before type conversion");

    const QVariant epochValue(epoch);
    QVERIFY2(uut.writeParam(otp::oath::parameters::totp::EPOCH, epochValue) == false, "Writing the epoch parameter should not yet succeed before type conversion");

    QVERIFY2(uut.writeTokenType((otp::storage::OTPTokenType) newTokenType), "Writing new token type should succeed");

    test::storage::db::checkType(uut, (otp::storage::OTPTokenType) newTokenType, "Reading token type should still succeed after type conversion");
    test::storage::db::checkInt(uut, otp::parameters::hashing::ALGORITHM, hash, "Reading the hash algorithm parameter should still succeed after type conversion");
    test::storage::db::checkInt(uut, otp::parameters::key::ENCODING, keyEncoding, "Reading the key encoding/type parameter should still succeed after type conversion");

    QVariant nil;
    QVERIFY2(uut.readParam(otp::dummy::parameters::MESSAGE, nil) == false, "Reading the message parameter should no loonger succeed after type conversion");

    QVERIFY2(initialMetadata.contains(otp::dummy::parameters::MESSAGE) == false, "The message parameter should no longer be stored in initial metadata after type conversion");
    QVERIFY2(initialMetadata.contains(otp::parameters::key::ENCODING), "The key encoding/type parameter should still be stored in initial metadata after type conversion");
    QVERIFY2(newMetadata.contains(otp::dummy::parameters::MESSAGE) == false, "The message parameter should not be stored in new metadata");

    QVERIFY2(uut.writeParam(otp::oath::parameters::totp::EPOCH, epochValue), "Writing the epoch parameter should succeed after the type conversion");

    QVERIFY2(initialMetadata.contains(otp::oath::parameters::totp::EPOCH) == false, "The epoch parameter should not be stored in initial metadata");
    QVERIFY2(newMetadata.contains(otp::oath::parameters::totp::EPOCH) == false, "The epoch parameter should not be stored in new metadata yet before commit()");

    test::storage::db::checkDateTime(uut, otp::oath::parameters::totp::EPOCH, epoch, "Reading the epoch parameter should succeed after type conversion");

    QVERIFY2(uut.commit(), "Comitting should succeed");

    QVERIFY2(initialMetadata.contains(otp::oath::parameters::totp::EPOCH) == false, "The epoch parameter should not be stored in initial metadata after commit()");
    QVERIFY2(newMetadata.contains(otp::oath::parameters::totp::EPOCH), "The epoch parameter should be stored in new metadata after commit()");
    //QVERIFY2(newMetadata.contains(otp::parameters::key::ENCODING), "The key encoding/type parameter should be stored in new metadata after commit()");

    test::storage::db::checkType(uut, (otp::storage::OTPTokenType) newTokenType, "Reading token type should still succeed after commit()");
    test::storage::db::checkInt(uut, otp::parameters::hashing::ALGORITHM, hash, "Reading the hash algorithm parameter should still succeed after commit()");
    test::storage::db::checkInt(uut, otp::parameters::key::ENCODING, keyEncoding, "Reading the key encoding/type parameter should still succeed after commit()");
    test::storage::db::checkDateTime(uut, otp::oath::parameters::totp::EPOCH, epoch, "Reading the epoch parameter should still succeed after commit()");
}

static void result(const QString& caseName,
                   const QString& entryId,
                   const QString& message,
                   const otp::storage::OTPTokenType& tokenType,
                   const QCryptographicHash::Algorithm& hash,
                   const otp::generator::EncodingType& keyEncoding,
                   const QDateTime& epoch)
{
    QTest::newRow(qPrintable(caseName)) << entryId << message << ((int) tokenType) << ((int) hash) << ((int) keyEncoding) << ((int) otp::storage::OTPTokenType::TOTP) << epoch;
}

void ConvertMetadataTest::testConverting_data(void)
{

    QTest::addColumn<QString>("entryId");
    QTest::addColumn<QString>("message");
    QTest::addColumn<int>("tokenType");
    QTest::addColumn<int>("hash");
    QTest::addColumn<int>("keyEncoding");
    QTest::addColumn<int>("newTokenType");
    QTest::addColumn<QDateTime>("epoch");

    result(QLatin1String("Everything defined"),
           QLatin1String("testId"),
           QLatin1String("Hello, world!"),
           otp::storage::OTPTokenType::DummyHMAC,
           QCryptographicHash::Sha1,
           otp::generator::EncodingType::Text,
           QDateTime(QDate(2017, 2, 15), QTime(8, 30), Qt::UTC));
}

QTEST_APPLESS_MAIN(ConvertMetadataTest)

#include "convert-metadata.moc"
