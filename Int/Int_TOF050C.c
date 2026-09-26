#include "Int_TOF050C.h"
#include "usart.h"
#include <stdio.h>

/* ================================================================
 *  VL6180X 距离传感器驱动 — I2C1 (PB8=SCL, PB9=SDA)
 *
 *  参考: Adafruit_VL6180X 库 (Arduino) → HAL 库移植
 *
 *  注意: VL6180X 使用 16 位寄存器地址 (不同于 MPU6050 的 8 位),
 *        因此 HAL_I2C_Mem_Read/Write 的第三个参数必须使用
 *        I2C_MEMADD_SIZE_16BIT
 * ================================================================ */

/* ---- 内部私有寄存器 (application note AN4545) ---- */
#define VL6180X_SYSRANGE__INTERMEASUREMENT_PERIOD  0x001B

/* ================================================================
 *  底层 I2C 读写 — 16位寄存器地址
 * ================================================================ */

/**
 * @brief  从 VL6180X 的 16 位寄存器地址读取 1 字节
 * @param  reg_addr: 16位寄存器地址
 * @param  data:     读出数据存放指针
 * @return HAL 状态码 (HAL_OK = 成功)
 */
static uint8_t VL6180X_ReadByte(uint16_t reg_addr, uint8_t *data)
{
    return HAL_I2C_Mem_Read(&hi2c1,
                            VL6180X_DEFAULT_I2C_ADDR << 1,
                            reg_addr,
                            I2C_MEMADD_SIZE_16BIT,
                            data, 1, 2000);
}

/**
 * @brief  向 VL6180X 的 16 位寄存器地址写入 1 字节
 * @param  reg_addr: 16位寄存器地址
 * @param  data:     要写入的数据
 * @return HAL 状态码
 */
static uint8_t VL6180X_WriteByte(uint16_t reg_addr, uint8_t data)
{
    return HAL_I2C_Mem_Write(&hi2c1,
                             VL6180X_DEFAULT_I2C_ADDR << 1,
                             reg_addr,
                             I2C_MEMADD_SIZE_16BIT,
                             &data, 1, 2000);
}

/* ================================================================
 *  VL6180X 私有配置加载 (application note AN4545 page 24)
 * ================================================================ */

/**
 * @brief  加载 VL6180X 测距推荐配置 (私有寄存器 + 公开寄存器)
 * @note   必须在芯片上电复位后调用一次
 *         参考: AN4545 应用笔记
 */
static void VL6180X_LoadSettings(void)
{
    /* ---- 私有寄存器配置 (AN4545 page 24) ---- */
    VL6180X_WriteByte(0x0207, 0x01);
    VL6180X_WriteByte(0x0208, 0x01);
    VL6180X_WriteByte(0x0096, 0x00);
    VL6180X_WriteByte(0x0097, 0xFD);
    VL6180X_WriteByte(0x00E3, 0x00);
    VL6180X_WriteByte(0x00E4, 0x04);
    VL6180X_WriteByte(0x00E5, 0x02);
    VL6180X_WriteByte(0x00E6, 0x01);
    VL6180X_WriteByte(0x00E7, 0x03);
    VL6180X_WriteByte(0x00F5, 0x02);
    VL6180X_WriteByte(0x00D9, 0x05);
    VL6180X_WriteByte(0x00DB, 0xCE);
    VL6180X_WriteByte(0x00DC, 0x03);
    VL6180X_WriteByte(0x00DD, 0xF8);
    VL6180X_WriteByte(0x009F, 0x00);
    VL6180X_WriteByte(0x00A3, 0x3C);
    VL6180X_WriteByte(0x00B7, 0x00);
    VL6180X_WriteByte(0x00BB, 0x3C);
    VL6180X_WriteByte(0x00B2, 0x09);
    VL6180X_WriteByte(0x00CA, 0x09);
    VL6180X_WriteByte(0x0198, 0x01);
    VL6180X_WriteByte(0x01B0, 0x17);
    VL6180X_WriteByte(0x01AD, 0x00);
    VL6180X_WriteByte(0x00FF, 0x05);
    VL6180X_WriteByte(0x0100, 0x05);
    VL6180X_WriteByte(0x0199, 0x05);
    VL6180X_WriteByte(0x01A6, 0x1B);
    VL6180X_WriteByte(0x01AC, 0x3E);
    VL6180X_WriteByte(0x01A7, 0x1F);
    VL6180X_WriteByte(0x0030, 0x00);

    /* ---- 推荐公开寄存器配置 (datasheet) ---- */
    VL6180X_WriteByte(0x0011, 0x10);  /* 使能轮询 "New Sample ready"   */
    VL6180X_WriteByte(0x010A, 0x30);  /* 平均采样周期 (噪声 vs 速度)   */
    VL6180X_WriteByte(0x003F, 0x46);  /* 光/暗增益 (暗增益不可改)      */
    VL6180X_WriteByte(0x0031, 0xFF);  /* 自动校准前的测距次数          */
    VL6180X_WriteByte(0x0041, 0x63);  /* ALS 积分时间 = 100ms          */
    VL6180X_WriteByte(0x002E, 0x01);  /* 执行单次温度校准              */

    /* ---- 可选公开寄存器 ---- */
    VL6180X_WriteByte(VL6180X_SYSRANGE__INTERMEASUREMENT_PERIOD,
                      0x09);          /* 测距间隔 100ms (10ms × 10)   */
    VL6180X_WriteByte(0x003E, 0x31);  /* ALS 间隔 500ms               */
    VL6180X_WriteByte(0x0014, 0x24);  /* 中断配置: New Sample Ready   */
}

/* ================================================================
 *  公开 API
 * ================================================================ */

/**
 * @brief  VL6180X 初始化 — 检测芯片并加载配置
 * @note   必须在上电后调用一次
 *         1. 读取 MODEL_ID 寄存器 (应为 0xB4)
 *         2. 检测 FRESH_OUT_OF_RESET 标志
 *         3. 加载私有 + 公开寄存器配置
 */
void Int_TOF050C_Init(void)
{
    uint8_t model_id = 0;

    /* 1. 读取芯片型号 ID (地址 0x000, 应为 0xB4) */
    VL6180X_ReadByte(VL6180X_REG_IDENTIFICATION_MODEL_ID, &model_id);
    if (model_id != 0xB4)
    {
        /* 芯片未应答或型号不匹配, 通过串口报告 */
        printf("[TOF050C] ERROR: VL6180X not found! Model ID = 0x%02X\r\n", model_id);
        return;
    }

    /* 2. 检查是否为上电复位后的首次初始化 */
    {
        uint8_t fresh_out = 0;
        VL6180X_ReadByte(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET, &fresh_out);
        if (fresh_out & 0x01)
        {
            /* 3. 加载推荐配置 */
            VL6180X_LoadSettings();

            /* 4. 清除 FRESH_OUT_OF_RESET 标志 */
            VL6180X_WriteByte(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET, 0x00);
        }
    }

    printf("[TOF050C] VL6180X initialized successfully.\r\n");
}

/**
 * @brief  读取测距结果的状态码
 * @return 状态码: 0 = 成功, 非0 = 错误 (见 VL6180X_ERROR_* 定义)
 */
uint8_t Int_TOF050C_ReadRangeStatus(void)
{
    uint8_t status = 0;
    VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_STATUS, &status);
    return (status >> 4);  /* 高4位是错误码 */
}

/**
 * @brief  单次测距 — 阻塞式, 带超时保护, 等待测量完成后返回距离
 * @return 距离值 (单位: mm)
 *
 * @note   流程:
 *         1. 等待设备就绪 (RESULT_RANGE_STATUS bit0 = 1), 超时 50ms
 *         2. 触发单次测距 (SYSRANGE_START = 0x01)
 *         3. 轮询等待测量完成 (INTERRUPT_STATUS_GPIO bit2 = 1), 超时 50ms
 *         4. 读取距离值 (RESULT_RANGE_VAL)
 *         5. 清除中断标志
 *
 *         【重要】调用本函数后, 务必用 ReadRangeStatus() 检查本次测量的
 *         状态码, 不要在本函数之前检查（那是上一次测量的状态）。
 */
uint8_t Int_TOF050C_ReadRange(void)
{
    uint8_t data;
    uint32_t timeout;

    /* 1. 等待设备空闲, 带超时保护 */
    timeout = HAL_GetTick() + 50;
    do {
        VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_STATUS, &data);
        if (HAL_GetTick() > timeout)
        {
            /* 设备卡死, 尝试清除中断并恢复到已知状态 */
            VL6180X_WriteByte(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);
            VL6180X_WriteByte(VL6180X_REG_SYSRANGE_START, 0x01);
            return 0;
        }
    } while (!(data & 0x01));

    /* 2. 启动单次测距 */
    VL6180X_WriteByte(VL6180X_REG_SYSRANGE_START, 0x01);

    /* 3. 轮询等待测量完成, 带超时保护 */
    timeout = HAL_GetTick() + 50;
    do {
        VL6180X_ReadByte(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO, &data);
        if (HAL_GetTick() > timeout)
        {
            VL6180X_WriteByte(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);
            return 0;
        }
    } while (!(data & 0x04));

    /* 4. 读取距离值 (单位: mm) */
    uint8_t range = 0;
    VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_VAL, &range);

    /* 5. 清除中断标志 */
    VL6180X_WriteByte(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

    return range;
}

/**
 * @brief  每 100ms 读取一次距离并通过串口1打印
 * @note   应在主循环 while(1) 中周期性调用;
 *         内部使用 HAL_GetTick() 实现非阻塞 100ms 间隔
 *
 *         输出格式: "[TOF050C] Distance: XX mm\r\n"
 *         读取错误时: "[TOF050C] Error code: X\r\n"
 */
void Int_TOF050C_PrintDistance(void)
{
    static uint32_t last_tick = 0;
    uint32_t now = HAL_GetTick();

    /* 非阻塞 100ms 定时 */
    if (now - last_tick < 100)
    {
        return;
    }
    last_tick = now;

    /* 【关键顺序】先触发新测量, 再检查状态!
     *
     *  错误写法: ReadRangeStatus() → 读的是上一次的旧状态, 如果上次出错,
     *            直接 return 不触发新测量, 状态寄存器永不更新 → 死锁。
     *  正确写法: ReadRange() → 先触发一次全新的测量, 再 ReadRangeStatus()
     *            读本次结果, 每次都是新鲜数据。
     */
    uint8_t distance = Int_TOF050C_ReadRange();
    uint8_t status   = Int_TOF050C_ReadRangeStatus();

    if (status != VL6180X_ERROR_NONE)
    {
        printf("[TOF050C] Error code: %d\r\n", status);
        return;
    }

    printf("[TOF050C] Distance: %d mm\r\n", distance);
}
