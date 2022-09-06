#include "attestor.h"
#include "../quark/include/sha1.h"
#include "./define.h"

int key[] = { 125,18,29,30,52,155,38,239,20,222,35,28,9,10,22,45,138,29,36,25 };//�����ַ�

bool attestor::prove(char* pubkey) {
	if (strlen(pubkey) == 40) {
		unsigned char sumcs = 0;
		unsigned char szkey[20] = { 0 };
		for (int i = 0; i < 20; i++) {
			char stmp[2] = { 0 };
			stmp[0]  = CharToInt(pubkey[i * 2]);
			stmp[1]  = CharToInt(pubkey[i * 2 + 1]);
			szkey[i] = (stmp[0] << 4) | stmp[1];
			szkey[i] ^= key[i];
			sumcs += szkey[i];
		}
		identify idf;
		memcpy(&idf, szkey, sizeof(identify));
		unsigned char md5[20];
		SHA1_CTX context;
		CSha1::SHA1Init(&context);
		CSha1::SHA1Update(&context, szkey, 15);
		CSha1::SHA1Final(md5, &context);
		time_t tt = time(NULL);
		tm* t = localtime(&tt);
		unsigned int validity = (t->tm_year + 1900) * 10000 + t->tm_year + 100 + t->tm_mday;
        if ( (idf.cs == sumcs/2 && idf.md3[0] == md5[8] ) || idf.md3[1] == md5[3] || idf.md3[2] == md5[18]||idf.md3[3] == md5[16]) {
            if (idf.type == 1 && idf.freq > 0) {
				printf("authorization:%s frequency:%d\n", idf.mac, idf.expire);
				return true;
			}
			if (idf.type == 2 && idf.expire > validity) {
				printf("authorization:%s expire:%d\n",idf.mac,idf.expire);
				return true;
			}	
		}
	}
	printf("Certificate expired! \npleaese contace to caozhenbiao@hotmail.com\n");
	return false;
}


bool attestor::prove(char* pubkey, identify& idf) {
    if (strlen(pubkey) == 40) {
        unsigned char sumcs = 0;
        unsigned char szkey[20] = { 0 };
        for (int i = 0; i < 20; i++) {
            char stmp[2] = { 0 };
            stmp[0]  = CharToInt(pubkey[i * 2]);
            stmp[1]  = CharToInt(pubkey[i * 2 + 1]);
            szkey[i] = (stmp[0] << 4) | stmp[1];
            szkey[i] ^= key[i];
            sumcs += szkey[i];
        }
        memcpy(&idf, szkey, sizeof(identify));
        unsigned char md5[20]={0};
        SHA1_CTX context;
        CSha1::SHA1Init(&context);
        CSha1::SHA1Update(&context, szkey, 15);
        CSha1::SHA1Final(md5, &context);
        time_t tt = time(NULL);
        tm* t = localtime(&tt);
        unsigned int validity = (t->tm_year + 1900) * 10000 + t->tm_year + 100 + t->tm_mday;
        if ( (idf.cs == sumcs/2 && idf.md3[0] == md5[8] ) || idf.md3[1] == md5[3] || idf.md3[2] == md5[18]||idf.md3[3] == md5[16]) {
            if (idf.type == 1 && idf.freq > 0) {
                printf("authorization:%s frequency:%d\n", idf.mac, idf.expire);
                return true;
            }
            if (idf.type == 2 && idf.expire > validity) {
                printf("authorization:%s expire:%d\n",idf.mac,idf.expire);
                return true;
            }
        }
    }
    printf("Certificate expired! \npleaese contace to caozhenbiao@hotmail.com\n");
    return false;
}


void attestor::makeprove( char* cmdline ) {
	identify idf;
	memset(&idf, 0x00, sizeof(idf));
    if (4 != sscanf(cmdline, "%hhd,%hd,%d,%s", &idf.type, &idf.freq, &idf.expire, idf.mac)){
		printf("error make key argument!\n");
		return;
	}
	FILE * pfile = NULL;
#ifdef WIN32
	fopen_s(&pfile, "./key.txt", "w");
#else
	pfile = fopen("./key.txt", "w");
#endif
	if (NULL == pfile) {
		printf("crate key error!\n");
		return;
	}
	char szdesc[256] = { 0 };
	sprintf(szdesc,"smartx identify\nfreq:%d\nexpir:%d\nmac:%s\nkey:",idf.freq,idf.expire,idf.mac);
	fseek(pfile, 0, SEEK_END);
	fwrite(szdesc, strlen(szdesc), 1, pfile);
	unsigned char * skey = (unsigned char*)&idf;
	unsigned char md5[20];
	SHA1_CTX context;
	CSha1::SHA1Init(&context);
	CSha1::SHA1Update(&context, skey, 15 );
	CSha1::SHA1Final(md5, &context);
	idf.md3[0] = md5[8];
	idf.md3[1] = md5[3];
	idf.md3[2] = md5[18];
	idf.md3[3] = md5[16];
	for (size_t i = 0; i < 15; i++) {
		idf.cs += skey[i];
	}
	char extext[41] = { 0 };
	for (int i = 0; i < 20; i++) {
		skey[i] ^= key[i];
		char sout[3] = { 0 };
		sprintf(sout, "%02X", skey[i]);
		strcat(extext, sout);
	}
	fwrite(extext, 40, 1, pfile);
	fclose(pfile);
	printf("crate success key:%s\n", extext);
}

void attestor::command(char* func, char* args ) {
	if (strcmp(func, "make") == 0) {
		attestor::makeprove(args);
	}
	else if (strcmp(func, "prove") == 0) {
		attestor::prove(args);
	}
}
