#ifndef FACTORKEY_APP_STORAGEPROVIDER_H
#define FACTORKEY_APP_STORAGEPROVIDER_H

#include "otp/storage/storageprovider.h"

#include <QDir>
#include <QFileInfo>
#include <QString>

namespace otp
{
    namespace app
    {
        namespace storage
        {
            class StorageBuilder
            {
            public:
                StorageBuilder();
                otp::storage::StorageProvider * build(void) const;
                void setSecretsDomain(const QString& domain = QString());
                void setDbConnectionName(const QString& connectionName = QString());
                void setDbName(const QString& name);
                void setDbDirectory(const QDir& dir);
                void setDbFile(const QFileInfo& dbFile);
                void setDbFile(const QString& dbFile);
                void setParent(QObject * parent);
            private:
                QString m_secretsDomain;
                QString m_dbConnectionName;
                QString m_dbName;
                QDir m_dir;
                bool m_dirSet;
                QObject * m_parent;
            };
        }
    }
}
#endif
