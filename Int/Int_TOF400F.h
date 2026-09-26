#ifndef __INT_TOF400F_H
#define __INT_TOF400F_H

#include "main.h"

/* TOF400F UART 配置 (默认串口自动输出模式) */
#define TOF400F_BAUDRATE        115200

void Int_TOF400F_Init(void);
void Int_TOF400F_PrintDistance(void);

#endif /* __INT_TOF400F_H */
