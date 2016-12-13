#ifndef FACTORKEY_AUTOTESTS_STUBBED_DUMMY_METADBH_H
#define FACTORKEY_AUTOTESTS_STUBBED_DUMMY_METADBH_H

#include "metadb.h"

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            class DummyMetadataDbManager: public MetadataDbManager
            {
            public:
                DummyMetadataDbManager(const QString& connName, QObject * parent = nullptr);
                DummyMetadataDbManager(const QString& connName, const QHash<QString, enum otp::storage::OTPTokenType>& entries, QObject * parent = nullptr);
                DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, QObject * parent = nullptr);
                DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries, QObject * parent = nullptr);
                DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries, bool openByDefault, QObject * parent = nullptr);

                const QStringList removedEntries(void) const;
                virtual ~DummyMetadataDbManager();
            private:
                const QHash<QString, enum otp::storage::OTPTokenType> m_entries;
                QStringList m_removed;
                bool m_opened = false;
            protected:
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
            };
        }
    }
}
#endif
