# Factorkey
Factorkey generates the one time passwords for your two-factor authentication setups, taking advantage of your keyring (wallet) to manage secrets in a secure fashion.

## Credit and inspiration
Factorkey was inspired by [SailOTP](https://github.com/seiichiro0185/sailotp).

## Supported 2FA types/schemes
Currently, the following token types are suppored in factorkey:
 * (OATH) HOTP
 * (OATH) TOTP
 * Google Authenticator (a variant of TOTP)
 * SteamGuard (a variant of TOTP)
 * S/Key

## Dependencies
Factorkey depends on:
 * Qt5 Core, Sql (with sqlite driver)
 * Sqlite
 * KF5 Wallet and/or QtKeychain (Qt5Keychain)
 * Qt5 Test
 * Extra Cmake Modules (ECM)

### Note on KWallet and Qt5Keychain
Factorkey requires Qt 5, therefore if QtKeychain is used it must be the Qt5Keychain version. Debian users please note: the packaged version of QtKeychain is a Qt 4 version, you will need to compile Qt5Keychain manually and point `cmake` to it using `-DCMAKE_PREFIX_PATH`.

### Git Submodules
Additionally the following dependencies are pulled in via git submodules:
 * Trompeloeil

## Build
Obtain necessary git submodules with:
```
    cd /path/to/sources/of/factorkey
    git submodule init
    git submodule update
```

It is recommended to build out of source, e.g.:
```
    cd /path/to/sources/of/factorkey
    mkdir -p ../build
    cd ../build

    cmake ../factorkey/
    make
```

### Picking storage backend for 'secrets'
Factorkey delegates storage of token 'secrets' (passwords) to specialised 'keychain' programs/libraries.
Factorkey may be compiled with support for direct KWallet access or through Qt5Keychain (which is portable to other keychain/wallet runtime implementations) or both.
If no choice is specified explicitly, Qt5Keychain is assumed by default.
Support may be enabled or disabled by passing `-DWITH_KWALLET` and `-DWITH_QT_KEYCHAIN` respectively as part of the cmake command:
```
    cmake -DWITH_KWALLET=1 -DWITH_QT_KEYCHAIN=1
```
Similarly, support may be be explicitly disabled by passing a `0` value with the respective`-DWITH_KWALLET` and `-DWITH_QT_KEYCHAIN` options.
Note: if all supported providers are explicitly deconfigured, the build will fail.

### Sample apps
Factorkey comes with a number of optional sample applications.
These 'sample' apps are disabled by default, but may be included by passing a 'true' value to `-DINCLUDE_SAMPLE_APPS` as part of the cmake command:
```
    cmake -DWITH_KWALLET=1 -DWITH_QT_KEYCHAIN=1 -DINCLUDE_SAMPLE_APPS=1 ../factorkey/
```

### Installation prefix
Use `-DCMAKE_INSTALL_PREFIX` to control where artefacts are installed during `make install`:
```
    cmake -DINCLUDE_SAMPLE_APPS=1 -DWITH_KWALLET=1 -DWITH_QT_KEYCHAIN=1 \
        -DCMAKE_INSTALL_PREFIX=../dist \
        ../factorkey/
```

### Using locally built dependencies
Use `-DCMAKE_PREFIX_PATH` to point cmake to locally built versions of factorkey dependencies:
```
    cmake -DINCLUDE_SAMPLE_APPS=1 -DWITH_KWALLET=1 -DWITH_QT_KEYCHAIN=1 \
        -DCMAKE_INSTALL_PREFIX=../dist \
        -DCMAKE_PREFIX_PATH="/path/to/locally/built/versions/of/kde-5;/path/to/locally/built/versions/of/qtkeychain/usr/local" ../factorkey/
        ../factorkey/
```

### Permitting debuggers to attach to the process with `ptrace()`
By default functionality is included with factorkey to prevent other processes from attaching to a running factorkey process with `ptrace()`. This is a counter measure against information leaks (of token secrets) through malicious use of `ptrace()` (e.g. by abusing debuggers). This does not prevent debugging when the process itself is launched (`fork()`'ed) by the debugger itself, but it does guard against processes attaching to a running factorkey process later.

To disable this protection and permit attaching to running factorkey processes with `ptrace()` regardless the `ALLOW_DUMPING` flag may be passed to `cmake` as follows:
```
    cmake -DALLOW_DUMPING=1
```

#### OS support
At this time the feature is available for Linux only as it relies on `prctl()` to work.

