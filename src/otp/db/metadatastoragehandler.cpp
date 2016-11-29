#include "metadatastoragehandler.h"
#include "metadb.h"
#include "../parameters.h"

#include <functional>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#define OTP_ENTRY_TABLE_NAME        (QLatin1String("otp_entries"))
#define OTP_ENTRY_TYPE_COLUMN       (QLatin1String("tokentype"))
#define OTP_ENTRY_NAME_COLUMN       (QLatin1String("tokenname"))
#define OTP_ENTRY_CHARSET_COLUMN    (QLatin1String("keycharset"))
#define OTP_ENTRY_ENCODING_COLUMN   (QLatin1String("keyencoding"))
#define OTP_ENTRY_ID_COLUMN         (QLatin1String("id"))

namespace otp
{
    namespace storage
    {
        namespace db
        {
            const QString MetadataStorageHandler::OTP_ENTRY_TABLE = OTP_ENTRY_TABLE_NAME;
            const QString MetadataStorageHandler::OTP_ENTRY_ID = OTP_ENTRY_ID_COLUMN;
            const QString MetadataStorageHandler::OTP_ENTRY_TYPE = OTP_ENTRY_TYPE_COLUMN;
            const QString MetadataStorageHandler::OTP_ENTRY_TABLE_SCHEMA = QString(QLatin1String("CREATE TABLE IF NOT EXISTS `%1` (`%2`, `%3`, `%4`, `%5`, `%6`);\n")).
                arg(OTP_ENTRY_TABLE_NAME).
                arg(OTP_ENTRY_ID_COLUMN).
                arg(OTP_ENTRY_TYPE_COLUMN).
                arg(OTP_ENTRY_NAME_COLUMN).
                arg(OTP_ENTRY_CHARSET_COLUMN).
                arg(OTP_ENTRY_ENCODING_COLUMN);

            bool MetadataStorageHandler::defaultMappings(const QString& param, const RegisterFunction& fn)
            {
                if(param == OTP_ENTRY_ID || param == otp::storage::Storage::OTP_TOKEN_TYPE)
                {
                    return true;
                }
                if(param == otp::parameters::key::NAME)
                {
                    return fn(OTP_ENTRY_TABLE, OTP_ENTRY_NAME_COLUMN);
                }
                if(param == otp::parameters::key::CHARSET)
                {
                    return fn(OTP_ENTRY_TABLE, OTP_ENTRY_CHARSET_COLUMN);
                }
                if(param == otp::parameters::key::ENCODING)
                {
                    return fn(OTP_ENTRY_TABLE, OTP_ENTRY_ENCODING_COLUMN);
                }
                return false;
            }

            MetadataStorageHandler * MetadataStorageHandler::build(otp::storage::OTPTokenType type, const QStringList& params, const MappingFunction& mapper, const SchemaFunction& schema)
            {
                QHash<QString, QString> paramsToTables;
                QHash<QString, QString> columnsToParams;
                QHash<QString, QString> tables;

                QString sql = OTP_ENTRY_TABLE_SCHEMA;

                bool called, invalid;

                paramsToTables.insert(otp::storage::Storage::OTP_TOKEN_TYPE, OTP_ENTRY_TABLE);
                columnsToParams.insert(OTP_ENTRY_TYPE, otp::storage::Storage::OTP_TOKEN_TYPE);
                tables.insertMulti(OTP_ENTRY_TABLE, OTP_ENTRY_TYPE);

                for(const auto p: params)
                {
                    const RegisterFunction fn([&called,&invalid,&paramsToTables,&columnsToParams,&tables,&p,&schema,&sql](const QString& table, const QString& column) -> bool
                    {
                        called = true;
                        invalid = columnsToParams.contains(column);
                        if(!invalid)
                        {
                            if(!tables.contains(table) && table != OTP_ENTRY_TABLE)
                            {
                                const QString& s = schema(table);
                                invalid = s.isNull() || s.isEmpty();
                                if(!invalid)
                                {
                                    sql += s;
                                }
                            }
                            if(!invalid)
                            {
                                paramsToTables.insert(p, table);
                                columnsToParams.insert(column, p);
                                tables.insertMulti(table, column);
                            }
                        }
                        return true;
                    });
                    called = false;

                    mapper(p, fn);

                    if(!called || invalid)
                    {
                        return nullptr;
                    }
                }

                return new MetadataStorageHandler(type, tables, sql, columnsToParams, paramsToTables);
            }

            MetadataStorageHandler::MetadataStorageHandler(otp::storage::OTPTokenType type,
                                                           const QHash<QString,QString>& tables,
                                                           const QString& schema,
                                                           const QHash<QString,QString> columnsToParams,
                                                           const QHash<QString,QString> paramsToTables): m_type(type), m_tables(tables), m_schema(schema), m_columnsToParams(columnsToParams), m_paramsToTables(paramsToTables) {}
            MetadataStorageHandler::~MetadataStorageHandler() {}

            const QString& MetadataStorageHandler::schema(void) const
            {
                return m_schema;
            }

            QStringList MetadataStorageHandler::keys(void) const
            {
                QStringList l;
                for(const auto k: m_paramsToTables.keys())
                {
                    l << k;
                }
                return l;
            }

            bool MetadataStorageHandler::isParamNameValid(const QString& param) const
            {
                return param != OTP_ENTRY_ID && m_paramsToTables.contains(param);
            }

            QString MetadataStorageHandler::tableForParam(const QString& param) const
            {
                QString invalid;
                return isParamNameValid(param) ? m_paramsToTables.value(param) : invalid;
            }

            QString MetadataStorageHandler::columnToParam(const QString& column) const
            {
                QString invalid;
                return m_columnsToParams.contains(column) ? m_columnsToParams.value(column) : invalid;
            }

            QSet<QString> MetadataStorageHandler::columnsInTable(const QString& table) const
            {
                QSet<QString> cols;
                if(m_tables.contains(table))
                {
                    for(const auto col: m_tables.values(table))
                    {
                        cols.insert(col);
                    }
                }
                return cols;
            }

            QString MetadataStorageHandler::deleteSql(const QString& table) const
            {
                QString fmt(QLatin1String("DELETE FROM `%1` WHERE `%2` = :%2;\n"));
                return fmt.arg(table).arg(OTP_ENTRY_ID);
            }

            bool MetadataStorageHandler::query(const QString& sql, const QString& entryId, QHash<QString,QVariant>& params, MetadataDbManager * db) const
            {
                const std::function<bool(QSqlQuery&)> fn([&params,this](QSqlQuery& query) -> bool
                {
                    if(query.exec() && query.first())
                    {
                        QSqlRecord r = query.record();
                        for(int i = 0, l = r.count(); i < l; ++i)
                        {
                            const QString f = r.fieldName(i);
                            if(f != OTP_ENTRY_ID)
                            {
                                const QString param = columnToParam(f);
                                if(!param.isNull() && !param.isEmpty())
                                {
                                    const QVariant val = query.value(i);
                                    if(!val.isNull())
                                    {
                                        params.insert(param, val);
                                    }
                                }
                            }
                        }
                        return query.lastError().type() == QSqlError::NoError;
                    }
                    return false;
                });
                return queryDb(sql, entryId, db, fn);
            }

            bool MetadataStorageHandler::execute(const QString& sql, const QString& entryId, const QHash<QString,QVariant>& params, const QHash<QString,QString>& mapping, MetadataDbManager * db) const
            {
                const std::function<bool(QSqlQuery&)> fn([&params,&mapping](QSqlQuery& query) -> bool
                {
                    for(const QString& p: params.keys())
                    {
                        if(mapping.contains(p))
                        {
                            const auto placeholder = mapping.value(p);
                            query.bindValue(placeholder, params.value(p));
                        }
                    }
                    return query.lastError().type() == QSqlError::NoError && query.exec() && query.lastError().type() == QSqlError::NoError;
                });
                return queryDb(sql, entryId, db, fn);
            }

            QString MetadataStorageHandler::fetchSql(void) const
            {
                const auto all = keys();
                QSet<QString> tables;
                int count = 0;
                QString sql(QLatin1String(""));

                for(const QString k: all)
                {
                    const QString tbl = tableForParam(k);
                    if(!tables.contains(tbl))
                    {
                        tables.insert(tbl);
                        QString fmt = count > 0 ?
                            QLatin1String(" JOIN (SELECT * FROM `%1` WHERE `%2` = :%2) AS s%3 ON `%2`"):
                            QLatin1String("(SELECT * FROM `%1` WHERE `%2` = :%2) AS s%3");

                        sql += fmt.arg(tbl).arg(OTP_ENTRY_ID).arg(count);
                        count ++;
                    }
                }

                sql += QLatin1String(";\n");

                return sql;
            }

            QString MetadataStorageHandler::upsertSql(const QString& table, const QHash<QString,QVariant>& params, QHash<QString,QString>& mapping) const
            {
                const auto columns = columnsInTable(table);

                QString fmt = QLatin1String("INSERT INTO OR REPLACE INTO `%1` (`%2`");
                QString sql = fmt.arg(table).arg(OTP_ENTRY_ID);

                for(const auto p: columns)
                {
                    if(p != OTP_ENTRY_ID)
                    {
                        sql += QLatin1String(", `");
                        sql += p;
                        sql += QLatin1Char('`');
                    }
                }

                sql += QLatin1String(") VALUES (:");
                sql += OTP_ENTRY_ID;

                fmt = QLatin1String(", (SELECT `%1` FROM `%2` WHERE `%3` = :%3)");

                for(const auto p: columns)
                {
                    if(p != OTP_ENTRY_ID)
                    {
                        const auto param = columnToParam(p);
                        if(params.contains(param))
                        {
                            const QString pfmt(QLatin1String(":%1"));
                            const auto placeholder = fmt.arg(p);
                            sql += QLatin1String(", ");
                            sql += placeholder;
                            mapping.insert(param, placeholder);
                        }
                        else
                        {
                            sql += fmt.arg(p).arg(table).arg(OTP_ENTRY_ID);
                        }
                    }
                }

                sql += QLatin1String(");\n");
                return sql;
            }

            bool MetadataStorageHandler::saveMetaData(const QString& entryId, const QHash<QString,QVariant>& params, MetadataDbManager * db) const
            {
                QString sql(QLatin1String(""));
                QSet<QString> tables;
                QHash<QString, QString> mapping;

                for(const QString p: params.keys())
                {
                    if(isParamNameValid(p))
                    {
                        const QString tbl = tableForParam(p);

                        if(!tables.contains(tbl))
                        {
                            tables.insert(tbl);
                            sql += upsertSql(tbl, params, mapping);
                        }
                    }
                    else
                    {
                        return false;
                    }
                }

                return execute(sql, entryId, params, mapping, db);
            }

            bool MetadataStorageHandler::fetchMetaData(const QString& entryId, QHash<QString,QVariant>& params, MetadataDbManager * db) const
            {
                return query(fetchSql(), entryId, params, db);
            }

            bool MetadataStorageHandler::deleteMetaData(const QString& entryId, const QStringList& params, MetadataDbManager * db) const
            {
                QString sql(QLatin1String(""));
                QSet<QString> tables;

                for(const QString p: params)
                {
                    if(isParamNameValid(p))
                    {
                        const QString tbl = tableForParam(p);

                        if(!tables.contains(tbl))
                        {
                            tables.insert(tbl);
                            sql += deleteSql(tbl);
                        }
                    }
                    else
                    {
                        return false;
                    }
                }

                const QHash<QString, QVariant> noParams;
                const QHash<QString, QString> noMapping;
                return execute(sql, entryId, noParams, noMapping, db);
            }

            static QMutex metadataHandlerMapLock;
            static QHash<int,MetadataStorageHandler::ConstructorFunction> metadataHandlerCtorMap;

            bool MetadataStorageHandler::registerType(otp::storage::OTPTokenType type, const ConstructorFunction& ctor)
            {
                QMutexLocker lock(&metadataHandlerMapLock);

                int t = (int) type;
                if(metadataHandlerCtorMap.contains(t))
                {
                    return false;
                }
                else
                {
                    metadataHandlerCtorMap.insert(t, ctor);
                    return true;
                }
            }

            const MetadataStorageHandler * MetadataStorageHandler::createHandler(const otp::storage::OTPTokenType type)
            {
                const int t = (int) type;
                if(!metadataHandlerCtorMap.contains(t))
                {
                    return nullptr;
                }
                else
                {
                    auto ctor = metadataHandlerCtorMap.value(t);
                    return ctor ? ctor() : nullptr;
                }
            }

            otp::storage::OTPTokenType MetadataStorageHandler::type(void) const
            {
                return m_type;
            }

            bool queryDb(const QString &sql, const QString& entryId, MetadataDbManager * db, const std::function<bool(QSqlQuery&)>& queryFn)
            {
                QSqlDatabase conn = db->open();
                if(conn.isValid() && conn.isOpen())
                {
                    QSqlQuery query(conn);
                    if(query.prepare(sql))
                    {
                        if(!entryId.isNull())
                        {
                            query.bindValue(QLatin1Char(':') + MetadataStorageHandler::OTP_ENTRY_ID, entryId);
                        }
                        return query.lastError().type() == QSqlError::NoError && queryFn(query);
                    }
                }
                else
                {
                    db->close();
                }
                return false;
            }
        }
    }
}
