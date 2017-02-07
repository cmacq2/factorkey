#ifndef FACTORKEY_METADB_H
#define FACTORKEY_METADB_H

#include "../storage/storage.h"
#include "metadatastoragehandler.h"

#include <QDir>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QString>
#include <QVariant>

namespace otp
{
    namespace storage
    {
        namespace db
        {
            class MetadataDbManager
            {
            public:
                typedef std::function<bool(QSqlDatabase&)> InitDb;
                MetadataDbManager(const QString& connectionName, const QHash<int, QSharedPointer<MetadataStorageHandler>>& handlers, const InitDb& configure_db);
                const QString& connectionName(void) const;
                virtual ~MetadataDbManager();
                virtual bool isOpened(void) const;
                virtual QSqlDatabase open(void);
                virtual bool close(void);

                virtual bool readType(const QString& entry, QVariant& value);
                virtual bool remove(const QString& entry);
                virtual bool removeEntries(const QStringList& entryList);
                virtual bool removeAll(void);
                virtual bool contains(const QString & entry);
                virtual bool entries(QStringList& entryList);

                virtual const QSharedPointer<MetadataStorageHandler> handler(otp::storage::OTPTokenType type);
                QList<otp::storage::OTPTokenType> supportedHandlers(void) const;
                bool supports(otp::storage::OTPTokenType type) const;
            protected:
                virtual bool ensureSchema(QSqlDatabase& db);
            private:
                const QString m_connectionName;
                const QHash<int, QSharedPointer<MetadataStorageHandler>> m_handlers;
                const InitDb m_configure;
            };

            class MetadataDbBuilder
            {
            public:
                bool registerType(const QSharedPointer<MetadataStorageHandler>& handler);
                void setConnectionName(const QString& name);
                virtual QSharedPointer<MetadataDbManager> create(void) const;
            protected:
                virtual MetadataDbManager * build(void) const;
            public:
                virtual ~MetadataDbBuilder();
            protected:
                QString m_connectionName;
                QHash<int, QSharedPointer<MetadataStorageHandler>> m_handlers;
            };

            class MetadataFileDbBuilder: public MetadataDbBuilder
            {
            public:
                static const QString METADATA_DB_FILE;
            public:
                MetadataFileDbBuilder();
                bool setDataDirectory(void);
                void setDataDirectory(const QDir& dir);
                void setDataDirectory(const QString& dir);
                bool setDataFile(const QString& filename);
            protected:
                MetadataDbManager * build(void) const override;
            protected:
                QString m_fileName;
                QDir m_directory;
            };
        }
    }
}
#endif
