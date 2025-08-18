# buffer

## 核心概念

1. **运行周期**：程序执行的时间间隔（FreeRTOS 任务周期或 while 循环周期），记为 $t$
2. **存储区**：`buf_t` 结构体中的固定长度队列（`queue[256]`）
3. **存储次数**：队列完全更新所需的调用次数，记为 $n$（**必须满足 $n < 256$**）
4. **存储周期**：队列完全更新的时间跨度 $T = t \times n$
5. **数据流**：
   - 输入：实时数据 `data`
   - 输出：$T$ 时间前存储的延迟数据

## 使用方法

```c
// 定义缓冲区指针
static buf_t *buffer_yaw, *buffer_pitch; 
// 接收延迟数据的变量
static float aligned_total_yaw, aligned_total_pitch; 

void init()
{
    // 初始化缓冲区（自动清零内存）
    buffer_yaw = BUFRegister();
    buffer_pitch = BUFRegister();
}

void task() // 周期执行的任务（建议1KHz+）
{
    // 更新yaw缓冲区：存入当前值，获取85周期前的值
    aligned_total_yaw = BUFUpdata(buffer_yaw, gimbal_fetch_data.yaw, 85);
    
    // 更新pitch缓冲区：存入当前值，获取85周期前的值
    aligned_total_pitch = BUFUpdata(buffer_pitch, gimbal_fetch_data.pitch, 85);
    
    // 此时 aligned_total_xxx 包含 T=85*t 前的历史数据
}
```

## 技术优势

1. **高效循环队列**：
   - O(1) 时间复杂度操作
   - 避免内存重复分配
   - 极低的内存开销（结构体仅 1KB）

2. **相位精确控制**：

   ```mermaid
   timeline
       title 时间对齐效果
       周期 1 ： 存入数据A
       周期 2 ： 存入数据B → 返回数据A
       周期 n+1 ： 存入数据X → 返回数据A（当队列满时）
   ```

3. **跨场景适用**：
   - 实时控制系统（1KHz+）
   - 嵌入式环境（无动态内存分配）
   - 时间敏感型应用

## 典型应用场景

### 1. 高阶预测滤波器

```python
# 伪代码示例：使用历史队列进行状态预测
position = [p0, p1, p2, ..., pn]  # 从buffer获取
velocity = (position[-1] - position[0]) / T
acceleration = (velocity - prev_velocity) / T
next_position = kalman_filter(position, velocity, acceleration)
```

### 2. 通信相位对齐

**问题场景**：

```mermaid
graph LR
    Vision[视觉处理] -->|延迟Δt| CAN[CAN总线]
    CAN --> Motor[电机响应]
    Motor --> Oscillation[系统震荡]
```

**Buffer解决方案**：

```c
// 电控端对齐视觉数据
aligned_vision = BUFUpdata(vision_buf, raw_vision_data, calc_delay(Δt));
set_motor_position(aligned_vision);  // 相位对齐的控制信号
```

### 3. 传感器数据窗口

```c
// 创建加速度历史窗口
buf_t *accel_buf = BUFRegister();

while(1) {
    float current_accel = read_accelerometer();
    float window_accel = BUFUpdata(accel_buf, current_accel, 50);
    
    // 使用50点窗口数据进行FFT分析
    fft_analysis(window_accel);  
}
```

## 注意事项

1. **初始化特性**：
   - 前 $n$ 次调用返回 `0.0f`（缓冲区未满）
   - $n+1$ 次后返回有效历史数据

2. **关键限制**：

   ```c
   // 危险：n 必须小于 256！
   BUFUpdata(buf, data, 255); // 合法
   BUFUpdata(buf, data, 256); // 数组越界！
   ```

3. **性能特性**：
   - 内存占用：1.03KB/buffer（含结构体开销）
   - 执行时间：< 500ns（Cortex-M7 @ 480MHz）

4. **设计建议**：
   - 单缓冲区适用场景：$n$ < 50
   - 大延迟需求：采用多级缓冲串联
   - 时间戳方案：当 $t$ > 1ms 时建议改用硬件时钟对齐
