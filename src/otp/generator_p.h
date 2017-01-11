#ifndef FACTORKEY_GENERATOR_PRIVATE_H
#define FACTORKEY_GENERATOR_PRIVATE_H

#include "generator.h"
#include "token/token.h"

namespace otp
{
    namespace generator
    {
        class TokenParametersPrivate
        {
        public:
            TokenParametersPrivate(otp::storage::Storage * store);
            virtual ~TokenParametersPrivate();
            bool setCodec(const QString& key, const QTextCodec * codec);
            bool lookupCodec(const QString& key, QTextCodec ** codec) const;
            bool lookupSecret(const otp::storage::secrets::SecretsAPIProvider::SecretAnswer& secret) const;
            bool writeSecret(const QString & secret, const otp::storage::secrets::SecretsAPIProvider::SecretConfirmation& confirmation);
            bool lookup(const QString & key, QVariant & value) const;
            otp::storage::Storage * storage(void) const;
        private:
            otp::storage::Storage * const m_storage;
        };

        class TokenGeneratorPrivate
        {
        public:
            TokenGeneratorPrivate(TokenParameters * p);
            virtual ~TokenGeneratorPrivate(void);
            TokenParameters * params(void) const;
            virtual otp::storage::OTPTokenType type(void) const;
            bool ensureStorage(void) const;
            virtual bool updateStorage(void);
            virtual bool message(otp::token::Message&) const;
            bool secret(const otp::storage::secrets::SecretsAPIProvider::SecretAnswer& secret) const;
            virtual bool key(otp::token::Key& key) const;
            virtual bool encoder(otp::token::Encoder&) const;
            virtual bool algorithm(otp::token::Algorithm& algo) const;
        protected:
            bool forCodec(otp::token::Key& key) const;

        private:
            TokenParameters * m_params;
        };

        namespace internal
        {
            template<typename T>
            inline T convertToNumeric(const QVariant& var, bool * ok);

            template<>
            inline quint64 convertToNumeric<quint64>(const QVariant& var, bool * ok)
            {
                return var.toULongLong(ok);
            }

            template<>
            inline qint64 convertToNumeric<qint64>(const QVariant& var, bool * ok)
            {
                return var.toLongLong(ok);
            }

            template<>
            inline int convertToNumeric<int>(const QVariant& var, bool * ok)
            {
                return var.toInt(ok);
            }

            template<>
            inline uint convertToNumeric<uint>(const QVariant& var, bool * ok)
            {
                return var.toUInt(ok);
            }

            template<typename T>
            inline bool lookupNumericValue(const TokenParametersPrivate * p, const QString& key, T& value, const T& defaultValue = 0)
            {
                QVariant val;
                if(p && p->lookup(key, val))
                {
                    if(val.isNull())
                    {
                        value = defaultValue;
                        return true;
                    }
                    else
                    {
                        bool ok = false;
                        auto v = convertToNumeric<T>(val, &ok);
                        if(ok)
                        {
                            value = v;
                            return true;
                        }
                    }
                }
                return false;
            }
        }
    }
}
#endif
