#ifndef FACTORKEY_GENERATOR_H
#define FACTORKEY_GENERATOR_H

#include "storage/storage.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QLocale>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QTextCodec>

#include <functional>

namespace otp
{
    namespace generator
    {
        class TokenParametersPrivate;
        class TokenGeneratorPrivate;

        enum EncodingType : int
        {
            Text, Base32, Unknown
        };

        class TokenParameters: public QObject
        {
            Q_OBJECT
        public:
            static const QString OTP_TOKEN_TYPE;
            static const QString OTP_TOKEN_NAME;
            static const QString OTP_KEY_ENCODING_CHARSET;
            static const QString OTP_KEY_ENCODING_TYPE;
        public:
            typedef std::function<TokenParameters*(otp::storage::Storage*, QObject*)> ConstructorFunction;
            bool setSecretEncoding(const QTextCodec * codec);
            bool setSecretEncodingType(const EncodingType& type);
            bool secretEncodingType(EncodingType& type);
            bool secretEncoding(QTextCodec ** codec) const;

            bool resync(void);
            bool commit(void);
            bool setSecret(const QString& key);
            bool setName(const QString& name);

            bool secret(QString& value) const;
            bool name(QString & name) const;
            QString id(void) const;
            otp::storage::OTPTokenType type(void) const;
            virtual ~TokenParameters();
            static TokenParameters * create(otp::storage::Storage * store, QObject * parent = 0);
        private:
            Q_DISABLE_COPY(TokenParameters)
            QScopedPointer<TokenParametersPrivate> const d_ptr;
        protected:
            Q_DECLARE_PRIVATE(TokenParameters)
        protected:
            static bool registerType(otp::storage::OTPTokenType type, const ConstructorFunction& ctor);
            TokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
        };

        class GenericTokenParameters: public TokenParameters
        {
            Q_OBJECT
        public:
            static const QString OTP_HMAC_HASH_ALGORITHM;
        public:
            bool setHashAlgorithm(const QCryptographicHash::Algorithm& hash);
            bool hashAlgorithm(QCryptographicHash::Algorithm& hash) const;
            virtual ~GenericTokenParameters();
        protected:
            GenericTokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
        };

        class TokenGenerator: public QObject
        {
            Q_OBJECT
        public:
            bool generateToken(QString& value);
            TokenParameters * params(void) const;
            TokenGenerator(TokenGeneratorPrivate * impl, QObject * parent = 0);
            ~TokenGenerator();
        private:
            Q_DISABLE_COPY(TokenGenerator)
            QScopedPointer<TokenGeneratorPrivate> const d_ptr;
            Q_DECLARE_PRIVATE(TokenGenerator)
        };
    }
}

#endif
