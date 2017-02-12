#include "dummy-metadatastoragehandler.h"

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

            FakeSingleEntryMetadataStorageHandler::FakeSingleEntryMetadataStorageHandler(
                const otp::storage::OTPTokenType& type, const QString& entryId, QHash<QString,QVariant> * const metadata, const QHash<QString,QString>& paramsToTables) :
                otp::storage::db::MetadataStorageHandler(type, QHash<QString,QString>(), QSet<QString>(), QHash<QString,QString>(), paramsToTables),
                m_entryId(entryId), m_metadata(metadata) {}


            FakeSingleEntryMetadataStorageHandler::FakeSingleEntryMetadataStorageHandler(const otp::storage::OTPTokenType& type, const QString& entryId, const QHash<QString,QString>& paramsToTables) :
                otp::storage::db::MetadataStorageHandler(type, QHash<QString,QString>(), QSet<QString>(), QHash<QString,QString>(), paramsToTables),
                m_entryId(entryId), m_metadata(nullptr) {}


            FakeSingleEntryMetadataStorageHandler::~FakeSingleEntryMetadataStorageHandler() {}

            bool FakeSingleEntryMetadataStorageHandler::resetMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager *) const
            {
                if(allowResetMetaData() && validate(entryId, keys))
                {
                    for(const auto& k: keys)
                    {
                        m_metadata->insert(k, QVariant());
                    }
                    return true;
                }
                return false;
            }

            bool FakeSingleEntryMetadataStorageHandler::allowSaveMetaData(void) const
            {
                return m_metadata;
            }

            bool FakeSingleEntryMetadataStorageHandler::allowFetchMetaData(void) const
            {
                return m_metadata;
            }

            bool FakeSingleEntryMetadataStorageHandler::allowDeleteMetaData(void) const
            {
                return m_metadata;
            }

            bool FakeSingleEntryMetadataStorageHandler::allowResetMetaData(void) const
            {
                return m_metadata;
            }

            bool FakeSingleEntryMetadataStorageHandler::allowPruneMetaData(void) const
            {
                return m_metadata;
            }

            bool FakeSingleEntryMetadataStorageHandler::pruneMetaData(const QString& entryId, const QSet<QString>& newKeys, const QSet<QString>& tablesToNullify, otp::storage::db::MetadataDbManager *) const
            {
                if(allowPruneMetaData() && entryId == m_entryId)
                {
                    QSet<QString> toNull = diff(newKeys);
                    for(const auto& k: m_metadata->keys())
                    {
                        if(!newKeys.contains(k))
                        {
                            const auto& t = tableForParam(k);
                            if(tablesToNullify.contains(t))
                            {
                                if(toNull.contains(k))
                                {
                                    m_metadata->insert(k, QVariant());
                                }
                            }
                            else
                            {
                                m_metadata->remove(k);
                            }
                        }
                    }
                    return true;
                }
                return false;
            }

            bool FakeSingleEntryMetadataStorageHandler::fetchMetaData(const QString& entryId, QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager *) const
            {
                if(allowFetchMetaData() && entryId == m_entryId)
                {
                    QHash<QString, QVariant> result(*m_metadata);
                    for(const auto& k: keys())
                    {
                        if(!result.contains(k))
                        {
                            result.insert(k, QVariant());
                        }
                    }
                    metadata = result;
                    return true;
                }
                return false;
            }

            bool FakeSingleEntryMetadataStorageHandler::validate(const QString& entryId, const QSet<QString>& keys) const
            {
                if(m_entryId != entryId)
                {
                    return false;
                }

                for(const auto& k: keys)
                {
                    if(!isParamNameValid(k))
                    {
                        return false;
                    }
                }

                return true;
            }

            bool FakeSingleEntryMetadataStorageHandler::deleteMetaData(const QString& entryId, const QSet<QString>& keys, otp::storage::db::MetadataDbManager *) const
            {
                if(allowDeleteMetaData() && validate(entryId, keys))
                {
                    for(const auto& k: keys)
                    {
                        m_metadata->remove(k);
                    }
                    return true;
                }
                return false;
            }

            bool FakeSingleEntryMetadataStorageHandler::saveMetaData(const QString& entryId, const QHash<QString,QVariant>& metadata, otp::storage::db::MetadataDbManager *) const
            {
                const auto& ks = metadata.keys().toSet();
                if(allowSaveMetaData() && validate(entryId, ks))
                {
                    for(const auto& k: metadata.keys())
                    {
                        m_metadata->insert(k, metadata.value(k));
                    }
                    return true;
                }
                return false;
            }

        }
    }
}
