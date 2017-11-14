#ifndef FACTORKEY_AUTOTEST_MOCKED_STORAGE_H
#define FACTORKEY_AUTOTEST_MOCKED_STORAGE_H

#include <QSet>
#include <QSharedPointer>

#include "otp/storage/storage_p.h"
#include "autotests/lib/trompeloeil.h"

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
            bool open(void) override;
            bool close(void) override;
            bool contains(const QString& entry) override;
            bool entries(QStringList&) override;
            otp::storage::Storage * lookup(const QString& entry) override;
            otp::storage::Storage * create(const QString& entry, otp::storage::OTPTokenType type) override;
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
            MockStorageProviderPrivate();
            virtual ~MockStorageProviderPrivate();
            MAKE_MOCK0(open, bool(void), override);
            MAKE_MOCK0(close, bool(void), override);
            MAKE_MOCK1(contains, bool(const QString& entry), override);
            MAKE_MOCK1(entries, bool(QStringList&), override);
            MAKE_MOCK1(lookup, otp::storage::Storage * (const QString& entry), override);
            MAKE_MOCK2(create, otp::storage::Storage * (const QString& entry, otp::storage::OTPTokenType type), override);
        };
    }
}

#endif
