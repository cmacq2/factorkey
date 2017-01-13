#include "generator.h"
#include "generator_p.h"

#include "token/token.h"
#include "parameters.h"

#include <QHash>
#include <QMutex>

namespace otp
{
    namespace generator
    {
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
            if(d->lookup(otp::parameters::key::NAME, str))
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
                d->storage()->writeParam(otp::parameters::key::NAME, QVariant(name));
                return true;
            }
            return false;
        }

        bool TokenParameters::askSecret(void) const
        {
            return askSecret([this](bool ok, const QString& entryId, const QString& secret) -> void
            {
                if(ok)
                {
                    emit retrievedSecret(ok, entryId, secret);
                }
                else
                {
                    emit retrievedSecret(false, entryId, QString());
                }
            });
        }

        bool TokenParameters::askSecret(const otp::secrets::SecretsAPIProvider::SecretAnswer& answer) const
        {
            Q_D(const TokenParameters);
            return d->lookupSecret(answer);
        }

        bool TokenParameters::setSecret(const QString & secret)
        {
            Q_D(TokenParameters);
            return d->writeSecret(secret, [this](bool ok, const QString& entryId) -> void
            {
                emit secretUpdated(ok, entryId);
            });
        }

        bool TokenParameters::secretEncodingType(EncodingType& type)
        {
            Q_D(TokenParameters);
            int v;
            if(otp::generator::internal::lookupNumericValue<int>(d, otp::parameters::key::ENCODING, v, (int) EncodingType::Unknown))
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
            return d->storage()->writeParam(otp::parameters::key::ENCODING, QVariant((int) type));
        }

        bool TokenParameters::secretEncoding(QTextCodec ** codec) const
        {
            Q_D(const TokenParameters);
            return d->lookupCodec(otp::parameters::key::CHARSET, codec);
        }

        bool TokenParameters::setSecretEncoding(const QTextCodec * codec)
        {
            Q_D(TokenParameters);
            return d->setCodec(otp::parameters::key::CHARSET, codec);
        }

        bool GenericTokenParameters::hashAlgorithm(QCryptographicHash::Algorithm& hash) const
        {
            Q_D(const TokenParameters);
            int v;
            if(otp::generator::internal::lookupNumericValue<int>(d, otp::parameters::hashing::ALGORITHM, v, (int) QCryptographicHash::Sha1))
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
            return d->storage()->writeParam(otp::parameters::hashing::ALGORITHM, QVariant((int) hash));
        }

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

        bool TokenGenerator::generateToken(void)
        {
            return generateToken([this](bool ok, const QString& token) -> void
            {
                emit tokenGenerated(ok, token);
            });
        }

        bool TokenGenerator::generateToken(QString& token)
        {
            bool finished = false;
            bool started = generateToken([this, &token, &finished](bool ok, const QString& result) -> void
            {
                if(ok)
                {
                    finished = ok;
                    token = result;
                }
            });
            return started && finished;
        }

        bool TokenGenerator::generateToken(const TokenResult result)
        {
            Q_D(TokenGenerator);

            otp::token::Key key;
            otp::token::Encoder encoder;
            otp::token::Message message;
            otp::token::Algorithm algo;

            if(d && d->ensureStorage() &&  d->message(message) && d->key(key) && d->algorithm(algo) && d->encoder(encoder))
            {
                return d->secret([this, d, key, encoder, message, algo, result](bool ok, const QString&, const QString& secret) -> void
                {
                    if(ok)
                    {
                        const QString token = otp::token::token(secret, message, key, algo, encoder);
                        if(d->updateStorage())
                        {
                            result(true, token);
                            return;
                        }
                    }
                    result(false, QString());
                });
            }
            return false;
        }
    }
}
