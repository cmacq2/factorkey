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

        bool FakeStoragePrivate::writePassword(const QString& password)
        {
            if(allowWritePassword())
            {
                m_password = password;
                return true;
            }
            return false;
        }

        bool FakeStoragePrivate::readPassword(QString& password) const
        {
            if(allowReadPassword())
            {
                password = m_password;
                return true;
            }
            return false;
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
        bool DelegatingMockStoragePrivate::delegateToFake(const QSharedPointer<otp::storage::StoragePrivate> fake)
        {
            if(fake)
            {
                m_fake = fake;
                ON_CALL(*this, entryId()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::entryId));
                ON_CALL(*this, type()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::type));
                ON_CALL(*this, exists()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::exists));
                ON_CALL(*this, commit()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::commit));
                ON_CALL(*this, poll()).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::poll));
                ON_CALL(*this, readParam(_, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::readParam));
                ON_CALL(*this, writeParam(_, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::writeParam));
                ON_CALL(*this, writeTokenType(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::writeTokenType));
                ON_CALL(*this, readTokenType(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::readTokenType));
                ON_CALL(*this, readPassword(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::readPassword));
                ON_CALL(*this, writePassword(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::StoragePrivate::writePassword));
            }
            return fake;
        }
    }
}
