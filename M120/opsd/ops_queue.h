#ifndef __OPS_QUEUE_H__
#define __OPS_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

int OPS_QueueStart(void);
void OPS_QueueStop(void);
void OPS_StateAdd(uint8_t ucState);

#ifdef __cplusplus
}
#endif
#endif