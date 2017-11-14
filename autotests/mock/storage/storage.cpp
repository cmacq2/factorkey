#include "storage.h"

namespace stubs
{
    namespace storage
    {
        FakeStoragePrivate::FakeStoragePrivate(const QString& entryId,otp::storage::OTPTokenType type, const QString& password, const QHash<QString,QVariant>& preset):
            otp::storage::StoragePrivate(), m_entryId(entryId), m_type(type), m_password(password), m_map(preset) {}

        FakeStoragePrivate::FakeStoragePrivate(const QString& entryId, otp::storage::OTPTokenType type, const QString& password):
            otp::storage::StoragePrivate(), m_entryId(entryId), m_type(type), m_password(password) {}

        FakeStoragePrivate::FakeStoragePrivate(otp::storage::OTPTokenType type, const QString& password):
            otp::storage::StoragePrivate(), m_type(type), m_password(password) {}

        FakeStoragePrivate::FakeStoragePrivate(otp::storage::OTPTokenType type, const QString& password, const QHash<QString,QVariant>& preset):
            otp::storage::StoragePrivate(), m_type(type), m_password(password), m_map(preset) {}

        FakeStoragePrivate::~FakeStoragePrivate() {}

        const QHash<QString,QVariant>& FakeStoragePrivate::rawStorage(void) const
        {
            return m_map;
        }

        otp::storage::OTPTokenType FakeStoragePrivate::type(void) const
        {
            return m_type;
        }

        QString FakeStoragePrivate::entryId(void) const
        {
            return m_entryId;
        }

        bool FakeStoragePrivate::allowWriteParam(void) const
        {
            return true;
        }

        bool FakeStoragePrivate::allowWriteTokenType(void) const
        {
            return true;
        }

        bool FakeStoragePrivate::allowWritePassword(void) const
        {
            return true;
        }

        bool FakeStoragePrivate::allowReadParam(void) const
        {
            return true;
        }

        bool FakeStoragePrivate::allowReadTokenType(void) const
        {
            return true;
        }

        bool FakeStoragePrivate::allowReadPassword(void) const
        {
            return true;
        }

        bool FakeStoragePrivate::writePassword(const QString& password, const otp::secrets::SecretsAPIProvider::SecretConfirmation& confirmation)
        {
            if(allowWritePassword())
            {
                m_password = password;
                confirmation(true, m_entryId);
                return true;
            }
            else
            {
                confirmation(false, m_entryId);
                return false;
            }
        }

        bool FakeStoragePrivate::readPassword(const otp::secrets::SecretsAPIProvider::SecretAnswer& secret) const
        {
            if(allowReadPassword())
            {
                secret(true, m_entryId, m_password);
                return true;
            }
            else
            {
                secret(false, m_entryId, QString());
                return false;
            }
        }

        bool FakeStoragePrivate::readTokenType(otp::storage::OTPTokenType& type) const
        {
            if(allowReadTokenType())
            {
                type = m_type;
                return true;
            }
            return false;
        }

        bool FakeStoragePrivate::writeTokenType(const otp::storage::OTPTokenType& type)
        {
            if(allowWriteTokenType())
            {
                m_type = type;
                return true;
            }
            return false;
        }

        bool FakeStoragePrivate::readParam(const QString& param, QVariant& value) const
        {
            if(allowReadParam() && m_map.contains(param))
            {
                value = m_map.value(param);
                return true;
            }
            return false;
        }

        bool FakeStoragePrivate::writeParam(const QString& param, const QVariant& value)
        {
            if(allowWriteParam())
            {
                m_map.insert(param, value);
                return true;
            }
            return false;
        }

        bool FakeStoragePrivate::exists(void) const
        {
            return false;
        }

        bool FakeStoragePrivate::commit(void)
        {
            return false;
        }

        bool FakeStoragePrivate::poll(void)
        {
            return false;
        }
    }
}

namespace mock
{
    namespace storage
    {
        MockStoragePrivate::MockStoragePrivate() {}
        MockStoragePrivate::~MockStoragePrivate() {}
    }
}
