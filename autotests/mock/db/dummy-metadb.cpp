#include "dummy-metadb.h"

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, QObject * parent):
                MetadataDbManager(connName, QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>(), parent) {}

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, const QHash<QString, enum otp::storage::OTPTokenType>& entries, QObject * parent):
                MetadataDbManager(connName, QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>(), parent), m_entries(entries) {}

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, QObject * parent):
                MetadataDbManager(connName, typeHandlers, parent) {}

            DummyMetadataDbManager::DummyMetadataDbManager(
                const QString& connName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers, const QHash<QString, enum otp::storage::OTPTokenType>& entries, QObject * parent):
                MetadataDbManager(connName, typeHandlers, parent), m_entries(entries) {}

            DummyMetadataDbManager::DummyMetadataDbManager(
                const QString& connName,
                const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers,
                const QHash<QString, enum otp::storage::OTPTokenType>& entries,
                bool openByDefault,
                QObject * parent):
                MetadataDbManager(connName, typeHandlers, parent), m_entries(entries), m_opened(openByDefault) {}

            DummyMetadataDbManager::~DummyMetadataDbManager()
            {
                close();
            }

            bool DummyMetadataDbManager::impl_initDb(QSqlDatabase&)
            {
                return allowInitDb();
            }

            const QStringList DummyMetadataDbManager::removedEntries(void) const
            {
                return m_removed;
            }

            QSqlDatabase DummyMetadataDbManager::impl_open(void)
            {
                if(allowOpen())
                {
                    m_opened = true;
                }
                return QSqlDatabase();
            }

            bool DummyMetadataDbManager::impl_close(void)
            {
                if(allowClose())
                {
                    m_opened = false;
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::impl_isOpened(void) const
            {
                return m_opened;
            }

            bool DummyMetadataDbManager::impl_readType(const QString& entry, QVariant& value)
            {
                if(allowReadType() && m_entries.contains(entry))
                {
                    const otp::storage::OTPTokenType type = m_entries.value(entry);
                    value = QVariant::fromValue(type);
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::impl_remove(const QString& entry)
            {
                if(allowRemove() && m_entries.contains(entry))
                {
                    m_removed << entry;
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::impl_removeEntries(const QStringList& entryList)
            {
                if(allowRemoveEntries())
                {
                    for(const auto& entry: entryList)
                    {
                        if(!m_removed.contains(entry) && m_entries.contains(entry))
                        {
                            m_removed << entry;
                        }
                    }
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::impl_removeAll(void)
            {
                if(allowRemoveAll())
                {
                    for(const auto& entry: m_entries.keys())
                    {
                        if(!m_removed.contains(entry))
                        {
                            m_removed << entry;
                        }
                    }
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::impl_contains(const QString& entry)
            {
                if(allowContains())
                {
                    return !m_removed.contains(entry) && m_entries.contains(entry);
                }
                return false;
            }

            bool DummyMetadataDbManager::impl_entries(QStringList& e)
            {
                if(allowEntries())
                {
                    e.clear();
                    for(const auto& a: m_entries.keys())
                    {
                        if(!m_removed.contains(a))
                        {
                            e << a;
                        }
                    }
                    return true;
                }
                return false;
            }
        }
    }
}
