#include "secrets.h"

namespace otp
{
    namespace secrets
    {
        SecretsAPIProvider::~SecretsAPIProvider() {}

        SecretsAPIBuilder::~SecretsAPIBuilder() {}

        QSharedPointer<SecretsAPIProvider> SecretsAPIBuilder::create(void) const
        {
            return QSharedPointer<SecretsAPIProvider>(build());
        }

        SecretsAPIProvider * SecretsAPIBuilder::build(void) const
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Supertype version of build() must not be called; sub type implementations should override it instead.");
            return nullptr;
        }
    }
}
