import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 参数配置
POINTS_COUNT = 500       # 一个周期内的点数
AMPLITUDE_YAW = 30.0      # Yaw轴最大偏转角度(度)
AMPLITUDE_PITCH = 30.0    # Pitch轴最大偏转角度(度)
CENTER_YAW = 90.0         # Yaw轴中心位置(度)
CENTER_PITCH = 90.0       # Pitch轴中心位置(度)
FREQUENCY_RATIO = 1.0     # 频率比例
SAVE_TO_FILE = True       # 是否保存到文件
PLOT_TRAJECTORY = True    # 是否绘制轨迹图

def generate_lissajous_trajectory():
    """生成Lissajous轨迹点"""
    # 创建时间序列 (0到2π)
    t = np.linspace(0, 2 * np.pi, POINTS_COUNT)
    
    # 生成Yaw轨迹 (正弦波)
    yaw = CENTER_YAW + AMPLITUDE_YAW * np.sin(FREQUENCY_RATIO * t)
    
    # 生成Pitch轨迹 (余弦波)
    pitch = CENTER_PITCH + AMPLITUDE_PITCH * np.cos(FREQUENCY_RATIO * t)
    
    return np.column_stack((yaw, pitch))

def save_trajectory_to_csv(trajectory):
    """保存轨迹到CSV文件"""
    filename = f"laser_trajectory_{POINTS_COUNT}points.csv"
    np.savetxt(filename, trajectory, delimiter=",", header="yaw,pitch", comments='')
    print(f"轨迹已保存到: {filename}")
    return filename

def plot_trajectory(trajectory):
    """绘制轨迹图"""
    plt.figure(figsize=(15, 10))
    
    # 1. 2D轨迹图
    plt.subplot(2, 2, 1)
    plt.plot(trajectory[:, 0], trajectory[:, 1], 'b-', linewidth=0.5)
    plt.title('2D轨迹图')
    plt.xlabel('Yaw (度)')
    plt.ylabel('Pitch (度)')
    plt.grid(True)
    plt.axis('equal')
    
    # 2. Yaw角度随时间变化
    plt.subplot(2, 2, 2)
    plt.plot(np.arange(POINTS_COUNT), trajectory[:, 0], 'r-', linewidth=0.5)
    plt.title('Yaw角度变化')
    plt.xlabel('时间点')
    plt.ylabel('Yaw (度)')
    plt.grid(True)
    
    # 3. Pitch角度随时间变化
    plt.subplot(2, 2, 3)
    plt.plot(np.arange(POINTS_COUNT), trajectory[:, 1], 'g-', linewidth=0.5)
    plt.title('Pitch角度变化')
    plt.xlabel('时间点')
    plt.ylabel('Pitch (度)')
    plt.grid(True)
    
    # 4. 3D轨迹图
    ax = plt.subplot(2, 2, 4, projection='3d')
    ax.plot(np.arange(POINTS_COUNT), trajectory[:, 0], trajectory[:, 1], 'm-', linewidth=0.5)
    ax.set_title('3D轨迹图')
    ax.set_xlabel('时间点')
    ax.set_ylabel('Yaw (度)')
    ax.set_zlabel('Pitch (度)')
    
    plt.tight_layout()
    plt.savefig('laser_trajectory_plot.png', dpi=300)
    plt.show()

def main():
    print("开始生成激光笔正弦波轨迹...")
    print(f"参数配置:")
    print(f"  点数: {POINTS_COUNT}")
    print(f"  Yaw振幅: {AMPLITUDE_YAW}°, 中心位置: {CENTER_YAW}°")
    print(f"  Pitch振幅: {AMPLITUDE_PITCH}°, 中心位置: {CENTER_PITCH}°")
    print(f"  频率比例: {FREQUENCY_RATIO}")
    
    # 生成轨迹
    trajectory = generate_lissajous_trajectory()
    
    # 打印前10个点
    print("\n前10个轨迹点:")
    print("索引\tYaw(度)\t\tPitch(度)")
    for i in range(10):
        print(f"{i}\t{trajectory[i, 0]:.6f}\t{trajectory[i, 1]:.6f}")
    
    # 保存到文件
    if SAVE_TO_FILE:
        filename = save_trajectory_to_csv(trajectory)
    
    # 绘制轨迹图
    if PLOT_TRAJECTORY:
        print("\n绘制轨迹图中...")
        plot_trajectory(trajectory)
    
    print("\n轨迹生成完成!")

if __name__ == "__main__":
    main()