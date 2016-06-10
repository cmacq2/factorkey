
#include "otp/skey/skey.h"
#include "otp/skey/words.h"

#include <QScopedPointer>
#include <QTest>
#include <QtDebug>

class SKeyValidateDefaultDictionaryTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSample(void);
    void testSample_data(void);
};

void SKeyValidateDefaultDictionaryTest::testSample(void)
{
    QFETCH(QStringList, words);
    const QScopedPointer<otp::skey::SKeyDictionary> dict(new otp::skey::DefaultDictionary(words, otp::skey::defaultWordComparator()));
    QTEST(dict->validate(), "expected");
}

static void result(const char * testCase, const QStringList& words, bool expected)
{
    QTest::newRow(qPrintable(QLatin1String(testCase))) << words << expected;
}

static void fill(QStringList& toFill, const QString& word)
{
    for(int i = 0; i < otp::skey::DICTIONARY_SIZE; ++i)
    {
        toFill += word;
    }
}

static void invertDefaults(QStringList& toFill)
{
    otp::skey::internal::populateRFC2289Dictionary(toFill);
    const int mid = otp::skey::DICTIONARY_SIZE / 2;
    for(int i = mid - 1; i < mid; ++i)
    {
        int upper = mid + i, lower = mid - i;
        const QString u = toFill.at(upper), l = toFill.at(lower);
        toFill[lower] = u;
        toFill[upper] = l;
    }
}

void SKeyValidateDefaultDictionaryTest::testSample_data(void)
{
    QTest::addColumn<QStringList>("words");
    QTest::addColumn<bool>("expected");

    QStringList empty;
    result("Empty word list (too few words)", empty, false);

    QStringList tooFew;
    tooFew << QLatin1String("A") << QLatin1String("B") << QLatin1String("C");
    result("Non-empty, too small word list", tooFew, false);

    QStringList dups;
    fill(dups, QLatin1String("Word"));
    result("Multiple instances of the same word", dups, false);

    QStringList inverted;
    invertDefaults(inverted);
    result("Inverted order of the word list", inverted, false);

    QStringList correct;
    otp::skey::internal::populateRFC2289Dictionary(correct);
    result("Actual default dictionary words", correct, true);
}

QTEST_APPLESS_MAIN(SKeyValidateDefaultDictionaryTest)

#include "skey-validate-default-dict.moc"