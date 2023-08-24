#ifndef __WEKA_PROTO_H__
#define __WEKA_PROTO_H__

#ifndef KERNEL_MODULE
#include <sys/stat.h>
#endif

#define WEKA_PROTO_IOCTL  (('L' << 24) | ('O' << 16) | ('C' << 8) | 'P')

#define WEKA_PROTO_MAX_SLOTS 1024
#define WEKA_PROTO_SLOT_SIZE 64
#define WEKA_PROTO_SHMEM_SIZE (WEKA_PROTO_SLOT_SIZE * WEKA_PROTO_MAX_SLOTS)

#define WEKA_PROTO_MAX_KEYS  1
#define WEKA_PROTO_MAX_DATA  32

#define WEKA_PROTO_DEV_NAME  "weka-proto"

typedef enum {
    WEKA_PROTO_OP_GET = 0,
    WEKA_PROTO_OP_PUT = 1,
    WEKA_PROTO_OP_MAP = 2
} weka_proto_op_t;

typedef enum {
   WEKA_PROTO_KEY_NONE = 0,
   WEKA_PROTO_KEY_ATIME = 1,
} weka_proto_key_t;

typedef struct weka_proto_atime_info {
#ifdef KERNEL_MODULE
    struct timespec64 am_times_array[2];
#else
    struct timespec am_times_array[2];
#endif   
} weka_proto_atime_info_t;

typedef struct weka_proto_keymap_table {
    weka_proto_key_t  keys[WEKA_PROTO_MAX_KEYS];
} weka_proto_keymap_table_t;

typedef struct weka_proto_ioctl {
    int   op;
    int   slot;
} weka_proto_ioctl_t;

#ifdef KERNEL_MODULE
extern int weka_proto_init(void);
extern int weka_proto_shutdown(void);
extern int weka_proto_get_atime_info(pid_t tid, weka_proto_atime_info_t *data);
extern int weka_proto_set_atime_info(pid_t tid, weka_proto_atime_info_t *data);
#else
extern int weka_proto_slots_init(void);
extern int weka_proto_slots_shutdown(void);
extern int weka_proto_alloc_slot();
extern int weka_proto_free_slot(int slot);
extern void weka_proto_clear_slot(int slot);
extern int weka_proto_get_atime_info(int slot, weka_proto_atime_info_t *data);
extern int weka_proto_set_atime_info(int slot, weka_proto_atime_info_t *data);
extern void weka_proto_clear_atime_info(int slot);
#endif

#endif /* __WEKA_PROTO_H__ */
