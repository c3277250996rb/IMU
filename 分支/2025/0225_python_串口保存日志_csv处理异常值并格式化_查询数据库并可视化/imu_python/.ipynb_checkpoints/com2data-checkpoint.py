import serial
import os
import time
from datetime import datetime
import serial.tools.list_ports


# ���ô��ڱ��⣨��Ȼ Python ��ֱ�ӿ��ƴ��ڱ��⣬�������ڱ�ǣ�
# �����Windows�����Կ���ʹ��ctypes�����������д��ڱ���
# os.system('title zat_com')  # Windowsƽ̨

# ��ȡ���п��õĴ��ں�
ports = [port.device for port in serial.tools.list_ports.comports()]

# ������ں�����
print(f"���д��ں�: {ports}")

# �����������һ��Ԫ��
if len(ports) > 0:
    last_port = ports[-1]
    print(f"���һ�����ں�: {last_port}")
else:
    print("û���ҵ����õĴ��ںš�")
    exit()  # ���û�д��ںţ��˳��ű�

# ��ȡ��ע
comment = input("(����0�����־�ļ�������Ͱ) �����뱸ע: ")
# comment = 0
is_save2TrashCan = 1 if comment == '0' else 0

log_date = datetime.now().strftime(f"%Y-%m-%d_��־_%H-%M-%S_��ע_{comment}")
log_year_month = datetime.now().strftime("%Y-%m")
log_year_month_date = datetime.now().strftime("%Y-%m-%d")

if is_save2TrashCan:
    print("��������Ͱ")
    log_file_name = f"D:/����Ͱ/LOG_����Ͱ/LOG_{log_year_month}/LOG_{log_year_month_date}/{log_date}.log"
else:
    log_file_name = f"C:/ZAT/LOG/LOG_{log_year_month}/LOG_{log_year_month_date}/{log_date}.log"

# ��ȡ�ļ���·�����������ļ�����
folder_path = os.path.dirname(log_file_name)

# ����ļ����Ƿ���ڣ���������ڣ��򴴽��ļ���
os.makedirs(folder_path, exist_ok=True)

# �����ļ���д�����ݣ�����ļ��Ѵ��ڣ��Ḳ�ǣ�
with open(log_file_name, 'w') as log_file:
    pass  # �ļ��Ѵ���������Ҫ����д������

# ��������
port_name = last_port  # ��������
baud_rate = 115200     # ������
# parity = serial.PARITY_NONE  # У��λ
# data_bits = 8          # ����λ
# stop_bits = serial.STOPBITS_ONE  # ֹͣλ
timeout = 1       # ��ȡ��ʱʱ�䣨�룩

# �������ڶ���
# serial_port = serial.Serial(port_name, baud_rate, parity=parity, bytesize=data_bits, stopbits=stop_bits)
serial_port = serial.Serial(port_name, baud_rate, timeout=timeout)

# �رմ��ڣ�ȷ���������������ӣ�
serial_port.close()

# �򿪴���
serial_port.open()

# ��¼��ʼʱ��
start_time = time.time()

# ���ڼ���ˢ���ʵ�ʱ�����ͼ�����
last_time = start_time
read_count = 0
output_count = 0
refresh_rate = 0

while serial_port.is_open:
    # ������������ݿɶ�ȡ
    if serial_port.in_waiting > 0:
        data = serial_port.readline().decode('gbk').strip()  # ��ȡһ�����ݲ�ȥ�����з�

        current_time = time.time()

        # �ж��Ƿ����10��֮��Ľ׶�
        if current_time - start_time > 10:
            interval = 2  # 10���ÿ��2�����һ��ˢ����
        else:
            interval = 1  # 10����ÿ��1�����һ��ˢ����

        # ÿ���趨ʱ�����ˢ����
        if current_time - last_time >= interval:
            if read_count > 0:
                time_elapsed = current_time - last_time
                refresh_rate = read_count / time_elapsed
                # print(f"ˢ����: {refresh_rate} ��/��")

            # ���ü�����
            read_count = 0
            last_time = current_time
        
        # ���Ӷ�ȡ����
        read_count += 1
        output_count += 1

        # ��ȡ���ʱ��
        output_date = datetime.now().strftime("%Y/%m/%d %H;%M;%S")
        
        # ��ʽ��������������ֲ��ո���λ��С��������λ
        output_fps = f"{refresh_rate:6.2f}"
        output_count_str = f"{output_count:8}"
        output_data = f"[{output_date}][{output_fps}][{output_count_str}]:     {data}"

        # ������յ�������
        print(output_data)

        # ������׷�ӵ���־�ļ�
        with open(log_file_name, 'a') as log_file:
            log_file.write(output_data + '\n')

# �رմ���
serial_port.close()
print("�����ѹرա�")
