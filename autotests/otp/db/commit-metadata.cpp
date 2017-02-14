#include <QTest>
#include <QtDebug>

#include "otp/generator.h"
#include "otp/storage/storage.h"
#include "otp/db/metadata.h"
#include "otp/parameters.h"
#include "otp/dummy/parameters.h"
#include "autotests/util/db/metadata-test-utils.h"

#include <QCryptographicHash>

class CommitMetadataTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCommitting(void);
    void testCommitting_data(void);
};

void CommitMetadataTest::testCommitting(void)
{
    QFETCH(QString, entryId);
    QFETCH(QString, message);
    QFETCH(int, tokenType);
    QFETCH(int, hash);
    QFETCH(int, keyEncoding);

    const QString dummyTable(QLatin1String("fake"));

    QHash<QString,QVariant> metadata;
    QHash<QString,QString> paramToTables;

    metadata.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant(tokenType));
    paramToTables.insert(otp::parameters::hashing::ALGORITHM, dummyTable);
    paramToTables.insert(otp::parameters::key::ENCODING, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);
    paramToTables.insert(otp::dummy::parameters::MESSAGE, dummyTable);
    paramToTables.insert(otp::storage::Storage::OTP_TOKEN_TYPE, otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);

    QSharedPointer<otp::storage::db::MetadataDbManager> db = test::storage::db::prepareFakeMetadataDB(entryId, (otp::storage::OTPTokenType) tokenType, metadata, paramToTables);

    otp::storage::db::Metadata uut(entryId, db);
    QVERIFY2(uut.exists(), "The entry should be considered to exist.");
    QVERIFY2(uut.poll(), "Fetching metadata with poll should succeed.");

    const QVariant hashValue(hash);
    QVERIFY2(uut.writeParam(otp::parameters::hashing::ALGORITHM, hashValue), "Writing hash algorithm should succeed");

    const QVariant encodingValue(keyEncoding);
    QVERIFY2(uut.writeParam(otp::parameters::key::ENCODING, encodingValue), "Writing the key encoding/type should succeed");

    QVERIFY2(metadata.contains(otp::parameters::hashing::ALGORITHM) == false, "Original metadata storage should not reflect changes to hash algoirthm prior to commit()");
    QVERIFY2(metadata.contains(otp::parameters::key::ENCODING) == false, "Original metadata storage should not reflect changes to key encoding/type prior to commit()");

    QVERIFY2(uut.commit(), "Comitting should succeed");

    QVERIFY2(metadata.contains(otp::parameters::hashing::ALGORITHM) == true, "Original metadata storage should reflect changes to hash algoirthm after commit()");
    QVERIFY2(metadata.contains(otp::parameters::key::ENCODING) == true, "Original metadata storage should reflect changes to key encoding/type after commit()");

    const auto storedHash = metadata.value(otp::parameters::hashing::ALGORITHM);
    QCOMPARE(storedHash.type(), QVariant::Int);
    QCOMPARE(storedHash.toInt(), hashValue.toInt());

    const auto storedEncoding = metadata.value(otp::parameters::key::ENCODING);
    QCOMPARE(storedEncoding.type(), QVariant::Int);
    QCOMPARE(storedEncoding.toInt(), encodingValue.toInt());
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

void CommitMetadataTest::testCommitting_data(void)
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

QTEST_APPLESS_MAIN(CommitMetadataTest)

#include "commit-metadata.moc"
