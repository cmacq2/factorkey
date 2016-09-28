#include "metadatastoragehandler.h"
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

            DummyMetadataStorageHandler::DummyMetadataStorageHandler(const otp::storage::OTPTokenType& type, const QStringList& validKeys):
                QObject(), otp::storage::db::MetadataStorageHandler(type), m_validKeys(validKeys)
            {
                init();
            }
            DummyMetadataStorageHandler::~DummyMetadataStorageHandler() {}

            QSignalSpy * DummyMetadataStorageHandler::spy_fetchParam(void) const
            {
                return m_spy_fetchParam;
            }

            QSignalSpy * DummyMetadataStorageHandler::spy_saveParam(void) const
            {
                return m_spy_saveParam;
            }

            QSignalSpy * DummyMetadataStorageHandler::spy_type(void) const
            {
                return m_spy_type;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_saveMetaData(void) const
            {
                return m_spy_saveMetaData;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_fetchMetaData(void) const
            {
                return m_spy_fetchMetaData;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_deleteMetaData(void) const
            {
                return m_spy_deleteMetaData;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_deleteParam(void) const
            {
                return m_spy_deleteParam;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_keys(void) const
            {
                return m_spy_keys;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_isParamNameValid(void) const
            {
                return m_spy_isParamNameValid;
            }

            bool DummyMetadataStorageHandler::saveParam(const QString& entryId, const QString& param, const QVariant& value) const
            {
                bool result = impl_saveParam(entryId, param, value);
                emit notify_saveParam(result, entryId, param, value);
                return result;
            }

            bool DummyMetadataStorageHandler::allowSaveParam(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_saveParam(const QString&, const QString&, const QVariant&) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::fetchParam(const QString& entryId, const QString& param, QVariant& value) const
            {
                bool result = impl_fetchParam(entryId, param, value);
                emit notify_fetchParam(result, entryId, param, value);
                return result;
            }

            bool DummyMetadataStorageHandler::impl_fetchParam(const QString&, const QString&, QVariant&) const
            {
                return false;
            }

            bool DummyMetadataStorageHandler::allowFetchParam(void) const
            {
                return true;
            }

            otp::storage::OTPTokenType DummyMetadataStorageHandler::type(void) const
            {
                otp::storage::OTPTokenType result = impl_type();
                emit notify_type(result);
                return result;
            }

            otp::storage::OTPTokenType DummyMetadataStorageHandler::impl_type(void) const
            {
                return otp::storage::db::MetadataStorageHandler::type();
            }

            bool DummyMetadataStorageHandler::saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata) const
            {
                bool result = impl_saveMetaData(entryId, metadata);
                emit notify_saveMetaData(result, entryId, metadata);
                return result;
            }

            bool DummyMetadataStorageHandler::allowSaveMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_saveMetaData(const QString&, const QHash<QString,QVariant>&) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata) const
            {
                bool result = impl_fetchMetaData(entryId, metadata);
                emit notify_fetchMetaData(result, entryId, metadata);
                return result;
            }

            bool DummyMetadataStorageHandler::allowFetchMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_fetchMetaData(const QString&, QHash<QString,QVariant>&) const
            {
                return false;
            }

            bool DummyMetadataStorageHandler::deleteMetaData(const QString& entryId, const QStringList& keys) const
            {
                bool result = impl_deleteMetaData(entryId, keys);
                emit notify_deleteMetaData(result, entryId, keys);
                return result;
            }

            bool DummyMetadataStorageHandler::allowDeleteMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_deleteMetaData(const QString&, const QStringList&) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::allowDeleteParam(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_deleteParam(const QString&, const QString&) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::deleteParam(const QString& entryId, const QString& key) const
            {
                bool result = impl_deleteParam(entryId, key);
                emit notify_deleteParam(result, entryId, key);
                return result;
            }

            const QStringList DummyMetadataStorageHandler::keys(void) const
            {
                const QStringList result = impl_keys();
                emit notify_keys(result);
                return result;
            }

            const QStringList DummyMetadataStorageHandler::impl_keys(void) const
            {
                return m_validKeys;
            }

            bool DummyMetadataStorageHandler::isParamNameValid(const QString& param) const
            {
                bool result = impl_isParamNameValid(param);
                emit notify_isParamNameValid(result, param);
                return result;
            }

            bool DummyMetadataStorageHandler::impl_isParamNameValid(const QString& param) const
            {
                return m_validKeys.contains(param);
            }

            void DummyMetadataStorageHandler::init(void)
            {
                stubs::storage::db::internal::register_meta_types();

                m_spy_type = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_type);
                m_spy_saveMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_saveMetaData);
                m_spy_fetchMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_fetchMetaData);
                m_spy_deleteMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_deleteMetaData);
                m_spy_deleteParam = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_deleteParam);
                m_spy_keys = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_keys);
                m_spy_isParamNameValid = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_isParamNameValid);
                m_spy_fetchParam = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_fetchParam);
                m_spy_saveParam = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_saveParam);
            }

            void DummyMetadataStorageHandler::check_deleteMetaData(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_deleteMetaData();
                QVERIFY2(spy && spy->isValid(), "deleteMetaData() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 3);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    QCOMPARE(p3.userType(), qMetaTypeId<QStringList>());

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 3, "Internal sanity check failed: size of expected call args should be 3");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e2.type() == qMetaTypeId<QStringList>(), "Internal sanity check failed: type of third expected call arg should be QStringList");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE(p3.value<QStringList>(), e3.value<QStringList>());
                }
            }

            void DummyMetadataStorageHandler::check_delete_param(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_deleteParam();
                QVERIFY2(spy && spy->isValid(), "deleteParam() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 3);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    QCOMPARE(p3.type(), QVariant::String);

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 3, "Internal sanity check failed: size of expected call args should be 3");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");

                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.type() == QVariant::String, "Internal sanity check failed: type of third expected call arg should be string");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE(p3.toString(), e3.toString());
                }
            }

            void DummyMetadataStorageHandler::check_no_delete_param(void) const
            {
                QList<QList<QVariant>> empty;
                check_delete_param(empty);
            }

            QList<QList<QVariant>>& DummyMetadataStorageHandler::expect_deleted_param(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QString& param)
            {
                QList<QVariant> args;
                args << status << entry << param;
                return callStack << args;
            }

            void DummyMetadataStorageHandler::check_isParamNameValid(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_isParamNameValid();
                QVERIFY2(spy && spy->isValid(), "isParamNameValid() should be spied on by a valid QSignalSpy");

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

            void DummyMetadataStorageHandler::check_no_isParamNameValid(void) const
            {
                QList<QList<QVariant>> empty;
                check_isParamNameValid(empty);
            }

            QList<QList<QVariant>>& DummyMetadataStorageHandler::expect_paramName(QList<QList<QVariant>>& callStack, bool status, const QString& param)
            {
                QList<QVariant> args;
                args << status << param;
                return callStack << args;
            }

            static void check_metadata(const QList<QList<QVariant>>& callArgs, QSignalSpy * spy, const char * spyValidMessage)
            {
                QVERIFY2(spy && spy->isValid(), spyValidMessage);

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 3);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    auto type = qMetaTypeId<QHash<QString,QVariant>>();
                    QCOMPARE(p3.userType(), type);

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 3, "Internal sanity check failed: size of expected call args should be 3");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2);

                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.userType() == type, "Internal sanity check failed: type of third expected call arg should be QHash<QString,QVariant>");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    auto v1 = p3.value<QHash<QString,QVariant>>(), v2 = e3.value<QHash<QString,QVariant>>();
                    QCOMPARE(v1, v2);
                }
            }

            void DummyMetadataStorageHandler::check_saveMetaData(const QList<QList<QVariant>>& callArgs) const
            {
                check_metadata(callArgs, spy_saveMetaData(), "saveMetaData() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataStorageHandler::check_no_saveMetaData(void) const
            {
                QList<QList<QVariant>> empty;
                check_saveMetaData(empty);
            }

            void DummyMetadataStorageHandler::check_fetchMetaData(const QList<QList<QVariant>>& callArgs) const
            {
                check_metadata(callArgs, spy_fetchMetaData(), "fetchMetaData() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataStorageHandler::check_no_fetchMetaData(void) const
            {
                QList<QList<QVariant>> empty;
                check_fetchMetaData(empty);
            }

            QList<QList<QVariant>>& DummyMetadataStorageHandler::expect_metadata(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QHash<QString,QVariant>& metadata)
            {
                QList<QVariant> args;
                args << status << entry << metadata;
                return callStack << args;
            }

            void DummyMetadataStorageHandler::check_keys(const QList<QStringList>& callArgs) const
            {
                QSignalSpy * spy = spy_keys();
                QVERIFY2(spy && spy->isValid(), "keys() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 1);

                    auto p1 = paramCallArgs.at(0);
                    QCOMPARE(p1.userType(), qMetaTypeId<QStringList>());

                    QCOMPARE(p1.value<QStringList>(), callArgs.at(i));
                }
            }

            void DummyMetadataStorageHandler::check_no_keys(void) const
            {
                QList<QStringList> empty;
                check_keys(empty);
            }

            void DummyMetadataStorageHandler::check_type(const QList<enum otp::storage::OTPTokenType>& callArgs) const
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

            void  DummyMetadataStorageHandler::check_no_type(void) const
            {
                QList<enum otp::storage::OTPTokenType> empty;
                check_type(empty);
            }

            QList<QList<QVariant>>& DummyMetadataStorageHandler::expect_param(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QString& param, const QVariant& value)
            {
                QList<QVariant> args;
                args << status << entry << param << value;
                return callStack << args;
            }

            static void check_param(const QList<QList<QVariant>>& callArgs, QSignalSpy * spy, const char * spyValidMessage)
            {
                QVERIFY2(spy && spy->isValid(), spyValidMessage);

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 4);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2), p4 = paramCallArgs.at(3);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    QCOMPARE(p3.type(), QVariant::String);

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 4, "Internal sanity check failed: size of expected call args should be 4");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2), e4 = expectedCallArgs.at(3);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");

                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.type() == QVariant::String, "Internal sanity check failed: type of third expected call arg should be string");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE(p3.toString(), e3.toString());

                    QCOMPARE(p4.userType(), e4.userType());
                    QCOMPARE(p4, e4);
                }
            }

            void DummyMetadataStorageHandler::check_fetchParam(const QList<QList<QVariant>>& callArgs) const
            {
                check_param(callArgs, spy_fetchParam(), "fetchParam() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataStorageHandler::check_no_fetchParam(void) const
            {
                QList<QList<QVariant>> empty;
                check_fetchParam(empty);
            }

            void DummyMetadataStorageHandler::check_saveParam(const QList<QList<QVariant>>& callArgs) const
            {
                check_param(callArgs, spy_saveParam(), "saveParam() should be spied on by a valid QSignalSpy");
            }

            void DummyMetadataStorageHandler::check_no_saveParam(void) const
            {
                QList<QList<QVariant>> empty;
                check_saveParam(empty);
            }
        }
    }
}

