#include "bluetooth.h"
#include "Int_TB6612.h"

void Bluetooth_Control(void)
{
    if (uart2_rx_flag)
    {
        uint8_t cmd = uart2_rx_data; /* 快照收到的字节，避免中断后续改写 */
        uart2_rx_flag = 0;
        switch (cmd)
        {
        /* ---- 移动函数（动作词首字母） ---- */
        case 'U':
            Motor_go_up();
            break; /* 上升 */
        case 'D':
            Motor_go_down();
            break; /* 下降 */
        case 'F':
            Motor_go_front();
            break; /* 前进 */
        case 'B':
            Motor_go_back();
            break; /* 后退 */
        case 'L':
            Motor_go_left();
            break; /* 左移 */
        case 'R':
            Motor_go_right();
            break; /* 右移 */
        case 'S':
            Motor_stop();
            break; /* 停机 */
        case 'Q':
            Motor_go_left_rotate();
            break; /* 左旋转 */
        case 'E':
            Motor_go_right_rotate();
            break; /* 右旋转 */
        default:
            break;
        }
    }
}
