import pandas as pd
import mysql.connector


def fun_import_single_csv(your_file_csv):
    # 读取没有表头的CSV文件
    df = pd.read_csv(your_file_csv, header=None)

    # 连接到MySQL数据库
    conn = mysql.connector.connect(
        host='192.168.1.5',          # 例如 'localhost'
        user='zat',      # MySQL 用户名
        password='123456',  # MySQL 密码
        database='my_db'   # 你的数据库名
    )

    cursor = conn.cursor()

    # 假设目标表有与CSV文件相同的列数
    # 在这里根据你数据库表的结构来调整列名和数据类型
    table_name = 'imu_batch'  # 目标表的名称
    columns = ['number', 'remark', 'time', 'ax', 'ay', 'az', 'gx', 'gy', 'gz', 'mx', 'my', 'mz', 'roll', 'yaw', 'pitch']


    # 插入数据
    # for row in df.itertuples(index=False, name=None):
    for index, row in df.iterrows():
        # 创建插入语句
        sql = f"INSERT INTO {table_name} ({', '.join(columns)}) VALUES ({', '.join(['%s'] * len(row))})"
        
        # 执行插入操作
        cursor.execute(sql, row)

    # 提交事务并关闭连接
    conn.commit()
    cursor.close()
    conn.close()


fun_import_single_csv('encode\\fixed_csv\\2025-02-27_日志_11-09-55_备注_y从-88到-83(05°)_01-时间_1740654596.csv')
