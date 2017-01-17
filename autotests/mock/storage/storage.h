#ifndef FACTORKEY_AUTOTEST_MOCKED_STORAGE_H
#define FACTORKEY_AUTOTEST_MOCKED_STORAGE_H

#include <QSharedPointer>

#include "otp/storage/storage_p.h"
#include "autotests/lib/googlemock.h"

using ::testing::_;
using ::testing::Invoke;

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
            QString entryId(void) const;
            otp::storage::OTPTokenType type(void) const;
            bool writePassword(const QString& password, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation);
            bool readTokenType(otp::storage::OTPTokenType& type) const;
            bool writeTokenType(const otp::storage::OTPTokenType& type);
            bool readParam(const QString& param, QVariant& value) const;
            bool readPassword(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret) const;
            bool writeParam(const QString& param, const QVariant& value);
            bool exists(void) const;
            bool commit(void);
            bool poll(void);
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
            MOCK_CONST_METHOD2(readParam, bool(const QString& param, QVariant& value));
            MOCK_CONST_METHOD1(readPassword, bool(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret));
            MOCK_CONST_METHOD1(readTokenType, bool(otp::storage::OTPTokenType& type));

            MOCK_METHOD2(writePassword, bool(const QString& password, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation));
            MOCK_METHOD1(writeTokenType, bool(const otp::storage::OTPTokenType& type));
            MOCK_METHOD2(writeParam, bool(const QString& param, const QVariant& value));

            MOCK_CONST_METHOD0(entryId, QString(void));
            MOCK_CONST_METHOD0(type, otp::storage::OTPTokenType(void));

            MOCK_CONST_METHOD0(exists, bool(void));
            MOCK_METHOD0(commit, bool(void));
            MOCK_METHOD0(poll, bool(void));
        };

        class DelegatingMockStoragePrivate: public otp::storage::StoragePrivate
        {
        public:
            MOCK_CONST_METHOD0(entryId, QString(void));
            MOCK_CONST_METHOD0(type, otp::storage::OTPTokenType(void));
            MOCK_CONST_METHOD2(readParam, bool(const QString& param, QVariant& value));
            MOCK_CONST_METHOD1(readPassword, bool(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret));
            MOCK_CONST_METHOD0(exists, bool(void));
            MOCK_CONST_METHOD1(readTokenType, bool(otp::storage::OTPTokenType& type));

            MOCK_METHOD2(writePassword, bool(const QString& password, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation));
            MOCK_METHOD1(writeTokenType, bool(const otp::storage::OTPTokenType& type));
            MOCK_METHOD2(writeParam, bool(const QString& param, const QVariant& value));
            MOCK_METHOD0(commit, bool(void));
            MOCK_METHOD0(poll, bool(void));

            bool delegateToFake(const QSharedPointer<otp::storage::StoragePrivate> fake);
        private:
            QSharedPointer<otp::storage::StoragePrivate> m_fake;
        };
    }
}

#endif
