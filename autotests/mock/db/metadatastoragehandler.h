#ifndef FACTORKEY_AUTOTESTS_MOCKED_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_MOCKED_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"
#include "otp/db/metadb.h"
#include "autotests/lib/googlemock.h"

namespace mock
{
    namespace storage
    {
        namespace db
        {
            class MockMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
            {
            public:
                MockMetadataStorageHandler(otp::storage::OTPTokenType type);
                virtual ~MockMetadataStorageHandler();
                MOCK_CONST_METHOD0(type, otp::storage::OTPTokenType());
                MOCK_CONST_METHOD0(schema, const QSet<QString>&());
                MOCK_CONST_METHOD3(saveMetaData, bool(const QString&, const QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD3(fetchMetaData, bool(const QString&, QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD4(pruneMetaData, bool(const QString&, const QSet<QString>&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD3(deleteMetaData, bool(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD3(resetMetaData, bool(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD0(keys, QSet<QString>());
                MOCK_CONST_METHOD1(isParamNameValid, bool(const QString&));
            };

            class DelegatingMockMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
            {
            public:
                DelegatingMockMetadataStorageHandler(otp::storage::OTPTokenType type);
                virtual ~DelegatingMockMetadataStorageHandler();
                MOCK_CONST_METHOD0(type, otp::storage::OTPTokenType());
                MOCK_CONST_METHOD0(schema, const QSet<QString>&());
                MOCK_CONST_METHOD3(saveMetaData, bool(const QString&, const QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD3(fetchMetaData, bool(const QString&, QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD4(pruneMetaData, bool(const QString&, const QSet<QString>&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD3(deleteMetaData, bool(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD3(resetMetaData, bool(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *));
                MOCK_CONST_METHOD0(keys, QSet<QString>());
                MOCK_CONST_METHOD1(isParamNameValid, bool(const QString&));
            private:
                bool delegateToFake(const QSharedPointer<otp::storage::db::MetadataStorageHandler> fake);
            private:
                QSharedPointer<otp::storage::db::MetadataStorageHandler> m_fake;
            };
        }
    }
}

#endif
