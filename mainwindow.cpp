#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include<QtDebug>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void startEncryption()
{
    //qInfo is qt5.5+ only.
    qInfo() << "C++ Style Info Message";
    qDebug() << "C++ Style Debug Message";

//	setbuf(stdout, NULL);
    /* Show the decrypted text */
    printf("Set up Openssl:\n");
    printf("%s\n", "Nikhil's OpenSSL;");

    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Message to be encrypted */
//    unsigned char *plaintext =
//        (unsigned char *)"The quick brown fox jumps over the lazy dog";

    unsigned char *plaintext =
        (unsigned char *)"The quick brown";

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char ciphertext[128];

    int ciphertext_len;
    int plaintext_len = strlen ((char *)plaintext);

    EVP_CIPHER_CTX *ctx;

    int len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        printf("This is first error.");

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        qInfo() << "This is second error.";

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        qInfo() << "This is third error.";
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        qInfo() << "This is EVP_EncryptFinal_ex error.";
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    std::cout << "Original Text: " << plaintext << std::endl;
    std::cout << "Original Text Length: " << plaintext_len << std::endl;
    std::cout << "Encrypted Text: " << ciphertext << std::endl;
    std::cout << "Encrypted Text Length: " << ciphertext_len << std::endl;

//    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    EVP_CIPHER_CTX *ctx1;

    /* Create and initialise the context */
    if(!(ctx1 = EVP_CIPHER_CTX_new()))
        qInfo() << "This is fourth error.";

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx1, EVP_aes_256_cbc(), NULL, key, iv))
        qInfo() << "This is 5th error.";

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];
    int decryptedtext_len;
    int len1;
    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    int result = EVP_DecryptUpdate(ctx1, decryptedtext, &len1, ciphertext, ciphertext_len);
    if(1 != result)
        qInfo() << "This is 6th error.";
    decryptedtext_len = len1;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    result = EVP_DecryptFinal_ex(ctx1, decryptedtext + len1, &len1);
    if(1 != result)
        qInfo() << "This is 7th error.";
    decryptedtext_len += len1;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx1);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    /* Show the decrypted text */
    std::cout << "Decrypted text is: ";
    std::cout << decryptedtext << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    startEncryption();
}
