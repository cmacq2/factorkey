#include "memory-metadb.h"

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            MemoryMetadataDbManager::~MemoryMetadataDbManager()
            {
                close();
            }

            MemoryMetadataDbManager::MemoryMetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers, QObject * parent) :
                MetadataDbManager(connectionName, handlers, parent) {}

            bool MemoryMetadataDbManager::impl_initDb(QSqlDatabase& db)
            {
                if(allowInitDb())
                {
                    db.setDatabaseName(QLatin1String(":memory:"));
                    return db.open();
                }
                else
                {
                    return false;
                }
            }

            QSqlDatabase MemoryMetadataDbManager::impl_open(void)
            {
                return allowOpen() ? otp::storage::db::MetadataDbManager::open() : QSqlDatabase();
            }

            bool MemoryMetadataDbManager::impl_close(void)
            {
                return allowClose() && otp::storage::db::MetadataDbManager::close();
            }

            bool MemoryMetadataDbManager::impl_removeAll(void)
            {
                return allowRemoveAll() && otp::storage::db::MetadataDbManager::removeAll();
            }

            bool MemoryMetadataDbManager::impl_isOpened(void) const
            {
                return otp::storage::db::MetadataDbManager::isOpened();
            }

            bool MemoryMetadataDbManager::impl_readType(const QString& entry, QVariant& value)
            {
                return allowReadType() && otp::storage::db::MetadataDbManager::readType(entry, value);
            }

            bool MemoryMetadataDbManager::impl_remove(const QString& entry)
            {
                return allowRemove() && otp::storage::db::MetadataDbManager::remove(entry);
            }

            bool MemoryMetadataDbManager::impl_removeEntries(const QStringList& entryList)
            {
                return allowRemoveEntries() && otp::storage::db::MetadataDbManager::removeEntries(entryList);
            }

            bool MemoryMetadataDbManager::impl_contains(const QString& entry)
            {
                return allowContains() && otp::storage::db::MetadataDbManager::contains(entry);
            }

            bool MemoryMetadataDbManager::impl_entries(QStringList& e)
            {
                return allowEntries() && otp::storage::db::MetadataDbManager::entries(e);
            }

            void MemoryMetadataDbBuilder::setParent(QObject * parent)
            {
                m_parent = parent;
            }

            MemoryMetadataDbManager * MemoryMetadataDbBuilder::build(void) const
            {
                return new MemoryMetadataDbManager(m_connectionName, m_handlers, m_parent);
            }
        }
    }
}
