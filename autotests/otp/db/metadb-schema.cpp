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
private:
    void drop(bool& ok);
    QSharedPointer<otp::storage::db::MetadataDbManager> m_uut;
};

void MetaDbSchemaTest::drop(bool& ok)
{
    ok = (bool) m_uut;
    QVERIFY2(m_uut, "Constructing the MemoryMetadataDbManager should succeed");
    if(ok)
    {
        std::function<bool(void)> fn([this]() -> bool
        {
            const auto& conn = m_uut->open();
            return conn.isValid() && conn.isOpen();
        });
        ok = fn();
        if(ok)
        {
            auto memDb = static_cast<stubs::storage::db::MemoryMetadataDbManager*>(m_uut.data());
            ok = (bool) memDb;
            QVERIFY2(memDb, "The MemoryMetadataDbManager cast should succeed");

            if(ok)
            {
                QVERIFY2(ok = memDb->drop(), "Dropping the schema should succeed");
            }
        }
        m_uut->close();
    }
}

void MetaDbSchemaTest::initTestCase(void)
{
    stubs::storage::db::MemoryMetadataDbBuilder builder;
    builder.setConnectionName(CONNECTION_NAME);

    otp::dummy::storage::db::DummyMetadataStorageHandler::registerWith(builder);
    otp::skey::storage::db::SKeyMetadataStorageHandler::registerWith(builder);
    otp::steam::storage::db::SteamGuardMetadataStorageHandler::registerWith(builder);
    otp::oath::storage::db::TOTPMetadataStorageHandler::registerWith(builder);
    otp::oath::storage::db::HOTPMetadataStorageHandler::registerWith(builder);

    m_uut = builder.create();
    QVERIFY2(m_uut, "Constructing the MemoryMetadataDbManager should succeed");
}

void MetaDbSchemaTest::testCount()
{
    QFETCH(QString, table);
    bool ok;
    drop(ok);
    QVERIFY2(ok, "Schema precondition should hold but drop() failed!");
    if(ok)
    {
        std::function<void(void)> test([this,&table]() -> void
        {
            auto conn = m_uut->open();
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
        test();
        bool ignored = true;
        drop(ignored);
    }
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
    QVERIFY2((bool) m_uut, "Constructing the MemoryMetadataDbManager should succeed");
    if(m_uut)
    {
        std::function<void(void)> test([this]() -> void
        {
            const auto& conn = m_uut->open();

            QVERIFY2(conn.isValid(), "Opening an in-memory DB should yield a valid connection");
            QVERIFY2(conn.isOpen(), "Opening an in-memory DB should yield an opened connection");
            QVERIFY2(m_uut->isOpened(), "The opened/closed state of the DB should be reflected");
            QCOMPARE(conn.connectionName(), QString(CONNECTION_NAME));

            const auto& conn2 = QSqlDatabase::database(CONNECTION_NAME, false);
            QVERIFY2(conn2.isValid(), "Retrieving the connection by name should yield a valid connection");
            QVERIFY2(conn2.isOpen(), "Retrieving the connection by name should yield an opened connection");
        });
        test();
        bool ignored=true;

        drop(ignored);
    }
}

QTEST_APPLESS_MAIN(MetaDbSchemaTest)

#include "metadb-schema.moc"
