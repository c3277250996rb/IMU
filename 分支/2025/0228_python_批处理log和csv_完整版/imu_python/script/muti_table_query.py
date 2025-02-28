# muti_table_query.py
# 查询数据库指定列并绘图

import pymysql.cursors
import pprint
 
#数据库链接信息
db = pymysql.connect(
    
    host="192.168.1.5",       # MySQL主机地址
    user="zat",       # 用户名
    password="123456", # 密码
    database="my_db"  # 数据库名

)

# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()



your_column = 'number'
your_table = 'imu_batch'

# 1. 查询指定列并取余100000后为1的元素，存入数组1
cursor.execute(f"SELECT {your_column} FROM {your_table}")
column_values = cursor.fetchall()
# print(column_values)

# 数组1存储余数为1的数字
array1 = [value[0] for value in column_values if value[0] % 100000 == 1]
# print(array1)

if 1:
    # 假设你要筛选的范围是 [min_value, max_value]
    min_value = 1740743020000001
    max_value = 1740743207099999

    # 筛选出范围内的元素
    array2 = [value for value in array1 if min_value <= value <= max_value]
    print(array2)
    array1 = array2


# 2. 查询符合条件的数据行
result = []
for n in array1:
    cursor.execute(f"SELECT * FROM {your_table} WHERE {your_column} >= %s AND {your_column} <= %s", (n, n + 999999))
    result.append(cursor.fetchall())

# 关闭连接
cursor.close()
db.close()

# 输出结果：二维数组
# print(result)
print(type(result))
# pprint.pprint(result)


new_result = []
for datas in result:
    column_index = -2  # 指定要获取的列索引（0-based）
    column_values = [row[column_index] for row in datas]

    new_result.append(column_values)

# print(new_result)


import matplotlib.pyplot as plt

# 假设你有一个数组
data = [1, 3, 2, 5, 4, 6, 8, 7]

add_value = 90

for data2plot in new_result:
    data2plot = [i + add_value for i in data2plot]
    # 创建折线图
    plt.plot(data2plot)

# 添加标题和标签
# plt.title('Line Plot of Array Data')
# plt.xlabel('Index')
# plt.ylabel('Value')

# 显示图像
plt.show()
































