#include "myssl.h"

#include <winsock.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/applink.c>
#include <openssl/ossl_typ.h>

#include<stdio.h>
#include<vector>
#include<iostream>
#include<unordered_set>


int create_socket(const char *ip_cstr, unsigned short port_num)
{

	printf("connectting\n");

	int fd;
	struct sockaddr_in dest_addr;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port_num);
	dest_addr.sin_addr.s_addr = inet_addr(ip_cstr);
	memset(&(dest_addr.sin_zero), '\0', 8);
	if (connect(fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1) {
		printf("connect failed!\n");
		return -1;
	}
	printf("connect success！\n");
	return fd;
}

int openSSL_client_init(){
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	if (SSL_library_init() < 0)
		return -1;
	return 0;
}

int openSSL_create_client_ctx(SSL_CTX **ctx){
	const SSL_METHOD *method = SSLv23_client_method();
	if ((*ctx = SSL_CTX_new(method)) == NULL)
		return -1;
	//SSL_CTX_set_options(*ctx, SSL_OP_NO_SSLv2);
	return 0;
}


void print_certificate(X509 *cert){
	char s[1204];
	printf(" version %li\n", X509_get_version(cert));
	printf(" not before %s\n", X509_get_notBefore(cert)->data);
	printf(" not after %s\n", X509_get_notAfter(cert)->data);
	printf(" signature type %i\n", X509_get_signature_type(cert));
	printf(" serial no %li\n", ASN1_INTEGER_get(X509_get_serialNumber(cert)));
	X509_NAME_oneline(X509_get_issuer_name(cert), s, 256);
	printf(" issuer %s\n", s);
	X509_NAME_oneline(X509_get_subject_name(cert), s, 256);
	printf(" subject %s\n", s);
	printf(" cert type %i\n", X509_certificate_type(cert, X509_get_pubkey(cert)));
}

 cmyssl::cmyssl() {
	 WSADATA wsdata;
	 WSAStartup(0x0202, &wsdata);
}

 cmyssl::~cmyssl() {
	 WSACleanup();
 }

 char* cmyssl::getPeerCert( const char* ip, unsigned short port ) {
	 if (openSSL_client_init()) {
		 printf("Could not initialize the OpenSSL library !\n");
		 return "";
	 }

	 SSL_CTX *ctx;
	 if (openSSL_create_client_ctx(&ctx)) {
		 printf("Unable to create a new SSL context structure.\n");
		 return "";
	 }

	 if (SSL_CTX_load_verify_locations(ctx, "D:\\Quantum\\projectX\\HGSSL\\run\\CA.pem", NULL) <= 0) {
		 printf("Unable to Load certificate\n");
		 return "";
	 }

	 SSL* ssl = SSL_new(ctx);
	 int server = create_socket( ip, port );
	 if (server < 0) {
		 printf("Error: Can't create TCP session");
		 return "";
	 }

	 SSL_set_fd(ssl, server);
	 if (SSL_connect(ssl) != 1) {
		 printf("Error: Could not build a SSL session to: 192.168.1.1");
		 return "";
	 }

	 //获取证书信息
	 X509* cert = SSL_get_peer_certificate(ssl);
	 if (cert == NULL) {
		 printf("Error: Could not get a certificate from: 192.168.1.1");
		 return "";
	 }

	 char s[1204] = {0};
	 printf("cert info\n");
	 printf(" version %li\n", X509_get_version(cert));
	 printf(" not before %s\n", X509_get_notBefore(cert)->data);
	 printf(" not after %s\n", X509_get_notAfter(cert)->data);
	 printf(" signature type %i\n", X509_get_signature_type(cert));
	 printf(" serial no %li\n", ASN1_INTEGER_get(X509_get_serialNumber(cert)));
	 X509_NAME_oneline(X509_get_issuer_name(cert), s, 256);
	 printf(" issuer %s\n", s);
	 X509_NAME_oneline(X509_get_subject_name(cert), s, 256);
	 printf(" subject %s\n", s);
	 printf(" cert type %i\n", X509_certificate_type(cert, X509_get_pubkey(cert)));


	 X509_get_X509_PUBKEY(cert);



	 //FILE *outbio = NULL;
	// X509_print_fp(outbio, cert);


	 /*
	 BIO* outbio = BIO_new_fp(stdout, BIO_NOCLOSE);
	 X509_NAME * certname = X509_get_subject_name(cert);
	 X509_NAME_print(outbio, certname, 0);
	 X509_NAME_print_ex(outbio, certname, 0, 0);
	 X509_print(outbio, cert);
	 EVP_PKEY * pkey = X509_get_pubkey(cert);
	 char pemCertString[] = "-----BEGIN/END CERTIFICATE-----";
	 size_t certLen = strlen(pemCertString);
	 X509* certX509 = PEM_read_bio_X509(outbio, NULL, NULL, NULL);
	 if (!certX509) {
		 fprintf(stderr, "unable to parse certificate in memory\n");
		 return "";
	 }
	 */

	 //获取证书链
	 STACK_OF(X509) *sk;
	 //STACK_OF(X509_NAME) *sk2;
	// X509_NAME *xn;
	 char buf[BUFSIZ];
	 sk = SSL_get_peer_cert_chain(ssl);
	 if (sk != NULL) {
		 printf("\nCertificate chain\n");
		 for (int i = 0; i < sk_X509_num(sk); i++) {
			 X509_NAME_oneline(X509_get_subject_name(sk_X509_value(sk, i)), buf, sizeof buf);
			 printf("%2d s:%s\n", i, buf);
			 X509_NAME_oneline(X509_get_issuer_name(sk_X509_value(sk, i)), buf, sizeof buf);
			 printf("   i:%s\n", buf);
			 //if (c_showcerts)
			 //	PEM_write_bio_X509(bio, sk_X509_value(sk, i));
		 }
	 }

	 /*
	 sk2 = SSL_get_client_CA_list(ssl);
	 if ((sk2 != NULL) && (sk_X509_NAME_num(sk2) > 0)) {
		 std::cout << "---\nAcceptable client certificate CA names\n";
		 for (int i = 0; i < sk_X509_NAME_num(sk2); i++) {
			 xn = sk_X509_NAME_value(sk2, i);
			 X509_NAME_oneline(xn, buf, sizeof(buf));
			 std::cout << buf << "\n";
		 }
	 }
	 */

	 char msg[100000] = "GET / HTTP/1.1\r\nHOST: www.baidu.com\r\n\r\n";
	 SSL_write(ssl, msg, strlen(msg));
	 SSL_read(ssl, msg, 100000);
	 printf( "\n Get Message is %s\n", msg);

	 SSL_free(ssl);
	 closesocket(server);
	 X509_free(cert);
	 SSL_CTX_free(ctx);
	 return "";
 }


int run( )
{



	BIO *outbio = NULL;

	X509 *cert;
	X509_NAME *certname = NULL;

	SSL_CTX *ctx;
	SSL *ssl;
	int server = 0;
	//int ret;

	if (openSSL_client_init()) {
		std::cerr << "Could not initialize the OpenSSL library !" << std::endl;
		return -1;
	}

	outbio = BIO_new_fp(stdout, BIO_NOCLOSE);

	if (openSSL_create_client_ctx(&ctx)) {
		std::cerr << "Unable to create a new SSL context structure." << std::endl;
		return -1;
	}


	std::cout << "Adding Certifcate" << std::endl;
	if (SSL_CTX_load_verify_locations(ctx, "D:\\Quantum\\projectX\\HGSSL\\run\\CA.pem", NULL) <= 0) {
		std::cerr << "Unable to Load certificate" << std::endl;
		return -1;
	}


	ssl = SSL_new(ctx);
	server = create_socket("192.168.1.1", 443);

	if (server < 0) {
		std::cerr << "Error: Can't create TCP session" << std::endl;
		return -1;
	}
 
	SSL_set_fd(ssl, server);

	if (SSL_connect(ssl) != 1) {
		std::cerr << "Error: Could not build a SSL session to: " << "192.168.1.1"<< std::endl;
		return -1;
	}

	std::cout << "Successfully enabled SSL/TLS session to: 443" << std::endl;
	//SSL_SESSION *ss = SSL_get_session(ssl);

	//获取证书信息
	cert = SSL_get_peer_certificate(ssl);
	if (cert == NULL) {
		std::cerr << "Error: Could not get a certificate from: 192.168.1.1"  << std::endl;
		return -1;
	}
	char s[1204];
	printf(" version %li\n", X509_get_version(cert));
	printf(" not before %s\n", X509_get_notBefore(cert)->data);
	printf(" not after %s\n", X509_get_notAfter(cert)->data);
	printf(" signature type %i\n", X509_get_signature_type(cert));
	printf(" serial no %li\n", ASN1_INTEGER_get(X509_get_serialNumber(cert)));
	X509_NAME_oneline(X509_get_issuer_name(cert), s, 256);
	printf(" issuer %s\n", s);
	X509_NAME_oneline(X509_get_subject_name(cert), s, 256);
	printf(" subject %s\n", s);
	printf(" cert type %i\n", X509_certificate_type(cert, X509_get_pubkey(cert)));




	certname = X509_get_subject_name(cert);
	X509_NAME_print(outbio, certname, 0);



	std::cout << "Displaying the certificate subject data:" << std::endl;
	X509_NAME_print_ex(outbio, certname, 0, 0);
	std::cout << std::endl;
	X509_print(outbio, cert);

	EVP_PKEY * pkey = X509_get_pubkey(cert);


	char pemCertString[] = "-----BEGIN/END CERTIFICATE-----";
	size_t certLen = strlen(pemCertString);


	X509* certX509 = PEM_read_bio_X509(outbio, NULL, NULL, NULL);
	if (!certX509) {
		fprintf(stderr, "unable to parse certificate in memory\n");
		return EXIT_FAILURE;
	}

	//获取证书链
	STACK_OF(X509) *sk;
	//STACK_OF(X509_NAME) *sk2;
	//X509_NAME *xn;
	char buf[BUFSIZ];
	sk = SSL_get_peer_cert_chain(ssl);
	if (sk != NULL) {
		std::cout << "---\nCertificate chain\n";
		for (int i = 0; i < sk_X509_num(sk); i++) {
			X509_NAME_oneline(X509_get_subject_name(sk_X509_value(sk, i)), buf, sizeof buf);
			printf("%2d s:%s\n", i, buf);
			X509_NAME_oneline(X509_get_issuer_name(sk_X509_value(sk, i)), buf, sizeof buf);
			printf("   i:%s\n", buf);
			//if (c_showcerts)
			//	PEM_write_bio_X509(bio, sk_X509_value(sk, i));
		}
	}


	/*
	sk2 = SSL_get_client_CA_list(ssl);
	if ((sk2 != NULL) && (sk_X509_NAME_num(sk2) > 0)) {
		std::cout << "---\nAcceptable client certificate CA names\n";
		for (i = 0; i < sk_X509_NAME_num(sk2); i++) {
			xn = sk_X509_NAME_value(sk2, i);
			X509_NAME_oneline(xn, buf, sizeof(buf));
			std::cout << buf << "\n";
		}
	}
	*/

	char msg[100000] = "GET / HTTP/1.1\r\nHOST: www.baidu.com\r\n\r\n";
	SSL_write(ssl, msg, strlen(msg));
	SSL_read(ssl, msg, 100000);
	std::cout << "Message is " << msg << std::endl;


	SSL_free(ssl);
	closesocket(server);
	X509_free(cert);
	SSL_CTX_free(ctx);
	std::cout << "Finished SSL/TLS connection with server" << std::endl;

	WSACleanup();
	return 0;
}


