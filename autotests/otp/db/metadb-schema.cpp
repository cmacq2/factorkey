#include "autotests/mock/db/memory-metadb.h"
#include "autotests/util/db/memory-metadata-test-utils.h"

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTest>
#include <QtDebug>

#define CONNECTION_NAME (QLatin1String(__FILE__))

class MetaDbSchemaTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSchema(void);
    void testCount(void);
    void testCount_data(void);

    void initTestCase(void);
private:
    QSharedPointer<otp::storage::db::MetadataDbManager> m_uut;
};

void MetaDbSchemaTest::initTestCase(void)
{
    m_uut = test::storage::db::createMemoryMetaDb(CONNECTION_NAME);
    QVERIFY2(m_uut, "Constructing the MemoryMetadataDbManager should succeed");
}

void MetaDbSchemaTest::testCount()
{
    QFETCH(QString, table);
    std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)> test([&table](QSharedPointer<otp::storage::db::MetadataDbManager> uut) -> void
    {
        auto conn = uut->open();
        QSqlQuery tableQ(conn);

        const QString tableSQL(QLatin1String("SELECT COUNT(*) FROM `%1`;\n"));
        QVERIFY2(tableQ.exec(tableSQL.arg(table)), "Counting the number of records should succeed");
        QVERIFY2(tableQ.first(), "Retrieving the first record of the count should succeed");
        const auto& tableCount = tableQ.value(0);
        QVERIFY2(!tableCount.isNull(), "The count should yield a NOT NULL value");
        QCOMPARE(tableCount.type(), QVariant::LongLong);
        QCOMPARE(tableCount.toLongLong(), 0);
        QVERIFY2(!tableQ.next(), "There should be no second record in the count");
    });
    test::storage::db::runTest(m_uut, test);
}

static void result(const QString& table)
{
    QTest::newRow(qPrintable(table)) << table;
}

void MetaDbSchemaTest::testCount_data(void)
{
    QTest::addColumn<QString>("table");
    result(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE);

    result(QLatin1String("totp_params"));
    result(QLatin1String("hotp_params"));
    result(QLatin1String("skey_params"));
    result(QLatin1String("dummy"));
}

void MetaDbSchemaTest::testSchema(void)
{
    std::function<void(QSharedPointer<otp::storage::db::MetadataDbManager>)> test([](QSharedPointer<otp::storage::db::MetadataDbManager> uut) -> void
    {
        const auto& conn = uut->open();

        QVERIFY2(conn.isValid(), "Opening an in-memory DB should yield a valid connection");
        QVERIFY2(conn.isOpen(), "Opening an in-memory DB should yield an opened connection");
        QVERIFY2(uut->isOpened(), "The opened/closed state of the DB should be reflected");
        QCOMPARE(conn.connectionName(), QString(CONNECTION_NAME));

        const auto& conn2 = QSqlDatabase::database(CONNECTION_NAME, false);
        QVERIFY2(conn2.isValid(), "Retrieving the connection by name should yield a valid connection");
        QVERIFY2(conn2.isOpen(), "Retrieving the connection by name should yield an opened connection");
    });
    test::storage::db::runTest(m_uut, test, false);
}

QTEST_APPLESS_MAIN(MetaDbSchemaTest)

#include "metadb-schema.moc"
