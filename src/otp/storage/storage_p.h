#ifndef FACTORKEY_STORAGE_PRIVATE_H
#define FACTORKEY_STORAGE_PRIVATE_H

#include "storage.h"

namespace otp
{
    namespace storage
    {
        class StoragePrivate
        {
        public:
            virtual ~StoragePrivate() = 0;
            virtual QString entryId(void) const = 0;
            virtual OTPTokenType type(void) const = 0;
            virtual bool writePassword(const QString&, const otp::secrets::SecretsAPIProvider::SecretConfirmation&) = 0;
            virtual bool readTokenType(OTPTokenType&) const = 0;
            virtual bool writeTokenType(const OTPTokenType&) = 0;
            virtual bool readParam(const QString&, QVariant&) const = 0;
            virtual bool readPassword(const otp::secrets::SecretsAPIProvider::SecretAnswer&) const = 0;
            virtual bool writeParam(const QString&, const QVariant&) =0;
            virtual bool exists(void) const = 0;
            virtual bool commit(void) = 0;
            virtual bool poll(void) = 0;
        };

        class StorageProviderPrivate
        {
        public:
            virtual bool open(void);
            virtual bool close(void);
            virtual bool contains(const QString& entry);
            virtual bool entries(QStringList&);
            virtual Storage * lookup(const QString& entry);
            virtual Storage * create(const QString& entry, OTPTokenType type);
            virtual ~StorageProviderPrivate();
        protected:
            virtual Storage * doLookup(const QString&);
            virtual Storage * doCreate(const QString&, OTPTokenType);
            virtual bool isOpened(void);
            virtual bool openBackend(void);
            virtual bool closeBackend(void);
        };
    }
}

#endif
