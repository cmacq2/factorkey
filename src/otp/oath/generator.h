#ifndef FACTORKEY_OATH_GENERATOR_H
#define FACTORKEY_OATH_GENERATOR_H

#include "../generator.h"
#include "../storage/storageprovider.h"
#include "../token/token.h"

namespace otp
{
    namespace oath
    {
        namespace generator
        {
            class GenericOTPParameters: public otp::generator::GenericTokenParameters
            {
                Q_OBJECT
            public:
                bool setTokenLength(uint length);
                bool setTokenLocale(const QLocale& l);

                bool tokenLength(uint & length) const;
                bool tokenLocale(QLocale& locale) const;
                virtual ~GenericOTPParameters();
            protected:
                GenericOTPParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            };

            class HOTPTokenParameters: public GenericOTPParameters
            {
                Q_OBJECT
            public:
                static HOTPTokenParameters * from(otp::storage::Storage * store, QObject * parent = 0);
                static HOTPTokenParameters * create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent = 0);
            public:
                otp::generator::TokenGenerator * generator(QObject * parent = nullptr);
                bool setTokenCounter(quint64 count);

                bool tokenCounter(quint64 & count) const;
                virtual ~HOTPTokenParameters();
            protected:
                HOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            private:
                static const bool isRegistered;
            };

            class TOTPTokenParameters: public GenericOTPParameters
            {
                Q_OBJECT
            public:
                static TOTPTokenParameters * from(otp::storage::Storage * store, QObject * parent = 0);
                static TOTPTokenParameters * create(const QString& entryId, otp::storage::StorageProvider * provider, QObject * parent = 0);
            public:
                otp::generator::TokenGenerator * generator(QObject * parent = nullptr);
                otp::generator::TokenGenerator * generator(qint64 currentMSec, QObject * parent = nullptr);
                bool setTokenEpoch(qint64 unixTime);
                bool setTokenEpoch(const QDateTime& epoch);
                bool setTokenTimeStep(quint64 msecs);

                bool tokenEpoch(qint64 & epoch) const;
                bool tokenTimeStep(quint64 & timeStep) const;
                virtual ~TOTPTokenParameters();
            protected:
                TOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            private:
                static const bool isRegistered;
            };
        }
    }
}
#endif
