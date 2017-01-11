#include "secrets.h"

namespace otp
{
    namespace storage
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
                return nullptr;
            }
        }
    }
}
