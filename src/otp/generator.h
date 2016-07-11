#ifndef FACTORKEY_GENERATOR_H
#define FACTORKEY_GENERATOR_H

#include "storage.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QLocale>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QTextCodec>

namespace otp
{
    namespace generator
    {
        class TokenParametersPrivate;
        class TokenGeneratorPrivate;

        class TokenParameters: public QObject
        {
            Q_OBJECT
        public:
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
            TokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
        };

        enum EncodingType : int
        {
            Text, Base32, Unknown
        };

        class GenericTokenParameters: public TokenParameters
        {
            Q_OBJECT
        public:
            bool setSecretEncoding(const QTextCodec * codec);
            bool setHashAlgorithm(const QCryptographicHash::Algorithm& hash);
            bool setSecretEncodingType(const EncodingType& type);

            bool secretEncodingType(EncodingType& type);
            bool secretEncoding(QTextCodec ** codec) const;
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
