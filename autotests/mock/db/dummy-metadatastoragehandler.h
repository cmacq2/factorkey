#ifndef FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"
#include "otp/db/metadb.h"

#include <QHash>

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
                virtual const QSet<QString>& schema(void) const override;
                virtual otp::storage::OTPTokenType type(void) const override;
                virtual bool saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool pruneMetaData(const QString& entryId, const QSet<QString>& newKeys, const QSet<QString>& tablesToNullify, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool deleteMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool resetMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const override;
                virtual QSet<QString> keys(void) const override;
                virtual bool isParamNameValid(const QString& param) const override;
            protected:
                virtual bool allowSaveMetaData(void) const;
                virtual bool allowFetchMetaData(void) const;
                virtual bool allowDeleteMetaData(void) const;
                virtual bool allowResetMetaData(void) const;
                virtual bool allowPruneMetaData(void) const;
            };

            class FakeSingleEntryMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
            {
            public:
                FakeSingleEntryMetadataStorageHandler(const otp::storage::OTPTokenType& type, const QString& entryId,
                                            QHash<QString,QVariant> * const metadata,
                                            const QHash<QString,QString>& paramsToTables);
                FakeSingleEntryMetadataStorageHandler(const otp::storage::OTPTokenType& type, const QString& entryId,
                                                      const QHash<QString,QString>& paramsToTables);
                virtual ~FakeSingleEntryMetadataStorageHandler();
            private:
                const QString m_entryId;
                QHash<QString, QVariant> * const m_metadata;
            public:
                virtual bool saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool pruneMetaData(const QString& entryId, const QSet<QString>& newKeys, const QSet<QString>& tablesToNullify, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool deleteMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const override;
                virtual bool resetMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const override;
            private:
                bool validate(const QString& entryId, const QSet<QString>& keys) const;
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

#endif
