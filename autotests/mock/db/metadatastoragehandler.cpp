#include "metadatastoragehandler.h"

using ::testing::_;
using ::testing::Invoke;

namespace mock
{
    namespace storage
    {
        namespace db
        {
            static const QHash<QString,QString> emptyHash;
            static const QSet<QString> emptySet;

            MockMetadataStorageHandler::~MockMetadataStorageHandler() {}
            MockMetadataStorageHandler::MockMetadataStorageHandler(otp::storage::OTPTokenType type) :
                otp::storage::db::MetadataStorageHandler(type, emptyHash, emptySet, emptyHash, emptyHash) {}

            DelegatingMockMetadataStorageHandler::~DelegatingMockMetadataStorageHandler() {}
            DelegatingMockMetadataStorageHandler::DelegatingMockMetadataStorageHandler(otp::storage::OTPTokenType type) :
                otp::storage::db::MetadataStorageHandler(type, emptyHash, emptySet, emptyHash, emptyHash) {}

            bool DelegatingMockMetadataStorageHandler::delegateToFake(const QSharedPointer<otp::storage::db::MetadataStorageHandler> fake)
            {
                if(fake)
                {
                    m_fake = fake;
                    ON_CALL(*this, type()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::type));
                    ON_CALL(*this, schema()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::schema));
                    ON_CALL(*this, saveMetaData(_, _, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::saveMetaData));
                    ON_CALL(*this, fetchMetaData(_, _, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::fetchMetaData));
                    ON_CALL(*this, pruneMetaData(_, _, _, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::pruneMetaData));
                    ON_CALL(*this, deleteMetaData(_, _, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::deleteMetaData));
                    ON_CALL(*this, resetMetaData(_, _, _)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::resetMetaData));
                    ON_CALL(*this, keys()).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::keys));
                    ON_CALL(*this, isParamNameValid(_)).WillByDefault(Invoke(m_fake.data(), &otp::storage::db::MetadataStorageHandler::isParamNameValid));
                }
                return fake;
            }
        }
    }
}
