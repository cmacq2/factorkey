#ifndef FACTORKEY_AUTOTEST_MOCKED_STORAGE_H
#define FACTORKEY_AUTOTEST_MOCKED_STORAGE_H

#include <QSet>
#include <QSharedPointer>

#include "otp/storage/storage_p.h"
#include "lib/googlemock.h"

using ::testing::_;
using ::testing::Invoke;

namespace stubs
{
    namespace storage
    {
        class FakeStorageProviderPrivate: public otp::storage::StorageProviderPrivate
        {
        public:
            typedef std::function<otp::storage::StoragePrivate *(const QString&)> LookupFunction;
            typedef std::function<otp::storage::StoragePrivate *(const QString&, otp::storage::OTPTokenType type)> CreateFunction;
            FakeStorageProviderPrivate(const QHash<QString, LookupFunction>& lookup, const QHash<QString, CreateFunction>& create);
            FakeStorageProviderPrivate(const QHash<QString, LookupFunction>& lookup);
            FakeStorageProviderPrivate(const QHash<QString, CreateFunction>& create);
            FakeStorageProviderPrivate();
            bool open(void);
            bool close(void);
            bool contains(const QString& entry);
            bool entries(QStringList&);
            otp::storage::Storage * lookup(const QString& entry);
            otp::storage::Storage * create(const QString& entry, otp::storage::OTPTokenType type);
            virtual ~FakeStorageProviderPrivate();
        protected:
            virtual bool allowContains(void) const;
            virtual bool allowLookup(void) const;
            virtual bool allowCreate(void) const;
            virtual bool allowEntries(void) const;
            virtual bool allowOpen(void) const;
            virtual bool allowClose(void) const;
        private:
            const QHash<QString, LookupFunction> m_lookup;
            const QHash<QString, CreateFunction> m_create;
            bool m_open;
            QSet<QString> m_created;
        };
    }
}

namespace mock
{
    namespace storage
    {
        class MockStorageProviderPrivate: public otp::storage::StorageProviderPrivate
        {
        public:
            MOCK_METHOD0(open, bool(void));
            MOCK_METHOD0(close, bool(void));
            MOCK_METHOD1(contains, bool(const QString& entry));
            MOCK_METHOD1(entries, bool(QStringList&));
            MOCK_METHOD1(lookup, otp::storage::Storage * (const QString& entry));
            MOCK_METHOD2(create, otp::storage::Storage * (const QString& entry, otp::storage::OTPTokenType type));
        };

        class DelegatingMockStorageProviderPrivate: public otp::storage::StorageProviderPrivate
        {
        public:
            MOCK_METHOD0(open, bool(void));
            MOCK_METHOD0(close, bool(void));
            MOCK_METHOD1(contains, bool(const QString& entry));
            MOCK_METHOD1(entries, bool(QStringList&));
            MOCK_METHOD1(lookup, otp::storage::Storage * (const QString& entry));
            MOCK_METHOD2(create, otp::storage::Storage * (const QString& entry, otp::storage::OTPTokenType type));

            bool delegateToFake(const QSharedPointer<otp::storage::StorageProviderPrivate> fake);
        private:
            QSharedPointer<otp::storage::StorageProviderPrivate> m_fake;
        };
    }
}

#endif
