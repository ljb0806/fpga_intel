# DE10 — Nios II 软核最小系统

## 概述

基于 **Terasic DE10-Standard** 开发板（Intel Cyclone V，5CSXFC6D6F31C6）的
**Nios II 软核最小系统**工程，使用 Quartus Prime 18.1 Standard +
Platform Designer（Qsys）搭建。包含完整的 PL 端硬件工程与 Nios II 软件
（Eclipse）工程，以及可直接烧写的固化文件。

## 目录内容

| 路径 | 说明 |
|---|---|
| `DE10_Demo/` | 基础版工程：DE10 + Nios II 最小系统（含固化文件） |
| `DE10_Demo_1.1/` | 完善版工程：已完成、可直接固化的版本 |
| `DE10_搭建_Nios_II_软核教程.pdf` | 从零搭建本软核系统的完整图文教程 |

> 说明：`db/`、`incremental_db/`、`simulation/` 等 Quartus 编译中间产物与
> Eclipse 元数据（`.metadata/`）、BSP 编译产物（`obj/`）未上传，可在本地
> 重新生成。

## 硬件顶层

顶层 `DE10_Demo.v` 只做引脚引出，全部功能封装在 `nios` 软核中：

```verilog
module DE10_Demo (
    input        clk,     // 50MHz 板载时钟
    input        rst_n,   // 复位（低有效）
    input        RX,      // UART 接收
    output       TX,      // UART 发送
    output [7:0] LED      // LED 输出
);
```

## Nios II 软核配置（nios.qsys）

最小系统包含以下组件（时钟 50 MHz）：

| 组件 | IP | 地址空间 | 说明 |
|---|---|---|---|
| cpu | Nios II Gen2 | — | 处理器核 |
| onchip_ram | On-Chip Memory | 0x10000 – 0x1A000 | 片上 RAM（约 40 KB），程序/数据存储 |
| uart | UART | 0x21000 | 串口外设（带中断），波特率可配 |
| led | PIO | 0x21020 | 8 位 LED 输出 |
| sysid | System ID | 0x21030 | 系统标识外设 |
| jtag_uart | JTAG UART | 0x21038 | 调试串口 |
| clk_0 | Clock Source | — | 50 MHz 时钟源 |

## Nios II 软件（hello_world）

`software/DE10_Demo/hello_world.c` 实现 UART 中断收发示例：

- 每 1 秒通过 UART 发送一次 `"Hajimi.\n"`
- 注册 UART 接收中断：收到数据后置 `rx_flag` 并**回显**该字节
- 主循环检测到 `rx_flag` 后打印 `"Got Data\n"`

软件工程位于 `software/DE10_Demo`（应用）与 `software/DE10_Demo_bsp`
（BSP 板级支持包，含 HAL/drivers 源码）。

## 固化文件

| 文件 | 位置 | 说明 |
|---|---|---|
| `output_file.jic`（约 16 MB） | `DE10_Demo/` 与 `DE10_Demo_1.1/output_files/` | JTAG Indirect Configuration 文件，烧写至板载 EPCS/EPCQ 配置 Flash，掉电不丢失 |
| `DE10_Demo.sof`（约 6.4 MB） | `output_files/` | SRAM 配置文件，直接通过 JTAG 下载到 FPGA，掉电丢失 |

固化方法：Quartus Programmer → 选择 `.jic` 文件 → 勾选 Program/Configure →
Start。烧写完成后断电重启，FPGA 将从配置 Flash 自动加载软核系统。

## 重新构建步骤

1. 用 Quartus 18.1 打开 `DE10_Demo.qpf`
2. 打开 Platform Designer 载入 `nios.qsys`，按需修改后 Generate
3. 全编译工程（生成 `.sof`）
4. 用 Nios II EDS 打开 `software/` 下的软件工程，编译应用（生成 `.elf`）
5. 如需固化：File → Convert Programming Files，将 `.sof` 与软件
   `.elf`/`onchip_ram.hex` 合并转换为 `.jic`，经 JTAG 烧写至配置 Flash

## 搭建教程

完整从零搭建过程（工程创建、Qsys 组件配置、软件工程、固化）见
`DE10_搭建_Nios_II_软核教程.pdf`。
