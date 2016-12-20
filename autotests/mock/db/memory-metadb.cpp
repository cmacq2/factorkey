#include "memory-metadb.h"
#include <QSqlError>

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            bool MemoryMetadataDbManager::drop(void)
            {
                const auto& supported = supportedHandlers();
                QSet<QString> tables;
                for(const auto& t: supported)
                {
                    const auto& ptr = handler(t);
                    for(const auto& k: ptr->keys())
                    {
                        const auto table = ptr->tableForParam(k);
                        if(table != otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE && !tables.contains(table))
                        {
                            tables.insert(table);
                        }
                    }
                }

                if(!tables.isEmpty())
                {
                    QSqlDatabase db  = open();
                    bool ok = db.isValid() && db.isOpen() && db.transaction();
                    if(ok)
                    {
                        const QString fmt(QLatin1String("DROP TABLE `%1`"));
                        QSqlQuery query(db);
                        for(const auto& table: tables)
                        {
                            ok = query.prepare(fmt.arg(table)) && query.exec() && query.lastError().type() == QSqlError::NoError;
                            if(!ok)
                            {
                                break;
                            }
                        }
                        if(ok)
                        {
                            ok = query.prepare(fmt.arg(otp::storage::db::MetadataStorageHandler::OTP_ENTRY_TABLE)) && query.exec() && query.lastError().type() == QSqlError::NoError;
                        }
                        if(ok)
                        {
                            return db.commit() && db.lastError().type() == QSqlError::NoError;
                        }
                        else
                        {
                            db.rollback();
                        }
                    }
                }
                return false;
            }

            MemoryMetadataDbManager::~MemoryMetadataDbManager()
            {
                close();
            }

            MemoryMetadataDbManager::MemoryMetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers) :
                otp::storage::db::MetadataDbManager(connectionName, handlers) {}

            bool MemoryMetadataDbManager::initDb(QSqlDatabase& db)
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

            bool MemoryMetadataDbManager::allowInitDb(void) const
            {
                return true;
            }

            MemoryMetadataDbManager * MemoryMetadataDbBuilder::build(void) const
            {
                return new MemoryMetadataDbManager(m_connectionName, m_handlers);
            }
        }
    }
}
