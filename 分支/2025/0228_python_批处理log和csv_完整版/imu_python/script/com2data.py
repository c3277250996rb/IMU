# com2data.py
# 读取串口并保存日志

import serial
import os
import time
from datetime import datetime
import serial.tools.list_ports


# 设置窗口标题（虽然 Python 不直接控制窗口标题，但可用于标记）
# 如果是Windows，可以考虑使用ctypes来更改命令行窗口标题
# os.system('title zat_com')  # Windows平台

# 获取所有可用的串口号
ports = [port.device for port in serial.tools.list_ports.comports()]

# 输出串口号数组
print(f"所有串口号: {ports}")

# 输出数组的最后一个元素
if len(ports) > 0:
    last_port = ports[-1]
    print(f"最后一个串口号: {last_port}")
else:
    print("没有找到可用的串口号。")
    exit()  # 如果没有串口号，退出脚本

# 读取备注
comment = input("(输入0则放日志文件入垃圾桶) 请输入备注: ")
# comment = 0
is_save2TrashCan = 1 if comment == '0' else 0

log_date = datetime.now().strftime(f"%Y-%m-%d_日志_%H-%M-%S_备注_{comment}")
log_year_month = datetime.now().strftime("%Y-%m")
log_year_month_date = datetime.now().strftime("%Y-%m-%d")

if is_save2TrashCan:
    print("放入垃圾桶")
    log_file_name = f"D:/垃圾桶/LOG_垃圾桶/LOG_{log_year_month}/LOG_{log_year_month_date}/{log_date}.log"
else:
    log_file_name = f"C:/ZAT/LOG/LOG_{log_year_month}/LOG_{log_year_month_date}/{log_date}.log"

# 获取文件夹路径（不包括文件名）
folder_path = os.path.dirname(log_file_name)

# 检查文件夹是否存在，如果不存在，则创建文件夹
os.makedirs(folder_path, exist_ok=True)

# 创建文件并写入内容（如果文件已存在，会覆盖）
with open(log_file_name, 'w') as log_file:
    pass  # 文件已创建，不需要额外写入内容

# 串口设置
port_name = last_port  # 串口名称
baud_rate = 115200     # 波特率
# parity = serial.PARITY_NONE  # 校验位
# data_bits = 8          # 数据位
# stop_bits = serial.STOPBITS_ONE  # 停止位
timeout = 1       # 读取超时时间（秒）

# 创建串口对象
# serial_port = serial.Serial(port_name, baud_rate, parity=parity, bytesize=data_bits, stopbits=stop_bits)
serial_port = serial.Serial(port_name, baud_rate, timeout=timeout)

# 关闭串口（确保不会有遗留连接）
serial_port.close()

# 打开串口
serial_port.open()

# 记录开始时间
start_time = time.time()

# 用于计算刷新率的时间间隔和计数器
last_time = start_time
read_count = 0
output_count = 0
refresh_rate = 0

while serial_port.is_open:
    # 如果串口有数据可读取
    if serial_port.in_waiting > 0:
        data = serial_port.readline().decode('gbk').strip()  # 读取一行数据并去除换行符

        current_time = time.time()

        # 判断是否进入10秒之后的阶段
        if current_time - start_time > 10:
            interval = 2  # 10秒后每隔2秒计算一次刷新率
        else:
            interval = 1  # 10秒内每隔1秒计算一次刷新率

        # 每隔设定时间计算刷新率
        if current_time - last_time >= interval:
            if read_count > 0:
                time_elapsed = current_time - last_time
                refresh_rate = read_count / time_elapsed
                # print(f"刷新率: {refresh_rate} 次/秒")

            # 重置计数器
            read_count = 0
            last_time = current_time
        
        # 增加读取计数
        read_count += 1
        output_count += 1

        # 获取输出时间
        output_date = datetime.now().strftime("%Y/%m/%d %H;%M;%S")
        
        # 格式化输出，整数部分补空格到三位，小数保留两位
        output_fps = f"{refresh_rate:6.2f}"
        output_count_str = f"{output_count:8}"
        output_data = f"[{output_date}][{output_fps}][{output_count_str}]:     {data}"

        # 输出接收到的数据
        print(output_data)

        # 将数据追加到日志文件
        with open(log_file_name, 'a') as log_file:
            log_file.write(output_data + '\n')

# 关闭串口
serial_port.close()
print("串口已关闭。")
