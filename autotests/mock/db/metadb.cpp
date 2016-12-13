#include "metadb.h"
#include "otp/storage/util.h"

#include <QtDebug>
#include <QTest>

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)
Q_DECLARE_METATYPE(stubs::storage::db::SqlDatabaseStatus)

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            namespace internal
            {
                static bool checkSqlDatabaseStatusIsMetaType(void)
                {
                    static auto type = qRegisterMetaType<stubs::storage::db::SqlDatabaseStatus>();
                    bool result = (bool) type;
                    if(!result)
                    {
                        qWarning() << "Failed to register meta types! Some signal/slot spying functionality may not work as expected.";
                    }
                    return result;
                }

                static bool register_meta_types(void)
                {
                    static bool result = otp::storage::checkOTPTokenTypeIsMetaType() && checkSqlDatabaseStatusIsMetaType();
                    return result;
                }
            }

            SqlDatabaseStatus::SqlDatabaseStatus(const QString& connName,
                                                 const QString& driverName,
                                                 const bool valid,
                                                 const bool open): m_connName(connName), m_driverName(driverName), m_valid(valid), m_open(open) {}
            SqlDatabaseStatus::SqlDatabaseStatus(const QSqlDatabase& db): m_connName(db.connectionName()), m_driverName(db.driverName()), m_valid(db.isValid()), m_open(db.isOpen()) {}
            SqlDatabaseStatus::SqlDatabaseStatus() : m_connName(QString()), m_driverName(QString()), m_valid(false), m_open(false) {}

            const QString& SqlDatabaseStatus::connectionName(void) const
            {
                return m_connName;
            }

            const QString& SqlDatabaseStatus::driverName(void) const
            {
                return m_driverName;
            }

            bool SqlDatabaseStatus::isOpen(void) const
            {
                return m_open;
            }

            bool SqlDatabaseStatus::isValid(void) const
            {
                return m_valid;
            }

            MetadataDbManager::~MetadataDbManager() {}
            MetadataDbManager::MetadataDbManager(const QString& connectionName,
                                                 const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                                 QObject * parent):
                QObject(parent), otp::storage::db::MetadataDbManager(connectionName, handlers)
            {
                init();
            }

            bool MetadataDbManager::removeEntries(const QStringList& entryList)
            {
                const auto result = impl_removeEntries(entryList);
                emit notify_removeEntries(result, entryList);
                return result;
            }

            bool MetadataDbManager::removeAll(void)
            {
                const auto result = impl_removeAll();
                emit notify_removeAll(result);
                return result;
            }

            bool MetadataDbManager::allowRemoveAll(void) const
            {
                return true;
            }

            bool MetadataDbManager::allowRemoveEntries(void) const
            {
                return true;
            }

            bool MetadataDbManager::impl_removeAll(void)
            {
                return false;
            }

            bool MetadataDbManager::impl_removeEntries(const QStringList&)
            {
                return false;
            }

            bool MetadataDbManager::initDb(QSqlDatabase& db)
            {
                const auto result = impl_initDb(db);
                emit notify_initDb(result, SqlDatabaseStatus(db));
                return result;
            }

            bool MetadataDbManager::impl_initDb(QSqlDatabase&)
            {
                return false;
            }

            bool MetadataDbManager::allowInitDb(void) const
            {
                return true;
            }

            QSqlDatabase MetadataDbManager::open(void)
            {
                auto result = impl_open();
                emit notify_open(SqlDatabaseStatus(result));
                return result;
            }

            bool MetadataDbManager::allowOpen(void) const
            {
                return true;
            }

            QSqlDatabase MetadataDbManager::impl_open(void)
            {
                return QSqlDatabase();
            }

            bool MetadataDbManager::impl_close(void)
            {
                return false;
            }

            bool MetadataDbManager::allowClose(void) const
            {
                return true;
            }

            bool MetadataDbManager::close(void)
            {
                auto result = impl_close();
                emit notify_close(result);
                return result;
            }

            bool MetadataDbManager::isOpened(void) const
            {
                auto result = impl_isOpened();
                emit notify_isOpened(result);
                return result;
            }

            bool MetadataDbManager::impl_isOpened(void) const
            {
                return false;
            }

            bool MetadataDbManager::readType(const QString& entry, QVariant& value)
            {
                auto result = impl_readType(entry, value);
                emit notify_readType(result, entry, value);
                return result;
            }

            bool MetadataDbManager::impl_readType(const QString&, QVariant&)
            {
                return false;
            }

            bool MetadataDbManager::allowReadType(void) const
            {
                return true;
            }

            bool MetadataDbManager::remove(const QString& entry)
            {
                auto result = impl_remove(entry);
                emit notify_remove(result, entry);
                return result;
            }

            bool MetadataDbManager::impl_remove(const QString&)
            {
                return false;
            }

            bool MetadataDbManager::allowRemove(void) const
            {
                return true;
            }

            bool MetadataDbManager::contains(const QString& entry)
            {
                auto result = impl_contains(entry);
                emit notify_contains(result, entry);
                return result;
            }

            bool MetadataDbManager::allowContains(void) const
            {
                return true;
            }

            bool MetadataDbManager::impl_contains(const QString&)
            {
                return false;
            }

            bool MetadataDbManager::entries(QStringList& entryList)
            {
                auto result = impl_entries(entryList);
                emit notify_entries(result, entryList);
                return result;
            }

            bool MetadataDbManager::allowEntries(void) const
            {
                return true;
            }

            bool MetadataDbManager::impl_entries(QStringList&)
            {
                return false;
            }

            const QSharedPointer<otp::storage::db::MetadataStorageHandler> MetadataDbManager::handler(otp::storage::OTPTokenType type)
            {
                const auto result = impl_handler(type);
                emit notify_handler(result, type);
                return result;
            }

            bool MetadataDbManager::allowHandler(void) const
            {
                return true;
            }

            const QSharedPointer<otp::storage::db::MetadataStorageHandler> MetadataDbManager::impl_handler(otp::storage::OTPTokenType type)
            {
                return allowHandler() ?
                    otp::storage::db::MetadataDbManager::handler(type) :
                    QSharedPointer<otp::storage::db::MetadataStorageHandler>();
            }

            void MetadataDbManager::init(void)
            {
                stubs::storage::db::internal::register_meta_types();

                m_spy_isOpened = new QSignalSpy(this, &MetadataDbManager::notify_isOpened);
                m_spy_open = new QSignalSpy(this, &MetadataDbManager::notify_open);
                m_spy_close = new QSignalSpy(this, &MetadataDbManager::notify_close);
                m_spy_readType = new QSignalSpy(this, &MetadataDbManager::notify_readType);
                m_spy_remove = new QSignalSpy(this, &MetadataDbManager::notify_remove);
                m_spy_contains = new QSignalSpy(this, &MetadataDbManager::notify_contains);
                m_spy_entries = new QSignalSpy(this, &MetadataDbManager::notify_entries);
                m_spy_handler = new QSignalSpy(this, &MetadataDbManager::notify_handler);
                m_spy_initDb = new QSignalSpy(this, &MetadataDbManager::notify_initDb);
                m_spy_removeEntries = new QSignalSpy(this, &MetadataDbManager::notify_removeEntries);
                m_spy_removeAll = new QSignalSpy(this, &MetadataDbManager::notify_removeAll);
            }

            QSignalSpy * MetadataDbManager::spy_initDb(void) const
            {
                return m_spy_initDb;
            }

            QSignalSpy * MetadataDbManager::spy_isOpened(void) const
            {
                return m_spy_isOpened;
            }

            QSignalSpy * MetadataDbManager::spy_open(void) const
            {
                return m_spy_open;
            }

            QSignalSpy * MetadataDbManager::spy_close(void) const
            {
                return m_spy_close;
            }

            QSignalSpy * MetadataDbManager::spy_readType(void) const
            {
                return m_spy_readType;
            }

            QSignalSpy * MetadataDbManager::spy_remove(void) const
            {
                return m_spy_remove;
            }

            QSignalSpy * MetadataDbManager::spy_contains(void) const
            {
                return m_spy_contains;
            }

            QSignalSpy * MetadataDbManager::spy_entries(void) const
            {
                return m_spy_entries;
            }

            QSignalSpy * MetadataDbManager::spy_handler(void) const
            {
                return m_spy_handler;
            }

            QSignalSpy * MetadataDbManager::spy_removeEntries(void) const
            {
                return m_spy_removeEntries;
            }

            QSignalSpy * MetadataDbManager::spy_removeAll(void) const
            {
                return m_spy_removeAll;
            }

            QList<QList<QVariant>>& MetadataDbManager::expect_token_type(QList<QList<QVariant>>& callStack, bool status, const QString& entry, enum otp::storage::OTPTokenType type)
            {
                QList<QVariant> args;
                args << status << entry << QVariant::fromValue(type);
                return callStack << args;
            }

            QList<QList<QVariant>>& MetadataDbManager::expect_string(QList<QList<QVariant>>& callStack, bool status, const QString& entry)
            {
                QList<QVariant> args;
                args << status << entry;
                return callStack << args;
            }

            QList<QList<QVariant>>& MetadataDbManager::expect_entries(QList<QList<QVariant>>& callStack, bool status, const QStringList& entries)
            {
                QList<QVariant> args;
                args << status << entries;
                return callStack << args;
            }

            static void check_io(const QList<bool>& results, QSignalSpy * spy, const char * validSpyMessage)
            {
                QVERIFY2(spy && spy->isValid(), validSpyMessage);
                int size = results.size();
                QCOMPARE(spy->count(), size);
                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 1);

                    auto p1 = paramCallArgs.at(0);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p1.toBool(), results.at(i));
                }
            }

            void MetadataDbManager::check_isOpened(const QList<bool>& callArgs) const
            {
                check_io(callArgs, spy_isOpened(), "isOpened should be spied on by a valid QSignalSpy");
            }

            void MetadataDbManager::check_no_isOpened(void) const
            {
                QList<bool> empty;
                check_isOpened(empty);
            }

            static void compareDbConnections(const SqlDatabaseStatus& db1, const SqlDatabaseStatus& db2)
            {
                QCOMPARE(db1.connectionName(), db2.connectionName());
                QCOMPARE(db1.driverName(), db2.driverName());
                QCOMPARE(db1.isValid(), db2.isValid());
                QCOMPARE(db1.isOpen(), db2.isOpen());
            }

            void MetadataDbManager::check_open(const QList<SqlDatabaseStatus>& callArgs) const
            {
                QSignalSpy * spy = spy_open();
                QVERIFY2(spy && spy->isValid(), "open() should be spied on by a valid QSignalSpy");
                int size = callArgs.size();
                QCOMPARE(spy->count(), size);
                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 1);

                    auto p1 = paramCallArgs.at(0);
                    QCOMPARE(p1.userType(), qMetaTypeId<SqlDatabaseStatus>());
                    compareDbConnections(p1.value<SqlDatabaseStatus>(), callArgs.at(i));
                }
            }

            void MetadataDbManager::check_no_open(void) const
            {
                QList<SqlDatabaseStatus> empty;
                check_open(empty);
            }

            void MetadataDbManager::check_removeAll(const QList<bool>& callArgs) const
            {
                check_io(callArgs, spy_removeAll(), "removeAll() should be spied on by a valid QSignalSpy");
            }

            void MetadataDbManager::check_no_removeAll(void) const
            {
                QList<bool> empty;
                check_removeAll(empty);
            }

            void MetadataDbManager::check_close(const QList<bool>& callArgs) const
            {
                check_io(callArgs, spy_close(), "close() should be spied on by a valid QSignalSpy");
            }

            void MetadataDbManager::check_no_close(void) const
            {
                QList<bool> empty;
                check_close(empty);
            }

            static void check_string(const QList<QList<QVariant>>& callArgs, QSignalSpy * spy, const char * spyValidMessage)
            {
                QVERIFY2(spy && spy->isValid(), spyValidMessage);

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 2);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 2, "Internal sanity check failed: size of expected call args should be 2");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");

                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                }
            }

            void MetadataDbManager::check_contains(const QList<QList<QVariant>>& callArgs) const
            {
                check_string(callArgs, spy_contains(), "contains() should be spied on by a valid QSignalSpy");
            }

            void MetadataDbManager::check_no_contains(void) const
            {
                QList<QList<QVariant>> empty;
                check_contains(empty);
            }

            void MetadataDbManager::check_remove(const QList<QList<QVariant>>& callArgs) const
            {
                check_string(callArgs, spy_remove(), "remove() should be spied on by a valid QSignalSpy");
            }

            void MetadataDbManager::check_no_remove(void) const
            {
                QList<QList<QVariant>> empty;
                check_remove(empty);
            }

            void MetadataDbManager::check_removeEntries(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_entries();
                QVERIFY2(spy && spy->isValid(), "removeEntries() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 2);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.userType(), qMetaTypeId<QStringList>());

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 2, "Internal sanity check failed: size of expected call args should be 2");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == qMetaTypeId<QStringList>(), "Internal sanity check failed: type of second expected call arg should be QStringList");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.value<QStringList>(), e2.value<QStringList>());
                }
            }

            void MetadataDbManager::check_no_removeEntries(void) const
            {
                QList<QList<QVariant>> empty;
                check_removeEntries(empty);
            }

            void MetadataDbManager::check_entries(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_entries();
                QVERIFY2(spy && spy->isValid(), "entries() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 2);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.userType(), qMetaTypeId<QStringList>());

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 2, "Internal sanity check failed: size of expected call args should be 2");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == qMetaTypeId<QStringList>(), "Internal sanity check failed: type of second expected call arg should be QStringList");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.value<QStringList>(), e2.value<QStringList>());
                }
            }

            void MetadataDbManager::check_no_entries(void) const
            {
                QList<QList<QVariant>> empty;
                check_entries(empty);
            }

            void MetadataDbManager::check_readType(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_readType();
                QVERIFY2(spy && spy->isValid(), "readType() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 3);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    QCOMPARE(p3.userType(), qMetaTypeId<enum otp::storage::OTPTokenType>());

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 3, "Internal sanity check failed: size of expected call args should be 3");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");

                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.userType() == qMetaTypeId<enum otp::storage::OTPTokenType>(), "Internal sanity check failed: type of third expected call arg should be OTPTokenType");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE(p3.value<enum otp::storage::OTPTokenType>(), e3.value<enum otp::storage::OTPTokenType>());
                }
            }

            void MetadataDbManager::check_no_readType(void) const
            {
                QList<QList<QVariant>> empty;
                check_readType(empty);
            }

            FakeMetadataDbBuilder::~FakeMetadataDbBuilder() {}
            FakeMetadataDbBuilder::FakeMetadataDbBuilder(otp::storage::db::MetadataDbManager * db): m_db(db) {}

            otp::storage::db::MetadataDbManager * FakeMetadataDbBuilder::build(void) const
            {
                return m_db;
            }
        }
    }
}
