#include "storageprovider.h"
#include "storage_p.h"

namespace otp
{
    namespace storage
    {

        StorageProvider::StorageProvider(StorageProviderPrivate * d, QObject * parent) : QObject(parent), d_ptr(d) {}
        StorageProvider::~StorageProvider() {}

        bool StorageProvider::open(void)
        {
            Q_D(StorageProvider);
            return d->open();
        }

        bool StorageProvider::close(void)
        {
            Q_D(StorageProvider);
            return d->close();
        }

        bool StorageProvider::contains(const QString & id)
        {
            Q_D(StorageProvider);
            return d->contains(id);
        }

        bool StorageProvider::entries(QStringList& entryList)
        {
            Q_D(StorageProvider);
            QStringList l;
            if(d->entries(l))
            {
                entryList = l;
                return true;
            }
            else
            {
                return false;
            }
        }

        Storage * StorageProvider::lookup(const QString& id)
        {
            Q_D(StorageProvider);
            return d->lookup(id);
        }

        Storage * StorageProvider::create(const QString& id, OTPTokenType type)
        {
            Q_D(StorageProvider);
            return d->create(id, type);
        }
    }
}
