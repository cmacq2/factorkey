#ifndef FACTORKEY_SKEY_WORDS_H
#define FACTORKEY_SKEY_WORDS_H

#include <QStringList>

namespace otp
{
    namespace skey
    {
        namespace internal
        {
            void populateRFC2289Dictionary(QStringList& list);
        }
    }
}

#endif
