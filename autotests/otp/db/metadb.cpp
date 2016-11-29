#include "metadb.h"
#include "otp/storage/util.h"

#include <QtDebug>
#include <QTest>

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            namespace internal
            {
                static bool register_meta_types(void)
                {
                    static bool result = otp::storage::checkOTPTokenTypeIsMetaType();
                    return result;
                }
            }

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName):
                QObject(), otp::storage::db::MetadataDbManager(connName)
            {
                init();
            }

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, const QHash<QString, enum otp::storage::OTPTokenType>& entries):
                QObject(), otp::storage::db::MetadataDbManager(connName), m_entries(entries)
            {
                init();
            }

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, const QHash<int,QSharedPointer<DummyMetadataStorageHandler>>& typeHandlers):
                QObject(), otp::storage::db::MetadataDbManager(connName), m_typeHandlers(typeHandlers)
            {
                init();
            }

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, const QHash<int,QSharedPointer<DummyMetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries):
                QObject(), otp::storage::db::MetadataDbManager(connName), m_typeHandlers(typeHandlers), m_entries(entries)
            {
                init();
            }

            DummyMetadataDbManager::~DummyMetadataDbManager() {}

            const QStringList DummyMetadataDbManager::removedEntries(void) const
            {
                return m_removed;
            }

            QSqlDatabase DummyMetadataDbManager::open(void)
            {
                auto result = impl_open();
                emit notify_open(result);
                return result;
            }

            bool DummyMetadataDbManager::allowOpen(void) const
            {
                return true;
            }

            QSqlDatabase DummyMetadataDbManager::impl_open(void)
            {
                return QSqlDatabase();
//                 if(allowOpen())
//                 {
//                     bool result = !m_opened;
//                     m_opened = true;
//                     return result;
//                 }
//                 return false;
            }

            bool DummyMetadataDbManager::impl_close(void)
            {
                if(allowClose())
                {
                    bool result = m_opened;
                    m_opened = false;
                    return result;
                }
                return false;
            }

            bool DummyMetadataDbManager::allowClose(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::close(void)
            {
                auto result = impl_close();
                emit notify_close(result);
                return result;
            }

            bool DummyMetadataDbManager::isOpened(void) const
            {
                auto result = impl_isOpened();
                emit notify_isOpened(result);
                return result;
            }

            bool DummyMetadataDbManager::impl_isOpened(void) const
            {
                return m_opened;
            }

            bool DummyMetadataDbManager::readType(const QString& entry, QVariant& value)
            {
                auto result = impl_readType(entry, value);
                emit notify_readType(result, entry, value);
                return result;
            }

            bool DummyMetadataDbManager::impl_readType(const QString& entry, QVariant& value)
            {
                if(allowReadType() && m_entries.contains(entry))
                {
                    const otp::storage::OTPTokenType type = m_entries.value(entry);
                    value = QVariant::fromValue(type);
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::allowReadType(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::remove(const QString& entry)
            {
                auto result = impl_remove(entry);
                emit notify_remove(result, entry);
                return result;
            }

            bool DummyMetadataDbManager::impl_remove(const QString& entry)
            {
                if(allowRemove() && m_entries.contains(entry))
                {
                    m_removed << entry;
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::allowRemove(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::contains(const QString& entry)
            {
                auto result = impl_contains(entry);
                emit notify_contains(result, entry);
                return result;
            }

            bool DummyMetadataDbManager::allowContains(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::impl_contains(const QString& entry)
            {
                if(allowContains())
                {
                    return !m_removed.contains(entry) && m_entries.contains(entry);
                }
                return false;
            }

            bool DummyMetadataDbManager::entries(QStringList& entryList)
            {
                auto result = impl_entries(entryList);
                emit notify_entries(result, entryList);
                return result;
            }

            bool DummyMetadataDbManager::allowEntries(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::impl_entries(QStringList& e)
            {
                if(allowEntries())
                {
                    e.clear();
                    for(const auto& a: m_entries.keys())
                    {
                        if(!m_removed.contains(a))
                        {
                            e << a;
                        }
                    }
                    return true;
                }
                return false;
            }

            QSharedPointer<otp::storage::db::MetadataStorageHandler> DummyMetadataDbManager::getHandler(otp::storage::OTPTokenType type)
            {
                auto result = impl_getHandler(type);
                emit notify_getHandler(result, type);
                return result;
            }

            bool DummyMetadataDbManager::allowGetHandler(void) const
            {
                return true;
            }

            QSharedPointer<otp::storage::db::MetadataStorageHandler> DummyMetadataDbManager::impl_getHandler(otp::storage::OTPTokenType type)
            {
                QSharedPointer<otp::storage::db::MetadataStorageHandler> result;
                int t = (int) type;
                if(allowGetHandler() && m_typeHandlers.contains(t))
                {
                    result = m_typeHandlers.value(t);
                }
                return result;
            }

            void DummyMetadataDbManager::init(void)
            {
                stubs::storage::db::internal::register_meta_types();

                m_spy_isOpened = new QSignalSpy(this, &DummyMetadataDbManager::notify_isOpened);
                m_spy_open = new QSignalSpy(this, &DummyMetadataDbManager::notify_open);
                m_spy_close = new QSignalSpy(this, &DummyMetadataDbManager::notify_close);
                m_spy_readType = new QSignalSpy(this, &DummyMetadataDbManager::notify_readType);
                m_spy_remove = new QSignalSpy(this, &DummyMetadataDbManager::notify_remove);
                m_spy_contains = new QSignalSpy(this, &DummyMetadataDbManager::notify_contains);
                m_spy_entries = new QSignalSpy(this, &DummyMetadataDbManager::notify_entries);
                m_spy_getHandler = new QSignalSpy(this, &DummyMetadataDbManager::notify_getHandler);
            }

            QSignalSpy * DummyMetadataDbManager::spy_isOpened(void) const
            {
                return m_spy_isOpened;
            }

            QSignalSpy * DummyMetadataDbManager::spy_open(void) const
            {
                return m_spy_open;
            }

            QSignalSpy * DummyMetadataDbManager::spy_close(void) const
            {
                return m_spy_close;
            }

            QSignalSpy * DummyMetadataDbManager::spy_readType(void) const
            {
                return m_spy_readType;
            }

            QSignalSpy * DummyMetadataDbManager::spy_remove(void) const
            {
                return m_spy_remove;
            }

            QSignalSpy * DummyMetadataDbManager::spy_contains(void) const
            {
                return m_spy_contains;
            }

            QSignalSpy * DummyMetadataDbManager::spy_entries(void) const
            {
                return m_spy_entries;
            }

            QSignalSpy * DummyMetadataDbManager::spy_getHandler(void) const
            {
                return m_spy_getHandler;
            }

            QList<QList<QVariant>>& DummyMetadataDbManager::expect_token_type(QList<QList<QVariant>>& callStack, bool status, const QString& entry, enum otp::storage::OTPTokenType type)
            {
                QList<QVariant> args;
                args << status << entry << QVariant::fromValue(type);
                return callStack << args;
            }

            QList<QList<QVariant>>& DummyMetadataDbManager::expect_string(QList<QList<QVariant>>& callStack, bool status, const QString& entry)
            {
                QList<QVariant> args;
                args << status << entry;
                return callStack << args;
            }

            QList<QList<QVariant>>& DummyMetadataDbManager::expect_entries(QList<QList<QVariant>>& callStack, bool status, const QStringList& entries)
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

            void DummyMetadataDbManager::check_isOpened(const QList<bool>& callArgs) const
            {
                check_io(callArgs, spy_isOpened(), "isOpened should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataDbManager::check_no_isOpened(void) const
            {
                QList<bool> empty;
                check_isOpened(empty);
            }

            void DummyMetadataDbManager::check_open(const QList<bool>& callArgs) const
            {
                check_io(callArgs, spy_open(), "open() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataDbManager::check_no_open(void) const
            {
                QList<bool> empty;
                check_open(empty);
            }

            void DummyMetadataDbManager::check_close(const QList<bool>& callArgs) const
            {
                check_io(callArgs, spy_close(), "close() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataDbManager::check_no_close(void)
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

            void DummyMetadataDbManager::check_contains(const QList<QList<QVariant>>& callArgs) const
            {
                check_string(callArgs, spy_contains(), "contains() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataDbManager::check_no_contains(void) const
            {
                QList<QList<QVariant>> empty;
                check_contains(empty);
            }

            void DummyMetadataDbManager::check_remove(const QList<QList<QVariant>>& callArgs) const
            {
                check_string(callArgs, spy_remove(), "remove() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataDbManager::check_no_remove(void) const
            {
                QList<QList<QVariant>> empty;
                check_remove(empty);
            }

            void DummyMetadataDbManager::check_entries(const QList<QList<QVariant>>& callArgs) const
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

            void DummyMetadataDbManager::check_no_entries(void) const
            {
                QList<QList<QVariant>> empty;
                check_entries(empty);
            }

            void DummyMetadataDbManager::check_readType(const QList<QList<QVariant>>& callArgs) const
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

            void DummyMetadataDbManager::check_no_readType(void) const
            {
                QList<QList<QVariant>> empty;
                check_readType(empty);
            }

        }
    }
}
