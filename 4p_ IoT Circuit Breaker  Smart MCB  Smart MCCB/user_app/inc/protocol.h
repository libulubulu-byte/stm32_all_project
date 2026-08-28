/*
 * Copyright © 2019 copyright ShenZhen AFU Smart Internet Technology Co.,Ltd.
 * All Right Reserved.
 *
 * Version Logs:
 * Date         Author         Notes
 * 2019.2.8     Yang Song      first version.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INCLUDES_H_
#define INCLUDES_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "prjlib.h"

/* Exported typedef ----------------------------------------------------------*/
typedef enum
{
  proc_idle,
  proc_scan,
  proc_scan_wait_rsp,
  proc_trans,
  proc_trans_wait_ack,
  proc_trans_reload,
  proc_trans_resend,
  proc_trans_begin,
  proc_cancel,
  proc_resume,
  proc_done,
} ProcStatusEnum;


/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern ProcStatusEnum status;

/* External functions --------------------------------------------------------*/
void ProtocolInit(void);
void ProtocolNewData(char newch);
void ProcPolling(void);
void UartSendDone(void);

#ifdef __cplusplus
}
#endif

#endif
