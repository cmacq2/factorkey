#ifndef FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"
#include "otp/db/metadb.h"

#include <QHash>
#include <QList>
#include <QSignalSpy>
#include <QString>
#include <QVariant>

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            class DummyMetadataStorageHandler: public QObject, public otp::storage::db::MetadataStorageHandler
            {
                Q_OBJECT
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
            private:
                QSignalSpy * m_spy_schema = nullptr;
                QSignalSpy * m_spy_type = nullptr;
                QSignalSpy * m_spy_saveMetaData = nullptr;
                QSignalSpy * m_spy_fetchMetaData = nullptr;
                QSignalSpy * m_spy_deleteMetaData = nullptr;
                QSignalSpy * m_spy_resetMetaData = nullptr;
                QSignalSpy * m_spy_pruneMetaData = nullptr;
                QSignalSpy * m_spy_keys = nullptr;
                QSignalSpy * m_spy_isParamNameValid = nullptr;
            public:
                QSignalSpy * spy_schema(void) const;
                QSignalSpy * spy_type(void) const;
                QSignalSpy * spy_saveMetaData(void) const;
                QSignalSpy * spy_fetchMetaData(void) const;
                QSignalSpy * spy_deleteMetaData(void) const;
                QSignalSpy * spy_resetMetaData(void) const;
                QSignalSpy * spy_pruneMetaData(void) const;
                QSignalSpy * spy_keys(void) const;
                QSignalSpy * spy_isParamNameValid(void) const;
            private:
                void init(void);
            protected:
                virtual const QSet<QString>& impl_schema(void) const;

                virtual bool impl_saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const;
                virtual bool impl_fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const;
                virtual bool impl_deleteMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const;
                virtual bool impl_resetMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const;
                virtual bool impl_pruneMetaData(const QString& entryId, const QSet<QString>& keys, const QSet<QString>& tables, otp::storage::db::MetadataDbManager * db) const;
                virtual QSet<QString> impl_keys(void) const;
                virtual otp::storage::OTPTokenType impl_type(void) const;
                virtual bool impl_isParamNameValid(const QString& param) const;

                virtual bool allowSaveMetaData(void) const;
                virtual bool allowFetchMetaData(void) const;
                virtual bool allowDeleteMetaData(void) const;
                virtual bool allowResetMetaData(void) const;
                virtual bool allowPruneMetaData(void) const;
            Q_SIGNALS:
                void notify_schema(const QSet<QString>& result) const;
                void notify_type(otp::storage::OTPTokenType) const;
                void notify_saveMetaData(bool result, const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const;
                void notify_fetchMetaData(bool result, const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db) const;
                void notify_deleteMetaData(bool result, const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const;
                void notify_resetMetaData(bool result, const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager * db) const;
                void notify_pruneMetaData(bool result, const QString& entryId, const QSet<QString>& keys, const QSet<QString>& tables, otp::storage::db::MetadataDbManager * db) const;
                void notify_keys(const QSet<QString>& result) const;
                void notify_isParamNameValid(bool result, const QString& param) const;
            public:
                void check_schema(const QList<QSet<QString>>& callArgs) const;
                void check_no_schema(void) const;
                void check_saveMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_saveMetaData(void) const;
                void check_fetchMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_fetchMetaData(void) const;
                void check_deleteMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_deleteMetaData(void) const;
                void check_resetMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_resetMetaData(void) const;
                void check_pruneMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_pruneMetaData(void) const;
                void check_type(const QList<enum otp::storage::OTPTokenType>& callArgs) const;
                void check_no_type(void) const;
                void check_keys(const QList<QStringList>& callArgs) const;
                void check_no_keys(void) const;
                void check_isParamNameValid(const QList<QList<QVariant>>& callArgs) const;
                void check_no_isParamNameValid(void) const;
            public:
                static QList<QList<QVariant>>& expect_metadata(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager * db);
                static QList<QList<QVariant>>& expect_params(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QSet<QString>& params, otp::storage::db::MetadataDbManager * db);
                static QList<QList<QVariant>>& expect_paramName(QList<QList<QVariant>>& callStack, bool status, const QString& param);
            };
        }
    }
}

#endif