
#include "otp/skey/skey.h"

#include <QScopedPointer>
#include <QTest>
#include <QtDebug>

class SKeyDefaultWordsRoundTripTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testRT(void);
};

void SKeyDefaultWordsRoundTripTest::testRT(void)
{
    const QScopedPointer<otp::skey::SKeyDictionary> dict(otp::skey::defaultDictionary());
    for(int i = 0; i < otp::skey::DICTIONARY_SIZE; ++i)
    {
        const QString w = dict->word(i);
        int valueOf = dict->valueOf(w);
        QVERIFY2(w.size() > 0, "Words from the default dictionary are not empty strings");
        QVERIFY2(w.size() < 5, "Words from the default dictionary are comprised of 1 to 4 characters (inclusive)");
        if(valueOf == -1)
        {
            qDebug() << "Oops:" << "index is:" << i << "word is:" << w << "returned value is:" << valueOf;
        }
        QCOMPARE(dict->valueOf(w), i);
    }
}

QTEST_APPLESS_MAIN(SKeyDefaultWordsRoundTripTest)

#include "skey-default-words-rt.moc"