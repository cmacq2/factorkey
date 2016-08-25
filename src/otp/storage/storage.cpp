#include "storage.h"
#include "storage_p.h"

namespace otp
{
    namespace storage
    {
        Storage::Storage(StoragePrivate * d, QObject * parent): QObject(parent), d_ptr(d) {}
        Storage::~Storage() {}

        const QString Storage::entryId(void) const
        {
            Q_D(const Storage);
            return d->entryId();
        }

        OTPTokenType Storage::type(void) const
        {
            Q_D(const Storage);
            return d->type();
        }

        bool Storage::writePassword(const QString& secret)
        {
            Q_D(Storage);
            return d->writePassword(secret);
        }

        bool Storage::readTokenType(OTPTokenType& value) const
        {
            Q_D(const Storage);
            return d->readTokenType(value);
        }

        bool Storage::writeTokenType(const OTPTokenType& value)
        {
            Q_D(Storage);
            return d->writeTokenType(value);
        }

        bool Storage::readParam(const QString& param, QVariant& value) const
        {
            Q_D(const Storage);
            return d->readParam(param, value);
        }

        bool Storage::readPassword(QString& secret) const
        {
            Q_D(const Storage);
            return d->readPassword(secret);
        }

        bool Storage::writeParam(const QString& param, const QVariant& value)
        {
            Q_D(Storage);
            return d->writeParam(param, value);
        }

        bool Storage::exists(void) const
        {
            Q_D(const Storage);
            return d->exists();
        }

        bool Storage::commit(void)
        {
            Q_D(Storage);
            return d->commit();
        }

        bool Storage::poll(void)
        {
            Q_D(Storage);
            return d->poll();
        }
    }
}