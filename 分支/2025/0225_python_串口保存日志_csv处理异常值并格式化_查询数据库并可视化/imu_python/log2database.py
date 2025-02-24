import os
import glob

from datetime import datetime
timestamp = datetime.now().timestamp()
timestamp = int(timestamp)
print(timestamp)


# 追加数组
csv2add = []


# 获取当前目录及子目录中的所有 .log 文件
log_files = glob.glob("**/*.log", recursive=True)

# 遍历每一个log文件
for logfile in log_files:
    print(f"正在处理: {logfile}")
    
    # 创建csv文件夹（如果不存在）
    csvfolder = os.path.join(os.path.dirname(logfile), "log_csv")
    if not os.path.exists(csvfolder):
        os.makedirs(csvfolder)
    
    # 设置生成的CSV文件路径
    csvfile = os.path.join(csvfolder, os.path.basename(logfile).replace('.log', '.csv'))
    
    # 检查目标CSV文件是否已存在，如果存在则跳过处理
    if os.path.exists(csvfile):
        print("跳过文件")
        # continue
    # else:
        
    csv2add.append(csvfile)
    
    # 清空csv文件
    with open(csvfile, 'w') as f:
        pass
    
    # # 处理log文件的每一行
    # with open(logfile, 'r') as log:
    #     for line in log:
    #         # 去掉行中的空格
    #         line = line.strip()
            
    #         # 如果行有内容，将处理后的内容写入csv文件
    #         if line:
    #             with open(csvfile, 'a') as f:
    #                 f.write(line + "\n")


    # 处理log文件的每一行
    with open(logfile, 'r') as log:
        for line in log:
            
            # 如果行有内容
            if line:
                time_and_number = line.split(':', 1)[0]
                
                # 1.将 '][' 替换成 ','
                time_and_number = time_and_number.replace('][', ',')
                # 2.将 '[' 去掉
                time_and_number = time_and_number.replace('[', '')
                # 3.将 ']' 去掉
                time_and_number = time_and_number.replace(']', ',')
                # print(time_and_number)
                
                # 将冒号左边的内容去掉，只保留冒号右边的部分
                line = line.split(':', 1)[-1].strip()  # 如果没有冒号，line就保持不变
                
                # 去掉行中的空格
                line = line.strip()
                line = time_and_number + line
                # print(line)
                # 将处理后的内容写入csv文件
                with open(csvfile, 'a') as f:
                    f.write(line + "\n")


print("mpm3l2c 完成！")


# 按照字符串的名称进行排序
csv2add.sort()

# 给每个字符串编号
# numbered_strings = [(index + 1, string) for index, string in enumerate(csv2add)]

# 给每个字符串编号并逐个打印
for index, string in enumerate(csv2add):
    print((index + 1, string))


# 读取编号
input_number = input("输入要处理的编号: ")
input_number = int(input_number) - 1





import pandas as pd

def read_and_clean_csv(file_path):
    # 读取原始文件内容
    # with open(file_path, 'r', encoding='utf-8') as f:
    with open(file_path, 'r', encoding='gbk') as f:
        lines = f.readlines()

    # 用来存储有效行数据
    valid_rows = []

    # 逐行处理文件
    for line in lines:
        # 去除首尾空格并跳过空行
        line = line.strip()
        if not line:
            continue
        
        # 分隔符为逗号
        parts = line.split(',')
        
        # 如果列数不正确，跳过这一行
        if len(parts) != 15:  # 根据你提供的数据格式
            continue
        
        # 尝试将数据转换为所需的格式
        try:
            # 前三列为字符串
            row = [str(parts[0]), str(parts[1]), (int(parts[2]) + 1000000 * timestamp)]
            
            # 后面列为浮点数
            for part in parts[3:]:
                row.append(float(part))
            
            valid_rows.append(row)
        except ValueError:
            # 如果有无法转换为float的值，跳过这一行
            continue

    # 将有效行数据转换为DataFrame
    df_cleaned = pd.DataFrame(valid_rows)

    # 假设 df_cleaned 已经存在，并且包含了数据
    # 交换第1列和第3列
    df_cleaned = df_cleaned[[df_cleaned.columns[2], df_cleaned.columns[1]] + [col for col in df_cleaned.columns if col not in [df_cleaned.columns[2], df_cleaned.columns[1]]]]

    # 去掉第一行
    df_cleaned = df_cleaned.reset_index(drop=True)


    output_file_path = csv2add[input_number]
    output_file_path = output_file_path.replace('log_csv', 'fixed_csv')
    df_cleaned.to_csv(output_file_path, index=False)

    print(f"清洗后的数据已保存为 {output_file_path}")

    return df_cleaned

# 示例文件路径
file_path = csv2add[input_number]

# 调用函数
cleaned_df = read_and_clean_csv(file_path)

# 打印清洗后的数据
# print(cleaned_df)



import pandas as pd

# 读取并打印指定路径的 CSV 文件
def read_and_print_csv(file_path):
    # 读取 CSV 文件
    df = pd.read_csv(file_path)

    # 打印 DataFrame
    print(df)


output_file_path = csv2add[input_number]
output_file_path = output_file_path.replace('log_csv', 'fixed_csv')
# 示例文件路径
file_path = output_file_path  # 请替换为你的文件路径

# 调用函数
read_and_print_csv(file_path)



