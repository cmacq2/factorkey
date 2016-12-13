#ifndef FACTORKEY_AUTOTESTS_STUBBED_MEMORY_METADBH_H
#define FACTORKEY_AUTOTESTS_STUBBED_MEMORY_METADBH_H

#include "metadb.h"

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            class MemoryMetadataDbManager: public MetadataDbManager
            {
            public:
                MemoryMetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers, QObject * parent = nullptr);
                virtual ~MemoryMetadataDbManager();
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

            class MemoryMetadataDbBuilder: public otp::storage::db::MetadataDbBuilder
            {
            public:
                MemoryMetadataDbManager * build(void) const;
                void setParent(QObject * parent = nullptr);
            private:
                QObject * m_parent = nullptr;
            };
        }
    }
}
#endif
