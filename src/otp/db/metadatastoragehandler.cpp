#include "metadatastoragehandler.h"
#include "metadb.h"
#include "../parameters.h"

#include <functional>
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
            const QString MetadataStorageHandler::OTP_ENTRY_TABLE_SCHEMA = QString(QLatin1String("CREATE TABLE IF NOT EXISTS `%1` (`%2` VARCHAR(36) NOT NULL PRIMARY KEY, `%3` INTEGER NOT NULL, `%4` VARCHAR(255), `%5` VARCHAR(36), `%6` INTEGER);\n")).
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

                QSet<QString> sql;
                sql.insert(OTP_ENTRY_TABLE_SCHEMA);

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
                                    sql.insert(s);
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
                                                           const QSet<QString>& schema,
                                                           const QHash<QString,QString>& columnsToParams,
                                                           const QHash<QString,QString>& paramsToTables): m_type(type), m_tables(tables), m_schema(schema), m_columnsToParams(columnsToParams), m_paramsToTables(paramsToTables) {}
            MetadataStorageHandler::~MetadataStorageHandler() {}

            const QSet<QString>& MetadataStorageHandler::schema(void) const
            {
                return m_schema;
            }

            QSet<QString> MetadataStorageHandler::keys(void) const
            {
                QSet<QString> l;
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
                int count = 1;

                QString sql = QString(QLatin1String("SELECT * FROM `%1` AS s0")).arg(OTP_ENTRY_TABLE);
                tables.insert(OTP_ENTRY_TABLE);

                for(const QString k: all)
                {
                    const QString tbl = tableForParam(k);
                    if(!tables.contains(tbl))
                    {
                        tables.insert(tbl);
                        QString fmt = QLatin1String(" LEFT JOIN `%1` AS s%3 ON s%3 . `%2` = s%4 . `%2`");
                        sql += fmt.arg(tbl).arg(OTP_ENTRY_ID).arg(count).arg(count -1);
                        count ++;
                    }
                }

                sql += QString(QLatin1String(" WHERE s0 . `%1` = :%1")).arg(OTP_ENTRY_ID);
                return sql;
            }


            QString MetadataStorageHandler::upsertSql(const QString& table, const QHash<QString,QVariant>& params, QHash<QString,QString>& mapping) const
            {
                const std::function<bool(const QString&)> ctest([&params](const QString& param) -> bool
                {
                    return params.contains(param);
                });
                const std::function<QString(const QString&,const QString&)> map([&params,&mapping](const QString& column, const QString& param) -> QString
                {
                    const QString pfmt(QLatin1String(":%1"));
                    const QString sfmt(QLatin1String(", %1"));
                    const auto placeholder = pfmt.arg(column);
                    mapping.insert(param, placeholder);
                    return sfmt.arg(placeholder);
                });
                return upsertSql(table, ctest, map);
            }

            QString MetadataStorageHandler::nullifySql(const QString& table, const QSet<QString>& paramsToNull) const
            {
                const std::function<bool(const QString&)> ctest([&paramsToNull](const QString& param) -> bool
                {
                    return paramsToNull.contains(param);
                });
                const std::function<QString(const QString&,const QString&)> nil([](const QString&,const QString&) -> QString
                {
                    const QString n(QLatin1String(", NULL"));
                    return n;
                });
                return upsertSql(table, ctest, nil);
            }

            QString MetadataStorageHandler::upsertSql(const QString& table,
                                                      const std::function<bool(const QString&)>& containTest,
                                                      const std::function<QString(const QString&, const QString&)>& placeHolder) const
            {
                const auto columns = columnsInTable(table);

                QString fmt = QLatin1String("INSERT OR REPLACE INTO `%1` (`%2`");
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
                        sql += containTest(param) ? placeHolder(p, param) : (fmt.arg(p).arg(table).arg(OTP_ENTRY_ID));
                    }
                }

                sql += QLatin1String(");\n");
                return sql;
            }

            bool MetadataStorageHandler::saveMetaData(const QString& entryId, const QHash<QString,QVariant>& params, MetadataDbManager * db) const
            {
                QSet<QString> tables;
                QHash<QString, QString> mapping;

                QSqlDatabase conn = db->open();
                bool ok = conn.isValid() && conn.isOpen() && conn.transaction();
                if(ok)
                {
                    for(const QString p: params.keys())
                    {
                        if(ok && isParamNameValid(p))
                        {
                            const auto& tbl = tableForParam(p);

                            if(!tables.contains(tbl))
                            {
                                tables.insert(tbl);
                                const auto& sql = upsertSql(tbl, params, mapping);
                                ok = execute(sql, entryId, params, mapping, db);
                            }
                        }
                        else
                        {
                            ok = false;
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
                return false;
            }

            bool MetadataStorageHandler::fetchMetaData(const QString& entryId, QHash<QString,QVariant>& params, MetadataDbManager * db) const
            {
                return query(fetchSql(), entryId, params, db);
            }

            bool MetadataStorageHandler::execute(const QString& entryId, const QSet<QString>& params, const std::function<QString(const QString&)>& sql, MetadataDbManager * db) const
            {
                QSet<QString> tables;

                const QHash<QString, QVariant> noParams;
                const QHash<QString, QString> noMapping;

                QSqlDatabase conn = db->open();
                bool ok = conn.isValid() && conn.isOpen() && conn.transaction();
                if(ok)
                {
                    for(const QString p: params)
                    {
                        if(ok && isParamNameValid(p))
                        {
                            const auto& tbl = tableForParam(p);

                            if(!tables.contains(tbl))
                            {
                                tables.insert(tbl);
                                const auto& q = sql(tbl);
                                ok = execute(q, entryId, noParams, noMapping, db);
                            }
                        }
                        else
                        {
                            ok = false;
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
                return false;
            }

            QSet<QString> MetadataStorageHandler::diff(const QSet<QString>& newKeys) const
            {
                return diff(keys(), newKeys);
            }

            QSet<QString> MetadataStorageHandler::diff(const QSet<QString>& origKeys, const QSet<QString>& newKeys) const
            {
                QSet<QString> result(origKeys);
                QMutableSetIterator<QString> iter(result);
                while(iter.hasNext())
                {
                    const QString key = iter.next();
                    if(newKeys.contains(key))
                    {
                        iter.remove();
                    }
                }
                return result;
            }

            bool MetadataStorageHandler::pruneMetaData(const QString& entryId, const QSet<QString>& newKeys, const QSet<QString>& tablesToNullify, MetadataDbManager * db) const
            {
                const QSet<QString> origKeys = keys();
                QSet<QString> toPrune = diff(origKeys, newKeys);

                const std::function<QString(const QString&)> sql([this,&tablesToNullify,&toPrune](const QString& table) -> QString
                {
                    return tablesToNullify.contains(table) ? nullifySql(table, toPrune) : deleteSql(table);
                });
                return toPrune.isEmpty() || execute(entryId, origKeys, sql, db);
            }

            bool MetadataStorageHandler::resetMetaData(const QString& entryId, const QSet<QString>& params, MetadataDbManager * db) const
            {
                const std::function<QString(const QString&)> sql([this,&params](const QString& table) -> QString
                {
                    return nullifySql(table, params);
                });
                return execute(entryId, params, sql, db);
            }

            bool MetadataStorageHandler::deleteMetaData(const QString& entryId, const QSet<QString>& params, MetadataDbManager * db) const
            {
                const std::function<QString(const QString&)> sql([this](const QString& table) -> QString
                {
                    return deleteSql(table);
                });
                return execute(entryId, params, sql, db);
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
