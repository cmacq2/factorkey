#include "generator.h"
#include "generator_p.h"

#include "token/token.h"

#include "dummy/generator.h"
#include "oath/generator.h"
#include "steam/generator.h"
#include "skey/generator.h"

#include <QHash>
#include <QMutex>

namespace otp
{
    namespace generator
    {
        const QString TokenParameters::OTP_TOKEN_TYPE = QLatin1String("otp.token.type");
        const QString TokenParameters::OTP_TOKEN_NAME = QLatin1String("otp.token.name");
        const QString TokenParameters::OTP_KEY_ENCODING_CHARSET = QLatin1String("otp.key.charset");
        const QString TokenParameters::OTP_KEY_ENCODING_TYPE = QLatin1String("otp.key.type");
        TokenParameters::TokenParameters(TokenParametersPrivate * d, QObject * parent) : QObject(parent), d_ptr(d) {}
        TokenParameters::~TokenParameters() {}

        otp::storage::OTPTokenType TokenParameters::type(void) const
        {
            Q_D(const TokenParameters);
            return d->storage()->type();
        }

        bool TokenParameters::resync(void)
        {
            Q_D(TokenParameters);
            return d->storage()->poll();
        }

        bool TokenParameters::commit(void)
        {
            Q_D(TokenParameters);
            return d->storage()->commit();
        }

        QString TokenParameters::id(void) const
        {
            Q_D(const TokenParameters);
            return d->storage()->entryId();
        }

        bool TokenParameters::name(QString & name) const
        {
            Q_D(const TokenParameters);
            QVariant str;
            if(d->lookup(OTP_TOKEN_NAME, str))
            {
                name  = str.isNull() ? QLatin1String("") : str.toString();
                return true;
            }
            return false;
        }

        bool TokenParameters::setName(const QString& name)
        {
            Q_D(TokenParameters);
            if(!name.isNull())
            {
                d->storage()->writeParam(OTP_TOKEN_NAME, QVariant(name));
                return true;
            }
            return false;
        }

        bool TokenParameters::secret(QString & secret) const
        {
            Q_D(const TokenParameters);
            return d->lookupSecret(secret);
        }

        bool TokenParameters::setSecret(const QString & secret)
        {
            Q_D(TokenParameters);
            return d->writeSecret(secret);
        }

        bool TokenParameters::secretEncodingType(EncodingType& type)
        {
            Q_D(TokenParameters);
            int v;
            if(otp::generator::internal::lookupNumericValue<int>(d, OTP_KEY_ENCODING_TYPE, v, (int) EncodingType::Unknown))
            {
                switch((EncodingType) v)
                {
                    case EncodingType::Text:
                    case EncodingType::Base32:
                    case EncodingType::Unknown:
                        type = (EncodingType) v;
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        bool TokenParameters::setSecretEncodingType(const EncodingType& type)
        {
            Q_D(TokenParameters);
            return d->storage()->writeParam(OTP_KEY_ENCODING_TYPE, QVariant((int) type));
        }

        bool TokenParameters::secretEncoding(QTextCodec ** codec) const
        {
            Q_D(const TokenParameters);
            return d->lookupCodec(OTP_KEY_ENCODING_CHARSET, codec);
        }

        bool TokenParameters::setSecretEncoding(const QTextCodec * codec)
        {
            Q_D(TokenParameters);
            return d->setCodec(OTP_KEY_ENCODING_CHARSET, codec);
        }

        bool GenericTokenParameters::hashAlgorithm(QCryptographicHash::Algorithm& hash) const
        {
            Q_D(const TokenParameters);
            int v;
            if(otp::generator::internal::lookupNumericValue<int>(d, OTP_HMAC_HASH_ALGORITHM, v, (int) QCryptographicHash::Sha1))
            {
                switch((QCryptographicHash::Algorithm) v)
                {
                    case QCryptographicHash::Md4:
                    case QCryptographicHash::Md5:
                    case QCryptographicHash::Sha1:
                    case QCryptographicHash::Sha224:
                    case QCryptographicHash::Sha256:
                    case QCryptographicHash::Sha384:
                    case QCryptographicHash::Sha512:
                    case QCryptographicHash::Sha3_224:
                    case QCryptographicHash::Sha3_256:
                    case QCryptographicHash::Sha3_384:
                    case QCryptographicHash::Sha3_512:
                        hash = (QCryptographicHash::Algorithm) v;
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        bool GenericTokenParameters::setHashAlgorithm(const QCryptographicHash::Algorithm& hash)
        {
            Q_D(TokenParameters);
            return d->storage()->writeParam(OTP_HMAC_HASH_ALGORITHM, QVariant((int) hash));
        }

        const QString GenericTokenParameters::OTP_HMAC_HASH_ALGORITHM = QLatin1String("otp.hmac.hash");
        GenericTokenParameters::GenericTokenParameters(TokenParametersPrivate * d, QObject * parent) : TokenParameters(d, parent) {}
        GenericTokenParameters::~GenericTokenParameters() {}

        static QHash<int,TokenParameters::ConstructorFunction> paramCtorMap;
        bool TokenParameters::registerType(otp::storage::OTPTokenType type, const ConstructorFunction& ctor)
        {
            static QMutex m;
            QMutexLocker lock(&m);

            int t = (int) type;
            if(paramCtorMap.contains(t))
            {
                return false;
            }
            else
            {
                paramCtorMap.insert(t, ctor);
                return true;
            }
        }

        TokenParameters * TokenParameters::create(otp::storage::Storage * store, QObject * parent)
        {
            if(!store)
            {
                return nullptr;
            }

            const int t = (int) store->type();
            if(!paramCtorMap.contains(t))
            {
                return nullptr;
            }
            else
            {
                auto ctor = paramCtorMap.value(t);
                return ctor ? ctor(store, parent) : nullptr;
            }
        }

        TokenGenerator::TokenGenerator(TokenGeneratorPrivate * impl, QObject * parent): QObject(parent), d_ptr(impl) {}
        TokenGenerator::~TokenGenerator() {}

        TokenParameters * TokenGenerator::params(void) const
        {
            Q_D(const TokenGenerator);
            return d ? d->params() : nullptr;
        }

        bool TokenGenerator::generateToken(QString& value)
        {
            Q_D(TokenGenerator);
            QString token;
            QString secret;
            otp::token::Key key;
            otp::token::Encoder encoder;
            otp::token::Message message;
            otp::token::Algorithm algo;

            if(d && d->ensureStorage() && d->secret(secret) &&  d->message(message) &&
                d->key(key) && d->algorithm(algo) && d->encoder(encoder))
            {
                token = otp::token::token(secret, message, key, algo, encoder);
                if(d->updateStorage())
                {
                    value = token;
                    return true;
                }
            }

            return false;
        }
    }
}
