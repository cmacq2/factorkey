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
    class TokenParametersPrivate;

    class TokenParameters: public QObject
    {
        Q_OBJECT
    public:
//         static TokenParameters * createNew(OTPTokenType type, Storage * store, QObject * parent = 0);
//         static TokenParameters * createNew(OTPTokenType type, const QString& entryId, Storage * store, QObject * parent = 0);
//         static TokenParameters * loadExisting(const QString& entryId, Storage * store, QObject * parent = 0);
        bool resync(void);
        bool commit(void);
        bool setSecret(const QString& key);
        bool setName(const QString& name);

        bool secret(QString& value) const;
        bool name(QString & name) const;
        QString id(void) const;
        OTPTokenType type(void) const;
        virtual ~TokenParameters();
        static TokenParameters * create(Storage * store, QObject * parent = 0);
    private:
        Q_DISABLE_COPY(TokenParameters)
        QScopedPointer<TokenParametersPrivate> const d_ptr;
//         static TokenParameters * forDptr(TokenParametersPrivate * d, QObject * parent = 0);
    protected:
        Q_DECLARE_PRIVATE(TokenParameters)
    protected:
        TokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };

    class GenericTokenParameters: public TokenParameters
    {
        Q_OBJECT
    public:
        bool setSecretEncoding(const QTextCodec * codec);
        bool setHashAlgorithm(const QCryptographicHash::Algorithm& hash);

        bool secretEncoding(QTextCodec ** codec) const;
        bool hashAlgorithm(QCryptographicHash::Algorithm& hash) const;
        virtual ~GenericTokenParameters();
    protected:
        GenericTokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };

    class DummyParameters: public GenericTokenParameters
    {
        Q_OBJECT
    public:
        OTPTokenType type(void) const;
        bool setTokenMessage(const QString& value);
        bool setTokenMessageEncoding(const QTextCodec * codec);

        bool tokenMessage(QString& message) const;
        bool tokenMessageEncoding(QTextCodec ** codec) const;
        static DummyParameters * create(Storage * store, QObject * parent = 0);
        virtual ~DummyParameters();
    protected:
        DummyParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };

    class GenericOTPParameters: public GenericTokenParameters
    {
        Q_OBJECT
    public:
        bool setTokenLength(uint length);
        bool setTokenLocale(const QLocale& l);

        bool tokenLength(uint & length) const;
        bool tokenLocale(QLocale& locale) const;
        virtual ~GenericOTPParameters();
    protected:
        GenericOTPParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };

    class HOTPTokenParameters: public GenericOTPParameters
    {
        Q_OBJECT
    public:
        bool setTokenCounter(quint64 count);

        bool tokenCounter(quint64 & count) const;
        static HOTPTokenParameters * create(Storage * store, QObject * parent = 0);
        virtual ~HOTPTokenParameters();
    protected:
        HOTPTokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };

    class TOTPTokenParameters: public GenericOTPParameters
    {
        Q_OBJECT
    public:
        bool setTokenEpoch(qint64 unixTime);
        bool setTokenEpoch(const QDateTime& epoch);
        bool setTokenTimeStep(quint64 msecs);

        bool tokenEpoch(qint64 & epoch) const;
        bool tokenTimeStep(quint64 & timeStep) const;
        static TOTPTokenParameters * create(Storage * store, QObject * parent = 0);
        virtual ~TOTPTokenParameters();
    protected:
        TOTPTokenParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };

    class SteamGuardParameters: public TokenParameters
    {
        Q_OBJECT
    public:
        static SteamGuardParameters * create(Storage * store, QObject * parent = 0);
        virtual ~SteamGuardParameters();
    protected:
        SteamGuardParameters(TokenParametersPrivate * d, QObject * parent = 0);
    };


    class TokenGeneratorPrivate;

    class TokenGenerator: public QObject
    {
        Q_OBJECT
    public:
        bool generateToken(QString& value);
        TokenParameters * params(void) const;
        TokenGenerator(TokenParameters * params, QObject * parent = 0);
        ~TokenGenerator();
    private:
        Q_DISABLE_COPY(TokenGenerator)
        QScopedPointer<TokenGeneratorPrivate> const d_ptr;
        Q_DECLARE_PRIVATE(TokenGenerator)
    };

}

#endif
