#ifndef FACTORKEY_OATH_GENERATOR_H
#define FACTORKEY_OATH_GENERATOR_H

#include "../generator.h"
#include "../token/token.h"

namespace otp
{
    namespace oath
    {
        namespace generator
        {
            bool algorithm(const otp::generator::GenericTokenParameters * params, otp::token::Algorithm& algo);

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
                static otp::generator::TokenGenerator * generator(HOTPTokenParameters * params, QObject * parent = 0);
                static HOTPTokenParameters * create(otp::storage::Storage * store, QObject * parent = 0);
            public:
                bool setTokenCounter(quint64 count);

                bool tokenCounter(quint64 & count) const;
                virtual ~HOTPTokenParameters();
            protected:
                HOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            };

            class TOTPTokenParameters: public GenericOTPParameters
            {
                Q_OBJECT
            public:
                static otp::generator::TokenGenerator * generator(TOTPTokenParameters * params, QObject * parent = 0);
                static otp::generator::TokenGenerator * generator(TOTPTokenParameters * params, qint64 currentMSec, QObject * parent = 0);
                static TOTPTokenParameters * create(otp::storage::Storage * store, QObject * parent = 0);
            public:
                bool setTokenEpoch(qint64 unixTime);
                bool setTokenEpoch(const QDateTime& epoch);
                bool setTokenTimeStep(quint64 msecs);

                bool tokenEpoch(qint64 & epoch) const;
                bool tokenTimeStep(quint64 & timeStep) const;
                virtual ~TOTPTokenParameters();
            protected:
                TOTPTokenParameters(otp::generator::TokenParametersPrivate * d, QObject * parent = 0);
            };
        }
    }
}
#endif
