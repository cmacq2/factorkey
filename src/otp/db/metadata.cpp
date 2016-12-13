#include "metadata.h"

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)

namespace otp
{
    namespace storage
    {
        namespace db
        {
            bool Metadata::remove(void)
            {
                return m_dbManager && m_dbManager->remove(m_entryId);
            }

            bool Metadata::exists(void) const
            {
                return m_dbManager && m_dbManager->contains(m_entryId);
            }

            bool Metadata::commit(void)
            {
                if(m_dataWrite.isEmpty())
                {
                    return true;
                }


                if(m_typeHandler && m_dbManager)
                {
                    /*
                     * Construct a QHash representation of the full, desired state of the meta data.
                     */
                    QHash<QString,QVariant> newParamState(m_dataWrite);
                    for(const auto k: m_dataRead.keys())
                    {
                        if(!newParamState.contains(k))
                        {
                            newParamState.insert(k, m_dataRead.value(k));
                        }
                    }

                    if(m_typeHandler->saveMetaData(m_entryId, m_dataWrite, m_dbManager.data()))
                    {
                        m_dataRead = newParamState;
                        m_typeRead = m_typeWrite;
                        m_dataWrite.clear();
                        return true;
                    }
                }
                return false;
            }

            bool Metadata::poll(void)
            {

                QHash<QString,QVariant> data;
                otp::storage::OTPTokenType t;
                if(establishTokenType(t) && m_typeHandler && m_dbManager && m_typeHandler->fetchMetaData(m_entryId, data, m_dbManager.data()))
                {
                    m_dataRead = data;
                    return true;
                }
                return false;
            }

            bool Metadata::readParam(const QString& param, QVariant& value) const
            {
                if(haveType() && m_typeHandler && m_typeHandler->isParamNameValid(param)) {
                    QVariant v;
                    if(m_dataRead.contains(param))
                    {
                        v = m_dataRead.value(param);
                    }
                    value = v;
                    return true;
                }
                return false;
            }

            bool Metadata::writeParam(const QString& param, const QVariant& value)
            {
                if(haveType() && m_typeHandler && m_typeHandler->isParamNameValid(param))
                {
                    m_dataWrite.insert(param, value);
                    return true;
                }
                return false;
            }

            bool Metadata::readTokenType(otp::storage::OTPTokenType& type) const
            {
                if(haveType())
                {
                    type = m_typeRead;
                    return true;
                }
                return false;
            }

            bool Metadata::establishTokenType(otp::storage::OTPTokenType& type)
            {
                if(haveType())
                {
                    type = m_typeRead;
                    return true;
                }
                else
                {
                    QVariant v;
                    if(m_dbManager && m_dbManager->readType(m_entryId, v) && !v.isNull())
                    {
                        m_typeRead = v.value<enum otp::storage::OTPTokenType>();
                        if(createHandler(m_typeRead))
                        {
                            m_dataRead.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant::fromValue(m_typeRead));
                            type = m_typeRead;
                            return true;
                        }
                    }
                    return false;
                }
            }

            bool Metadata::containsType(const QHash<QString,QVariant>& data) const
            {
                return data.contains(otp::storage::Storage::OTP_TOKEN_TYPE) &&
                    !m_dataRead.value(otp::storage::Storage::OTP_TOKEN_TYPE).isNull();
            }

            bool Metadata::haveType(void) const
            {
                return containsType(m_dataRead);
            }

            bool Metadata::createHandler(const otp::storage::OTPTokenType& type)
            {
                if(m_dbManager)
                {
                    m_typeHandler = m_dbManager->handler(type);
                }
                if(m_typeHandler)
                {
                    m_dataRead.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant::fromValue(type));
                    m_typeRead = type;
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool Metadata::convertHandler(const QSharedPointer<MetadataStorageHandler> newHandler)
            {
                bool result;
                if(m_typeHandler) {
                    QSet<QString> currentKeys = newHandler->keys();
                    currentKeys.insert(otp::storage::Storage::OTP_TOKEN_TYPE);

                    QSet<QString> tablesToNullify;
                    for(const auto p: currentKeys)
                    {
                        const auto t = newHandler->tableForParam(p);
                        tablesToNullify.insert(t);
                    }

                    result = m_dbManager && m_typeHandler->pruneMetaData(m_entryId, currentKeys, tablesToNullify, m_dbManager.data());
                }
                else {
                    result = true;
                }
                m_typeHandler = newHandler;
                return result;
            }

            bool Metadata::typeWritten(void) const
            {
                return containsType(m_dataWrite);
            }

            bool Metadata::writeTokenType(const otp::storage::OTPTokenType& type)
            {
                bool result = false;
                if(haveType()) {
                    // already in desired state, then nothing needs to be done.
                    if((typeWritten() && m_typeWrite == type) || (!typeWritten() && m_typeRead == type)) {
                        result = true;
                    }
                    else {
                        if(m_dbManager)
                        {
                            const auto newHandler = m_dbManager->handler(type);
                            if(newHandler)
                            {
                                result = convertHandler(newHandler);
                                m_dataWrite.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant::fromValue(type));
                                m_typeWrite = type;
                            }
                        }
                    }
                }
                else {
                    if(createHandler(type)) {
                        m_dataWrite.insert(otp::storage::Storage::OTP_TOKEN_TYPE, QVariant::fromValue(type));
                        m_typeWrite = type;
                        result = true;
                    }
                }
                return result;
            }
        }
    }
}
