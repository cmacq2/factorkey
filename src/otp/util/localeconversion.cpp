#include "localeconversion.h"

#include <QMetaEnum>

namespace otp
{
    namespace util
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
}
