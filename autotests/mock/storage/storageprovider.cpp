#include "storageprovider.h"


namespace stubs
{
    namespace storage
    {
        FakeStorageProviderPrivate::FakeStorageProviderPrivate(const QHash<QString, LookupFunction>& lookup, const QHash<QString, CreateFunction>& create) :
            otp::storage::StorageProviderPrivate(), m_lookup(lookup), m_create(create), m_open(false) {}
        FakeStorageProviderPrivate::FakeStorageProviderPrivate(const QHash<QString, LookupFunction>& lookup) :
            otp::storage::StorageProviderPrivate(), m_lookup(lookup), m_create(QHash<QString, CreateFunction>()), m_open(false) {}
        FakeStorageProviderPrivate::FakeStorageProviderPrivate(const QHash<QString, CreateFunction>& create) :
            otp::storage::StorageProviderPrivate(), m_lookup(QHash<QString, LookupFunction>()), m_create(create), m_open(false) {}
        FakeStorageProviderPrivate::~FakeStorageProviderPrivate() {}

        bool FakeStorageProviderPrivate::allowEntries(void) const
        {
            return true;
        }

        bool FakeStorageProviderPrivate::allowCreate(void) const
        {
            return true;
        }

        bool FakeStorageProviderPrivate::allowOpen(void) const
        {
            return true;
        }

        bool FakeStorageProviderPrivate::allowLookup(void) const
        {
            return true;
        }

        bool FakeStorageProviderPrivate::allowContains(void) const
        {
            return true;
        }

        bool FakeStorageProviderPrivate::allowClose(void) const
        {
            return true;
        }

        bool FakeStorageProviderPrivate::open(void)
        {
            if(allowOpen())
            {
                m_open = true;
                return true;
            }
            return false;
        }

        bool FakeStorageProviderPrivate::close(void)
        {
            if(allowClose())
            {
                m_open = false;
                return true;
            }
            return false;
        }


        bool FakeStorageProviderPrivate::contains(const QString& entry)
        {
            if(allowContains())
            {
                return m_lookup.contains(entry) || m_created.contains(entry);
            }
            return false;
        }

        bool FakeStorageProviderPrivate::entries(QStringList& entries)
        {
            if(allowEntries())
            {
                for(const QString& e1: m_created)
                {
                    entries << e1;
                }
                for(const QString& e2: m_lookup.keys())
                {
                    if(!m_created.contains(e2))
                    {
                        entries << e2;
                    }
                }
            }
            return false;
        }

        otp::storage::Storage * FakeStorageProviderPrivate::lookup(const QString& entry)
        {
            if(allowLookup())
            {
                if(m_lookup.contains(entry))
                {
                    const auto result = m_lookup.value(entry)(entry);
                    if(result)
                    {
                        m_created.insert(entry);
                        return new otp::storage::Storage(result);
                    }
                }
            }
            return nullptr;
        }

        otp::storage::Storage * FakeStorageProviderPrivate::create(const QString& entry, otp::storage::OTPTokenType type)
        {
            if(allowCreate())
            {
                if(m_create.contains(entry) && !m_created.contains(entry))
                {
                    const auto result = m_create.value(entry)(entry, type);
                    if(result)
                    {
                        m_created.insert(entry);
                        return new otp::storage::Storage(result);
                    }
                }
            }
            return nullptr;
        }
    }
}

namespace mock
{
    namespace storage
    {
        MockStorageProviderPrivate::MockStorageProviderPrivate() {}
        MockStorageProviderPrivate::~MockStorageProviderPrivate() {}
        DelegatingMockStorageProviderPrivate::DelegatingMockStorageProviderPrivate() {}
        DelegatingMockStorageProviderPrivate::~DelegatingMockStorageProviderPrivate() {}

        bool DelegatingMockStorageProviderPrivate::delegateToFake(const QSharedPointer<otp::storage::StorageProviderPrivate> fake)
        {
            if(fake)
            {
                m_fake = fake;
                ON_CALL(*this, open()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StorageProviderPrivate::open));
                ON_CALL(*this, close()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StorageProviderPrivate::close));
                ON_CALL(*this, contains(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StorageProviderPrivate::contains));
                ON_CALL(*this, entries(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StorageProviderPrivate::entries));
                ON_CALL(*this, lookup(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StorageProviderPrivate::lookup));
                ON_CALL(*this, create(_, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StorageProviderPrivate::create));
            }
            return fake;
        }
    }
}
