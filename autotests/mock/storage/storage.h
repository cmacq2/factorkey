#ifndef FACTORKEY_AUTOTEST_MOCKED_STORAGE_H
#define FACTORKEY_AUTOTEST_MOCKED_STORAGE_H

#include <QSharedPointer>

#include "otp/storage/storage_p.h"
#include "autotests/lib/trompeloeil.h"

namespace stubs
{
    namespace storage
    {
        class FakeStoragePrivate: public otp::storage::StoragePrivate
        {
        private:
            QString m_entryId;
            otp::storage::OTPTokenType m_type;
            QString m_password;
            QHash<QString,QVariant> m_map;
        public:
            FakeStoragePrivate(const QString& entryId, otp::storage::OTPTokenType type, const QString& password, const QHash<QString,QVariant>& preset);
            FakeStoragePrivate(const QString& entryId,  otp::storage::OTPTokenType type, const QString& password);
            FakeStoragePrivate(otp::storage::OTPTokenType type, const QString& password, const QHash<QString,QVariant>& preset);
            FakeStoragePrivate(otp::storage::OTPTokenType type, const QString& password);
            virtual ~FakeStoragePrivate();
        public:
            const QHash<QString,QVariant>& rawStorage(void) const;
        public:
            QString entryId(void) const override;
            otp::storage::OTPTokenType type(void) const override;
            bool writePassword(const QString& password, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation) override;
            bool readTokenType(otp::storage::OTPTokenType& type) const override;
            bool writeTokenType(const otp::storage::OTPTokenType& type) override;
            bool readParam(const QString& param, QVariant& value) const override;
            bool readPassword(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret) const override;
            bool writeParam(const QString& param, const QVariant& value) override;
            bool exists(void) const override;
            bool commit(void) override;
            bool poll(void) override;
        protected:
            bool allowWritePassword(void) const;
            bool allowWriteParam(void) const;
            bool allowReadPassword(void) const;
            bool allowReadParam(void) const;
            bool allowWriteTokenType(void) const;
            bool allowReadTokenType(void) const;
        };
    }
}

namespace mock
{
    namespace storage
    {
        class MockStoragePrivate: public otp::storage::StoragePrivate
        {
        public:
            MockStoragePrivate();
            virtual ~MockStoragePrivate();
            MAKE_CONST_MOCK2(readParam, bool(const QString& param, QVariant& value), override);
            MAKE_CONST_MOCK1(readPassword, bool(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret), override);
            MAKE_CONST_MOCK1(readTokenType, bool(otp::storage::OTPTokenType& type), override);

            MAKE_MOCK2(writePassword, bool(const QString& password, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation), override);
            MAKE_MOCK1(writeTokenType, bool(const otp::storage::OTPTokenType& type), override);
            MAKE_MOCK2(writeParam, bool(const QString& param, const QVariant& value), override);

            MAKE_CONST_MOCK0(entryId, QString(void), override);
            MAKE_CONST_MOCK0(type, otp::storage::OTPTokenType(void), override);

            MAKE_CONST_MOCK0(exists, bool(void), override);
            MAKE_MOCK0(commit, bool(void), override);
            MAKE_MOCK0(poll, bool(void), override);
        };
    }
}

#endif
