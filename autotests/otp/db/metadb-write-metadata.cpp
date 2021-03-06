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

#include <QCoreApplication>
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
    void testWriting(void);
    void testWriting_data(void);

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

void MetaDbCreateMetadataTest::testWriting_data(void)
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

void MetaDbCreateMetadataTest::testWriting(void)
{

    QFETCH(QString, entryId);
    QFETCH(QString, message);
    QFETCH(int, tokenType);
    QFETCH(int, hash);
    QFETCH(int, keyEncoding);

    const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>, bool&)> setup(
        [&entryId, tokenType](QSharedPointer<otp::storage::db::MetadataDbManager> db, bool& ok) -> void
    {
        const QString TYPE_COLUMN(QLatin1String("tokentype"));
        const QString sql = QString(QLatin1String("INSERT INTO `%1` (`%2`, `%3`) VALUES (:%2, :%3);\n")).
            arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE).
            arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
            arg(TYPE_COLUMN);
        QSqlDatabase conn = db->open();
        ok = conn.isValid() && conn.isOpen() && conn.transaction();
        QVERIFY2(ok, "Starting a transaction for populating the metadata DB should succeed");

        bool insertedOK = otp::storage::db::queryDb(sql, entryId, db.data(), [tokenType, &sql, &TYPE_COLUMN](QSqlQuery& query) -> bool
        {
            query.bindValue(QLatin1Char(':') + TYPE_COLUMN, tokenType);

            bool result = query.lastError().type() == QSqlError::NoError && query.exec() && query.lastError().type() == QSqlError::NoError;
            if(!result) {
                qDebug() << "Failed to insert metadata; SQL is:" << sql;
                qDebug() << "Error:" << query.lastError().text();
            }
            return result;
        });

        ok = insertedOK && conn.commit() && conn.lastError().type() == QSqlError::NoError;
        if(!ok) {
            conn.rollback();
        }

        QVERIFY2(ok, "Populating the metadata DB should succeed");
    });

    const auto& test = test::storage::db::createMetadataTest(
        entryId, [&entryId, hash, keyEncoding, tokenType, &message](QSharedPointer<otp::storage::db::MetadataDbManager> db, QSharedPointer<otp::storage::db::Metadata> metadata) -> void
    {
        QVERIFY2(metadata->exists(), "The metadata should be considered to exist");
        QVERIFY2(metadata->poll(), "Polling the metadata should succeed");
        otp::storage::OTPTokenType type;
        QVERIFY2(metadata->readTokenType(type), "Reading the token type should succeed");
        QCOMPARE(type, (otp::storage::OTPTokenType) tokenType);

        QVERIFY2(metadata->writeParam(otp::parameters::hashing::ALGORITHM, hash), "Writing the hashing algorithm should succeed");
        QVERIFY2(metadata->writeParam(otp::parameters::key::ENCODING, keyEncoding), "Writing the key encoding/type should succeed");
        QVERIFY2(metadata->writeParam(otp::dummy::parameters::MESSAGE, message), "Writing the message should succeed");

        QVERIFY2(metadata->commit(), "Comitting the metadata should succeed.");

        QSharedPointer<otp::storage::db::Metadata> reader(new otp::storage::db::Metadata(entryId, db));

        QVERIFY2(reader->exists(), "The metadata should still be considered to exist");
        QVERIFY2(reader->poll(), "Polling the metadata should still succeed");

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
    test::storage::db::runTest(m_uut, setup, test);
}

QTEST_APPLESS_MAIN(MetaDbCreateMetadataTest)

#include "metadb-write-metadata.moc"
