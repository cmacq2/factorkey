#include "otp/util/localeconversion.h"

#include <QLocale>
#include <QString>
#include <QTest>
#include <QtDebug>


class LocaleRTTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testLocaleToString(void);
    void testLocaleToString_data(void);
    void testStringToLocale(void);
    void testStringToLocale_data(void);
};

void LocaleRTTest::testLocaleToString(void)
{
    struct otp::util::LocaleTriplet sample;
    QFETCH(int, language);
    QFETCH(int, country);
    QFETCH(int, script);

    sample.language = (QLocale::Language) language;
    sample.country = (QLocale::Country) country;
    sample.script = (QLocale::Script) script;

    QLocale s(sample.language, sample.script, sample.country);
    qDebug() << "Sample in BCP-47:" << s.bcp47Name() << "name:" << s.name();

    QTEST(otp::util::localeToString(sample), "expected");
}

static void result(int l, int c, int s, const QString& name)
{
    QTest::newRow(qPrintable(name)) << l << c << s << name;
}

void LocaleRTTest::testLocaleToString_data(void)
{
    QTest::addColumn<int>("language");
    QTest::addColumn<int>("country");
    QTest::addColumn<int>("script");
    QTest::addColumn<QString>("expected");

    result((int) QLocale::C, (int) QLocale::AnyCountry, (int) QLocale::AnyScript, QLatin1String("C"));
    result((int) QLocale::French, (int) QLocale::AnyCountry,(int) QLocale::AnyScript,  QLatin1String("French"));
    result((int) QLocale::French, (int) QLocale::Canada, (int) QLocale::AnyScript, QLatin1String("French+Canada"));
    result((int) QLocale::Greek, (int) QLocale::AnyCountry, (int) QLocale::GreekScript, QLatin1String("Greek:GreekScript"));
    result((int) QLocale::AncientGreek, (int) QLocale::Greece, (int) QLocale::GreekScript, QLatin1String("AncientGreek+Greece:GreekScript"));
    result((int) QLocale::AnyLanguage, (int) QLocale::Belgium, (int) QLocale::AnyScript, QLatin1String("+Belgium"));
    result((int) QLocale::AnyLanguage, (int) QLocale::Belgium, (int) QLocale::LatinScript, QLatin1String("+Belgium:LatinScript"));
    result((int) QLocale::AnyLanguage, (int) QLocale::AnyCountry, (int) QLocale::TraditionalHanScript, QLatin1String(":TraditionalHanScript"));
}

void LocaleRTTest::testStringToLocale(void)
{
    struct otp::util::LocaleTriplet comparison;
    QFETCH(QString, name);

    QFETCH(int, expectedLanguage);
    QFETCH(int, expectedCountry);
    QFETCH(int, expectedScript);


    QVERIFY2(otp::util::stringToLocale(name, comparison), "Expected string -> locale conversion step to work");
    QCOMPARE(comparison.language, (QLocale::Language) expectedLanguage);
    QCOMPARE(comparison.country, (QLocale::Country) expectedCountry);
    QCOMPARE(comparison.script, (QLocale::Script) expectedScript);
}

static void result(const QString& name, int l, int c, int s)
{
    QTest::newRow(qPrintable(name)) << name << l << c << s;
}

void LocaleRTTest::testStringToLocale_data(void)
{
    QTest::addColumn<QString>("name");

    QTest::addColumn<int>("expectedLanguage");
    QTest::addColumn<int>("expectedCountry");
    QTest::addColumn<int>("expectedScript");

    result(QLatin1String("C"), QLocale::C, QLocale::AnyCountry, QLocale::AnyScript);
    result(QLatin1String("French"), (int) QLocale::French, (int) QLocale::AnyCountry, (int) QLocale::AnyScript);
    result(QLatin1String("French+Canada"), (int) QLocale::French, (int) QLocale::Canada, (int) QLocale::AnyScript);
    result(QLatin1String("Greek:GreekScript"), (int) QLocale::Greek, (int) QLocale::AnyCountry, (int) QLocale::GreekScript);
    result(QLatin1String("AncientGreek+Greece:GreekScript"), (int) QLocale::AncientGreek, (int) QLocale::Greece, (int) QLocale::GreekScript);
    result(QLatin1String("+Belgium"), (int) QLocale::AnyLanguage, (int) QLocale::Belgium, (int) QLocale::AnyScript);
    result(QLatin1String("+Belgium:LatinScript"), (int) QLocale::AnyLanguage, (int) QLocale::Belgium, (int) QLocale::LatinScript);
    result(QLatin1String(":TraditionalHanScript"), (int) QLocale::AnyLanguage, (int) QLocale::AnyCountry, (int) QLocale::TraditionalHanScript);
}

QTEST_APPLESS_MAIN(LocaleRTTest)

#include "locale-rt.moc"
