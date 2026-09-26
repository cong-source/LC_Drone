# LC_Drone

基于 **STM32F103C8T6** 的无人机控制项目（Keil MDK-ARM + STM32CubeMX）。

## 硬件平台

| 模块 | 型号 / 说明 |
|---|---|
| 主控 MCU | STM32F103C8T6 |
| 姿态传感器 (IMU) | MPU6050 |
| 测距传感器 | VL6180X / TOF050C / TOF400F |
| 电机驱动 | TB6612 |
| 通信 | 蓝牙（USART） |
| 显示 | OLED |

## 功能模块

- **姿态解算**：MPU6050 数据采集与 IMU 解算（`Com/IMU`）
- **闭环控制**：PID 控制器（`Com/Com_PID`）
- **数字滤波**：传感器数据滤波（`Com/Com_Filter`）
- **电机驱动**：TB6612 直流电机控制（`Int/Int_TB6612`）
- **距离检测**：VL6180X / TOF 激光测距（`Int/Int_TOF050C`、`Int/Int_TOF400F`、`Adafruit_VL6180X`）
- **蓝牙通信**：遥控 / 上位机通信（`Int/bluetooth`）
- **OLED 显示**：状态显示（`Int/OLED`）

## 目录结构

```
lc-Drone/
├── App/          # 应用层（App_Drone 主任务）
├── Com/          # 通用组件（PID、滤波、IMU 解算）
├── Core/         # STM32 内核与初始化（CubeMX 生成）
├── Drivers/      # HAL 库 + CMSIS
├── Int/          # 外设接口驱动（电机、传感器、蓝牙、OLED）
├── MDK-ARM/      # Keil 工程（CAR_HAL.uvprojx）
├── doc/          # 原理图等文档
└── smt/          # 器件资料
```

## 开发环境

- **IDE**：Keil MDK-ARM 5
- **代码生成**：STM32CubeMX
- **目标芯片**：STM32F103C8T6

## 编译

用 Keil 打开 `MDK-ARM/CAR_HAL.uvprojx`，编译目标 `CAR_HAL`。

## 许可证

[MIT](./LICENSE)
