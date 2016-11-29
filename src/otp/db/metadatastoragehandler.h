#ifndef FACTORKEY_METADATASTORAGEHANDLER_H
#define FACTORKEY_METADATASTORAGEHANDLER_H

#include "../storage/storage.h"

#include <functional>

#include <QHash>
#include <QSet>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class MetadataDbManager;
            bool queryDb(const QString &sql, const QString& entryId, MetadataDbManager * db, const std::function<bool(QSqlQuery&)>& queryFn);

            class MetadataStorageHandler
            {
            public:
                typedef std::function<const MetadataStorageHandler*(void)> ConstructorFunction;
                typedef std::function<bool(const QString&, const QString&)> RegisterFunction;
                typedef std::function<const QString& (const QString&)> SchemaFunction;

                static const MetadataStorageHandler * createHandler(const otp::storage::OTPTokenType type);
                static const QString OTP_ENTRY_TABLE;
                static const QString OTP_ENTRY_ID;
                static const QString OTP_ENTRY_TYPE;
            public:
                virtual ~MetadataStorageHandler();
                virtual otp::storage::OTPTokenType type(void) const;

                virtual const QString& schema(void) const;

                virtual bool saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, MetadataDbManager * db) const;
                virtual bool fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, MetadataDbManager * db) const;
                virtual bool deleteMetaData(const QString& entryId, const QStringList& keys, MetadataDbManager * db) const;

                virtual QStringList keys(void) const;
                virtual bool isParamNameValid(const QString& param) const;

                virtual QString tableForParam(const QString& param) const;
                virtual QString columnToParam(const QString& param) const;
                virtual QSet<QString> columnsInTable(const QString& table) const;

            protected:
                typedef std::function<void(const QString&, const RegisterFunction&)> MappingFunction;
                static bool defaultMappings(const QString& param, const RegisterFunction& fn);
                static MetadataStorageHandler * build(otp::storage::OTPTokenType type, const QStringList& params, const MappingFunction& mapper, const SchemaFunction& schema);

            protected:

                virtual bool query(const QString& sql, const QString& entryId, QHash<QString,QVariant>& params, MetadataDbManager * db) const;
                virtual bool execute(const QString& sql, const QString& entryId, const QHash<QString,QVariant>& params, const QHash<QString,QString>& mapping, MetadataDbManager * db) const;
                virtual QString upsertSql(const QString& table, const QHash<QString,QVariant>& params, QHash<QString,QString>& mapping) const;
                virtual QString deleteSql(const QString& table) const;
                virtual QString fetchSql(void) const;

                MetadataStorageHandler(otp::storage::OTPTokenType type,
                                       const QHash<QString,QString>& tables,
                                       const QString& schema,
                                       const QHash<QString,QString> columnsToParams,
                                       const QHash<QString,QString> paramsToTables);
            protected:
                static const QString OTP_ENTRY_TABLE_SCHEMA;
                static bool registerType(otp::storage::OTPTokenType type, const ConstructorFunction& ctor);
            private:
                const otp::storage::OTPTokenType m_type;
            private:
                const QHash<QString,QString> m_tables;
                const QString m_schema;
                const QHash<QString,QString> m_columnsToParams;
                const QHash<QString,QString> m_paramsToTables;
            };
        }
    }
}
#endif
