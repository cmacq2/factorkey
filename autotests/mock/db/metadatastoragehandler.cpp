#include "metadatastoragehandler.h"

using ::testing::_;
using ::testing::Invoke;

namespace stubs
{
    namespace storage
    {
        namespace db
        {
            DummyMetadataStorageHandler::DummyMetadataStorageHandler(const otp::storage::OTPTokenType& type,
                                                                     const QHash<QString,QString>& tables,
                                                                     const QSet<QString>& schema,
                                                                     const QHash<QString,QString> columnsToParams,
                                                                     const QHash<QString,QString> paramsToTables):
                otp::storage::db::MetadataStorageHandler(type, tables, schema, columnsToParams, paramsToTables) {}
            DummyMetadataStorageHandler::~DummyMetadataStorageHandler() {}

            const QSet<QString>& DummyMetadataStorageHandler::schema(void) const
            {
                return MetadataStorageHandler::schema();
            }

            otp::storage::OTPTokenType DummyMetadataStorageHandler::type(void) const
            {
                return otp::storage::db::MetadataStorageHandler::type();
            }

            bool DummyMetadataStorageHandler::allowSaveMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::saveMetaData(const QString&, const QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *) const
            {
                return allowSaveMetaData();
            }

            bool DummyMetadataStorageHandler::allowFetchMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::fetchMetaData(const QString&, QHash<QString,QVariant>&, otp::storage::db::MetadataDbManager *) const
            {
                return false;
            }

            bool DummyMetadataStorageHandler::allowDeleteMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::allowResetMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::allowPruneMetaData(void) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::deleteMetaData(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::resetMetaData(const QString&, const QSet<QString>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            bool DummyMetadataStorageHandler::pruneMetaData(const QString&, const QSet<QString>&, const QSet<QString>&, otp::storage::db::MetadataDbManager *) const
            {
                return true;
            }

            QSet<QString> DummyMetadataStorageHandler::keys(void) const
            {
                return MetadataStorageHandler::keys();
            }

            bool DummyMetadataStorageHandler::isParamNameValid(const QString& param) const
            {
                return MetadataStorageHandler::isParamNameValid(param);
            }
        }
    }
}

namespace mock
{
    namespace storage
    {
        namespace db
        {
            static const QHash<QString,QString> emptyHash;
            static const QSet<QString> emptySet;

            MockMetadataStorageHandler::MockMetadataStorageHandler(otp::storage::OTPTokenType type) :
                otp::storage::db::MetadataStorageHandler(type, emptyHash, emptySet, emptyHash, emptyHash) {}

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
