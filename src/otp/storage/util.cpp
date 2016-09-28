#include "util.h"
#include "storage.h"

#include <QtDebug>

Q_DECLARE_METATYPE(enum otp::storage::OTPTokenType)

namespace otp
{
    namespace storage
    {
        bool checkOTPTokenTypeIsMetaType(void)
        {
            static auto type = qRegisterMetaType<enum otp::storage::OTPTokenType>();
            bool result = (bool) type;
            if(!result)
            {
                qWarning() << "Failed to register meta types! Some signal/slot spying functionality may not work as expected.";
            }
            return result;
        }
    }
}
