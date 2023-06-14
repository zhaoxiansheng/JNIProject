//
// Created by zhaoyu on 2023/6/6.
//

#include "zc_common_init.h"
#include <cstdint>
#include <sys/types.h>
#include <jni.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#pragma clang diagnostic ignored "-Wcast-align"

//system info
int           zx_common_api_level         = 0;
char         *zx_common_os_version        = NULL;
char         *zx_common_abi_list          = NULL;
char         *zx_common_manufacturer      = NULL;
char         *zx_common_brand             = NULL;
char         *zx_common_model             = NULL;
char         *zx_common_build_fingerprint = NULL;
char         *zx_common_kernel_version    = NULL;
long          zx_common_time_zone         = 0;

//app info
char         *zc_common_app_id            = NULL;
char         *zc_common_app_version       = NULL;
char         *zc_common_app_lib_dir       = NULL;
char         *zc_common_log_dir           = NULL;

//process info
pid_t         zc_common_process_id        = 0;
char         *zc_common_process_name      = NULL;
uint64_t      zc_common_start_time        = 0;
JavaVM       *zc_common_vm                = NULL;
jclass        zc_common_cb_class          = NULL;
int           zc_common_fd_null           = -1;

//process statue
//sig_atomic_t  zx_common_native_crashed    = 0;
//sig_atomic_t  zx_common_java_crashed      = 0;

static int    zx_common_crash_prepared_fd = -1;
static int    zx_common_trace_prepared_fd = -1;
