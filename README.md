# Factorkey
Factorkey generates the one time passwords for all your two-factor authentication setups, taking advantage of your keyring (wallet) to manage secrets in a secure fashion.

## Supported 2FA types/schemes
Currently factorkey contains support for:
 * (OATH) HOTP
 * (OATH) TOTP
 * Google Authenticator (a variant of TOTP)
 * SteamGuard (a variant of TOTP)
 * S/Key

## Why bother, what makes factorkey 'better'?
Since two factor authentication primarily consists of generating a one time password from a single, preshared key or 'secret', the secrecy of this key is paramount for the security of the authenticated session.
By delegating to the KDE Wallet subsystem for handling storage and retrieval of your secrets, it is hoped a superior 2FA solution can be achieved for KDE on the desktop and on Plasma Mobile (eventually).
Benefits of integrating with the KDE Wallet stack include:
 * A secure storage mechanism, which may ensure data at rest is fully encrypted. Using GPG, if you like.
 * Not re-inventing the security wheel. It's a feature so good, it's probably worth mentioning twice!
 * If the wallet is configured to, it is automatically closed after a set period of inactivity... further helping to prevent abuse. As a result, factorkey also inherits this safeguard.
 * All of the convenience features KWallet provides for password management if you choose to use them.

## Build
It is recommended to build out of source, e.g.:
```
    cd /path/to/sources/of/factorkey
    mkdir -p ../build
    cd ../build

    cmake ../factorkey/
    make
```

### Sample apps
Factorkey comes with a number of optional sample applications.
These 'sample' apps are disabled by default, but may be included by passing a 'true' value to `-DINCLUDE_SAMPLE_APPS` as part of the cmake command:
```
    cmake -DINCLUDE_SAMPLE_APPS=1 ../factorkey/
```

### Installation prefix
Use `-DCMAKE_INSTALL_PREFIX` to control where artefacts are installed during `make install`:
```
    cmake -DINCLUDE_SAMPLE_APPS=1 \
        -DCMAKE_INSTALL_PREFIX=../dist \
        ../factorkey/
```

### Using locally built dependencies
Use `-DCMAKE_PREFIX_PATH` to point cmake to locally built versions of factorkey dependencies:
```
    cmake -DINCLUDE_SAMPLE_APPS=1 \
        -DCMAKE_INSTALL_PREFIX=../dist \
        -DCMAKE_PREFIX_PATH=/path/to/locally/built/versions/of/kde-5 \
        ../factorkey/
```

## Dependencies
Factorkey depends on:
 * Qt 5 (Core, SQL, SQL lite)
 * KF5 Wallet, I18n
