#ifndef FACTORKEY_SKEY_GENERATOR_H
#define FACTORKEY_SKEY_GENERATOR_H

#include "../generator.h"
#include "skey.h"

namespace otp
{
    namespace skey
    {
        namespace generator
        {
            class DictionaryProvider
            {
            public:
                virtual ~DictionaryProvider() = 0;
                virtual otp::skey::SKeyDictionary * dictionary(const QString& name) const =0;
            };

            enum class SKeyEncodingType: int
            {
                Words, Hex
            };

            class SKeyTokenParameters: public otp::generator::TokenParameters
            {
                Q_OBJECT
            public:
                static const QString OTP_SKEY_DICTIONARY_NAME;
                static const QString OTP_SKEY_ENCODING_TYPE;
                static const QString DEFAULT_DICTIONARY_VALUE;
            public:
                bool dictionaryProvider(DictionaryProvider ** provider) const;
                bool dictionaryName(QString& name) const;
                bool dictionary(otp::skey::SKeyDictionary ** dict) const;
                bool setDictionaryProvider(DictionaryProvider * provider = nullptr);
                bool setDictionaryName(const QString& name);
            public:
                bool setSKeyEncoding(const SKeyEncodingType& value);
                bool sKeyEncoding(SKeyEncodingType& value) const;
                bool rounds(quint64& value) const;
                bool setRounds(quint64 value);

                bool seed(QString& value) const;
                bool setSeed(const QString& value);

                bool setHashAlgorithm(const QCryptographicHash::Algorithm& hash);
                bool hashAlgorithm(QCryptographicHash::Algorithm& hash) const;

                bool challenge(QString& value) const;
                bool setChallenge(const QString& value);
            public:
                static SKeyTokenParameters * create(otp::storage::Storage * store, QObject * parent = 0);
                static otp::generator::TokenGenerator * generator(SKeyTokenParameters * params, QObject * parent = 0);
            public:
                virtual ~SKeyTokenParameters();
            protected:
                SKeyTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            private:
                static const bool isRegistered;
            };
        }
    }
}
#endif
