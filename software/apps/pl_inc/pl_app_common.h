/* SPDX-License-Identifier: MIT
 * Small helpers shared by the Ping Lite cFS app wrappers (cFE v7 API).
 */
#ifndef PL_APP_COMMON_H
#define PL_APP_COMMON_H
#include "cfe.h"
#include "pl_msg.h"

#define PL_PIPE_DEPTH 32
#define PL_RX_TIMEOUT_MS 1000

/* Copies the payload that follows the command header, checking its size. */
static inline int PL_GetPayload(const CFE_SB_Buffer_t *buf, void *dst, size_t len)
{
    CFE_MSG_Size_t sz = 0;
    CFE_MSG_GetSize(&buf->Msg, &sz);
    if (sz != sizeof(CFE_MSG_CommandHeader_t) + len) return 0;
    memcpy(dst, (const uint8 *)buf + sizeof(CFE_MSG_CommandHeader_t), len);
    return 1;
}
#endif
