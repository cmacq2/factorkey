#include "dummy-metadb.h"

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName):
                otp::storage::db::MetadataDbManager(connName,
                                                    QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>(),
                                                    [this](QSqlDatabase&) -> bool { return allowInitDb(); }) {}

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName, const QHash<QString, enum otp::storage::OTPTokenType>& entries):
                otp::storage::db::MetadataDbManager(connName,
                                                    QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>(),
                                                    [this](QSqlDatabase&) -> bool { return allowInitDb(); }), m_entries(entries) {}

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName,
                                                           const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers):
                otp::storage::db::MetadataDbManager(connName, typeHandlers, [this](QSqlDatabase&) -> bool { return allowInitDb(); }) {}

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName,
                                                           const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers,
                                                           const QHash<QString, enum otp::storage::OTPTokenType>& entries):
                otp::storage::db::MetadataDbManager(connName, typeHandlers, [this](QSqlDatabase&) -> bool { return allowInitDb(); }), m_entries(entries) {}

            DummyMetadataDbManager::DummyMetadataDbManager(const QString& connName,
                                                           const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& typeHandlers,
                                                           const QHash<QString, enum otp::storage::OTPTokenType>& entries,
                                                           bool openByDefault):
                otp::storage::db::MetadataDbManager(connName, typeHandlers, [this](QSqlDatabase&) -> bool { return allowInitDb(); }), m_entries(entries), m_opened(openByDefault) {}

            bool DummyMetadataDbManager::allowHandler(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowRemoveAll(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowRemoveEntries(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowClose(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowInitDb(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowOpen(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowReadType(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowEntries(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowContains(void) const
            {
                return true;
            }

            bool DummyMetadataDbManager::allowRemove(void) const
            {
                return true;
            }

            const QSharedPointer<otp::storage::db::MetadataStorageHandler> DummyMetadataDbManager::handler(otp::storage::OTPTokenType type)
            {
                return allowHandler() ?
                    otp::storage::db::MetadataDbManager::handler(type) :
                    QSharedPointer<otp::storage::db::MetadataStorageHandler>();
            }

            DummyMetadataDbManager::~DummyMetadataDbManager()
            {
                close();
            }

            const QStringList DummyMetadataDbManager::removedEntries(void) const
            {
                return m_removed;
            }

            QSqlDatabase DummyMetadataDbManager::open(void)
            {
                if(allowOpen())
                {
                    m_opened = true;
                }
                return QSqlDatabase();
            }

            bool DummyMetadataDbManager::close(void)
            {
                if(allowClose())
                {
                    m_opened = false;
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::isOpened(void) const
            {
                return m_opened;
            }

            bool DummyMetadataDbManager::readType(const QString& entry, QVariant& value)
            {
                if(allowReadType() && m_entries.contains(entry))
                {
                    const otp::storage::OTPTokenType type = m_entries.value(entry);
                    value = QVariant((int) type); //::fromValue(type);
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::remove(const QString& entry)
            {
                if(allowRemove() && m_entries.contains(entry))
                {
                    m_removed << entry;
                    return true;
                }
                return false;
            }

            bool DummyMetadataDbManager::removeEntries(const QStringList& entryList)
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

            bool DummyMetadataDbManager::removeAll(void)
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

            bool DummyMetadataDbManager::contains(const QString& entry)
            {
                if(allowContains())
                {
                    return !m_removed.contains(entry) && m_entries.contains(entry);
                }
                return false;
            }

            bool DummyMetadataDbManager::entries(QStringList& e)
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

            QHash<QString, enum otp::storage::OTPTokenType> FakeSingleItemMetadataDbManager::entryMap(const QString& entryId, QSharedPointer<otp::storage::db::MetadataStorageHandler>& typeHandler)
            {
                QHash<QString, enum otp::storage::OTPTokenType> result;
                if(typeHandler)
                {
                    result.insert(entryId, typeHandler->type());
                }
                return result;
            }

            QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>> FakeSingleItemMetadataDbManager::typeMap(QSharedPointer<otp::storage::db::MetadataStorageHandler>& typeHandler)
            {
                QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>> result;
                if(typeHandler)
                {
                    result.insert((int) typeHandler->type(), typeHandler);
                }
                return result;
            }

            FakeSingleItemMetadataDbManager::FakeSingleItemMetadataDbManager(
                const QString& connName, const QString& entryId, QSharedPointer<otp::storage::db::MetadataStorageHandler>& typeHandler) :
                DummyMetadataDbManager(connName, typeMap(typeHandler), entryMap(entryId, typeHandler)) {}

            FakeSingleItemMetadataDbManager::FakeSingleItemMetadataDbManager(
                const QString& connName, const QString& entryId, QSharedPointer<otp::storage::db::MetadataStorageHandler>& typeHandler, bool openByDefault):
                DummyMetadataDbManager(connName, typeMap(typeHandler), entryMap(entryId, typeHandler), openByDefault) {}

            FakeSingleItemMetadataDbManager::~FakeSingleItemMetadataDbManager() {}

            bool FakeSingleItemMetadataDbManager::allowRemove(void) const
            {
                return false;
            }

            bool FakeSingleItemMetadataDbManager::allowRemoveEntries(void) const
            {
                return false;
            }

            bool FakeSingleItemMetadataDbManager::allowRemoveAll(void) const
            {
                return false;
            }
        }
    }
}
