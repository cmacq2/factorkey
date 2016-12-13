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
            class SqlDatabaseStatus
            {
            public:
                SqlDatabaseStatus(const QString& connName, const QString& driverName, const bool valid, const bool open);
                SqlDatabaseStatus(const QSqlDatabase& db);
                SqlDatabaseStatus();
                const QString& connectionName(void) const;
                const QString& driverName(void) const;
                bool isValid(void) const;
                bool isOpen(void) const;
            private:
                const QString m_connName;
                const QString m_driverName;
                const bool m_valid;
                const bool m_open;
            };

            class MetadataDbManager: public QObject, public otp::storage::db::MetadataDbManager
            {
                Q_OBJECT
            public:
                MetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers, QObject * parent = nullptr);
                virtual ~MetadataDbManager();

                bool isOpened(void) const;
                QSqlDatabase open(void);
                bool close(void);

                bool readType(const QString& entry, QVariant& value);
                bool removeEntries(const QStringList& entryList);
                bool removeAll(void);
                bool remove(const QString& entry);
                bool contains(const QString & entry);
                bool entries(QStringList& entryList);

                const QSharedPointer<otp::storage::db::MetadataStorageHandler> handler(otp::storage::OTPTokenType type);

            protected:
                virtual bool initDb(QSqlDatabase& db);
            protected:
                virtual bool allowInitDb(void) const;

                virtual bool allowOpen(void) const;
                virtual bool allowClose(void) const;

                virtual bool allowReadType(void) const;
                virtual bool allowRemove(void) const;
                virtual bool allowContains(void) const;
                virtual bool allowEntries(void) const;
                virtual bool allowRemoveEntries(void) const;
                virtual bool allowRemoveAll(void) const;

                virtual bool allowHandler(void) const;

                virtual bool impl_initDb(QSqlDatabase& db);
                virtual bool impl_isOpened(void) const;
                virtual QSqlDatabase impl_open(void);
                virtual bool impl_close(void);

                virtual bool impl_readType(const QString& entry, QVariant& value);
                virtual bool impl_remove(const QString& entry);
                virtual bool impl_contains(const QString & entry);
                virtual bool impl_entries(QStringList& entryList);
                virtual bool impl_removeEntries(const QStringList& entryList);
                virtual bool impl_removeAll(void);

                virtual const QSharedPointer<otp::storage::db::MetadataStorageHandler> impl_handler(otp::storage::OTPTokenType type);
            Q_SIGNALS:
                void notify_initDb(bool result, const stubs::storage::db::SqlDatabaseStatus conn);
                void notify_isOpened(bool result) const;
                void notify_open(const stubs::storage::db::SqlDatabaseStatus result);
                void notify_close(bool result);

                void notify_readType(bool result, const QString& entry, QVariant value);
                void notify_remove(bool result, const QString& entry);
                void notify_removeEntries(bool result, const QStringList entryList);
                void notify_removeAll(bool result);
                void notify_contains(bool result, const QString & entry);
                void notify_entries(bool result, QStringList entryList);

                void notify_handler(bool result, otp::storage::OTPTokenType type);

            private:
                void init(void);
            public:
                QSignalSpy * spy_isOpened(void) const;
                QSignalSpy * spy_open(void) const;
                QSignalSpy * spy_close(void) const;
                QSignalSpy * spy_readType(void) const;
                QSignalSpy * spy_remove(void) const;
                QSignalSpy * spy_removeEntries(void) const;
                QSignalSpy * spy_removeAll(void) const;
                QSignalSpy * spy_contains(void) const;
                QSignalSpy * spy_entries(void) const;
                QSignalSpy * spy_handler(void) const;
                QSignalSpy * spy_initDb(void) const;
            private:
                QSignalSpy * m_spy_initDb = nullptr;
                QSignalSpy * m_spy_isOpened = nullptr;
                QSignalSpy * m_spy_open = nullptr;
                QSignalSpy * m_spy_close = nullptr;
                QSignalSpy * m_spy_readType = nullptr;
                QSignalSpy * m_spy_remove = nullptr;
                QSignalSpy * m_spy_contains = nullptr;
                QSignalSpy * m_spy_entries = nullptr;
                QSignalSpy * m_spy_handler = nullptr;
                QSignalSpy * m_spy_removeEntries = nullptr;
                QSignalSpy * m_spy_removeAll = nullptr;
            public:
                void check_isOpened(const QList<bool>& callArgs) const;
                void check_no_isOpened(void) const;
                void check_open(const QList<SqlDatabaseStatus>& callArgs) const;
                void check_no_open(void) const;
                void check_close(const QList<bool>& callArgs) const;
                void check_no_close(void) const;
                void check_removeAll(const QList<bool>& callArgs) const;
                void check_no_removeAll(void) const;
                void check_entries(const QList<QList<QVariant>>& callArgs) const;
                void check_no_entries(void) const;
                void check_contains(const QList<QList<QVariant>>& callArgs) const;
                void check_no_contains(void) const;
                void check_removeEntries(const QList<QList<QVariant>>& callArgs) const;
                void check_no_removeEntries(void) const;
                void check_remove(const QList<QList<QVariant>>& callArgs) const;
                void check_no_remove(void) const;
                void check_readType(const QList<QList<QVariant>>& callArgs) const;
                void check_no_readType(void) const;
            public:
                static QList<QList<QVariant>>& expect_token_type(QList<QList<QVariant>>& callStack, bool status, const QString& entry, enum otp::storage::OTPTokenType type);
                static QList<QList<QVariant>>& expect_string(QList<QList<QVariant>>& callStack, bool status, const QString& entry);
                static QList<QList<QVariant>>& expect_entries(QList<QList<QVariant>>& callStack, bool status, const QStringList& entries);
            };

            class FakeMetadataDbBuilder: public otp::storage::db::MetadataDbBuilder
            {
            public:
                FakeMetadataDbBuilder(otp::storage::db::MetadataDbManager * db);
                virtual ~FakeMetadataDbBuilder();
                otp::storage::db::MetadataDbManager * build(void) const;
            private:
                otp::storage::db::MetadataDbManager * const m_db;
            };
        }
    }
}
#endif
