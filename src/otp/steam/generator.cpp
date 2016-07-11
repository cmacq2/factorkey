#include "generator.h"
#include "../generator_p.h"
#include "../oath/token.h"
#include "../oath/oath.h"
#include "steam.h"

namespace otp
{
    namespace steam
    {
        namespace generator
        {
            SteamGuardParameters::SteamGuardParameters(otp::generator::TokenParametersPrivate * d, QObject * parent) : otp::generator::TokenParameters(d, parent) {}
            SteamGuardParameters::~SteamGuardParameters() {}
            SteamGuardParameters * SteamGuardParameters::create(otp::storage::Storage * store, QObject * parent)
            {
                return store && store->type() == otp::storage::OTPTokenType::SteamGuard ? new SteamGuardParameters(new otp::generator::TokenParametersPrivate(store), parent) : nullptr;
            }

            class SteamGuardTokenGeneratorPrivate: public otp::generator::TokenGeneratorPrivate
            {
            public:
                SteamGuardTokenGeneratorPrivate(otp::steam::generator::SteamGuardParameters * p) : otp::generator::TokenGeneratorPrivate(p) {}

                bool algorithm(otp::token::Algorithm& algo) const
                {
                    algo = otp::oath::hmacAlgorithm(QCryptographicHash::Sha1);
                    return true;
                }

                bool encoder(otp::token::Encoder& encoder) const
                {
                    QString(* func)(const QByteArray&) = otp::steam::encodeSteamGuardToken;
                    encoder = otp::token::Encoder(func);
                    return true;
                }

                bool message(otp::token::Message& message) const
                {
                    message = otp::oath::totpMessage(0, otp::oath::DEFAULT_TIMESTEP_MSEC);
                    return true;
                }
            };

            otp::generator::TokenGenerator * SteamGuardParameters::generator(SteamGuardParameters * params, QObject * parent)
            {
                return new otp::generator::TokenGenerator(new SteamGuardTokenGeneratorPrivate(params), parent);
            }
        }
    }
}
