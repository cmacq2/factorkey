#include "generator_p.h"

namespace otp
{
    namespace generator
    {
        TokenParametersPrivate::TokenParametersPrivate(otp::storage::Storage * store): m_storage(store) {}
        TokenParametersPrivate::~TokenParametersPrivate() {}

        bool TokenParametersPrivate::setCodec(const QString& key, const QTextCodec * codec)
        {
            if(codec)
            {

                auto v = QString::fromUtf8(codec->name());
                if(v.isNull() || v.isEmpty())
                {
                    return false;
                }
                else
                {
                    storage()->writeParam(key, QVariant(v));
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        bool TokenParametersPrivate::lookupCodec(const QString& key, QTextCodec ** codec) const
        {
            QVariant val;
            if(lookup(key, val))
            {
                if(val.isNull())
                {
                    *codec = nullptr;
                    return true;
                }
                else
                {
                    auto name = val.toString().toUtf8();
                    QTextCodec * cdc = QTextCodec::codecForName(name);
                    if(cdc)
                    {
                        *codec = cdc;
                        return true;
                    }
                }
            }
            return false;
        }

        bool TokenParametersPrivate::lookupSecret(const otp::storage::secrets::SecretsAPIProvider::SecretAnswer& secret) const
        {
            return m_storage->readPassword([secret](bool ok, const QString& entryId, const QString& value) -> void
            {
                if(ok && !value.isNull() && !value.isEmpty())
                {
                    secret(ok, entryId, value);
                }
                else
                {
                    secret(false, entryId, QString());
                }
            });
        }

        bool TokenParametersPrivate::writeSecret(const QString & secret, const otp::storage::secrets::SecretsAPIProvider::SecretConfirmation& confirmation)
        {
            return !secret.isNull() && !secret.isEmpty() && m_storage->writePassword(secret, confirmation);
        }

        bool TokenParametersPrivate::lookup(const QString & key, QVariant & value) const
        {
            QVariant v;
            if(m_storage->readParam(key, v))
            {
                value = v;
                return true;
            }
            return false;
        }

        otp::storage::Storage * TokenParametersPrivate::storage(void) const
        {
            return m_storage;
        }

        TokenGeneratorPrivate::TokenGeneratorPrivate(TokenParameters * p) : m_params(p) {}
        TokenGeneratorPrivate::~TokenGeneratorPrivate(void) {}

        TokenParameters * TokenGeneratorPrivate::params(void) const
        {
            return m_params;
        }

        otp::storage::OTPTokenType TokenGeneratorPrivate::type(void) const
        {
            return otp::storage::OTPTokenType::DummyHMAC;
        }

        bool TokenGeneratorPrivate::ensureStorage(void) const
        {
            return (bool) m_params;
        }

        bool TokenGeneratorPrivate::updateStorage(void)
        {
            return true;
        }

        bool TokenGeneratorPrivate::message(otp::token::Message&) const
        {
            return false;
        }

        bool TokenGeneratorPrivate::secret(const otp::storage::secrets::SecretsAPIProvider::SecretAnswer& answer) const
        {
            return m_params->askSecret(answer);
        }

        bool TokenGeneratorPrivate::key(otp::token::Key& key) const
        {
            auto p = params();
            EncodingType type;
            if(p && p->secretEncodingType(type))
            {
                switch(type)
                {
                    case EncodingType::Unknown:
                    case EncodingType::Base32:
                        key = otp::token::keyForBase32();
                        return true;
                    case EncodingType::Text:
                        return forCodec(key);
                    default:
                        return false;
                }
            }
            return false;
        }

        bool TokenGeneratorPrivate::encoder(otp::token::Encoder&) const
        {
            return false;
        }

        bool TokenGeneratorPrivate::algorithm(otp::token::Algorithm&) const
        {
            return false;
        }

        bool TokenGeneratorPrivate::forCodec(otp::token::Key& key) const
        {
            QTextCodec * codec = nullptr;
            auto p = params();
            if(p && p->secretEncoding(&codec))
            {
                key = otp::token::Key(otp::token::keyForCodec(codec));
                return true;
            }
            return false;
        }
    }
}
