#include "storage.h"
#include "otp/storage/util.h"

#include <QtDebug>
#include <QTest>

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)

namespace stubs
{
    namespace storage
    {
        namespace internal
        {
            static bool register_meta_types(void)
            {
                static bool result = otp::storage::checkOTPTokenTypeIsMetaType();
                return result;
            }
        }

        DummyStoragePrivate::DummyStoragePrivate(const QString& entryId,
                            otp::storage::OTPTokenType type,
                            const QString& password,
                            const QHash<QString,QVariant>& preset, QObject * parent): QObject(parent), FailingStoragePrivate(), m_entryId(entryId), m_type(type), m_password(password), m_map(preset)
        {
            init();
        }

        DummyStoragePrivate::DummyStoragePrivate(const QString& entryId,
                            otp::storage::OTPTokenType type,
                            const QString& password, QObject * parent): QObject(parent), FailingStoragePrivate(), m_entryId(entryId), m_type(type), m_password(password)
        {
            init();
        }

        DummyStoragePrivate::DummyStoragePrivate(
                            otp::storage::OTPTokenType type,
                            const QString& password, QObject * parent): QObject(parent), FailingStoragePrivate(), m_type(type), m_password(password)
        {
            init();
        }

        DummyStoragePrivate::DummyStoragePrivate(
                            otp::storage::OTPTokenType type,
                            const QString& password,
                            const QHash<QString,QVariant>& preset, QObject * parent): QObject(parent), FailingStoragePrivate(), m_type(type), m_password(password), m_map(preset)
        {
            init();
        }

        DummyStoragePrivate::~DummyStoragePrivate()
        {
        }

        void DummyStoragePrivate::init(void)
        {
            stubs::storage::internal::register_meta_types();

            m_spy_writePassword = new QSignalSpy(this, &DummyStoragePrivate::notify_writePassword);
            m_spy_readTokenType = new QSignalSpy(this, &DummyStoragePrivate::notify_readTokenType);
            m_spy_writeTokenType = new QSignalSpy(this, &DummyStoragePrivate::notify_writeTokenType);
            m_spy_readParam = new QSignalSpy(this, &DummyStoragePrivate::notify_readParam);
            m_spy_readPassword = new QSignalSpy(this, &DummyStoragePrivate::notify_readPassword);
            m_spy_writeParam = new QSignalSpy(this, &DummyStoragePrivate::notify_writeParam);
            m_spy_exists = new QSignalSpy(this, &DummyStoragePrivate::notify_exists);
            m_spy_commit = new QSignalSpy(this, &DummyStoragePrivate::notify_commit);
            m_spy_poll = new QSignalSpy(this, &DummyStoragePrivate::notify_poll);
            m_spy_type = new QSignalSpy(this, &DummyStoragePrivate::notify_type);
            m_spy_entryId = new QSignalSpy(this, &DummyStoragePrivate::notify_entryId);
        }

        const QHash<QString,QVariant>& DummyStoragePrivate::rawStorage(void) const
        {
            return m_map;
        }

        QSignalSpy * DummyStoragePrivate::spy_type(void) const
        {
            return m_spy_type;
        }

        QSignalSpy * DummyStoragePrivate::spy_entryId(void) const
        {
            return m_spy_entryId;
        }

        QSignalSpy * DummyStoragePrivate::spy_writePassword(void) const
        {
            return m_spy_writePassword;
        }

        QSignalSpy * DummyStoragePrivate::spy_readTokenType(void) const
        {
            return m_spy_readTokenType;
        }

        QSignalSpy * DummyStoragePrivate::spy_writeTokenType(void) const
        {
            return m_spy_writeTokenType;
        }

        QSignalSpy * DummyStoragePrivate::spy_readParam(void) const
        {
            return m_spy_readParam;
        }

        QSignalSpy * DummyStoragePrivate::spy_readPassword(void) const
        {
            return m_spy_readPassword;
        }

        QSignalSpy * DummyStoragePrivate::spy_writeParam(void) const
        {
            return m_spy_writeParam;
        }

        QSignalSpy * DummyStoragePrivate::spy_exists(void) const
        {
            return m_spy_exists;
        }

        QSignalSpy * DummyStoragePrivate::spy_commit(void) const
        {
            return m_spy_commit;
        }

        QSignalSpy * DummyStoragePrivate::spy_poll(void) const
        {
            return m_spy_poll;
        }

        bool DummyStoragePrivate::allowWriteParam(void) const
        {
            return true;
        }

        bool DummyStoragePrivate::allowWriteTokenType(void) const
        {
            return true;
        }

        bool DummyStoragePrivate::allowWritePassword(void) const
        {
            return true;
        }

        bool DummyStoragePrivate::allowReadParam(void) const
        {
            return true;
        }

        bool DummyStoragePrivate::allowReadTokenType(void) const
        {
            return true;
        }
        bool DummyStoragePrivate::allowReadPassword(void) const
        {
            return true;
        }

        QString DummyStoragePrivate::entryId(void) const
        {
            auto result = impl_entryId();
            emit notify_entryId(result);
            return result;
        }

        QString DummyStoragePrivate::impl_entryId(void) const
        {
            return m_entryId;
        }

        otp::storage::OTPTokenType DummyStoragePrivate::type(void) const
        {
            auto result = impl_type();
            emit notify_type(result);
            return result;
        }

        otp::storage::OTPTokenType DummyStoragePrivate::impl_type(void) const
        {
            return m_type;
        }

        bool DummyStoragePrivate::impl_writePassword(const QString& password)
        {
            if(allowWritePassword())
            {
                m_password = password;
                return true;
            }
            return false;
        }

        bool DummyStoragePrivate::writePassword(const QString& password)
        {
            auto result = impl_writePassword(password);
            emit notify_writePassword(result, password);
            return result;
        }

        bool DummyStoragePrivate::readPassword(QString& password) const
        {
            auto result = impl_readPassword(password);
            emit notify_readPassword(result, password);
            return result;
        }

        bool DummyStoragePrivate::impl_readPassword(QString& password) const
        {
            if(allowReadPassword())
            {
                password = m_password;
                return true;
            }
            return false;
        }

        bool DummyStoragePrivate::readTokenType(otp::storage::OTPTokenType& type) const
        {
            auto result = impl_readTokenType(type);
            emit notify_readTokenType(result, type);
            return result;
        }

        bool DummyStoragePrivate::impl_readTokenType(otp::storage::OTPTokenType& type) const
        {
            if(allowReadTokenType())
            {
                type = m_type;
                return true;
            }
            return false;
        }

        bool DummyStoragePrivate::writeTokenType(const otp::storage::OTPTokenType& type)
        {
            auto result = impl_writeTokenType(type);
            emit notify_writeTokenType(result, type);
            return result;
        }

        bool DummyStoragePrivate::impl_writeTokenType(const otp::storage::OTPTokenType& type)
        {
            if(allowWriteTokenType())
            {
                m_type = type;
                return true;
            }
            return false;
        }

        bool DummyStoragePrivate::readParam(const QString& param, QVariant& value) const
        {
            auto result = impl_readParam(param, value);
            emit notify_readParam(result, param, value);
            return result;
        }

        bool DummyStoragePrivate::impl_readParam(const QString& param, QVariant& value) const
        {
            if(allowReadParam() && m_map.contains(param))
            {
                value = m_map.value(param);
                return true;
            }
            return false;
        }

        bool DummyStoragePrivate::writeParam(const QString& param, const QVariant& value)
        {
            auto result = impl_writeParam(param, value);
            emit notify_writeParam(result, param, value);
            return result;
        }

        bool DummyStoragePrivate::impl_writeParam(const QString& param, const QVariant& value)
        {
            if(allowWriteParam())
            {
                m_map.insert(param, value);
                return true;
            }
            return false;
        }

        bool DummyStoragePrivate::exists(void) const
        {
            auto result = impl_exists();
            emit notify_exists(result);
            return result;
        }

        bool DummyStoragePrivate::impl_exists(void) const
        {
            return false;
        }

        bool DummyStoragePrivate::commit(void)
        {
            auto result = impl_commit();
            emit notify_commit(result);
            return result;
        }

        bool DummyStoragePrivate::impl_commit(void)
        {
            return false;
        }

        bool DummyStoragePrivate::poll(void)
        {
            auto result = impl_poll();
            emit notify_poll(result);
            return result;
        }

        bool DummyStoragePrivate::impl_poll(void)
        {
            return false;
        }

        void DummyStoragePrivate::check_entryId(const QList<QString>& callArgs) const
        {
            QSignalSpy * spy = spy_entryId();
            QVERIFY2(spy && spy->isValid(), "entryId() should be spied on by a valid QSignalSpy");

            int size = callArgs.size();
            QCOMPARE(spy->count(), size);

            for(int i = 0; i < size; ++i)
            {
                auto paramCallArgs = spy->at(i);
                QCOMPARE(paramCallArgs.size(), 1);

                auto p1 = paramCallArgs.at(0);
                QCOMPARE(p1.type(), QVariant::String);
                QCOMPARE(p1.toString(), callArgs.at(i));
            }
        }

        void DummyStoragePrivate::check_no_entryId(void) const
        {
            QList<QString> empty;
            check_entryId(empty);
        }

        void DummyStoragePrivate::check_type(const QList<enum otp::storage::OTPTokenType>& callArgs) const
        {
            QSignalSpy * spy = spy_type();
            QVERIFY2(spy && spy->isValid(), "type() should be spied on by a valid QSignalSpy");

            int size = callArgs.size();
            QCOMPARE(spy->count(), size);

            for(int i = 0; i < size; ++i)
            {
                auto paramCallArgs = spy->at(i);
                QCOMPARE(paramCallArgs.size(), 1);

                auto p1 = paramCallArgs.at(0);
                QCOMPARE(p1.userType(), qMetaTypeId<enum otp::storage::OTPTokenType>());
                QCOMPARE(p1.value<enum otp::storage::OTPTokenType>(), callArgs.at(i));
            }
        }

        void DummyStoragePrivate::check_no_type(void) const
        {
            QList<enum otp::storage::OTPTokenType> empty;
            check_type(empty);
        }

        QList<QList<QVariant>>& DummyStoragePrivate::expect_token_type(QList<QList<QVariant>>& callStack, bool status, enum otp::storage::OTPTokenType type)
        {
            QList<QVariant> args;
            args << status << QVariant::fromValue(type);
            return callStack << args;
        }

        static void check_token_type(const QList<QList<QVariant>>& callArgs, QSignalSpy * spy, const char * validSpyMessage)
        {
            QVERIFY2(spy && spy->isValid(), validSpyMessage);

            int size = callArgs.size();
            QCOMPARE(spy->count(), size);

            for(int i = 0; i < size; ++i)
            {
                auto paramCallArgs = spy->at(i);
                QCOMPARE(paramCallArgs.size(), 2);

                auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1);
                QCOMPARE(p1.type(), QVariant::Bool);
                QCOMPARE(p2.userType(), qMetaTypeId<enum otp::storage::OTPTokenType>());

                auto expectedCallArgs = callArgs.at(i);
                QVERIFY2(expectedCallArgs.size() == 2, "Internal sanity check failed: size of expected call args should be 2");

                auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1);
                QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");

                QVERIFY2(e2.userType() == qMetaTypeId<enum otp::storage::OTPTokenType>(), "Internal sanity check failed: type of second expected call arg should be OTPTokenType");

                QCOMPARE(p1.toBool(), e1.toBool());
                QCOMPARE(p2.value<enum otp::storage::OTPTokenType>(), e2.value<enum otp::storage::OTPTokenType>());
            }
        }

        void DummyStoragePrivate::check_write_token_type(const QList<QList<QVariant>>& callArgs) const
        {
            check_token_type(callArgs, spy_writeTokenType(), "writeTokenType() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_no_write_token_type(void) const
        {
            QList<QList<QVariant>> empty;
            check_write_token_type(empty);
        }

        void DummyStoragePrivate::check_read_token_type(const QList<QList<QVariant>>& callArgs) const
        {
            check_token_type(callArgs, spy_readTokenType(), "readTokenType() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_no_read_token_type(void) const
        {
            QList<QList<QVariant>> empty;
            check_read_token_type(empty);
        }

        QList<QList<QVariant>>& DummyStoragePrivate::expect_password(QList<QList<QVariant>>& callStack, bool status, const QString& password)
        {
            QList<QVariant> args;
            args << status << password;
            return callStack << args;
        }

        static void check_password(const QList<QList<QVariant>>& callArgs, QSignalSpy * spy, const char * validSpyMessage)
        {
            QVERIFY2(spy && spy->isValid(), validSpyMessage);

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

        void DummyStoragePrivate::check_write_password(const QList<QList<QVariant>>& callArgs) const
        {
            check_password(callArgs, spy_writePassword(), "writePassword() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_no_write_password(void) const
        {
            QList<QList<QVariant>> empty;
            check_write_password(empty);
        }

        void DummyStoragePrivate::check_read_password(const QList<QList<QVariant>>& callArgs) const
        {
            check_password(callArgs, spy_readPassword(), "readPassword() should be spied on by a valid QSignalSpy");
        }

        QList<QList<QVariant>>& DummyStoragePrivate::expect_param(QList<QList<QVariant>>& callStack, bool status, const QString& param, const QVariant& value)
        {
            QList<QVariant> args;
            args << status << param << value;
            return callStack << args;
        }

        static void check_param(const QList<QList<QVariant>>& callArgs, QSignalSpy * spy, const char * validSpyMessage)
        {
            QVERIFY2(spy && spy->isValid(), validSpyMessage);

            int size = callArgs.size();
            QCOMPARE(spy->count(), size);

            for(int i = 0; i < size; ++i)
            {
                auto paramCallArgs = spy->at(i);
                QCOMPARE(paramCallArgs.size(), 3);

                auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2);
                QCOMPARE(p1.type(), QVariant::Bool);
                QCOMPARE(p2.type(), QVariant::String);

                auto expectedCallArgs = callArgs.at(i);
                QVERIFY2(expectedCallArgs.size() == 3, "Internal sanity check failed: size of expected call args should be 3");

                auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2);
                QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");


                QCOMPARE(p1.toBool(), e1.toBool());
                QCOMPARE(p2.toString(), e2.toString());

                QCOMPARE(p3.type(), e3.type());
                QCOMPARE(p3, e3);
            }
        }

        void DummyStoragePrivate::check_write_param(const QList<QList<QVariant>>& callArgs) const
        {
            check_param(callArgs, spy_writeParam(), "writeParam() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_no_write_param(void) const
        {
            QList<QList<QVariant>> empty;
            check_write_param(empty);
        }

        void DummyStoragePrivate::check_read_param(const QList<QList<QVariant>>& callArgs) const
        {
            check_param(callArgs, spy_readParam(), "readParam() should be spied on by a valid QSignalSpy");
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

        void DummyStoragePrivate::check_no_commit(void) const
        {
            QList<bool> empty;
            check_io(empty, spy_commit(), "commit() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_commit(const QList<bool>& callArgs) const
        {
            check_io(callArgs, spy_commit(), "commit() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_no_poll(void) const
        {
            QList<bool> empty;
            check_io(empty, spy_poll(), "poll() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_poll(const QList<bool>& callArgs) const
        {
            check_io(callArgs, spy_poll(), "poll() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_no_exists(void) const
        {
            QList<bool> empty;
            check_io(empty, spy_exists(), "exists() should be spied on by a valid QSignalSpy");
        }

        void DummyStoragePrivate::check_exists(const QList<bool>& callArgs) const
        {
            check_io(callArgs, spy_exists(), "exists() should be spied on by a valid QSignalSpy");
        }


    }
}
