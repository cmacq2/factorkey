#include "generator.h"
#include "../generator_p.h"
#include "skey.h"
#include "token.h"
#include "parameters.h"

#include <QMutex>
#include <QScopedPointer>

namespace otp
{
    namespace skey
    {
        namespace generator
        {

            const QString SKeyTokenParameters::DEFAULT_DICTIONARY_VALUE = QLatin1String("otp.skey.default.dictionary");

            DictionaryProvider::~DictionaryProvider() {}

            class DefaultDictionaryProvider: public DictionaryProvider
            {
            public:
                otp::skey::SKeyDictionary * dictionary(const QString& name) const
                {
                    return name == SKeyTokenParameters::DEFAULT_DICTIONARY_VALUE ? init() : nullptr;
                }
            private:
                static otp::skey::SKeyDictionary * init(void)
                {
                    static QMutex m;
                    static QScopedPointer<otp::skey::DefaultDictionary> DEFAULT_DICT(nullptr);
                    QMutexLocker lock(&m);
                    if(!DEFAULT_DICT)
                    {
                        DEFAULT_DICT.reset(otp::skey::defaultDictionary());
                    }
                    return DEFAULT_DICT.data();
                };
            };

            class SKeyTokenParametersPrivate: public otp::generator::TokenParametersPrivate
            {
            public:
                SKeyTokenParametersPrivate(otp::storage::Storage * store): otp::generator::TokenParametersPrivate(store) {}
                ~SKeyTokenParametersPrivate() {}

                otp::skey::SKeyDictionary * lookupDictionary(const QString& name) const
                {
                    otp::skey::SKeyDictionary * result = nullptr;

                    DictionaryProvider * cfgProvider = nullptr;
                    if(dictionaryProvider(&cfgProvider) && cfgProvider)
                    {
                        result = cfgProvider->dictionary(name);
                    }

                    if(!result)
                    {
                        result = m_defaultProvider.dictionary(name);
                    }
                    return result;
                }

                bool dictionaryProvider(DictionaryProvider ** provider = nullptr) const
                {
                    if(provider)
                    {
                        *provider = m_dictProvider;
                        return true;
                    }
                    return false;
                }

                bool setDictionaryProvider(DictionaryProvider * provider = nullptr)
                {
                    m_dictProvider = provider;
                    return true;
                }

                bool challenge(QString& value) const
                {
                    QString result;
                    if(m_challengeItems == 0x7 && otp::skey::formatSKeyChallenge(m_challengeAlgo, m_challengeSeedMessage, m_challengeRounds, result))
                    {
                        value = result;
                        return true;
                    }
                    return false;
                }

                bool setChallenge(const QString& challenge)
                {
                    QCryptographicHash::Algorithm h;
                    quint64 rounds;
                    QString seed;
                    if(!challenge.isNull() && !challenge.isEmpty() && otp::skey::parseSKeyChallenge(challenge, h, seed, rounds))
                    {
                        m_challengeAlgo = h;
                        m_challengeRounds = rounds;
                        m_challengeSeedMessage = seed;
                        m_challengeItems = 0x7;
                        return true;
                    }
                    return false;
                }

                bool setChallengeAlgorithm(const QCryptographicHash::Algorithm& algorithm)
                {
                    switch(algorithm)
                    {
                        case QCryptographicHash::Sha1:
                        case QCryptographicHash::Md4:
                        case QCryptographicHash::Md5:
                            m_challengeAlgo = algorithm;
                            m_challengeItems |= 0x1;
                            return true;
                        default:
                            return false;
                    }
                }

                bool challengeAlgorithm(QCryptographicHash::Algorithm& h) const
                {
                    if(m_challengeItems & 0x1)
                    {
                        h = m_challengeAlgo;
                        return true;
                    }
                    return false;
                }

                bool challengeSeed(QString& s) const
                {
                    if(m_challengeItems & 0x2)
                    {
                        s = m_challengeSeedMessage;
                        return true;
                    }
                    return false;
                }

                bool setChallengeSeed(const QString& seed)
                {
                    switch(otp::skey::validateSKeySeed(seed))
                    {
                        case otp::skey::SKeySeedError::Conformant:
                        case otp::skey::SKeySeedError::Acceptable:
                            m_challengeSeedMessage = seed;
                            m_challengeItems |= 0x2;
                            return true;
                        default:
                            return false;
                    }
                }

                bool challengeRounds(quint64& r) const
                {
                    if(m_challengeItems & 0x4)
                    {
                        r = m_challengeRounds;
                        return true;
                    }
                    return false;
                }

                bool setChallengeRounds(const quint64& rounds)
                {
                    m_challengeRounds = rounds;
                    m_challengeItems |= 0x4;
                    return true;
                }

            private:
                QCryptographicHash::Algorithm m_challengeAlgo;
                QString m_challengeSeedMessage;
                quint64 m_challengeRounds = 0U;
                DictionaryProvider * m_dictProvider = nullptr;
                char m_challengeItems = 0;
            private:
                DefaultDictionaryProvider m_defaultProvider;
            };

            SKeyTokenParameters::SKeyTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : otp::generator::TokenParameters(d, parent) {}
            SKeyTokenParameters::~SKeyTokenParameters() {}

            const bool SKeyTokenParameters::isRegistered = otp::generator::TokenParameters::registerType(otp::storage::OTPTokenType::SKey, create);

            SKeyTokenParameters * SKeyTokenParameters::create(otp::storage::Storage * store, QObject * parent)
            {
                return isRegistered && store && store->type() == otp::storage::OTPTokenType::SKey ? new SKeyTokenParameters(new SKeyTokenParametersPrivate(store), parent) : nullptr;
            }

            bool SKeyTokenParameters::setSKeyEncoding(const SKeyEncodingType& value)
            {
                Q_D(otp::generator::TokenParameters);
                return d->storage()->writeParam(otp::skey::parameters::ENCODING, QVariant((int) value));
            }

            bool SKeyTokenParameters::sKeyEncoding(SKeyEncodingType& value) const
            {
                Q_D(const otp::generator::TokenParameters);
                int v;
                if(otp::generator::internal::lookupNumericValue<int>(d, otp::skey::parameters::ENCODING, v, (int) SKeyEncodingType::Words))
                {
                    switch((SKeyEncodingType) v)
                    {
                        case SKeyEncodingType::Words:
                        case SKeyEncodingType::Hex:
                            value = (SKeyEncodingType) v;
                            return true;
                        default:
                            return false;
                    }
                }
                return false;
            }

            bool SKeyTokenParameters::challenge(QString& value) const
            {
                Q_D(const otp::generator::TokenParameters);
                if(d)
                {
                    const SKeyTokenParametersPrivate * const p = static_cast<const SKeyTokenParametersPrivate*>(d);
                    return p && p->challenge(value);
                }
                return false;
            }

            bool SKeyTokenParameters::setChallenge(const QString& value)
            {
                Q_D(otp::generator::TokenParameters);
                if(d)
                {
                    SKeyTokenParametersPrivate * const p = static_cast<SKeyTokenParametersPrivate*>(d);
                    return p && p->setChallenge(value);
                }
                return false;
            }

            bool SKeyTokenParameters::rounds(quint64& value) const
            {
                Q_D(const otp::generator::TokenParameters);
                if(d)
                {
                    const SKeyTokenParametersPrivate * const p = static_cast<const SKeyTokenParametersPrivate*>(d);
                    return p && p->challengeRounds(value);
                }
                return false;
            }

            bool SKeyTokenParameters::setRounds(quint64 value)
            {
                Q_D(otp::generator::TokenParameters);
                if(d)
                {
                    SKeyTokenParametersPrivate * const p = static_cast<SKeyTokenParametersPrivate*>(d);
                    return p && p->setChallengeRounds(value);
                }
                return false;
            }

            bool SKeyTokenParameters::seed(QString& value) const
            {
                Q_D(const otp::generator::TokenParameters);
                if(d)
                {
                    const SKeyTokenParametersPrivate * const p = static_cast<const SKeyTokenParametersPrivate*>(d);
                    return p && p->challengeSeed(value);
                }
                return false;
            }

            bool SKeyTokenParameters::setSeed(const QString& value)
            {
                Q_D(otp::generator::TokenParameters);
                if(d)
                {
                    SKeyTokenParametersPrivate * const p = static_cast<SKeyTokenParametersPrivate*>(d);
                    return p && p->setChallengeSeed(value);
                }
                return false;
            }

            bool SKeyTokenParameters::setHashAlgorithm(const QCryptographicHash::Algorithm& hash)
            {
                Q_D(otp::generator::TokenParameters);
                if(d)
                {
                    SKeyTokenParametersPrivate * const p = static_cast<SKeyTokenParametersPrivate*>(d);
                    return p && p->setChallengeAlgorithm(hash);
                }
                return false;
            }

            bool SKeyTokenParameters::hashAlgorithm(QCryptographicHash::Algorithm& hash) const
            {
                Q_D(const otp::generator::TokenParameters);
                if(d)
                {
                    const SKeyTokenParametersPrivate * const p = static_cast<const SKeyTokenParametersPrivate*>(d);
                    return p && p->challengeAlgorithm(hash);
                }
                return false;
            }

            bool SKeyTokenParameters::dictionaryProvider(DictionaryProvider ** provider) const
            {
                Q_D(const otp::generator::TokenParameters);
                if(d)
                {
                    const SKeyTokenParametersPrivate * const p = static_cast<const SKeyTokenParametersPrivate*>(d);
                    return p && p->dictionaryProvider(provider);
                }
                return false;
            }

            bool SKeyTokenParameters::setDictionaryProvider(DictionaryProvider * provider)
            {
                Q_D(otp::generator::TokenParameters);
                if(d)
                {
                    SKeyTokenParametersPrivate * const p = static_cast<SKeyTokenParametersPrivate*>(d);
                    return p && p->setDictionaryProvider(provider);
                }
                return false;
            }

            bool SKeyTokenParameters::setDictionaryName(const QString& name)
            {
                Q_D(otp::generator::TokenParameters);
                return d && d->storage()->writeParam(otp::skey::parameters::DICTIONARY, QVariant(name));
            }

            bool SKeyTokenParameters::dictionaryName(QString& name) const
            {
                Q_D(const otp::generator::TokenParameters);
                QVariant value;
                if(d && d->lookup(otp::skey::parameters::DICTIONARY, value))
                {
                    if(value.isNull())
                    {
                        name = DEFAULT_DICTIONARY_VALUE;
                        return true;
                    }
                    else
                    {
                        name = value.toString();
                        return true;
                    }
                }
                return false;
            }

            bool SKeyTokenParameters::dictionary(otp::skey::SKeyDictionary ** dict) const
            {
                Q_D(const otp::generator::TokenParameters);
                QString name;
                otp::skey::SKeyDictionary * result = nullptr;
                if(d)
                {
                    const SKeyTokenParametersPrivate * const p = static_cast<const SKeyTokenParametersPrivate*>(d);
                    if(p && dictionaryName(name))
                    {
                        result = p->lookupDictionary(name);
                        if(result)
                        {
                            *dict = result;
                            return true;
                        }
                    }
                }
                return false;
            }

            class SKeyTokenGeneratorPrivate: public otp::generator::TokenGeneratorPrivate
            {
            public:
                SKeyTokenGeneratorPrivate(SKeyTokenParameters * p) : otp::generator::TokenGeneratorPrivate(p) {}

                bool algorithm(otp::token::Algorithm& algo) const
                {
                    auto p = qobject_cast<otp::skey::generator::SKeyTokenParameters *>(params());
                    QCryptographicHash::Algorithm h;
                    quint64 r;
                    if(p && p->rounds(r) && p->hashAlgorithm(h))
                    {
                        algo = otp::skey::token::sKeyAlgorithm(r, h);
                        return true;
                    }
                    return false;
                }

                bool encoder(otp::token::Encoder& encoder) const
                {
                    auto p = qobject_cast<otp::skey::generator::SKeyTokenParameters *>(params());
                    SKeyEncodingType type;
                    QCryptographicHash::Algorithm h;
                    if(p && p->sKeyEncoding(type) && p->hashAlgorithm(h))
                    {
                        otp::skey::SKeyDictionary * dict = nullptr;
                        switch(type)
                        {
                            case SKeyEncodingType::Hex:
                                encoder = otp::skey::token::hexEncoder(h);
                                return true;
                            case SKeyEncodingType::Words:
                                if(p->dictionary(&dict) && dict)
                                {
                                    encoder = otp::skey::token::wordsEncoder(h, dict);
                                    return true;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    return false;
                }

                bool message(otp::token::Message& message) const
                {
                    auto p = qobject_cast<otp::skey::generator::SKeyTokenParameters *>(params());
                    QString v;
                    if(p && p->seed(v))
                    {
                        message = otp::token::Message([v](void) -> QByteArray
                        {
                            return otp::skey::sKeySeedMessage(v);
                        });
                        return true;
                    }
                    return false;
                }
            };

            otp::generator::TokenGenerator * SKeyTokenParameters::generator(SKeyTokenParameters * params, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new SKeyTokenGeneratorPrivate(params), parent);
            }
        }
    }
}
