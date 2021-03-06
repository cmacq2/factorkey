#ifndef FACTORKEY_STORAGEPROVIDER_H
#define FACTORKEY_STORAGEPROVIDER_H

#include "storage.h"

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QStringList>

namespace otp
{
    namespace storage
    {
        class StorageProviderPrivate;

        class StorageProvider : public QObject
        {
            Q_OBJECT
        public:

            bool open(void);
            bool close(void);

            bool contains(const QString& entryId);
            bool entries(QStringList& entries);

            Storage * lookup(const QString & entryId);
            Storage * create(const QString & entryId, OTPTokenType type);

            ~StorageProvider();
        private:
            Q_DISABLE_COPY(StorageProvider)
            const QScopedPointer<StorageProviderPrivate> d_ptr;
            Q_DECLARE_PRIVATE(StorageProvider)
        protected:
            StorageProvider(StorageProviderPrivate * d, QObject * parent = 0);
        };
    }
}
#endif
