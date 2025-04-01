import pandas as pd
import os

# 1.手动修改
if True:
    def fun_manual():
        # 定义文件路径
        file_path = input('输入csv路径: ')

        # 加载 CSV 文件
        df = pd.read_csv(file_path)

        # 查看原始数据（可选）
        print("Original DataFrame:")
        print(df)

        # 指定列名和替换的字符串
        column_number = 2  # 你要修改的列名
        new_value = input('输入要替换的字符串: ')  # 替换为的字符串

        # 修改第二列（假设第二列的列名是 'Column2'，如果你知道列名的话）
        df.iloc[:, (column_number - 1)] = new_value  # 使用 .iloc 访问第二列并替换为 "y"

        # 查看修改后的数据（可选）
        print("\nModified DataFrame:")
        print(df)

        # 将修改后的 DataFrame 保存回 CSV 文件
        df.to_csv(file_path, index=False)

        print("\nCSV file has been updated.")

# 2.自动修改
if True:
    def get_string_between(s, start_substring, end_substring):
        start_index = s.find(start_substring)
        end_index = s.find(end_substring, start_index + len(start_substring))
        
        if start_index != -1 and end_index != -1:
            return s[start_index + len(start_substring): end_index]
        return None  # 如果找不到子字符串，返回 None
    # # 示例
    # s = "This is a sample string with [start]content[/end] between tags."
    # result = get_string_between(s, "[start]", "[/end]")
    # print(result)  # 输出: content

    
    def fun_automatic(directory_path, ext, column_number):
        
        # 获取指定目录下的所有文件
        csv_list = [file for file in os.listdir(directory_path) if file.endswith(ext)]
        
        for i in csv_list:
            # 定义文件路径
            file_path = f'{directory_path}\\{i}'

            # 加载 CSV 文件
            df = pd.read_csv(file_path)

            # 查看原始数据（可选）
            print("Original DataFrame:")
            print(df)

            # 指定列名和替换的字符串
            # column_number = 2  # 你要修改的列名
            # new_value = input('输入要替换的字符串: ')  # 替换为的字符串
            new_value = get_string_between(i, '_备注_', '-时间_')

            # 修改第二列（假设第二列的列名是 'Column2'，如果你知道列名的话）
            df.iloc[:, (column_number - 1)] = new_value  # 使用 .iloc 访问第二列并替换为 "y"

            # 查看修改后的数据（可选）
            print("\nModified DataFrame:")
            print(df)

            # 将修改后的 DataFrame 保存回 CSV 文件
            df.to_csv(file_path, index=False)

            print("\nCSV file has been updated.")

    fun_automatic('encode\\fixed_csv', '.csv', 2)


