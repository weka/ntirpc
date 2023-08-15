#ifndef __WEKA_PROTO_H__
#define __WEKA_PROTO_H__

#ifndef KERNEL_MODULE
#include <sys/stat.h>
#endif

#define WEKA_PROTO_MAX_KEYS  1
#define WEKA_PROTO_MAX_DATA  32

#define WEKA_PROTO_MAX_SLOTS 1024
#define WEKA_PROTO_SLOT_SIZE 72
#define WEKA_PROTO_SHMEM_SIZE 72 * 1024

#define WEKA_PROTO_IOCTL  (('L' << 24) | ('O' << 16) | ('C' << 8) | 'P')

#define WEKA_PROTO_DEV_NAME  "weka-proto"

typedef enum {
   WEKA_PROTO_KEY_NONE = 0,
   WEKA_PROTO_KEY_ENDPOINTS = 1
} weka_proto_key_t;

typedef enum {
    WEKA_PROTO_OP_NONE = 0,
    WEKA_PROTO_OP_SYMTIME = 1,
    WEKA_PROTO_OP_GETSLOT = 2,
    WEKA_PROTO_OP_PUTSLOT = 3
} weka_proto_op_t;

typedef struct weka_proto_keymap_table {
    weka_proto_key_t  keys[WEKA_PROTO_MAX_KEYS];
} weka_proto_keymap_table_t;

typedef struct weka_proto_symtime {
    int fid;
#ifdef KERNEL_MODULE
    struct timespec64 tm[2];
#else
    struct timespec tm[2];
#endif
} weka_proto_symtime_t;

typedef struct weka_proto_slot {
    int slot;
} weka_proto_slot_t;

typedef struct weka_proto_ioctl {
    int   op;
    union {
        struct weka_proto_symtime   symtime;
        struct weka_proto_slot      getslot;
        struct weka_proto_slot      putslot;
    } u;
} weka_proto_ioctl_t;

typedef enum {
    WEKA_PROTO_NAME_INVALID = 0,
    WEKA_PROTO_NAME_NFS = 1,
    WEKA_PROTO_NAME_S3 = 2,
    WEKA_PROTO_NAME_SMB = 3
} weka_proto_name_t;

typedef struct weka_proto_client_info {
    unsigned int ipaddr;
    unsigned int xid;
    unsigned char uspace_sub_xid;
    unsigned char kspace_sub_xid;
} weka_proto_client_info_t;

typedef struct weka_proto_target_info {
    unsigned int ipaddr;
    unsigned short version;
} weka_proto_target_info_t;

typedef struct weka_proto_endpoints_info {
    unsigned char proto;
    weka_proto_client_info_t client;
    weka_proto_target_info_t target;
} weka_proto_endpoints_info_t;

#ifdef KERNEL_MODULE
extern int weka_proto_init(void);
extern int weka_proto_shutdown(void);
extern int weka_proto_get_endpoints_info(pid_t tid, weka_proto_endpoints_info_t *data);
extern int weka_proto_set_endpoints_info(pid_t tid, weka_proto_endpoints_info_t *data);
extern int weka_proto_clear_endpoints_info(pid_t tid);
#else
extern int weka_proto_init(void);
extern int weka_proto_shutdown(void);
extern int weka_proto_alloc_slot(void);
extern int weka_proto_free_slot(int slot);
extern int weka_proto_set_symtime(int fid, struct timespec tm[2]);
extern int weka_proto_get_endpoints_info(int slot, weka_proto_endpoints_info_t *data);
extern int weka_proto_set_endpoints_info(int slot, weka_proto_endpoints_info_t *data);
extern int weka_proto_clear_endpoints_info(int slot);
#endif

#endif /* __WEKA_PROTO_H__ */
