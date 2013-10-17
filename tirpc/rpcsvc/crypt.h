/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CRYPT_H_RPCGEN
#define _CRYPT_H_RPCGEN

#include <rpc/rpc.h>

#ifndef IXDR_GET_INT32
#define IXDR_GET_INT32(buf) IXDR_GET_LONG((buf))
#endif
#ifndef IXDR_PUT_INT32
#define IXDR_PUT_INT32(buf, v) IXDR_PUT_LONG((buf), (v))
#endif
#ifndef IXDR_GET_U_INT32
#define IXDR_GET_U_INT32(buf) IXDR_GET_U_LONG((buf))
#endif
#ifndef IXDR_PUT_U_INT32
#define IXDR_PUT_U_INT32(buf, v) IXDR_PUT_U_LONG((buf), (v))
#endif

enum des_dir {
	ENCRYPT_DES = 0,
	DECRYPT_DES = 1,
};
typedef enum des_dir des_dir;
#ifdef __cplusplus
extern "C" bool xdr_des_dir(XDR *, des_dir *);
#elif __STDC__
extern bool xdr_des_dir(XDR *, des_dir *);
#else				/* Old Style C */
bool xdr_des_dir();
#endif				/* Old Style C */

enum des_mode {
	CBC_DES = 0,
	ECB_DES = 1,
};
typedef enum des_mode des_mode;
#ifdef __cplusplus
extern "C" bool xdr_des_mode(XDR *, des_mode *);
#elif __STDC__
extern bool xdr_des_mode(XDR *, des_mode *);
#else				/* Old Style C */
bool xdr_des_mode();
#endif				/* Old Style C */

struct desargs {
	u_char des_key[8];
	des_dir des_dir;
	des_mode des_mode;
	u_char des_ivec[8];
	struct {
		u_int desbuf_len;
		char *desbuf_val;
	} desbuf;
};
typedef struct desargs desargs;
#ifdef __cplusplus
extern "C" bool xdr_desargs(XDR *, desargs *);
#elif __STDC__
extern bool xdr_desargs(XDR *, desargs *);
#else				/* Old Style C */
bool xdr_desargs();
#endif				/* Old Style C */

struct desresp {
	struct {
		u_int desbuf_len;
		char *desbuf_val;
	} desbuf;
	u_char des_ivec[8];
	int stat;
};
typedef struct desresp desresp;
#ifdef __cplusplus
extern "C" bool xdr_desresp(XDR *, desresp *);
#elif __STDC__
extern bool xdr_desresp(XDR *, desresp *);
#else				/* Old Style C */
bool xdr_desresp();
#endif				/* Old Style C */

#define CRYPT_PROG ((u_int32_t)600100029)
#define CRYPT_VERS ((u_int32_t)1)

#ifdef __cplusplus
#define DES_CRYPT ((u_int32_t)1)
extern "C" desresp * des_crypt_1(desargs *, CLIENT *);
extern "C" desresp * des_crypt_1_svc(desargs *, struct svc_req *);

#elif __STDC__
#define DES_CRYPT ((u_int32_t)1)
extern desresp *des_crypt_1(desargs *, CLIENT *);
extern desresp *des_crypt_1_svc(desargs *, struct svc_req *);

#else				/* Old Style C */
#define DES_CRYPT ((u_int32_t)1)
extern desresp *des_crypt_1();
extern desresp *des_crypt_1_svc();
#endif				/* Old Style C */

#endif				/* !_CRYPT_H_RPCGEN */
