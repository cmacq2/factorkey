#include "storage_p.h"

namespace otp
{
    namespace storage
    {
        StoragePrivate::~StoragePrivate() {}

        StorageProviderPrivate::~StorageProviderPrivate() {}

        bool StorageProviderPrivate::open(void)
        {
            bool open = isOpened();
            if(!open)
            {
                return openBackend();
            }
            return open;
        }

        bool StorageProviderPrivate::close(void)
        {
            if(isOpened())
            {
                return closeBackend();
            }
            return true;
        }

        bool StorageProviderPrivate::contains(const QString& entry)
        {
            QStringList l;
            if(entries(l))
            {
                return l.indexOf(entry) >= 0;
            }
            return false;
        }

        bool StorageProviderPrivate::entries(QStringList&)
        {
            return false;
        }

        Storage * StorageProviderPrivate::lookup(const QString& entry)
        {
            return open() ? doLookup(entry) : nullptr;
        }

        Storage * StorageProviderPrivate::create(const QString& entry, OTPTokenType type)
        {
            if(open())
            {
                if(contains(entry))
                {
                    Storage * s = lookup(entry);
                    if(s && s->type() == type)
                    {
                        return s;
                    }
                }
                else
                {
                    return doCreate(entry, type);
                }
            }
            return nullptr;
        }

        Storage * StorageProviderPrivate::doLookup(const QString&)
        {
            return nullptr;
        }

        Storage * StorageProviderPrivate::doCreate(const QString&, OTPTokenType)
        {
            return nullptr;
        }

        bool StorageProviderPrivate::isOpened(void)
        {
            return false;
        }

        bool StorageProviderPrivate::openBackend(void)
        {
            return false;
        }

        bool StorageProviderPrivate::closeBackend(void)
        {
            return false;
        }
    }
}
