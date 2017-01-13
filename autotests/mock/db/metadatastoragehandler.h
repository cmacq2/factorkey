#ifndef FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"
#include "otp/db/metadb.h"
#include "lib/googlemock.h"

#include <QHash>
#include <QString>
#include <QVariant>

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            class DummyMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
            {
            public:
                DummyMetadataStorageHandler(const otp::storage::OTPTokenType& type,
                                            const QHash<QString,QString>& tables,
                                            const QSet<QString>& schema,
                                            const QHash<QString,QString> columnsToParams,
                                            const QHash<QString,QString> paramsToTables);
                virtual ~DummyMetadataStorageHandler();
                virtual const QSet<QString>& schema(void) const;
                virtual otp::storage::OTPTokenType type(void) const;
                virtual bool saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const;
                virtual bool fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const;
                virtual bool pruneMetaData(const QString& entryId, const QSet<QString>& newKeys, const QSet<QString>& tablesToNullify, otp::storage::db::MetadataDbManager * db) const;
                virtual bool deleteMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const;
                virtual bool resetMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const;
                virtual QSet<QString> keys(void) const;
                virtual bool isParamNameValid(const QString& param) const;
            protected:
                virtual bool allowSaveMetaData(void) const;
                virtual bool allowFetchMetaData(void) const;
                virtual bool allowDeleteMetaData(void) const;
                virtual bool allowResetMetaData(void) const;
                virtual bool allowPruneMetaData(void) const;
            };
        }
    }
}

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
