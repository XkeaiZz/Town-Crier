/*
 *  TCP/IP or UDP/IP networking functions
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "Defs.h"
#include "TLSlib.h"
#include "TLSlib_t.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_NET_C)

#include "mbedtls/net_v.h"
#include "mbedtls/net_f.h"

#include <string.h>

#if (defined(_WIN32) || defined(_WIN32_WCE)) && !defined(EFIX64) && \
    !defined(EFI32)

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
/* Enables getaddrinfo() & Co */
#define _WIN32_WINNT 0x0501

#if defined(_MSC_VER)
#if defined(_WIN32_WCE)
#pragma comment( lib, "ws2.lib" )
#else
#pragma comment( lib, "ws2_32.lib" )
#endif
#endif /* _MSC_VER */

#define read(fd,buf,len)        recv(fd,(char*)buf,(int) len,0)
#define write(fd,buf,len)       send(fd,(char*)buf,(int) len,0)
#define close(fd)               closesocket(fd)

static int wsa_init_done = 0;

#else /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

#endif /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */

/* Some MS functions want int and MSVC warns if we pass size_t,
 * but the standard fucntions use socklen_t, so cast only for MSVC */
#if defined(_MSC_VER)
#define MSVC_INT_CAST   (int)
#else
#define MSVC_INT_CAST
#endif

#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#include <stdint.h>

/*
 * Prepare for using the sockets interface
 */

// deleted net_prepare

/*
 * Initialize a context
 */
void mbedtls_net_init( mbedtls_net_context *ctx )
{
    ctx->fd = -1;
}

/*
 * Initiate a TCP connection with host:port and the given protocol
 */
int mbedtls_net_connect( mbedtls_net_context *ctx, const char *host, const char *port, int proto )
{
    int ret;
    ocall_mbedtls_net_connect(&ret, ctx, host, port, proto);
    return( ret );
}

/*
 * Create a listening socket on bind_ip:port
 */
int mbedtls_net_bind( mbedtls_net_context *ctx, const char *bind_ip, const char *port, int proto )
{
    int ret;
    ocall_mbedtls_net_bind(&ret, ctx, bind_ip, port, proto);
    return( ret );

}

// deleted network_would_block

/*
 * Accept a connection from a remote client
 */
int mbedtls_net_accept( mbedtls_net_context *bind_ctx,
                        mbedtls_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len )
{
    printf__x("Not support yet\n");
    return -123;
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
    int ret;
    ocall_mbedtls_net_set_block(&ret, ctx);
    return ret;
}

int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
    int ret;
    ocall_mbedtls_net_set_nonblock(&ret, ctx);
    return ret;
}

/*
 * Portable usleep helper
 */
void mbedtls_net_usleep( unsigned long usec )
{
    ocall_mbedtls_net_usleep (usec);
}

/*
 * Read at most 'len' characters
 */
int mbedtls_net_recv( void *ctx, unsigned char *buf, size_t len )
{
    int ret;
    ocall_mbedtls_net_recv(&ret, (mbedtls_net_context*) ctx, buf, len);
    return ret;
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout( void *ctx, unsigned char *buf, size_t len,
                      uint32_t timeout )
{
    int ret;
    ocall_mbedtls_net_recv_timeout(&ret, (mbedtls_net_context*) ctx, buf, len, timeout);
    return ret;
}

/*
 * Write at most 'len' characters
 */
int mbedtls_net_send( void *ctx, const unsigned char *buf, size_t len )
{
    int ret;
    ocall_mbedtls_net_send(&ret, (mbedtls_net_context*) ctx, buf, len);
    return ret;
}

/*
 * Gracefully close the connection
 */
void mbedtls_net_free( mbedtls_net_context *ctx )
{
    MKOCALL_VOID(mbedtls_net_free, (mbedtls_net_context*) ctx)
}

#endif /* MBEDTLS_NET_C */
