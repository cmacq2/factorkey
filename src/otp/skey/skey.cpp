#include "skey.h"
#include "words.h"

#include <QtDebug>

namespace otp
{
    namespace skey
    {
        SKeyDictionary::~SKeyDictionary() {}

        static int valueOfSKeyWord(const DefaultDictionary * dict, const SKeyWordComparator& compare, const QString& query, int idx, int partSize)
        {
            int sz = dict->size();
            if(sz <= idx || idx < 0)
            {
                return -1;
            }

            const QString& wordAt = dict->word(idx);
            int order = compare(query, wordAt);

            if(order == 0)
            {
                return idx;
            }

            if(partSize == 0)
            {
                return -1;
            }

            partSize = partSize / 2;
            if(order > 0)
            {
                return valueOfSKeyWord(dict, compare, query, partSize ? idx + partSize : idx + 1, partSize);
            }
            else
            {
                return valueOfSKeyWord(dict, compare, query, partSize ? idx - partSize : idx - 1, partSize);
            }
        }

        DefaultDictionary::DefaultDictionary(const QStringList& words, const SKeyWordComparator& cmp) : SKeyDictionary(), m_words(words), m_comparator(cmp) {}


        int DefaultDictionary::size(void) const
        {
            return m_words.size();
        }

        bool DefaultDictionary::validate(void) const
        {
            if(size() == DICTIONARY_SIZE)
            {
                const QString * prevWord = nullptr;
                for(const auto& w: m_words)
                {
                    if(prevWord)
                    {
                        if(m_comparator(*prevWord, w) >= 0)
                        {
                            return false;
                        }

                        if(word(valueOf(w)) != w)
                        {
                            return false;
                        }
                    }
                    prevWord = &w;
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        QString DefaultDictionary::word(int bits) const
        {
            if(bits > DICTIONARY_SIZE || bits < 0 || bits >= size())
            {
                return QString();
            }
            else
            {
                return m_words.at(bits);
            }
        }

        int DefaultDictionary::valueOf(const QString& query) const
        {
            int sz = size();
            return sz == DICTIONARY_SIZE ? valueOfSKeyWord(this, m_comparator, query, sz / 2, (sz % 2 ? (sz + 1) : sz) / 2) : -1;
        }

        SKeyWordComparator defaultWordComparator(void)
        {
            return SKeyWordComparator([](const QString& w1, const QString& w2) -> int
            {
                if(w1.isNull())
                {
                    return w2.isNull() ? 0 : -1;
                }

                if(w2.isNull())
                {
                    return 1;
                }

                if(w2.size() == 4)
                {
                    return w1.size() == 4 ? w1.compare(w2) : -1;
                }
                else
                {
                    return w1.size() == 4 ? 1 : w1.compare(w2);
                }
            });
        }

        SKeyDictionary * defaultDictionary(void)
        {
            QStringList words;
            internal::populateRFC2289Dictionary(words);
            return new DefaultDictionary(words, defaultWordComparator());
        }

        static int sKeyParity(quint64 bits)
        {
            int csum = 0;
            for(int i = 0; i < 32; ++i)
            {
                csum += (int) ((bits >> (2 * i)) & 0x3);
                csum &= 0x3;
            }
            return csum;
        }

        QStringList encodeSKeyWords(quint64 value, const SKeyDictionary * dict)
        {
            QStringList words;
            for(int i = 1; i < 6; ++i)
            {
                words << dict->word((value >> (64 - i * 11)) & 0x7FF);
            }
            words << dict->word(((value & 0x1FF) << 2) | sKeyParity(value));
            return words;
        }

        quint64 decodeSKeyWords(const QStringList& words, const SKeyDictionary * dict, bool * ok)
        {
            quint64 value = 0U;
            int w;
            * ok = false;
            if(words.size() != 6)
            {
                return value;
            }
            for(int i = 1; i < 6; ++i)
            {
                w = dict->valueOf(words[i - 1]);
                if(w < 0)
                {
                    return value;
                }
                else
                {
                    value |= ((quint64) w) << (64 - i * 11);
                }
            }
            w = dict->valueOf(words[5]);
            value |= ((quint64) w) >> 2;
            int csum = (int) (w & 0x3U);

            if(sKeyParity(value) == csum)
            {
                * ok = true;
            }
            return value;
        }

        SKeySeedError validateSKeySeed(const QString& seed)
        {
            auto s = seed.size();
            SKeySeedError err = SKeySeedError::Conformant;

            if(s == 0)
            {
                return SKeySeedError::Invalid;
            }

            if(s > 16)
            {
                err = SKeySeedError::Acceptable;
            }

            for(const auto& c: seed)
            {
                if(c.isSpace())
                {
                    return SKeySeedError::Invalid;
                }

                if( (c < QLatin1Char('0') || c > QLatin1Char('9')) &&
                    (c < QLatin1Char('A') || c > QLatin1Char('Z')) &&
                    (c < QLatin1Char('a') || c > QLatin1Char('z'))) {
                    err = SKeySeedError::Acceptable;
                }
            }

            return err;
        }

        QByteArray sKeySeedMessage(const QString& seed, const QTextCodec * codec)
        {
            if(validateSKeySeed(seed) == SKeySeedError::Invalid)
            {
                return QByteArray();
            }
            else
            {
                QString lower = seed.toLower();
                return codec ? codec->fromUnicode(lower) : lower.toUtf8();
            }
        }

        static quint64 sKeyFold(const char d1, const char d2, int shift)
        {
            quint64 result = (d1 ^ d2) & 0xFF;
            return result << shift;
        }

        static quint32 sKeyCollate(const char * c1)
        {
            quint32 result = 0U;
            for(int i = 0; i < 4; ++i)
            {
                result |= ((quint32) (c1[i] & 0xFF)) << ((7 - i) * 8);
            }
            return result;
        }

        quint64 sKeyTokenValue(const QByteArray& token, const QCryptographicHash::Algorithm& hash)
        {
            quint64 folded = 0U;

            switch(hash)
            {
                case QCryptographicHash::Md4:
                case QCryptographicHash::Md5:
                    if(token.size() == 16)
                    {
                        const char * data = token.constData();
                        for(auto i = 0; i < 8; ++i)
                        {
                            folded |= sKeyFold(data[i], data[i + 8], (7 - i ) * 8);
                        }
                    }
                    break;
                case QCryptographicHash::Sha1:
                    if(token.size() == 20)
                    {
                        const char * data = token.constData();
                        quint32 fst = sKeyCollate(&data[0]) ^ sKeyCollate(&data[8]);
                        quint32 snd = sKeyCollate(&data[4]) ^ sKeyCollate(&data[12]);
                        fst ^= sKeyCollate(&data[16]);

                        folded |= ((quint64) ((fst >> 0) & 0xFFU)) << 56;
                        folded |= ((quint64) ((fst >> 8) & 0xFFU)) << 48;
                        folded |= ((quint64) ((fst >> 16) & 0xFFU)) << 40;
                        folded |= ((quint64) ((fst >> 24) & 0xFFU)) << 32;

                        folded |= ((quint64) ((snd >> 0) & 0xFFU)) << 24;
                        folded |= ((quint64) ((snd >> 8) & 0xFFU)) << 16;
                        folded |= ((quint64) ((snd >> 16) & 0xFFU)) << 8;
                        folded |= ((quint64) ((snd >> 24) & 0xFFU)) << 0;
                    }
                    break;
                default:
                    break;
            }

            return folded;
        }

        static QByteArray encodeSKeyValue(quint64 value)
        {
            QByteArray result(8, '\0');
            char * data = result.data();
            for(int j = 0; j < 8; ++j)
            {
                data[j] = (value >> ((7 - j) * 8)) & 0xFF;
            }
            return result;
        }


        QByteArray sKey(const QByteArray& key, const QByteArray& seed, const QCryptographicHash::Algorithm& hash, quint64 rounds)
        {
            QByteArray result(seed);
            result += key;

            QCryptographicHash h(hash);
            h.addData(seed);
            h.addData(key);

            QByteArray initial = h.result();

            for(quint64 i = 0; i < rounds; ++i)
            {
                QByteArray next = encodeSKeyValue(sKeyTokenValue(initial, hash));
                initial = QCryptographicHash::hash(next, hash);
            }

            return initial;
        }
    }
}