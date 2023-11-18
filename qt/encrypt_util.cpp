#include "encrypt_util.h"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/pkcspad.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <iostream>

using namespace std;
using namespace CryptoPP;

QString encrypt_util::md5(const QString& input) {
    CryptoPP::MD5 md5;
    std::string hash;

    StringSource _ = StringSource(input.toStdString(), true,
        new HashFilter(md5, new HexEncoder(new StringSink(hash))));

    return QString::fromStdString(hash);
}


void encrypt_util::calc_key_iv(const QString& keyText,
    CryptoPP::byte(&key)[AES::BLOCKSIZE], CryptoPP::byte(&iv)[AES::BLOCKSIZE]) {

    string hash = md5(keyText).toStdString();

    for (int i = 0; i < hash.size(); i++) {
        if (i & 1) {
            key[i / 2] = hash[i];
        }
        else {
            iv[i / 2] = hash[i];
        }
    }
}

QString encrypt_util::encrypt(const QString& plainText, const QString& keyText) {

    CryptoPP::byte iv[AES::BLOCKSIZE];
    CryptoPP::byte key[AES::BLOCKSIZE];

    calc_key_iv(keyText, key, iv);

    CBC_Mode<AES>::Encryption encryption(key, AES::BLOCKSIZE, iv);
    string ciphertext;
    StringSource _ = StringSource(plainText.toStdString(), true,
        new StreamTransformationFilter(encryption, new HexEncoder(new StringSink(ciphertext), false)));

    return QString::fromStdString(ciphertext);
}

QString encrypt_util::decrypt(const QString& cipherText, const QString& keyText) {

    CryptoPP::byte iv[AES::BLOCKSIZE];
    CryptoPP::byte key[AES::BLOCKSIZE];

    calc_key_iv(keyText, key, iv);

    std::string hexDecodedCipherText;
    StringSource ignore = StringSource(cipherText.toStdString(), true, new HexDecoder(new StringSink(hexDecodedCipherText)));

    CBC_Mode<AES>::Decryption decryption(key, AES::BLOCKSIZE, iv);
    string decryptedtext;
    StringSource _ = StringSource(hexDecodedCipherText, true, new StreamTransformationFilter(decryption, new StringSink(decryptedtext)));

    return QString::fromStdString(decryptedtext);
}