#ifndef FACTORKEY_SKEY_H
#define FACTORKEY_SKEY_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QLocale>
#include <QString>
#include <QTextCodec>

#include <functional>

namespace otp
{
    namespace skey
    {

        /**
         * Expected size of a dictionary (word list) for alternate S/Key encodings.
         * Since each word encodes 11 bits, the number of words in a dictionary must be 2048.
         */
        static const int DICTIONARY_SIZE = 2048;


        /**
         * Interface describing a dictionary for alternate S/Key encodings.
         */
        class SKeyDictionary
        {
        public:
            /**
             * Return the size of the dictionary. This is expected to be exactly the same as
             * {@link #DICTIONARY_SIZE} because each word is expected to correspond to an 11 bit
             * numeric value.
             */
            virtual int size(void) const = 0;
            /**
             * \brief Sanity check the dictionary for internal consistency and to make sure
             * that it matches necessary preconditions. If dictionaries are constructed from external
             * input, this method should validate the input. In particular this method should check:
             *
             * <ul><li>That the dictionary contains at least {@link #DICTIONARY_SIZE} number of words
             * </li><li>That each word is unique.
             * </li><li>That each word corresponds to an 11 bit numeric value ({@link #valueOf()})
             * </li><li>That this value corresponds to the same word. ({@link #word()})
             * </li></ul>
             * @return true if the dictionary should be considered 'valid', false if it should
             * be rejected as invalid/improperly constructed.
             */
            virtual bool validate(void) const = 0;
            virtual int valueOf(const QString& word) const = 0;
            virtual QString word(int value) const = 0;
            virtual ~SKeyDictionary() = 0;
        };

        typedef std::function<int(const QString&, const QString&)> SKeyWordComparator;

        class DefaultDictionary: public SKeyDictionary
        {
        public:
            DefaultDictionary(const QStringList& words, const SKeyWordComparator& cmp);
            int size(void) const;
            bool validate(void) const;
            int valueOf(const QString& word) const;
            QString word(int value) const;
        private:
            const QStringList m_words;
            const SKeyWordComparator m_comparator;
        };

        SKeyWordComparator defaultWordComparator(void);

        SKeyDictionary * defaultDictionary(void);

        QStringList encodeSKeyWords(quint64 value, const SKeyDictionary * dict);
        quint64 decodeSKeyWords(const QStringList& words, const SKeyDictionary * dict, bool * ok);

        enum class SKeySeedError: int
        {
            Conformant,
            Invalid,
            Acceptable
        };

        SKeySeedError validateSKeySeed(const QString & seed);

        QByteArray sKeySeedMessage(const QString& seed, const QTextCodec * codec = nullptr);
        quint64 sKeyTokenValue(const QByteArray& token, const QCryptographicHash::Algorithm& hash = QCryptographicHash::Sha1);


        QByteArray sKey(const QByteArray& key,
                        const QByteArray& seed,
                        const QCryptographicHash::Algorithm& hash,
                        quint64 rounds);
    }

}
#endif