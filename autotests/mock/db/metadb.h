#ifndef FACTORKEY_AUTOTESTS_MOCKED_METADBH_H
#define FACTORKEY_AUTOTESTS_MOCKED_METADBH_H


#include "otp/db/metadb.h"
#include "otp/db/metadatastoragehandler.h"
#include "autotests/lib/googlemock.h"

namespace mock
{
    namespace storage
    {
        namespace db
        {
            class MockMetadataDbManager: public otp::storage::db::MetadataDbManager
            {
            public:
                MockMetadataDbManager(const QString& connectionName,
                                      const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                      const otp::storage::db::MetadataDbManager::InitDb& configure);
                virtual ~MockMetadataDbManager();

                MOCK_CONST_METHOD0(isOpened, bool(void));
                MOCK_METHOD0(open, QSqlDatabase(void));
                MOCK_METHOD0(close, bool(void));

                MOCK_METHOD2(readType, bool(const QString& entry, QVariant& value));
                MOCK_METHOD1(removeEntries, bool(const QStringList& entryList));
                MOCK_METHOD0(removeAll, bool(void));
                MOCK_METHOD1(remove, bool(const QString& entry));
                MOCK_METHOD1(contains, bool(const QString & entry));
                MOCK_METHOD1(entries, bool(QStringList& entryList));

                MOCK_METHOD1(handler, const QSharedPointer<otp::storage::db::MetadataStorageHandler>(otp::storage::OTPTokenType type));

                MOCK_CONST_METHOD0(supportedHandlers, QList<otp::storage::OTPTokenType>(void));
                MOCK_CONST_METHOD1(supports, bool(otp::storage::OTPTokenType));
            };

            class DelegatingMockMetadataDbManager: public otp::storage::db::MetadataDbManager
            {
            public:
                DelegatingMockMetadataDbManager(const QString& connectionName,
                                                const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                                const otp::storage::db::MetadataDbManager::InitDb& configure);
                virtual ~DelegatingMockMetadataDbManager();

                MOCK_CONST_METHOD0(isOpened, bool(void));
                MOCK_METHOD0(open, QSqlDatabase(void));
                MOCK_METHOD0(close, bool(void));

                MOCK_METHOD2(readType, bool(const QString& entry, QVariant& value));
                MOCK_METHOD1(removeEntries, bool(const QStringList& entryList));
                MOCK_METHOD0(removeAll, bool(void));
                MOCK_METHOD1(remove, bool(const QString& entry));
                MOCK_METHOD1(contains, bool(const QString & entry));
                MOCK_METHOD1(entries, bool(QStringList& entryList));

                MOCK_METHOD1(handler, const QSharedPointer<otp::storage::db::MetadataStorageHandler>(otp::storage::OTPTokenType type));

                MOCK_CONST_METHOD0(supportedHandlers, QList<otp::storage::OTPTokenType>(void));
                MOCK_CONST_METHOD1(supports, bool(otp::storage::OTPTokenType));


                bool delegateToFake(const QSharedPointer<otp::storage::db::MetadataDbManager> fake);
            private:
                QSharedPointer<otp::storage::db::MetadataDbManager> m_fake;
            };
        }
    }
}
#endif
