# buffer

## 核心概念

1. **运行周期**：程序执行的时间间隔（FreeRTOS 任务周期或 while 循环周期），记为 t。
2. **存储区**：`buf_t` 结构体中的固定长度队列（默认由宏 `BUFFER_SIZE` 决定，头文件中默认值为 32）。
3. **存储次数**：调用时的延迟参数 time（对应文档中以前称为 n）。函数会在内部把超出范围的 time 截取为 `size - 1`。
4. **存储周期**：队列完全更新的时间跨度 T = t × time。
5. **数据流**：
   - 输入：实时数据 `data`
   - 输出：time 周期前存储的延迟数据或通过其它 API 获取历史/滑窗统计

## API 概览

```c
buf_t *BUFRegister(void);
float BUFUpdata(buf_t *_buf, float n, uint8_t time);
float BUFGetHistory(buf_t *_buf, uint8_t steps_back);
float BUFMovingAverage(buf_t *_buf, uint8_t window_size);
float BUFWeightedAverage(buf_t *_buf, const float *weights, uint8_t window_size);
uint8_t BUFIsReady(buf_t *_buf);
void BUFFree(buf_t *_buf);
```

## 使用方法（示例）

```c
// 定义缓冲区指针
static buf_t *buffer_yaw, *buffer_pitch; 
static float aligned_total_yaw, aligned_total_pitch;

void init(void)
{
    // 分配并初始化缓冲区（malloc + memset）
    buffer_yaw = BUFRegister();
    buffer_pitch = BUFRegister();
}

void task(void) // 周期执行的任务（示例）
{
    // 调用 BUFUpdata：存入当前值，并返回 time 周期前的值（若未准备好则返回 0）
    aligned_total_yaw = BUFUpdata(buffer_yaw, gimbal_fetch_data.yaw, 85);
    aligned_total_pitch = BUFUpdata(buffer_pitch, gimbal_fetch_data.pitch, 85);
}
```

## 读取历史与统计示例

```c
// 直接按步数读取历史数据（0 表示上一次写入的数据)
float prev = BUFGetHistory(accel_buf, 0);

// 移动平均（window_size 不得大于 buffer->size）
float ma = BUFMovingAverage(accel_buf, 8);

// 加权平均（weights 长度为 window_size）
float weights[3] = {0.2f, 0.3f, 0.5f};
float wa = BUFWeightedAverage(accel_buf, weights, 3);
```

## 实现与行为要点

1. BUFRegister 会分配并清零一个 buf_t，默认字段：
   - id：写索引
   - od：读索引（实现里用于返回历史）
   - size：等于宏 BUFFER_SIZE（默认 32）
   - data_count：已写入的数据计数（最多为 size）

2. BUFUpdata 的重要行为：
   - 如果 !_buf 或 size == 0，BUFIsReady 返回假，BUFUpdata 返回 0.0f。
   - 当 time >= size 时，内部会把 time 截取为 `size - 1`（安全保护）。
   - BUFUpdata 会写入新数据并返回按照内部逻辑计算出的历史值（实现中使用 id/od 环绕索引）。

3. 安全与限制：
   - 默认队列长度由 BUFFER_SIZE 控制；若需要更大历史深度，请在编译时定义更大的 BUFFER_SIZE。
   - 虽然实现对 time 做了截断，但调用方应避免传入不合理的 time（例如远大于期望值），并在高可靠场景下做输入校验。
   - 在不再使用时请调用 BUFFree 释放资源。

## 性能与设计建议

1. 单缓冲适用于小延迟需求（BUFFER_SIZE 默认 32）。
2. 如果需要非常大的延迟或层级缓存，考虑多级缓冲或外部存储。
3. 对实时系统，BUFIsReady 可用于检查返回值的有效性；若需严格不返回 0 值，必须额外判断 data_count 是否达到期望窗口大小。
