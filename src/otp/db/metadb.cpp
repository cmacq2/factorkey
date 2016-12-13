#include "metadb.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlDriver>
#include <QSqlError>
#include <QStandardPaths>

#define DB_DRIVER_NAME (QLatin1String("QSQLITE"))

namespace otp
{
    namespace storage
    {
        namespace db
        {

            const QString MetadataDbManager::METADATA_DB_FILE = QLatin1String("factorkey.db");

            MetadataDbManager::MetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<MetadataStorageHandler>>& handlers) :
                                                 m_connectionName(connectionName), m_handlers(handlers) {}

            MetadataDbManager::~MetadataDbManager()
            {
                close();
            }

            bool MetadataDbManager::isOpened(void) const
            {
                const auto& db = QSqlDatabase::database(m_connectionName, false);
                return db.isValid() && db.isOpen();
            }

            bool MetadataDbManager::initDb(QSqlDatabase& db)
            {
                const QString dataDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
                if(!dataDirPath.isNull() && !dataDirPath.isEmpty())
                {
                    QDir dir(dataDirPath);
                    QFileInfo info(dir, METADATA_DB_FILE);
                    if(dir.mkpath(dataDirPath))
                    {
                        const QString path = info.filePath();
                        if(!path.isNull() && !path.isEmpty())
                        {
                            db.setDatabaseName(path);
                            return db.open();
                        }
                    }
                }
                return false;
            }

            bool MetadataDbManager::ensureSchema(QSqlDatabase& db)
            {

                if(db.isValid() && db.isOpen())
                {
                    QSqlQuery query(db);
                    if(db.transaction())
                    {
                        bool ok = true;
                        std::function<bool(const QString&)> qfn([&ok,&query](const QString& sql) -> bool
                        {
                            if(ok && !sql.isNull() && !sql.isEmpty() && query.prepare(sql))
                            {
                                ok = query.exec() && query.lastError().type() == QSqlError::NoError;
                            }
                            return ok;
                        });

                        if(qfn(QLatin1String("PRAGMA foreign_keys = ON;\n")) && qfn(MetadataStorageHandler::OTP_ENTRY_TABLE_SCHEMA))
                        {
                            for(const auto& h: m_handlers)
                            {
                                const auto& schemas = h->schema();
                                for (const auto& sql: schemas)
                                {
                                    if(sql != MetadataStorageHandler::OTP_ENTRY_TABLE_SCHEMA && !qfn(sql))
                                    {
                                        break;
                                    }
                                }
                            }
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

            bool MetadataDbManager::removeAll(void)
            {
                QSqlDatabase conn = open();
                bool ok = conn.isValid() && conn.isOpen();
                if(ok)
                {
                    const QString sql = QString(QLatin1String("DELETE FROM `%1`;\n")).
                        arg(MetadataStorageHandler::OTP_ENTRY_TABLE);
                    QSqlQuery query(conn);
                    return query.prepare(sql) && query.lastError().type() == QSqlError::NoError
                        && query.exec() && query.lastError().type() == QSqlError::NoError;
                }
                return false;
            }

            bool MetadataDbManager::removeEntries(const QStringList& entries)
            {
                if(entries.isEmpty())
                {
                    return true;
                }
                else
                {
                    QSqlDatabase conn = open();
                    bool ok = conn.isValid() && conn.isOpen();
                    if(ok)
                    {
                        const QString sql = QString(QLatin1String("DELETE FROM `%1` WHERE `%2` = :%2;\n")).
                            arg(MetadataStorageHandler::OTP_ENTRY_TABLE).
                            arg(MetadataStorageHandler::OTP_ENTRY_ID);
                        QSqlQuery query(conn);

                        if(query.prepare(sql) && query.lastError().type() == QSqlError::NoError && conn.transaction())
                        {
                            for(const QString entryId: entries)
                            {
                                query.bindValue(QLatin1Char(':') + MetadataStorageHandler::OTP_ENTRY_ID, entryId);
                                ok = query.exec() && query.lastError().type() == QSqlError::NoError;
                                if(!ok)
                                {
                                    break;
                                }
                            }

                            if(ok)
                            {
                                return conn.commit() && conn.lastError().type() == QSqlError::NoError;
                            }
                            else
                            {
                                conn.rollback();
                            }
                        }
                    }
                    return false;
                }
            }

            QSqlDatabase MetadataDbManager::open(void)
            {
                QSqlDatabase db = QSqlDatabase::contains(m_connectionName) ?
                    QSqlDatabase::database(m_connectionName) :
                    QSqlDatabase::addDatabase(DB_DRIVER_NAME, m_connectionName);

                if(db.isValid() && db.driverName() == DB_DRIVER_NAME)
                {
                    if(db.isOpen())
                    {
                        return db;
                    }
                    if(initDb(db) && ensureSchema(db))
                    {
                        return db;
                    }

                }

                QSqlDatabase::removeDatabase(m_connectionName);
                return QSqlDatabase();
            }

            const QSharedPointer<MetadataStorageHandler> MetadataDbManager::handler(otp::storage::OTPTokenType type)
            {
                int t = (int) type;
                return m_handlers.contains(t) ? m_handlers.value(t) : QSharedPointer<MetadataStorageHandler>();
            }

            const QString& MetadataDbManager::connectionName(void) const
            {
                return m_connectionName;
            }

            bool MetadataDbManager::close(void)
            {
                if(isOpened())
                {
                    /*
                     * Ensure the connection is torn down fully (via destructors) before the connectin is removed,
                     * by closing it in a different scope that is torn down before this one.
                     * Such precautions are ncessary according to the QSqlDatabase::removeDatabase() docs.
                     */
                    const std::function<bool()> inner_fn([this]() -> bool
                    {
                        auto db = QSqlDatabase::database(m_connectionName, false);
                        auto r = db.isValid();
                        db.close();
                        return r;
                    });
                    auto r = inner_fn();
                    QSqlDatabase::removeDatabase(m_connectionName);
                    return r;
                }
                else
                {
                    return true;
                }
            }

            bool MetadataDbManager::readType(const QString& entry, QVariant& value)
            {
                const QString sql = QString(QLatin1String("SELECT `%1` FROM `%2` WHERE `%3` = :%3;\n")).
                    arg(MetadataStorageHandler::OTP_ENTRY_TYPE).
                    arg(MetadataStorageHandler::OTP_ENTRY_TABLE).
                    arg(MetadataStorageHandler::OTP_ENTRY_ID);
                const std::function<bool(QSqlQuery&)> fn([&value](QSqlQuery& query) -> bool
                {
                    if(query.exec() && query.first())
                    {
                        QVariant v = query.value(1);
                        if(!v.isNull())
                        {
                            value = v;
                        }
                        return query.lastError().type() == QSqlError::NoError;
                    }
                    return false;
                });
                return queryDb(sql, entry, this, fn);
            }

            bool MetadataDbManager::remove(const QString& entry)
            {
                const QString sql = QString(QLatin1String("DELETE FROM `%1` WHERE `%2` = :%2;\n")).
                    arg(MetadataStorageHandler::OTP_ENTRY_TABLE).
                    arg(MetadataStorageHandler::OTP_ENTRY_ID);
                const std::function<bool(QSqlQuery&)> fn([](QSqlQuery& query) -> bool
                {
                    return query.exec() && query.lastError().type() == QSqlError::NoError;
                });
                return queryDb(sql, entry, this, fn);
            }

            bool MetadataDbManager::contains(const QString& entry)
            {
                const QString sql = QString(QLatin1String("SELECT COUNT(`%1`) FROM `%2` WHERE `%1` = :%1;\n")).
                    arg(MetadataStorageHandler::OTP_ENTRY_ID).
                    arg(MetadataStorageHandler::OTP_ENTRY_TABLE);

                int count = 0;
                bool ok = false;

                const std::function<bool(QSqlQuery&)> fn([&count,&ok](QSqlQuery& query) -> bool
                {
                    if(query.exec() && query.first())
                    {
                        QVariant v = query.value(1);
                        if(!v.isNull() && v.type() == QVariant::Int)
                        {
                           count = v.toInt(&ok);
                        }
                        return query.lastError().type() == QSqlError::NoError;
                    }
                    return false;
                });
                return queryDb(sql, entry, this, fn) && ok && count > 0;
            }

            bool MetadataDbManager::entries(QStringList& entryList)
            {
                const QString sql = QString(QLatin1String("SELECT `%1` FROM `%2`;\n")).
                    arg(MetadataStorageHandler::OTP_ENTRY_ID).
                    arg(MetadataStorageHandler::OTP_ENTRY_TABLE);
                const QString entry;
                const std::function<bool(QSqlQuery&)> fn([&entryList](QSqlQuery& query) -> bool
                {
                    QStringList entries;
                    if(query.exec() && query.first())
                    {
                        do
                        {
                            QVariant v = query.value(1);
                            if(v.isNull() || v.type() != QVariant::String)
                            {
                                return false;
                            }
                            else
                            {
                                const QString e = v.toString();
                                if(e.isEmpty())
                                {
                                    return false;
                                }
                                else
                                {
                                    entries << e;
                                }
                            }
                        }
                        while(query.next());

                        if(query.lastError().type() == QSqlError::NoError)
                        {
                            entryList = entries;
                            return true;
                        }
                    }
                    return false;
                });
                return queryDb(sql, entry, this, fn);
            }

            MetadataDbBuilder::~MetadataDbBuilder() {}

            void MetadataDbBuilder::setConnectionName(const QString& name)
            {
                m_connectionName = name;
            }

            QSharedPointer<MetadataDbManager> MetadataDbBuilder::create(void) const
            {
                return QSharedPointer<MetadataDbManager>(build());
            }

            MetadataDbManager * MetadataDbBuilder::build(void) const
            {
                return new MetadataDbManager(m_connectionName, m_handlers);
            }

            bool MetadataDbBuilder::registerType(const QSharedPointer<MetadataStorageHandler>& handler)
            {
                if(handler)
                {
                    m_handlers.insert((int) handler->type(), handler);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
}
