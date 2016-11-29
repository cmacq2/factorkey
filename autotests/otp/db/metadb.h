#ifndef FACTORKEY_AUTOTESTS_STUBBED_METADBH_H
#define FACTORKEY_AUTOTESTS_STUBBED_METADBH_H

#include "otp/db/metadb.h"
#include "otp/db/metadatastoragehandler.h"
#include "metadatastoragehandler.h"

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
            class DummyMetadataDbManager: public QObject, public otp::storage::db::MetadataDbManager
            {
                Q_OBJECT
            public:
                DummyMetadataDbManager(const QString& connName);
                DummyMetadataDbManager(const QString& connName, const QHash<QString, enum otp::storage::OTPTokenType>& entries);
                DummyMetadataDbManager(const QString& connName, const QHash<int,QSharedPointer<DummyMetadataStorageHandler>>& typeHandlers);
                DummyMetadataDbManager(const QString& connName, const QHash<int,QSharedPointer<DummyMetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries);
                virtual ~DummyMetadataDbManager();

                bool isOpened(void) const;
                QSqlDatabase open(void);
                bool close(void);

                bool readType(const QString& entry, QVariant& value);
                bool remove(const QString& entry);
                bool contains(const QString & entry);
                bool entries(QStringList& entryList);

                const QStringList removedEntries(void) const;

                QSharedPointer<otp::storage::db::MetadataStorageHandler> getHandler(otp::storage::OTPTokenType type);

            protected:
                virtual bool allowOpen(void) const;
                virtual bool allowClose(void) const;

                virtual bool allowReadType(void) const;
                virtual bool allowRemove(void) const;
                virtual bool allowContains(void) const;
                virtual bool allowEntries(void) const;

                virtual bool allowGetHandler(void) const;

                virtual bool impl_isOpened(void) const;
                virtual QSqlDatabase impl_open(void);
                virtual bool impl_close(void);

                virtual bool impl_readType(const QString& entry, QVariant& value);
                virtual bool impl_remove(const QString& entry);
                virtual bool impl_contains(const QString & entry);
                virtual bool impl_entries(QStringList& entryList);

                virtual QSharedPointer<otp::storage::db::MetadataStorageHandler> impl_getHandler(otp::storage::OTPTokenType type);
            Q_SIGNALS:
                void notify_isOpened(bool result) const;
                void notify_open(QSqlDatabase result);
                void notify_close(bool result);

                void notify_readType(bool result, const QString& entry, QVariant& value);
                void notify_remove(bool result, const QString& entry);
                void notify_contains(bool result, const QString & entry);
                void notify_entries(bool result, QStringList& entryList);

                void notify_getHandler(bool result, otp::storage::OTPTokenType type);

            private:
                void init(void);
            private:
                const QHash<int,QSharedPointer<DummyMetadataStorageHandler>> m_typeHandlers;
                const QHash<QString, enum otp::storage::OTPTokenType> m_entries;
                QStringList m_removed;
                bool m_opened = false;
            public:
                QSignalSpy * spy_isOpened(void) const;
                QSignalSpy * spy_open(void) const;
                QSignalSpy * spy_close(void) const;
                QSignalSpy * spy_readType(void) const;
                QSignalSpy * spy_remove(void) const;
                QSignalSpy * spy_contains(void) const;
                QSignalSpy * spy_entries(void) const;
                QSignalSpy * spy_getHandler(void) const;
            private:
                QSignalSpy * m_spy_isOpened = nullptr;
                QSignalSpy * m_spy_open = nullptr;
                QSignalSpy * m_spy_close = nullptr;
                QSignalSpy * m_spy_readType = nullptr;
                QSignalSpy * m_spy_remove = nullptr;
                QSignalSpy * m_spy_contains = nullptr;
                QSignalSpy * m_spy_entries = nullptr;
                QSignalSpy * m_spy_getHandler = nullptr;
            public:
                void check_isOpened(const QList<bool>& callArgs) const;
                void check_no_isOpened(void) const;
                void check_open(const QList<bool>& callArgs) const;
                void check_no_open(void) const;
                void check_close(const QList<bool>& callArgs) const;
                void check_no_close(void);
                void check_entries(const QList<QList<QVariant>>& callArgs) const;
                void check_no_entries(void) const;
                void check_contains(const QList<QList<QVariant>>& callArgs) const;
                void check_no_contains(void) const;
                void check_remove(const QList<QList<QVariant>>& callArgs) const;
                void check_no_remove(void) const;
                void check_readType(const QList<QList<QVariant>>& callArgs) const;
                void check_no_readType(void) const;
            public:
                static QList<QList<QVariant>>& expect_token_type(QList<QList<QVariant>>& callStack, bool status, const QString& entry, enum otp::storage::OTPTokenType type);
                static QList<QList<QVariant>>& expect_string(QList<QList<QVariant>>& callStack, bool status, const QString& entry);
                static QList<QList<QVariant>>& expect_entries(QList<QList<QVariant>>& callStack, bool status, const QStringList& entries);
            };
        }
    }
}
#endif
