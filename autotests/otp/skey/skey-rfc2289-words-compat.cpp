
#include "otp/skey/skey.h"

#include <QByteArray>
#include <QCryptographicHash>

#include <QTest>
#include <QtDebug>

class SKeyRFC2289WordsCompatibilityTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase(void);
    void testSample(void);
    void testSample_data(void);
private:
    QScopedPointer<otp::skey::SKeyDictionary> m_dict;
};

void SKeyRFC2289WordsCompatibilityTest::initTestCase(void)
{
    m_dict.reset(otp::skey::defaultDictionary());
}

void SKeyRFC2289WordsCompatibilityTest::testSample(void)
{
    QFETCH(quint64, tokenValue);
    QTEST(otp::skey::encodeSKeyWords(tokenValue, m_dict.data()), "expected");
}

static void result(quint64 tokenValue, const char * w1, const char * w2, const char * w3, const char * w4, const char * w5, const char * w6)
{
    QString fmt(QLatin1String("token value: 0x%1"));
    QStringList words;
    words << QLatin1String(w1) << QLatin1String(w2) << QLatin1String(w3)
        << QLatin1String(w4) << QLatin1String(w5) << QLatin1String(w6);
    QTest::newRow(qPrintable(fmt.arg(tokenValue, 16, 16))) << tokenValue << words;
}

void SKeyRFC2289WordsCompatibilityTest::testSample_data(void)
{
    QTest::addColumn<quint64>("tokenValue");
    QTest::addColumn<QStringList>("expected");

    result(0xD1854218EBBB0B51U, "ROME", "MUG", "FRED", "SCAN", "LIVE", "LACE");
    result(0x63473EF01CD0B444U, "CARD", "SAD", "MINI", "RYE", "COL", "KIN");

    result(0xC5E612776E6C237AU, "NOTE", "OUT", "IBIS", "SINK", "NAVE", "MODE");
    result(0x50076F47EB1ADE4EU, "AWAY", "SEN", "ROOK", "SALT", "LICE", "MAP");
    result(0x65D20D1949B5F7ABU, "CHEW", "GRIM", "WU", "HANG", "BUCK", "SAID");
    result(0xD150C82CCE6F62D1U, "ROIL", "FREE", "COG", "HUNK", "WAIT", "COCA");
    result(0x849C79D4F6F55388U, "FOOL", "STEM", "DONE", "TOOL", "BECK", "NILE");
    result(0x8C0992FB250847B1U, "GIST", "AMOS", "MOOT", "AIDS", "FOOD", "SEEM");
    result(0x3F3BF4B4145FD74BU, "TAG", "SLOW", "NOV", "MIN", "WOOL", "KENO");

    result(0x9E876134D90499DDU, "INCH", "SEA", "ANNE", "LONG", "AHEM", "TOUR");
    result(0x7965E05436F5029FU, "EASE", "OIL", "FUM", "CURE", "AWRY", "AVIS");
    result(0x50FE1962C4965880U, "BAIL", "TUFT", "BITS", "GANG", "CHEF", "THY");
    result(0x87066DD9644BF206U, "FULL", "PEW", "DOWN", "ONCE", "MORT", "ARC");
    result(0x7CD34C1040ADD14BU, "FACT", "HOOF", "AT", "FIST", "SITE", "KENT");
    result(0x5AA37A81F212146CU, "BODE", "HOP", "JAKE", "STOW", "JUT", "RAP");
    result(0xF205753943DE4CF9U, "ULAN", "NEW", "ARMY", "FUSE", "SUIT", "EYED");
    result(0xDDCDAC956F234937U, "SKIM", "CULT", "LOB", "SLAM", "POE", "HOWL");
    result(0xB203E28FA525BE47U, "LONG", "IVY", "JULY", "AJAR", "BOND", "LEE");

    result(0xBB9E6AE1979D8FF4U, "MILT", "VARY", "MAST", "OK", "SEES", "WENT");
    result(0x63D936639734385BU, "CART", "OTTO", "HIVE", "ODE", "VAT", "NUT");
    result(0x87FEC7768B73CCF9U, "GAFF", "WAIT", "SKID", "GIG", "SKY", "EYED");
    result(0xAD85F658EBE383C9U, "LEST", "OR", "HEEL", "SCOT", "ROB", "SUIT");
    result(0xD07CE229B5CF119BU, "RITE", "TAKE", "GELD", "COST", "TUNE", "RECK");
    result(0x27BC71035AAF3DC6U, "MAY", "STAR", "TIN", "LYON", "VEDA", "STAN");
    result(0xD51F3E99BF8E6F0BU, "RUST", "WELT", "KICK", "FELL", "TAIL", "FRAU");
    result(0x82AEB52D943774E4U, "FLIT", "DOSE", "ALSO", "MEW", "DRUM", "DEFY");
    result(0x4F296A74FE1567ECU, "AURA", "ALOE", "HURL", "WING", "BERG", "WAIT");
}

QTEST_APPLESS_MAIN(SKeyRFC2289WordsCompatibilityTest)

#include "skey-rfc2289-words-compat.moc"