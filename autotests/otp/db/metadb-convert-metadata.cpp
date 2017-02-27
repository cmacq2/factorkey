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
#include "otp/oath/parameters.h"
#include "autotests/util/db/memory-metadata-test-utils.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTest>
#include <QtDebug>

#define CONNECTION_NAME (QLatin1String(__FILE__))

class MetaDbConvertMetadataTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testConverting(void);
    void testConverting_data(void);

    void initTestCase(void);
private:
    QSharedPointer<otp::storage::db::MetadataDbManager> m_uut;
};

void MetaDbConvertMetadataTest::initTestCase(void)
{
    m_uut = test::storage::db::createMemoryMetaDb(CONNECTION_NAME);
    QVERIFY2(m_uut, "Constructing the MemoryMetadataDbManager should succeed");
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

void MetaDbConvertMetadataTest::testConverting_data(void)
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
           QDateTime(QDate(2017, 2, 27), QTime(7, 0), Qt::UTC));
}

void MetaDbConvertMetadataTest::testConverting(void)
{
    QFETCH(QString, entryId);
    QFETCH(QString, message);
    QFETCH(int, tokenType);
    QFETCH(int, hash);
    QFETCH(int, keyEncoding);
    QFETCH(int, newTokenType);
    QFETCH(QDateTime, epoch);

    const std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>, bool&)> setup(
        [&entryId, tokenType, hash, keyEncoding, &message](QSharedPointer<otp::storage::db::MetadataDbManager> db, bool& ok) -> void
    {
        const QString KEY_ENCODING_COLUMN(QLatin1String("keyencoding"));
        const QString TYPE_COLUMN(QLatin1String("tokentype"));
        const QString sql1 = QString(QLatin1String("INSERT INTO `%1` (`%2`, `%3`, `%4`) VALUES (:%2, :%3, :%4);\n")).
            arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE).
            arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
            arg(TYPE_COLUMN).
            arg(KEY_ENCODING_COLUMN);
        QSqlDatabase conn = db->open();
        ok = conn.isValid() && conn.isOpen() && conn.transaction();
        QVERIFY2(ok, "Starting a transaction for populating the metadata DB should succeed");

        bool insertedOK = otp::storage::db::queryDb(sql1, entryId, db.data(), [tokenType, keyEncoding, &sql1, &TYPE_COLUMN, &KEY_ENCODING_COLUMN](QSqlQuery& query) -> bool
        {
            query.bindValue(QLatin1Char(':') + TYPE_COLUMN, tokenType);
            query.bindValue(QLatin1Char(':') + KEY_ENCODING_COLUMN, keyEncoding);

            bool result = query.lastError().type() == QSqlError::NoError && query.exec() && query.lastError().type() == QSqlError::NoError;
            if(!result) {
                qDebug() << "Failed to insert metadata; SQL is:" << sql1;
                qDebug() << "Error:" << query.lastError().text();
            }
            return result;
        });

        const QString MESSAGE_COLUMN(QLatin1String("message"));
        const QString ALGORITHM_COLUMN(QLatin1String("algorithm"));
        const QString DUMMY_TABLE(QLatin1String("dummy"));
        const QString sql2 = QString(QLatin1String("INSERT INTO `%1` (`%2`, `%3`, `%4`) VALUES (:%2, :%3, :%4);\n")).
            arg(DUMMY_TABLE).
            arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_ID).
            arg(MESSAGE_COLUMN).
            arg(ALGORITHM_COLUMN);

        insertedOK = insertedOK && otp::storage::db::queryDb(sql2, entryId, db.data(), [hash, &message, &sql2, &MESSAGE_COLUMN, &ALGORITHM_COLUMN](QSqlQuery& query) -> bool
        {
            query.bindValue(QLatin1Char(':') + MESSAGE_COLUMN, message);
            query.bindValue(QLatin1Char(':') + ALGORITHM_COLUMN, hash);

            bool result = query.lastError().type() == QSqlError::NoError && query.exec() && query.lastError().type() == QSqlError::NoError;
            if(!result) {
                qDebug() << "Failed to insert metadata; SQL is:" << sql2;
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
        entryId, [&entryId, hash, keyEncoding, tokenType, newTokenType, &epoch, &message](QSharedPointer<otp::storage::db::MetadataDbManager> db, QSharedPointer<otp::storage::db::Metadata> metadata) -> void
    {
        QVERIFY2(metadata->exists(), "The metadata should be considered to exist");
        QVERIFY2(metadata->poll(), "Polling the metadata should succeed");
        otp::storage::OTPTokenType type;
        QVERIFY2(metadata->readTokenType(type), "Reading the token type should succeed");
        QCOMPARE(type, (otp::storage::OTPTokenType) tokenType);

        QVariant actualHash, actualKeyEncoding, actualMessage, actualEpoch;
        QVERIFY2(metadata->readParam(otp::parameters::hashing::ALGORITHM, actualHash), "Reading the hashing algorithm should succeed");
        QCOMPARE(actualHash.type(), QVariant::LongLong);
        QCOMPARE(actualHash.toInt(), hash);

        QVERIFY2(metadata->readParam(otp::parameters::key::ENCODING, actualKeyEncoding), "Reading the key encoding/type should succeed");
        QCOMPARE(actualKeyEncoding.type(), QVariant::LongLong);
        QCOMPARE(actualKeyEncoding.toInt(), keyEncoding);

        QVERIFY2(metadata->readParam(otp::dummy::parameters::MESSAGE, actualMessage), "Reading the message should succeed");
        QCOMPARE(actualMessage.type(), QVariant::String);
        QCOMPARE(actualMessage.toString(), message);

        QVERIFY2(!metadata->readParam(otp::oath::parameters::totp::EPOCH, actualEpoch), "Reading the epoch should not succeed prior to type conversion");

        QVERIFY2(metadata->writeTokenType((otp::storage::OTPTokenType) newTokenType), "Writing the new token type should succeed");
        QVERIFY2(metadata->writeParam(otp::oath::parameters::totp::EPOCH, epoch.toMSecsSinceEpoch()), "Writing the epoch parameter should succeed after the type conversion");
        QVERIFY2(metadata->commit(), "Comitting should succeed");

        QSharedPointer<otp::storage::db::Metadata> reader(new otp::storage::db::Metadata(entryId, db));

        QVERIFY2(reader->exists(), "The metadata should still be considered to exist");
        QVERIFY2(reader->poll(), "Polling the metadata should still succeed");

        QVERIFY2(reader->readTokenType(type), "Reading the token type should still succeed");
        QCOMPARE(type, (otp::storage::OTPTokenType) newTokenType);

        QVERIFY2(reader->readParam(otp::parameters::hashing::ALGORITHM, actualHash), "Reading the hashing algorithm should succeed");
        QCOMPARE(actualHash.type(), QVariant::LongLong);
        QCOMPARE(actualHash.toInt(), hash);

        QVERIFY2(reader->readParam(otp::parameters::key::ENCODING, actualKeyEncoding), "Reading the key encoding/type should succeed");
        QCOMPARE(actualKeyEncoding.type(), QVariant::LongLong);
        QCOMPARE(actualKeyEncoding.toInt(), keyEncoding);

        QVERIFY2(!reader->readParam(otp::dummy::parameters::MESSAGE, actualMessage), "Reading the message should no longer succeed after type conversion");

        QVERIFY2(reader->readParam(otp::oath::parameters::totp::EPOCH, actualEpoch), "Reading the epoch should succeed after type conversion");
        QCOMPARE(actualEpoch.type(), QVariant::LongLong);
        QCOMPARE(QDateTime::fromMSecsSinceEpoch(actualEpoch.toLongLong()), epoch);
    });
    test::storage::db::runTest(m_uut, setup, test);
}

QTEST_APPLESS_MAIN(MetaDbConvertMetadataTest)

#include "metadb-convert-metadata.moc"
