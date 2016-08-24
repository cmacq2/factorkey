#ifndef FACTORKEY_STORAGE_H
#define FACTORKEY_STORAGE_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QVariant>

namespace otp
{
    namespace storage
    {
        enum class OTPTokenType: int
        {
            DummyHMAC = 0,
            HOTP = 1,
            TOTP = 2,
            SteamGuard = 3,
            SKey = 4
        };

        class StoragePrivate;

        class Storage: public QObject
        {
            Q_OBJECT
        public:
            const QString entryId(void) const;
            OTPTokenType type(void) const;
            bool readPassword(QString& secret) const;
            bool writePassword(const QString& secret);
            bool readTokenType(OTPTokenType& value) const;
            bool writeTokenType(const OTPTokenType& value);
            bool readParam(const QString& param, QVariant& value) const;
            bool writeParam(const QString& param, const QVariant& value);

            bool exists(void) const;
            bool commit(void);
            bool poll(void);

            ~Storage();
            Storage(StoragePrivate * d, QObject * parent = 0);
        private:
            Q_DISABLE_COPY(Storage)
            const QScopedPointer<StoragePrivate> d_ptr;
            Q_DECLARE_PRIVATE(Storage)
        };
    }
}
#endif
