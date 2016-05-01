#include "generator.h"
#include "token.h"

namespace otp
{

    static QString localeToString(const QLocale& locale)
    {
        auto lang = locale.language();
        if(lang == QLocale::C)
        {
            return QLatin1String("C");
        }
        else
        {
            QString aName(QLatin1String(""));
            QString result;
            if(lang != QLocale::AnyLanguage)
            {
                aName += QLocale::languageToString(lang);
                result = aName;
            }
            auto script = locale.script();
            if(script != QLocale::AnyScript)
            {
                if(!aName.isEmpty()) aName += QLatin1Char('_');
                aName += QLocale::scriptToString(script);
                result = aName;
            }
            auto country = locale.country();
            if(country != QLocale::AnyCountry)
            {
                if(!aName.isEmpty()) aName += QLatin1Char('_');
                aName += QLocale::countryToString(country);
                result = aName;
            }
            return result;
        }
    }

//     static QString typeToString(OTPTokenType type)
//     {
//         auto l  = QLocale::c();
//         return l.toString((int) type);
//     }

//     static bool typeOf(const QMap<QString, QString>& params, OTPTokenType& type);

    class TokenParametersPrivate
    {
    public:
        TokenParametersPrivate(Storage * store): m_storage(store) {}
//         TokenParametersPrivate(OTPTokenType type, const QString& entryId, Storage * store):
//         m_type(type), m_entryId(entryId), m_storage(store)
//         {
//             set(Storage::OTP_TOKEN_TYPE, typeToString(type));
//         }

//         TokenParametersPrivate(OTPTokenType type, const QString& entryId, Storage * store, QMap<QString,QString>& params):
//             m_type(type), m_entryId(entryId), m_storage(store), m_params(params)
//         {
//             set(Storage::OTP_TOKEN_TYPE, typeToString(type));
//             m_sync = true;
//         }

    public:

//         static TokenParametersPrivate * createNew(OTPTokenType type, const QString& entryId, Storage * store)
//         {
//             if(entryId.isNull() || entryId.isEmpty() || !store || store->contains(entryId))
//             {
//                 return nullptr;
//             }
//             return new TokenParametersPrivate(type, entryId, store);
//         }
//
//         static TokenParametersPrivate * createNew(OTPTokenType type, Storage * store)
//         {
//             if(!store)
//             {
//                 return nullptr;
//             }
//
//             QString entryId;
//             do {
//                 auto uuid = QUuid::createUuid();
//                 entryId = uuid.toString();
//             }
//             while (store->contains(entryId));
//
//             return new TokenParametersPrivate(type, entryId, store);
//         }
//
//         static TokenParametersPrivate * loadExisting(const QString& entryId, Storage * store)
//         {
//             if(entryId.isNull() || entryId.isEmpty())
//             {
//                 return nullptr;
//             }
//
//             QMap<QString,QString> params;
//             OTPTokenType type;
//             if(store && store->readParams(entryId, params) && typeOf(params, type))
//             {
//                 return new TokenParametersPrivate(type, entryId, store, params);
//             }
//
//             return nullptr;
//         }

    public:
        virtual ~TokenParametersPrivate() {}

//         OTPTokenType type(void) const
//         {
//             return m_storage->type();
//         }

        bool setCodec(const QString& key, const QTextCodec * codec)
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

        bool lookupCodec(const QString& key, QTextCodec ** codec) const
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
                    QTextCodec * cdc = QTextCodec::codecForName(val.toString().toUtf8());
                    if(cdc)
                    {
                        *codec = cdc;
                        return true;
                    }
                }
            }
            return false;
        }

        bool lookupSecret(QString & value) const
        {
            QString str;
            if(m_storage->readPassword(str)  && !str.isNull() && !str.isEmpty())
            {
                value = str;
                return true;
            }
            return false;
        }

        bool writeSecret(const QString & secret)
        {
            return !secret.isNull() && !secret.isEmpty() && m_storage->writePassword(secret);
        }

        bool lookup(const QString & key, QVariant & value) const
        {
            QVariant v;
            if(m_storage->readParam(key, v))
            {
                value = v;
                return true;
            }
            return false;
        }

//         QString id(void) const
//         {
//             return m_storage->entryId(); //m_entryId;
//         }

        Storage * storage(void) const
        {
            return m_storage;
        }

//         bool set(const QString& key, const QVariant& value)
//         {
//             return m_storage->writeParam(key, value);
//             //m_params.insert(key, value);
//         }
//
//         bool resync(void)
//         {
//             return m_storage->poll();
//         }
//
//         bool commitParams()
//         {
//             return m_storage->commit();
//         }
        /*
            m_sync = false;
            QMap<QString,QString> params;
            for(const QString& k: names)
            {
                if(m_params.contains(k))
                {
                    params.insert(k, m_params.value(k));
                }
            }
            return m_storage->writeParams(id(), m_params);
        }

        bool readParams(void)
        {
            if(!m_sync)
            {
                m_sync = m_storage->readParams(id(), m_params);
            }
            return m_sync;
        }

        bool resync(void)
        {
            m_sync = false;
            return readParams();
        }*/

    private:
        Storage * const m_storage;
    };

    template<typename T>
    static T convertToNumeric(const QVariant& var, bool * ok);

    template<>
    quint64 convertToNumeric<quint64>(const QVariant& var, bool * ok)
    {
        return var.toULongLong(ok);
    }

    template<>
    qint64 convertToNumeric<qint64>(const QVariant& var, bool * ok)
    {
        return var.toLongLong(ok);
    }

    template<>
    int convertToNumeric<int>(const QVariant& var, bool * ok)
    {
        return var.toInt(ok);
    }

    template<>
    uint convertToNumeric<uint>(const QVariant& var, bool * ok)
    {
        return var.toUInt(ok);
    }
/*
    static bool typeOf(const QMap<QString, QString>& params, OTPTokenType& type)
    {
        if(params.contains(Storage::OTP_TOKEN_TYPE))
        {
            bool ok = false;
            auto v = convertToNumeric<int>(Storage::OTP_TOKEN_TYPE, &ok);
            if(ok)
            {
                switch((OTPTokenType) v)
                {
                    case HOTP:
                    case TOTP:
                    case SteamGuard:
                    case DummyHMAC:
                        type = (OTPTokenType) v;
                        return true;
                    default:
                        return false;
                }
            }
        }
        return false;
    }*/

    template<typename T>
    static bool lookupNumericValue(const TokenParametersPrivate * p, const QString& key, T& value, const T& defaultValue = 0)
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

    TokenParameters::TokenParameters(TokenParametersPrivate * d, QObject * parent) : QObject(parent), d_ptr(d) {}
    TokenParameters::~TokenParameters() {}

    OTPTokenType TokenParameters::type(void) const
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
        if(d->lookup(Storage::OTP_TOKEN_NAME, str))
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
            d->storage()->writeParam(Storage::OTP_TOKEN_NAME, QVariant(name));
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

    bool GenericTokenParameters::secretEncodingType(EncodingType& type)
    {
        Q_D(TokenParameters);
        int v;
        if(lookupNumericValue<int>(d, Storage::OTP_KEY_ENCODING_TYPE, v, (int) EncodingType::Unknown))
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

    bool GenericTokenParameters::setSecretEncodingType(const EncodingType& type)
    {
        Q_D(TokenParameters);
        return d->storage()->writeParam(Storage::OTP_KEY_ENCODING_TYPE, QVariant((int) type));
    }

    bool GenericTokenParameters::secretEncoding(QTextCodec ** codec) const
    {
        Q_D(const TokenParameters);
        return d->lookupCodec(Storage::OTP_KEY_ENCODING_CHARSET, codec);
    }

    bool GenericTokenParameters::setSecretEncoding(const QTextCodec * codec)
    {
        Q_D(TokenParameters);
        return d->setCodec(Storage::OTP_KEY_ENCODING_CHARSET, codec);
    }

    bool GenericTokenParameters::hashAlgorithm(QCryptographicHash::Algorithm& hash) const
    {
        Q_D(const TokenParameters);
        int v;
        if(lookupNumericValue<int>(d, Storage::OTP_HMAC_HASH_ALGORITHM, v, (int) QCryptographicHash::Sha1))
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
        return d->storage()->writeParam(Storage::OTP_HMAC_HASH_ALGORITHM, QVariant((int) hash));
    }

    GenericTokenParameters::GenericTokenParameters(TokenParametersPrivate * d, QObject * parent) : TokenParameters(d, parent) {}
    GenericTokenParameters::~GenericTokenParameters() {}

    bool GenericOTPParameters::tokenLocale(QLocale& locale) const
    {
        Q_D(const TokenParameters);
        QVariant val;
        if(d->lookup(Storage::OTP_ENCODER_TOKEN_LOCALE, val))
        {
            if(val.isNull())
            {
                locale = QLocale::c();
                return true;
            }
            else
            {
                const QString str = val.toString();
                QLocale l(str);
                if(localeToString(l) == str)
                {
                    locale = l;
                    return true;
                }
            }
        }
        return false;
    }

    bool GenericOTPParameters::setTokenLocale(const QLocale& locale)
    {
        Q_D(TokenParameters);
        d->storage()->writeParam(Storage::OTP_ENCODER_TOKEN_LOCALE, QVariant(localeToString(locale)));
        return true;
    }

    bool GenericOTPParameters::tokenLength(uint& length) const
    {
        Q_D(const TokenParameters);
        return lookupNumericValue<uint>(d, Storage::OTP_ENCODER_TOKEN_LENGTH, length, DEFAULT_OTP_LENGTH);
    }

    bool GenericOTPParameters::setTokenLength(uint length)
    {
        Q_D(TokenParameters);
        return d->storage()->writeParam(Storage::OTP_ENCODER_TOKEN_LENGTH, QVariant(length));
    }

    GenericOTPParameters::GenericOTPParameters(TokenParametersPrivate * d, QObject * parent) : GenericTokenParameters(d, parent) {}
    GenericOTPParameters::~GenericOTPParameters() {}

    class TokenGeneratorPrivate
    {
    public:
        TokenGeneratorPrivate(TokenParameters * p) : m_params(p) {}
        virtual ~TokenGeneratorPrivate(void) {}

        TokenParameters * params(void) const
        {
            return m_params;
        }

        virtual OTPTokenType type(void) const
        {
            return OTPTokenType::DummyHMAC;
        }

        bool ensureStorage(void) const
        {
            return (bool) m_params;
        }

        virtual bool updateStorage(void)
        {
            return m_params->commit();
        }

        virtual bool message(Message&) const
        {
            return false;
        }

        bool secret(QString& secret) const
        {
            return m_params->secret(secret);
        }

        virtual bool key(Key& key) const
        {
            GenericOTPParameters * p = qobject_cast<GenericOTPParameters *>(params());
            EncodingType type;
            if(p && p->secretEncodingType(type))
            {
                switch(type)
                {
                    case EncodingType::Unknown:
                    case EncodingType::Base32:
                        key = otp::keyForAuthenticator();
                        return true;
                    case EncodingType::Text:
                        return forCodec(key);
                    default:
                        return false;
                }
            }
            return false;
        }

        virtual bool locale(QLocale& locale) const
        {
            GenericOTPParameters * p = qobject_cast<GenericOTPParameters *>(params());
            return p ? p->tokenLocale(locale): false;
        }

        virtual bool encoder(Encoder& encoder) const
        {
            QLocale l;
            uint length;
            if(locale(l) && tokenLength(length))
            {
                encoder = Encoder(otpEncoder(length, l));
                return true;
            }
            return false;
        }

        virtual bool tokenLength(uint& value) const
        {
            GenericOTPParameters * p = qobject_cast<GenericOTPParameters *>(params());
            return p ? p->tokenLength(value): false;
        }

        bool algorithm(Algorithm& algo) const
        {
            QCryptographicHash::Algorithm h;
            if(hash(h))
            {
                algo = Algorithm(hmacAlgorithm(h));
                return true;
            }
            return false;
        }

        virtual bool hash(QCryptographicHash::Algorithm& hash) const
        {
            QCryptographicHash::Algorithm a;
            GenericTokenParameters * p = qobject_cast<GenericTokenParameters *>(params());
            if(p && p->hashAlgorithm(a))
            {
                hash = a;
                return true;
            }
            return false;
        }
    protected:
        bool forCodec(Key& key) const
        {
            QTextCodec * codec = nullptr;
            GenericTokenParameters * p = qobject_cast<GenericTokenParameters *>(params());
            if(p && p->secretEncoding(&codec))
            {
                key = Key(otp::keyForCodec(codec));
                return true;
            }
            return false;
        }
    private:
        TokenParameters * m_params;
    };

    DummyParameters::DummyParameters(TokenParametersPrivate * d, QObject * parent) : GenericTokenParameters(d, parent) {}
    DummyParameters::~DummyParameters() {}
    DummyParameters * DummyParameters::create(Storage * store, QObject * parent)
    {
        return store && store->type() == OTPTokenType::DummyHMAC ? new DummyParameters(new TokenParametersPrivate(store), parent) : nullptr;
    }

    bool DummyParameters::tokenMessage(QString & message) const
    {
        Q_D(const TokenParameters);
        QVariant str;
        if(d->lookup(Storage::DUMMY_TOKEN_MESSAGE_VALUE, str) && !str.isNull())
        {
            message = str.toString();
            return true;
        }
        return false;
    }

    bool DummyParameters::setTokenMessage(const QString& message)
    {
        Q_D(TokenParameters);
        if(message.isNull())
        {
            return false;
        }
        else
        {
            d->storage()->writeParam(Storage::DUMMY_TOKEN_MESSAGE_VALUE, QVariant(message));
            return true;
        }
    }

    bool DummyParameters::tokenMessageEncoding(QTextCodec ** codec) const
    {
        Q_D(const TokenParameters);
        return d->lookupCodec(Storage::DUMMY_TOKEN_MESSAGE_CHARSET, codec);
    }

    bool DummyParameters::setTokenMessageEncoding(const QTextCodec * codec)
    {
        Q_D(TokenParameters);
        return d->setCodec(Storage::DUMMY_TOKEN_MESSAGE_CHARSET, codec);
    }

    class DummyTokenGeneratorPrivate: public TokenGeneratorPrivate
    {
    public:
        DummyTokenGeneratorPrivate(DummyParameters * p) : TokenGeneratorPrivate(p) {}
        bool message(Message& message) const
        {
            DummyParameters * p = qobject_cast<DummyParameters *>(params());
            QString str;
            QTextCodec * codec = nullptr;
            if(p && p->tokenMessage(str) && p->tokenMessageEncoding(&codec))
            {
                message = codec ? dummyMessage(str, codec) : Message([str](void) -> QByteArray
                {
                    return str.toUtf8();
                });
                return true;
            }

            return false;
        }

        bool encoder(Encoder& encoder) const
        {
            encoder = Encoder(encodeDummyFormat);
            return true;
        }
    };

    HOTPTokenParameters::HOTPTokenParameters(TokenParametersPrivate * d, QObject * parent) : GenericOTPParameters(d, parent) {}
    HOTPTokenParameters::~HOTPTokenParameters() {}
    HOTPTokenParameters * HOTPTokenParameters::create(Storage * store, QObject * parent)
    {
        return store && store->type() == OTPTokenType::HOTP ? new HOTPTokenParameters(new TokenParametersPrivate(store), parent) : nullptr;
    }

    bool HOTPTokenParameters::tokenCounter(quint64 & count) const
    {
        Q_D(const TokenParameters);
        return lookupNumericValue<quint64>(d, Storage::HOTP_TOKEN_COUNTER, count);
    }

    bool HOTPTokenParameters::setTokenCounter(quint64 count)
    {
        Q_D(TokenParameters);
        return d->storage()->writeParam(Storage::HOTP_TOKEN_COUNTER, QVariant(count));
    }

    class HOTPTokenGeneratorPrivate: public TokenGeneratorPrivate
    {
    public:
        HOTPTokenGeneratorPrivate(HOTPTokenParameters * p) : TokenGeneratorPrivate(p) {}
        bool message(Message& message) const
        {
            quint64 count;
            HOTPTokenParameters * p = qobject_cast<HOTPTokenParameters *>(params());
            if(p && p->tokenCounter(count))
            {
                message = Message([count](void) -> QByteArray
                {
                    return hotpTokenMessage(count);
                });
                return true;
            }
            return false;
        }

        bool updateStorage(void)
        {
            quint64 count;
            HOTPTokenParameters * p = qobject_cast<HOTPTokenParameters *>(params());
            if(p && p->tokenCounter(count))
            {
                return (count + 1) > count && p->setTokenCounter(count + 1) && p->commit();
            }
            return false;
        }
    };

    TOTPTokenParameters::TOTPTokenParameters(TokenParametersPrivate * d, QObject * parent) : GenericOTPParameters(d, parent) {}
    TOTPTokenParameters::~TOTPTokenParameters() {}
    TOTPTokenParameters * TOTPTokenParameters::create(Storage * store, QObject * parent)
    {
        return store && store->type() == OTPTokenType::TOTP ? new TOTPTokenParameters(new TokenParametersPrivate(store), parent) : nullptr;
    }

    bool TOTPTokenParameters::tokenTimeStep(quint64 & timeStepMSec) const
    {
        Q_D(const TokenParameters);
        return lookupNumericValue<quint64>(d, Storage::TOTP_TOKEN_TIMESTEP, timeStepMSec, DEFAULT_TIMESTEP_MSEC);
    }

    bool TOTPTokenParameters::setTokenTimeStep(quint64 timeStepMSec)
    {
        Q_D(TokenParameters);
        return d->storage()->writeParam(Storage::TOTP_TOKEN_TIMESTEP, QVariant(timeStepMSec));
    }

    bool TOTPTokenParameters::tokenEpoch(qint64 & epoch) const
    {
        Q_D(const TokenParameters);
        return lookupNumericValue<qint64>(d, Storage::TOTP_TOKEN_EPOCH, epoch, DEFAULT_EPOCH);
    }

    bool TOTPTokenParameters::setTokenEpoch(const QDateTime& epoch)
    {
        return setTokenEpoch(epoch.toMSecsSinceEpoch());
    }

    bool TOTPTokenParameters::setTokenEpoch(qint64 unixTime)
    {
        Q_D(TokenParameters);
        return d->storage()->writeParam(Storage::TOTP_TOKEN_EPOCH, QVariant(unixTime));
    }

    class TOTPTokenGeneratorPrivate: public TokenGeneratorPrivate
    {
    public:
        TOTPTokenGeneratorPrivate(TOTPTokenParameters * p) : TokenGeneratorPrivate(p) {}
        bool message(Message& message) const
        {
            TOTPTokenParameters * p = qobject_cast<TOTPTokenParameters *>(params());
            qint64 epoch;
            quint64 ival;
            if(p && p->tokenEpoch(epoch) && p->tokenTimeStep(ival))
            {
                message = totpMessage(epoch, ival);
                return true;
            }

            return false;
        }
    };

    SteamGuardParameters::SteamGuardParameters(TokenParametersPrivate * d, QObject * parent) : TokenParameters(d, parent) {}
    SteamGuardParameters::~SteamGuardParameters() {}
    SteamGuardParameters * SteamGuardParameters::create(Storage * store, QObject * parent)
    {
        return store && store->type() == OTPTokenType::SteamGuard ? new SteamGuardParameters(new TokenParametersPrivate(store), parent) : nullptr;
    }

    class SteamGuardTokenGeneratorPrivate: public TokenGeneratorPrivate
    {
    public:
        SteamGuardTokenGeneratorPrivate(SteamGuardParameters * p) : TokenGeneratorPrivate(p) {}
        bool hash(QCryptographicHash::Algorithm& hash) const
        {
            hash = QCryptographicHash::Sha1;
            return true;
        }

        bool encoder(Encoder& encoder) const
        {
            QString(* func)(const QByteArray&) = otp::encodeSteamGuardToken;
            encoder = Encoder(func);
            return true;
        }

        bool message(Message& message) const
        {
            message = totpMessage(0, DEFAULT_TIMESTEP_MSEC);
            return true;
        }
    };

    TokenParameters * TokenParameters::create(Storage * store, QObject * parent)
    {
        if(!store)
        {
            return nullptr;
        }
        switch(store->type())
        {
            case OTPTokenType::HOTP: return HOTPTokenParameters::create(store, parent);
            case OTPTokenType::TOTP: return TOTPTokenParameters::create(store, parent);
            case OTPTokenType::SteamGuard: return SteamGuardParameters::create(store, parent);
            case OTPTokenType::DummyHMAC: return DummyParameters::create(store, parent);
            default:
                return nullptr;
        }
    }

//     static TokenParameters * forDptr(TokenParametersPrivate * d, QObject * parent)
//     {
//         if(!d)
//         {
//             return nullptr;
//         }
//
//         switch(d->type())
//         {
//             case otp::OTPTokenType::HOTP: return new HOTPTokenParameters(d, parent);
//             case otp::OTPTokenType::TOTP: return new TOTPTokenParameters(d, parent);
//             case otp::OTPTokenType::SteamGuard: return new SteamGuardParameters(d, parent);
//             case otp::OTPTokenType::DummyHMAC: return new DummyParameters(d, parent);
//             default:
//                 delete d;
//                 return nullptr;
//         }
//     }

//     TokenParameters * TokenParameters::createNew(OTPTokenType type, Storage * store, QObject * parent)
//     {
//         return forDptr(TokenParametersPrivate::createNew(type, store), parent);
//     }
//
//     TokenParameters * TokenParameters::createNew(OTPTokenType type, const QString& entryId, Storage * store, QObject * parent)
//     {
//         return forDptr(TokenParametersPrivate::createNew(type, entryId, store), parent);
//     }
//
//     TokenParameters * TokenParameters::loadExisting(const QString& entryId, Storage * store, QObject * parent)
//     {
//         return forDptr(TokenParametersPrivate::loadExisting(entryId, store), parent);
//     }

    static TokenGeneratorPrivate * forParams(TokenParameters * p)
    {
        if(!p)
        {
            return nullptr;
        }

        switch(p->type())
        {
            case OTPTokenType::SteamGuard:
                return new SteamGuardTokenGeneratorPrivate(qobject_cast<SteamGuardParameters *>(p));
            case OTPTokenType::HOTP:
                return new HOTPTokenGeneratorPrivate(qobject_cast<HOTPTokenParameters *>(p));
            case OTPTokenType::TOTP:
                return new TOTPTokenGeneratorPrivate(qobject_cast<TOTPTokenParameters *>(p));
            case OTPTokenType::DummyHMAC:
                return new DummyTokenGeneratorPrivate(qobject_cast<DummyParameters *>(p));
            default:
                return nullptr;
        }
    }

    TokenGenerator::TokenGenerator(TokenParameters * params, QObject * parent): QObject(parent), d_ptr(forParams(params)) {}
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
        Key key;
        Encoder encoder;
        Message message;
        Algorithm algo;

        if(d && d->ensureStorage() && d->secret(secret) &&  d->message(message) &&
            d->key(key) && d->algorithm(algo) && d->encoder(encoder))
        {
            token = otp::token(secret, message, key, algo, encoder);
            if(d->updateStorage())
            {
                value = token;
                return true;
            }
        }

        return false;
    }

}
