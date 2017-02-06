#include "metadb.h"

using ::testing::_;
using ::testing::Invoke;

namespace mock
{
    namespace storage
    {
        namespace db
        {
            MockMetadataDbManager::MockMetadataDbManager(const QString& connectionName,
                                                         const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                                         const otp::storage::db::MetadataDbManager::InitDb& configure) :
                otp::storage::db::MetadataDbManager(connectionName, handlers, configure) {}

            DelegatingMockMetadataDbManager::DelegatingMockMetadataDbManager(const QString& connectionName,
                                                                             const QHash<int, QSharedPointer<otp::storage::db::MetadataStorageHandler>>& handlers,
                                                                             const otp::storage::db::MetadataDbManager::InitDb& configure) :
                otp::storage::db::MetadataDbManager(connectionName, handlers, configure) {}

            bool DelegatingMockMetadataDbManager::delegateToFake(const QSharedPointer<otp::storage::db::MetadataDbManager> fake)
            {
                if(fake)
                {
                    m_fake = fake;
                    ON_CALL(*this, isOpened()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::isOpened));
                    ON_CALL(*this, open()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::open));
                    ON_CALL(*this, close()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::close));
                    ON_CALL(*this, readType(_, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::readType));
                    ON_CALL(*this, removeEntries(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::removeEntries));
                    ON_CALL(*this, removeAll()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::removeAll));
                    ON_CALL(*this, remove(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::remove));
                    ON_CALL(*this, contains(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::contains));
                    ON_CALL(*this, entries(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::entries));
                    ON_CALL(*this, handler(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::handler));
                    ON_CALL(*this, supportedHandlers()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::supportedHandlers));
                    ON_CALL(*this, supports(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataDbManager::supports));
                }
                return fake;
            }
        }
    }
}
