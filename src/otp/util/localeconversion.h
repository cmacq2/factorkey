#ifndef FACTORKEY_LOCALE_CONVERSION_UTIL_H
#define FACTORKEY_LOCALE_CONVERSION_UTIL_H

#include <QLocale>
#include <QString>

namespace otp
{
    namespace util
    {
        struct LocaleTriplet {
            QLocale::Language language;
            QLocale::Script script;
            QLocale::Country country;
        };
        QString localeToString(const QLocale& locale);
        QString localeToString(const struct LocaleTriplet& locale);
        bool stringToLocale(const QString& string, QLocale& locale);
        bool stringToLocale(const QString& string, struct LocaleTriplet& out);
    }
}
#endif
