#include "metadatastoragehandler.h"
#include "otp/storage/util.h"

#include <QtDebug>
#include <QTest>

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)
Q_DECLARE_METATYPE(otp::storage::db::MetadataDbManager *)

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            namespace internal
            {
                static bool checkMetaDbManagerIsMetaType(void)
                {
                    static auto type = qRegisterMetaType<otp::storage::db::MetadataDbManager *>();
                    bool result = (bool) type;
                    if(!result)
                    {
                        qWarning() << "Failed to register meta types! Some signal/slot spying functionality may not work as expected.";
                    }
                    return result;
                }

                static bool register_meta_types(void)
                {
                    static bool result = otp::storage::checkOTPTokenTypeIsMetaType() && checkMetaDbManagerIsMetaType();
                    return result;
                }
            }

            DummyMetadataStorageHandler::DummyMetadataStorageHandler(const otp::storage::OTPTokenType& type,
                                                                     const QHash<QString,QString>& tables,
                                                                     const QSet<QString>& schema,
                                                                     const QHash<QString,QString> columnsToParams,
                                                                     const QHash<QString,QString> paramsToTables):
                QObject(), otp::storage::db::MetadataStorageHandler(type, tables, schema, columnsToParams, paramsToTables)
            {
                init();
            }
            DummyMetadataStorageHandler::~DummyMetadataStorageHandler() {}

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

            QSignalSpy * DummyMetadataStorageHandler::spy_resetMetaData(void) const
            {
                return m_spy_resetMetaData;
            }

            QSignalSpy * DummyMetadataStorageHandler::spy_pruneMetaData(void) const
            {
                return m_spy_pruneMetaData;
            }

            QSignalSpy * DummyMetadataStorageHandler::spy_keys(void) const
            {
                return m_spy_keys;
            }
            QSignalSpy * DummyMetadataStorageHandler::spy_isParamNameValid(void) const
            {
                return m_spy_isParamNameValid;
            }

            QSignalSpy * DummyMetadataStorageHandler::spy_schema(void) const
            {
                return m_spy_schema;
            }

            const QSet<QString>& DummyMetadataStorageHandler::schema(void) const
            {
                const auto& result = impl_schema();
                emit notify_schema(result);
                return result;
            }

            const QSet<QString>& DummyMetadataStorageHandler::impl_schema(void) const
            {
                return MetadataStorageHandler::schema();
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

            bool DummyMetadataStorageHandler::saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const
            {
                bool result = impl_saveMetaData(entryId, metadata, db);
                emit notify_saveMetaData(result, entryId, metadata, db);
                return result;
            }

            bool DummyMetadataStorageHandler::allowSaveMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_saveMetaData(const QString&, const QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const
            {
                bool result = impl_fetchMetaData(entryId, metadata, db);
                emit notify_fetchMetaData(result, entryId, metadata, db);
                return result;
            }

            bool DummyMetadataStorageHandler::allowFetchMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_fetchMetaData(const QString&, QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *) const
            {
                return false;
            }

            bool DummyMetadataStorageHandler::resetMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const
            {
                bool result = impl_resetMetaData(entryId, keys, db);
                emit notify_resetMetaData(result, entryId, keys, db);
                return result;
            }

            bool DummyMetadataStorageHandler::pruneMetaData(const QString& entryId, const QSet<QString>& keys, const QSet<QString>& tables, otp::storage::db::MetadataDbManager * db) const
            {
                bool result = impl_pruneMetaData(entryId, keys, tables, db);
                emit notify_pruneMetaData(result, entryId, keys, tables, db);
                return result;
            }

            bool DummyMetadataStorageHandler::deleteMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const
            {
                bool result = impl_deleteMetaData(entryId, keys, db);
                emit notify_deleteMetaData(result, entryId, keys, db);
                return result;
            }

            bool DummyMetadataStorageHandler::allowDeleteMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::allowResetMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::allowPruneMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_deleteMetaData(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_resetMetaData(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::impl_pruneMetaData(const QString&, const QSet<QString>&, const QSet<QString>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            QSet<QString> DummyMetadataStorageHandler::keys(void) const
            {
                const auto result = impl_keys();
                emit notify_keys(result);
                return result;
            }

            QSet<QString> DummyMetadataStorageHandler::impl_keys(void) const
            {
                return MetadataStorageHandler::keys();
            }

            bool DummyMetadataStorageHandler::isParamNameValid(const QString& param) const
            {
                bool result = impl_isParamNameValid(param);
                emit notify_isParamNameValid(result, param);
                return result;
            }

            bool DummyMetadataStorageHandler::impl_isParamNameValid(const QString& param) const
            {
                return MetadataStorageHandler::isParamNameValid(param);
            }

            void DummyMetadataStorageHandler::init(void)
            {
                stubs::storage::db::internal::register_meta_types();

                m_spy_schema = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_schema);
                m_spy_type = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_type);
                m_spy_saveMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_saveMetaData);
                m_spy_fetchMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_fetchMetaData);
                m_spy_deleteMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_deleteMetaData);
                m_spy_resetMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_resetMetaData);
                m_spy_pruneMetaData = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_pruneMetaData);
                m_spy_keys = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_keys);
                m_spy_isParamNameValid = new QSignalSpy(this, &DummyMetadataStorageHandler::notify_isParamNameValid);
            }

            void DummyMetadataStorageHandler::check_pruneMetaData(const QList<QList<QVariant>>& callArgs) const
            {
                QSignalSpy * spy = spy_pruneMetaData();
                QVERIFY2(spy && spy->isValid(), "pruneMetaData() should be spid on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 5);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2), p4 = paramCallArgs.at(3), p5 = paramCallArgs.at(4);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    QCOMPARE(p3.userType(), qMetaTypeId<QSet<QString>>());
                    QCOMPARE(p4.userType(), qMetaTypeId<QSet<QString>>());
                    QCOMPARE(p5.userType(), qMetaTypeId<otp::storage::db::MetadataDbManager*>());

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 5, "Internal sanity check failed: size of expected call args should be 5");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2), e4 = expectedCallArgs.at(3), e5 = expectedCallArgs.at(4);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.userType() == qMetaTypeId<QSet<QString>>(), "Internal sanity check failed: type of third expected call arg should be QSet<QString>");
                    QVERIFY2(e4.userType() == qMetaTypeId<QSet<QString>>(), "Internal sanity check failed: type of fourth expected call arg should be QSet<QString>");
                    QVERIFY2(e5.userType() == qMetaTypeId<otp::storage::db::MetadataDbManager*>(), "Internal sanity check failed: type of fifth expected parameter should be otp::storage::db::MetadataDbManager *");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE(p3.value<QSet<QString>>(), e3.value<QSet<QString>>());
                    QCOMPARE(p4.value<QSet<QString>>(), e4.value<QSet<QString>>());
                    QCOMPARE(p5.value<otp::storage::db::MetadataDbManager*>(), e5.value<otp::storage::db::MetadataDbManager*>());
                }
            }

            void DummyMetadataStorageHandler::check_no_pruneMetaData(void) const
            {
                const QList<QList<QVariant>> empty;
                check_pruneMetaData(empty);
            }

            static void check_reset_deleted_metadata(QSignalSpy * spy, const char * spyValidMessage, const QList<QList<QVariant>>& callArgs)
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
                    QCOMPARE(p3.userType(), qMetaTypeId<QSet<QString>>());
                    QCOMPARE(p4.userType(), qMetaTypeId<otp::storage::db::MetadataDbManager*>());

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 4, "Internal sanity check failed: size of expected call args should be 4");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2), e4 = expectedCallArgs.at(3);
                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.userType() == qMetaTypeId<QSet<QString>>(), "Internal sanity check failed: type of third expected call arg should be QSet<QString>");
                    QVERIFY2(e4.userType() == qMetaTypeId<otp::storage::db::MetadataDbManager*>(), "Internal sanity check failed: type of fourth expected parameter should be otp::storage::db::MetadataDbManager *");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE(p3.value<QSet<QString>>(), e3.value<QSet<QString>>());
                    QCOMPARE(p4.value<otp::storage::db::MetadataDbManager*>(), e4.value<otp::storage::db::MetadataDbManager*>());
                }
            }

            void DummyMetadataStorageHandler::check_resetMetaData(const QList<QList<QVariant>>& callArgs) const
            {
                check_reset_deleted_metadata(spy_resetMetaData(), "resetMetaData() should be spied on by a valid QSignalSpy", callArgs);
            }

            void DummyMetadataStorageHandler::check_no_resetMetaData(void) const
            {
                const QList<QList<QVariant>> empty;
                check_resetMetaData(empty);
            }

            void DummyMetadataStorageHandler::check_deleteMetaData(const QList<QList<QVariant>>& callArgs) const
            {
                check_reset_deleted_metadata(spy_deleteMetaData(), "deleteMetaData() should be spied on by a valid QSignalSpy", callArgs);
            }

            void DummyMetadataStorageHandler::check_no_deleteMetaData(void) const
            {
                const QList<QList<QVariant>> empty;
                check_deleteMetaData(empty);
            }

            void DummyMetadataStorageHandler::check_schema(const QList<QSet<QString>>& callArgs) const
            {
                QSignalSpy * spy = spy_schema();
                QVERIFY2(spy && spy->isValid(), "schema() should be spied on by a valid QSignalSpy");

                int size = callArgs.size();
                QCOMPARE(spy->count(), size);

                for(int i = 0; i < size; ++i)
                {
                    auto paramCallArgs = spy->at(i);
                    QCOMPARE(paramCallArgs.size(), 1);

                    auto p1 = paramCallArgs.at(0);
                    QCOMPARE(p1.userType(), qMetaTypeId<QSet<QString>>());

                    auto e1 = callArgs.at(i);

                    QCOMPARE(p1.value<QSet<QString>>(), e1);
                }
            }

            void DummyMetadataStorageHandler::check_no_schema(void) const
            {
                QList<QSet<QString>> empty;
                return check_schema(empty);
            }


            QList<QList<QVariant>>& DummyMetadataStorageHandler::expect_params(QList<QList<QVariant>>& callStack,
                                                                               bool status,
                                                                               const QString& entry,
                                                                               const QSet<QString>& params,
                                                                               otp::storage::db::MetadataDbManager * db)
            {
                QList<QVariant> args;
                args << status << entry << QVariant::fromValue<QSet<QString>>(params) << QVariant::fromValue<otp::storage::db::MetadataDbManager *>(db);
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
                    QCOMPARE(paramCallArgs.size(), 4);

                    auto p1 = paramCallArgs.at(0), p2  = paramCallArgs.at(1), p3 = paramCallArgs.at(2), p4 = paramCallArgs.at(3);
                    QCOMPARE(p1.type(), QVariant::Bool);
                    QCOMPARE(p2.type(), QVariant::String);
                    QCOMPARE(p3.userType(), (qMetaTypeId<QHash<QString,QVariant>>()) );
                    QCOMPARE(p4.userType(), (qMetaTypeId<otp::storage::db::MetadataDbManager*>()) );

                    auto expectedCallArgs = callArgs.at(i);
                    QVERIFY2(expectedCallArgs.size() == 4, "Internal sanity check failed: size of expected call args should be 4");

                    auto e1 = expectedCallArgs.at(0), e2 = expectedCallArgs.at(1), e3 = expectedCallArgs.at(2), e4 = expectedCallArgs.at(3);

                    QVERIFY2(e1.type() == QVariant::Bool, "Internal sanity check failed: type of first expected call arg should be boolean");
                    QVERIFY2(e2.type() == QVariant::String, "Internal sanity check failed: type of second expected call arg should be string");
                    QVERIFY2(e3.userType() == (qMetaTypeId<QHash<QString,QVariant>>()), "Internal sanity check failed: type of third expected call arg should be QHash<QString,QVariant>");
                    QVERIFY2(e4.userType() == (qMetaTypeId<otp::storage::db::MetadataDbManager*>()), "Internal sanity check failed: type of fourth expected parameter should be otp::storage::db::MetadataDbManager *");

                    QCOMPARE(p1.toBool(), e1.toBool());
                    QCOMPARE(p2.toString(), e2.toString());
                    QCOMPARE((p3.value<QHash<QString,QVariant>>()), (e3.value<QHash<QString,QVariant>>()));
                    QCOMPARE((p4.value<otp::storage::db::MetadataDbManager*>()), (e4.value<otp::storage::db::MetadataDbManager*>()));
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

            QList<QList<QVariant>>& DummyMetadataStorageHandler::expect_metadata(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db)
            {
                QList<QVariant> args;
                args << status << entry << metadata << QVariant::fromValue<otp::storage::db::MetadataDbManager *>(db);
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
        }
    }
}

