#include "otp/token.h"

#include <QString>
#include <QtDebug>

/**
 * Example to generate a Google Authenticator style TOTP token using default settings.
 * Usage:
 *   $ generate-totp-token-sample mysecretkey
 *
 * See otp/token.h for more advanced options and other token types.
 *
 * @param argc number of arguments passed to main in `argv'
 * @param argv command line arguments passed to the sample app.
 * @return 0 (success)
 */
int main(int argc, const char** argv)
{
    QString secret(QLatin1String("Hello, world!"));

    if(argc > 1)
    {
        // secret = QString::fromUtf8(argv[1]);
        secret = QString::fromLocal8Bit(argv[1]);
    }
    /*
     * Use default hashing algorithm
     */
    const otp::Algorithm& algorithm = otp::hmacAlgorithm();
    /*
     * Use 'Google Authenticator' key format (i.e. base 32). See section #5 of RFC 3548:
     * https://tools.ietf.org/html/rfc3548#section-5
     */
    const otp::Key& keyEncoder = otp::keyForAuthenticator();
    /*
     * Use default epoch and default time step
     */
    const otp::Message& message = otp::totpMessage();
    /*
     * Use default length and default locale
     */
    const otp::Encoder& encoder = otp::otpEncoder();

    qDebug() << "Token is:" << otp::token(secret, message, keyEncoder, algorithm, encoder);
    return 0;
}