#include "metadb.h"

#include <QSqlError>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            MetadataDbManager::MetadataDbManager(const QString& connectionName) : m_connectionName(connectionName) {}
            MetadataDbManager::~MetadataDbManager() {}

            bool MetadataDbManager::isOpened(void) const
            {
                if(QSqlDatabase::contains(m_connectionName))
                {
                    const auto& db = QSqlDatabase::database(m_connectionName, false);
                    return db.isOpen();
                }
                return false;
            }

            QSqlDatabase MetadataDbManager::open(void)
            {
                //TODO
                return QSqlDatabase();
            }

            const MetadataStorageHandler * MetadataDbManager::handler(otp::storage::OTPTokenType) const
            {
                //TODO
                return nullptr; //MetadataStorageHandler::createHandler(type);
            }

            const QString& MetadataDbManager::connectionName(void) const
            {
                return m_connectionName;
            }

            bool MetadataDbManager::close(void)
            {
                if(isOpened())
                {
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
        }
    }
}
