#include "generator_p.h"

#include <QMetaEnum>

namespace otp
{
    namespace generator
    {
        namespace internal
        {
            template<typename T>
            static inline QString localeFieldToString(const T& field)
            {
                static QMetaEnum fields = QMetaEnum::fromType<T>();
                QByteArray name = fields.valueToKey((int) field);
                return QString(QLatin1String(name));
            }

            QString localeToString(const struct LocaleTriplet& locale)
            {
                if(locale.language == QLocale::C)
                {
                    return QLatin1String("C");
                }
                else
                {
                    QString aName(QLatin1String(""));
                    QString result;
                    if(locale.language != QLocale::AnyLanguage)
                    {
                        aName += localeFieldToString<QLocale::Language>(locale.language);
                        result = aName;
                    }
                    if(locale.country != QLocale::AnyCountry)
                    {
                        aName += QLatin1Char('+');
                        aName += localeFieldToString<QLocale::Country>(locale.country);
                        result = aName;
                    }
                    if(locale.script != QLocale::AnyScript)
                    {
                        aName += QLatin1Char(':');
                        aName += localeFieldToString<QLocale::Script>(locale.script);
                        result = aName;
                    }
                    return result;
                }
            }

            QString localeToString(const QLocale& locale)
            {
                struct LocaleTriplet lc {
                     locale.language(),
                     locale.script(),
                     locale.country()
                };
                return localeToString(lc);
            }

            template<typename T>
            static inline T localeFieldFromString(const QString& name, bool * ok, const T& defaultValue)
            {
                static QMetaEnum fields = QMetaEnum::fromType<T>();
                QByteArray chars = name.toUtf8();
                int result = fields.keyToValue(chars.constData(), ok);
                return *ok ? (T) result: defaultValue;
            }

            bool stringToLocale(const QString& string, QLocale& locale)
            {
                struct LocaleTriplet out;
                bool ok = stringToLocale(string, out);
                locale = QLocale(out.language, out.script, out.country);
                return ok && locale.language() == out.language && locale.country() == out.country && locale.script() == out.script;
            }

            bool stringToLocale(const QString& string, struct LocaleTriplet& out)
            {
                if(string == QLatin1String("C"))
                {
                    out.language = QLocale::C;
                    out.country = QLocale::AnyCountry;
                    out.script = QLocale::AnyScript;
                    return true;
                }
                else
                {
                    int countryIdx = string.indexOf(QLatin1Char('+'));
                    int scriptIdx = string.indexOf(QLatin1Char(':'));
                    int size = string.size();
                    bool langValid = false, countryValid = false, scriptValid = false;

                    if(countryIdx > -1)
                    {
                        if(countryIdx > 0)
                        {
                            out.language = localeFieldFromString<QLocale::Language>(string.mid(0, countryIdx), &langValid, QLocale::AnyLanguage);
                        }
                        else
                        {
                            out.language = QLocale::AnyLanguage;
                            langValid = true;
                        }
                        if(scriptIdx > (1 + countryIdx))
                        {
                            out.country = localeFieldFromString<QLocale::Country>(string.mid(countryIdx + 1, scriptIdx - countryIdx - 1), &countryValid, QLocale::AnyCountry);
                            if(size > (1 + scriptIdx))
                            {
                                out.script = localeFieldFromString<QLocale::Script>(string.mid(scriptIdx + 1, size - scriptIdx - 1), &scriptValid, QLocale::AnyScript);
                            }
                        }
                        else
                        {
                            if(size > (1 + countryIdx))
                            {
                                out.country = localeFieldFromString<QLocale::Country>(string.mid(countryIdx + 1, size - countryIdx - 1), &countryValid, QLocale::AnyCountry);
                            }
                        }

                        if(scriptIdx == -1)
                        {
                            out.script = QLocale::AnyScript;
                            scriptValid = true;
                        }
                    }
                    else
                    {
                        out.country = QLocale::AnyCountry;
                        countryValid = true;
                        if(scriptIdx > -1)
                        {
                            if(scriptIdx > 0)
                            {
                                out.language = localeFieldFromString<QLocale::Language>(string.mid(0, scriptIdx), &langValid, QLocale::AnyLanguage);
                            }
                            else
                            {
                                langValid = true;
                            }
                            if(size > (1 + scriptIdx))
                            {
                                out.script = localeFieldFromString<QLocale::Script>(string.mid(scriptIdx + 1, size - scriptIdx - 1), &scriptValid, QLocale::AnyScript);
                            }
                        }
                        else
                        {
                            out.script = QLocale::AnyScript;
                            scriptValid = true;
                            if(size > 0)
                            {
                                out.language = localeFieldFromString<QLocale::Language>(string, &langValid, QLocale::AnyLanguage);
                            }
                            else
                            {
                                out.language = QLocale::AnyLanguage;
                                langValid = true;
                            }
                        }
                    }

                    if(!langValid)
                    {
                        out.language = QLocale::AnyLanguage;
                    }
                    if(!scriptValid)
                    {
                        out.script = QLocale::AnyScript;
                    }
                    if(!countryValid)
                    {
                        out.country = QLocale::AnyCountry;
                    }
                    return langValid && countryValid && scriptValid;
                }
            }
        }


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

        bool TokenParametersPrivate::lookupSecret(QString & value) const
        {
            QString str;
            if(m_storage->readPassword(str)  && !str.isNull() && !str.isEmpty())
            {
                value = str;
                return true;
            }
            return false;
        }

        bool TokenParametersPrivate::writeSecret(const QString & secret)
        {
            return !secret.isNull() && !secret.isEmpty() && m_storage->writePassword(secret);
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
            return m_params->commit();
        }

        bool TokenGeneratorPrivate::message(otp::token::Message&) const
        {
            return false;
        }

        bool TokenGeneratorPrivate::secret(QString& secret) const
        {
            return m_params->secret(secret);
        }

        bool TokenGeneratorPrivate::key(otp::token::Key& key) const
        {
            auto p = qobject_cast<otp::generator::GenericTokenParameters *>(params());
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
            GenericTokenParameters * p = qobject_cast<GenericTokenParameters *>(params());
            if(p && p->secretEncoding(&codec))
            {
                key = otp::token::Key(otp::token::keyForCodec(codec));
                return true;
            }
            return false;
        }
    }
}
