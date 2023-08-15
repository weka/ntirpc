#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "weka_proto.h"

static int  _fd = -1;;
static char *_shmem = NULL;

int weka_proto_init(void)
{
    char dev[128];
    sprintf(dev, "/dev/%s", WEKA_PROTO_DEV_NAME);
    _fd = open(dev, O_RDWR);
    if (_fd < 0) {
        return -EBADF;
    }

    _shmem = (char*)mmap(0,
                WEKA_PROTO_SHMEM_SIZE,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                _fd,
                0);

    if (_shmem == NULL) {
       close(_fd);
       _fd = -1;
       return -ENOMEM;
    }

    return 0;
}

int weka_proto_shutdown(void)
{
    if (_fd < 0)
        return -EBADF;

    close(_fd);
    _fd = -1;

    return 0;
}

int weka_proto_alloc_slot(void)
{
    if (_fd < 0)
        return -EBADF;

    weka_proto_ioctl_t ioctl_data;
    ioctl_data.op = WEKA_PROTO_OP_GETSLOT;
    ioctl_data.u.getslot.slot = -1;
    int rc = ioctl(_fd, WEKA_PROTO_IOCTL , &ioctl_data);
    if (rc < 0)
        return rc;

    return ioctl_data.u.getslot.slot;
}

int weka_proto_free_slot(int slot)
{
    if (_fd < 0)
        return -EBADF;

    weka_proto_ioctl_t ioctl_data;
    ioctl_data.op = WEKA_PROTO_OP_PUTSLOT;
    ioctl_data.u.putslot.slot = slot;
    int rc = ioctl(_fd, WEKA_PROTO_IOCTL , &ioctl_data);
    if (rc < 0)
        return rc;

    return 0;
}

int weka_proto_get_info(int slot, weka_proto_key_t ikey, char *data, int datalen)
{
     int i;

     if (_fd < 0)
         return -EBADF;

     char *loc = _shmem + slot * WEKA_PROTO_SLOT_SIZE;
     weka_proto_keymap_table_t *table = (weka_proto_keymap_table_t *)loc;
     for (i=0; i < WEKA_PROTO_MAX_KEYS; i++) {
         if (table->keys[i] == ikey) {
         char *source_loc = loc + sizeof(weka_proto_keymap_table_t) + i * WEKA_PROTO_MAX_DATA;
         memcpy(data, source_loc, datalen);
        return 0;
        }
    }

    return -1;
}

int weka_proto_set_info(int slot, weka_proto_key_t ikey, char *data, int datalen)
{
    int i;
    char *loc = _shmem + slot * WEKA_PROTO_SLOT_SIZE;

    if (_fd < 0)
        return -EBADF;

    weka_proto_keymap_table_t *table = (weka_proto_keymap_table_t *)loc;
    int found = -1;
    for (i=0; i < WEKA_PROTO_MAX_KEYS; i++) {
        if (table->keys[i] == ikey) {
            found = i;
            break;
        } else if (table->keys[i] == WEKA_PROTO_KEY_NONE) {
            found = i;
            continue;
        }
    }
    if (found != -1) {
        table->keys[found] = ikey;
        char *target_loc = loc + sizeof(weka_proto_keymap_table_t) + found * WEKA_PROTO_MAX_DATA;
        memcpy(target_loc, data, datalen);
        return 0;
    }

    return -1;
}

int weka_proto_clear_info(int slot, weka_proto_key_t ikey)
{
    int i;
    char *loc = _shmem + slot * WEKA_PROTO_SLOT_SIZE;

    if (_fd < 0)
        return -EBADF;

    weka_proto_keymap_table_t *table = (weka_proto_keymap_table_t *)loc;
    for (i=0; i < WEKA_PROTO_MAX_KEYS; i++) {
        if (table->keys[i] == ikey) {
            table->keys[i] = WEKA_PROTO_KEY_NONE;
            return 0;
        }
    }

    return -1;
}

int weka_proto_get_endpoints_info(int slot, weka_proto_endpoints_info_t *data) 
{
    if (_fd < 0)
         return -EBADF;

    return weka_proto_get_info(slot, WEKA_PROTO_KEY_ENDPOINTS, (char *)data, sizeof(weka_proto_endpoints_info_t));
}

int weka_proto_set_endpoints_info(int slot, weka_proto_endpoints_info_t *data)
{
    if (_fd < 0)
         return -EBADF;

    return weka_proto_set_info(slot, WEKA_PROTO_KEY_ENDPOINTS, (char *)data, sizeof(weka_proto_endpoints_info_t));
}

int weka_proto_clear_endpoints_info(int slot)
{
    if (_fd < 0)
         return -EBADF;

    return weka_proto_clear_info(slot, WEKA_PROTO_KEY_ENDPOINTS);
}
