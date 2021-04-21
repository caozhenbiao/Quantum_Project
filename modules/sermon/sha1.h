
#include <string.h>


	typedef struct {
		unsigned int state[5];
		unsigned int count[2];
		unsigned char buffer[64];
	} SHA1_CTX;


	#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

		/* blk0() and blk() perform the initial expand. */
		/* I got the idea of expanding during the round function from SSLeay */
	#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) |(rol(block->l[i],8)&0x00FF00FF))



	#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
		^block->l[(i+2)&15]^block->l[i&15],1))

		/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
	#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
	#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
	#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
	#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
	#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

	class CSha1
	{
	public:
		static void		HashBuffer(void *buf, int buf_len, unsigned char *RetDigest);
		//static CString	HashString(CString s);
		//static CString	HashFile(CString strFilename);
		//static CString	HashFileHeader(CString strFilename, DWORD dwHeaderBytes);		//只算前xxx字节的hash
		//static CString	Convert2String(BYTE *RetDigest);

		static void		SHA1Final(unsigned char digest[20], SHA1_CTX* context);
		static void		SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned int len);
		static void		SHA1Init(SHA1_CTX* context);

	private:
		static void		SHA1Transform(unsigned int state[5], unsigned char buffer[64]);

	};
