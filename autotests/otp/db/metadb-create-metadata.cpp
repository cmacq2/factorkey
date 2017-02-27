 #include "autotests/mock/db/memory-metadb.h"
#include "otp/dummy/metadatastoragehandler.h"
#include "otp/skey/metadatastoragehandler.h"
#include "otp/steam/metadatastoragehandler.h"
#include "otp/oath/metadatastoragehandler.h"
#include "otp/generator.h"
#include "otp/storage/storage.h"
#include "otp/db/metadata.h"
#include "otp/parameters.h"
#include "otp/dummy/parameters.h"
#include "autotests/util/db/memory-metadata-test-utils.h"

#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTest>
#include <QtDebug>

#define CONNECTION_NAME (QLatin1String(__FILE__))

class MetaDbCreateMetadataTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCreating(void);
    void testCreating_data(void);

    void initTestCase(void);
private:
    QSharedPointer<otp::storage::db::MetadataDbManager> m_uut;
};

void MetaDbCreateMetadataTest::initTestCase(void)
{
    m_uut = test::storage::db::createMemoryMetaDb(CONNECTION_NAME);
    QVERIFY2(m_uut, "Constructing the MemoryMetadataDbManager should succeed");
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

void MetaDbCreateMetadataTest::testCreating_data(void)
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
           QCryptographicHash::Md5,
           otp::generator::EncodingType::Text);
}

void MetaDbCreateMetadataTest::testCreating(void)
{
    QFETCH(QString, entryId);
    QFETCH(QString, message);
    QFETCH(int, tokenType);
    QFETCH(int, hash);
    QFETCH(int, keyEncoding);

    const auto& test = test::storage::db::createMetadataTest(
        entryId, [&entryId, hash, keyEncoding, tokenType, &message](QSharedPointer<otp::storage::db::MetadataDbManager> db, QSharedPointer<otp::storage::db::Metadata> metadata) -> void
    {
        QVERIFY2(!metadata->exists(), "The metadata should not be considered to exist prior to commit()");

        QVERIFY2(metadata->writeTokenType((otp::storage::OTPTokenType) tokenType), "Writing the token type should succeed");

        QVERIFY2(metadata->writeParam(otp::parameters::hashing::ALGORITHM, hash), "Writing the hashing algorithm should succeed");
        QVERIFY2(metadata->writeParam(otp::parameters::key::ENCODING, keyEncoding), "Writing the key encoding/type should succeed");
        QVERIFY2(metadata->writeParam(otp::dummy::parameters::MESSAGE, message), "Writing the message should succeed");

        QVERIFY2(!metadata->exists(), "The metadata should still not be considered to exist prior to commit()");

        QVERIFY2(metadata->commit(), "Comitting the metadata should succeed.");

        QSharedPointer<otp::storage::db::Metadata> reader(new otp::storage::db::Metadata(entryId, db));

        QVERIFY2(reader->exists(), "The metadata should be considered to exist after commit()");
        QVERIFY2(reader->poll(), "Polling the metadata succeed after creating it");

        otp::storage::OTPTokenType type;
        QVERIFY2(reader->readTokenType(type), "Reading the token type should still succeed");
        QCOMPARE(type, (otp::storage::OTPTokenType) tokenType);

        QVariant actualHash, actualKeyEncoding, actualMessage;
        QVERIFY2(reader->readParam(otp::parameters::hashing::ALGORITHM, actualHash), "Reading the hashing algorithm should succeed");
        QCOMPARE(actualHash.type(), QVariant::LongLong);
        QCOMPARE(actualHash.toInt(), hash);

        QVERIFY2(reader->readParam(otp::parameters::key::ENCODING, actualKeyEncoding), "Reading the key encoding/type should succeed");
        QCOMPARE(actualKeyEncoding.type(), QVariant::LongLong);
        QCOMPARE(actualKeyEncoding.toInt(), keyEncoding);

        QVERIFY2(reader->readParam(otp::dummy::parameters::MESSAGE, actualMessage), "Reading the message should succeed");
        QCOMPARE(actualMessage.type(), QVariant::String);
        QCOMPARE(actualMessage.toString(), message);
    });
    test::storage::db::runTest(m_uut, test);
}

QTEST_APPLESS_MAIN(MetaDbCreateMetadataTest)

#include "metadb-create-metadata.moc"
