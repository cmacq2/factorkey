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
            typedef std::function<TokenParameters*(otp::storage::Storage*, QObject*)> ConstructorFunction;
            bool setSecretEncoding(const QTextCodec * codec);
            bool setSecretEncodingType(const EncodingType& type);
            bool secretEncodingType(EncodingType& type);
            bool secretEncoding(QTextCodec ** codec) const;

            bool resync(void);
            bool commit(void);
            bool setSecret(const QString& key);
            bool setName(const QString& name);

            bool askSecret(void) const;
            bool askSecret(const otp::secrets::SecretsAPIProvider::SecretAnswer& answer) const;
            bool name(QString & name) const;
            QString id(void) const;
            otp::storage::OTPTokenType type(void) const;
            virtual ~TokenParameters();
            static TokenParameters * create(otp::storage::Storage * store, QObject * parent = 0);
        Q_SIGNALS:
            void retrievedSecret(bool ok, const QString& entryId, const QString& secret) const;
            void secretUpdated(bool ok, const QString& entryId) const;
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
            typedef std::function<void(bool, const QString&)> TokenResult;
            bool generateToken(void);
            bool generateToken(QString& token);
            bool generateToken(const TokenResult result);
            TokenParameters * params(void) const;
            TokenGenerator(TokenGeneratorPrivate * impl, QObject * parent = 0);
            ~TokenGenerator();
        Q_SIGNALS:
            void tokenGenerated(bool ok, const QString& token) const;
        private:
            Q_DISABLE_COPY(TokenGenerator)
            QScopedPointer<TokenGeneratorPrivate> const d_ptr;
            Q_DECLARE_PRIVATE(TokenGenerator)
        };
    }
}

#endif
