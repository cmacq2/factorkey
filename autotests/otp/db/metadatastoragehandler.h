#ifndef FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_STUBBED_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"

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
                DummyMetadataStorageHandler(const otp::storage::OTPTokenType& type, const QStringList& validKeys);
                virtual ~DummyMetadataStorageHandler();
                virtual otp::storage::OTPTokenType type(void) const;
                virtual bool saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata) const;
                virtual bool saveParam(const QString& entryId, const QString& param, const QVariant& value) const;
                virtual bool fetchParam(const QString& entryId, const QString& param, QVariant& value) const;
                virtual bool fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata) const;
                virtual bool deleteMetaData(const QString& entryId, const QStringList& keys) const;
                virtual bool deleteParam(const QString& entryId, const QString& key) const;
                virtual const QStringList keys(void) const;
                virtual bool isParamNameValid(const QString& param) const;
            private:
                QSignalSpy * m_spy_type = nullptr;
                QSignalSpy * m_spy_saveMetaData = nullptr;
                QSignalSpy * m_spy_fetchMetaData = nullptr;
                QSignalSpy * m_spy_deleteMetaData = nullptr;
                QSignalSpy * m_spy_deleteParam = nullptr;
                QSignalSpy * m_spy_keys = nullptr;
                QSignalSpy * m_spy_isParamNameValid = nullptr;
                QSignalSpy * m_spy_saveParam = nullptr;
                QSignalSpy * m_spy_fetchParam = nullptr;
            public:
                QSignalSpy * spy_saveParam(void) const;
                QSignalSpy * spy_fetchParam(void) const;
                QSignalSpy * spy_type(void) const;
                QSignalSpy * spy_saveMetaData(void) const;
                QSignalSpy * spy_fetchMetaData(void) const;
                QSignalSpy * spy_deleteMetaData(void) const;
                QSignalSpy * spy_deleteParam(void) const;
                QSignalSpy * spy_keys(void) const;
                QSignalSpy * spy_isParamNameValid(void) const;
            private:
                void init(void);
            protected:
                virtual bool impl_saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata) const;
                virtual bool impl_fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata) const;
                virtual bool impl_deleteMetaData(const QString& entryId, const QStringList& keys) const;
                virtual bool impl_deleteParam(const QString& entryId, const QString& key) const;
                virtual const QStringList impl_keys(void) const;
                virtual otp::storage::OTPTokenType impl_type(void) const;
                virtual bool impl_isParamNameValid(const QString& param) const;
                virtual bool impl_saveParam(const QString& entryId, const QString& param, const QVariant& value) const;
                virtual bool impl_fetchParam(const QString& entryId, const QString& param, QVariant& value) const;

                virtual bool allowSaveMetaData(void) const;
                virtual bool allowFetchMetaData(void) const;
                virtual bool allowDeleteMetaData(void) const;
                virtual bool allowDeleteParam(void) const;
                virtual bool allowSaveParam(void) const;
                virtual bool allowFetchParam(void) const;

            Q_SIGNALS:
                void notify_saveParam(bool result, const QString& entryId, const QString& param, const QVariant& value) const;
                void notify_fetchParam(bool result, const QString& entryId, const QString& param, QVariant& value) const;
                void notify_type(otp::storage::OTPTokenType) const;
                void notify_saveMetaData(bool result, const QString& entryId, const QHash<QString,QVariant>& metadata) const;
                void notify_fetchMetaData(bool result, const QString& entryId, const QHash<QString,QVariant>& metadata) const;
                void notify_deleteMetaData(bool result, const QString& entryId, const QStringList& keys) const;
                void notify_deleteParam(bool result, const QString& entryId, const QString& key) const;
                void notify_keys(const QStringList result) const;
                void notify_isParamNameValid(bool result, const QString& param) const;
            public:
                void check_saveMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_saveMetaData(void) const;
                void check_fetchMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_fetchMetaData(void) const;
                void check_deleteMetaData(const QList<QList<QVariant>>& callArgs) const;
                void check_no_deleteMetaData(void) const;
                void check_delete_param(const QList<QList<QVariant>>& callArgs) const;
                void check_no_delete_param(void) const;
                void check_type(const QList<enum otp::storage::OTPTokenType>& callArgs) const;
                void check_no_type(void) const;
                void check_keys(const QList<QStringList>& callArgs) const;
                void check_no_keys(void) const;
                void check_isParamNameValid(const QList<QList<QVariant>>& callArgs) const;
                void check_no_isParamNameValid(void) const;
                void check_fetchParam(const QList<QList<QVariant>>& callArgs) const;
                void check_no_fetchParam(void) const;
                void check_saveParam(const QList<QList<QVariant>>& callArgs) const;
                void check_no_saveParam(void) const;
            public:
                static QList<QList<QVariant>>& expect_param(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QString& param, const QVariant& value);
                static QList<QList<QVariant>>& expect_metadata(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QHash<QString,QVariant>& metadata);
                static QList<QList<QVariant>>& expect_deleted_param(QList<QList<QVariant>>& callStack, bool status, const QString& entry, const QString& param);
                static QList<QList<QVariant>>& expect_paramName(QList<QList<QVariant>>& callStack, bool status, const QString& param);
            private:
                const QStringList m_validKeys;
            };
        }
    }
}

#endif
