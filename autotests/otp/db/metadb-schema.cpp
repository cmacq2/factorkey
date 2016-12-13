#include "autotests/mock/db/memory-metadb.h"
#include "otp/dummy/metadatastoragehandler.h"
#include "otp/skey/metadatastoragehandler.h"
#include "otp/steam/metadatastoragehandler.h"
#include "otp/oath/metadatastoragehandler.h"

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
    void cleanupTestCase(void);
private:
    QObject * m_parent = new QObject();
    stubs::storage::db::MemoryMetadataDbManager * m_uut;
};

void MetaDbSchemaTest::cleanupTestCase(void)
{
    m_parent->deleteLater();
}

void MetaDbSchemaTest::initTestCase(void)
{
    stubs::storage::db::MemoryMetadataDbBuilder builder;
    builder.setParent(m_parent);
    builder.setConnectionName(CONNECTION_NAME);

    otp::dummy::storage::db::DummyMetadataStorageHandler::registerWith(builder);
    otp::skey::storage::db::SKeyMetadataStorageHandler::registerWith(builder);
    otp::steam::storage::db::SteamGuardMetadataStorageHandler::registerWith(builder);
    otp::oath::storage::db::TOTPMetadataStorageHandler::registerWith(builder);
    otp::oath::storage::db::HOTPMetadataStorageHandler::registerWith(builder);

    m_uut = builder.build();
}

void MetaDbSchemaTest::testCount()
{
    QFETCH(QString, table);

    QSqlDatabase conn(QSqlDatabase::database(CONNECTION_NAME, false));
    QSqlQuery tableQ(conn);

    const QString tableSQL(QLatin1String("SELECT COUNT(*) FROM `%1`;\n"));
    QVERIFY2(tableQ.exec(tableSQL.arg(table)), "Counting the number of records should succeed");
    QVERIFY2(tableQ.first(), "Retrieving the first record of the count should succeed");
    const auto& tableCount = tableQ.value(0);
    QVERIFY2(!tableCount.isNull(), "THe count should yield a NOT NULL value");
    QCOMPARE(tableCount.type(), QVariant::LongLong);
    QCOMPARE(tableCount.toLongLong(), 0);
    QVERIFY2(!tableQ.next(), "There should be no second record in the count");
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

    const auto& conn = m_uut->open();

    QVERIFY2(conn.isValid(), "Opening an in-memory DB should yield a valid connection");
    QVERIFY2(conn.isOpen(), "Opening an in-memory DB should yield an opened connection");
    QVERIFY2(m_uut->isOpened(), "The opened/closed state of the DB should be reflected");
    QCOMPARE(conn.connectionName(), QString(CONNECTION_NAME));

    const auto& conn2 = QSqlDatabase::database(CONNECTION_NAME, false);
    QVERIFY2(conn2.isValid(), "Retrieving the connection by name should yield a valid connection");
    QVERIFY2(conn2.isOpen(), "Retrieving the connection by name should yield an opened connection");
}

QTEST_MAIN(MetaDbSchemaTest)

#include "metadb-schema.moc"
