import os

def remove_first_line_from_files(directory_path, extension):
    # 遍历指定目录
    for filename in os.listdir(directory_path):
        if filename.endswith(extension):  # 筛选出指定扩展名的文件
            file_path = os.path.join(directory_path, filename)
            
            # 打开文件，读取内容
            with open(file_path, 'r', encoding='utf-8') as file:
                lines = file.readlines()
            
            # 删除第一行
            if len(lines) > 1:
                lines = lines[1:]
            else:
                lines = []  # 如果只有一行，就变为空文件
            
            # 将修改后的内容写回文件
            with open(file_path, 'w', encoding='utf-8') as file:
                file.writelines(lines)

# 示例：删除指定目录下所有 .txt 文件的第一行
directory_path = 'encode\\fixed_csv'
extension = '.csv'
remove_first_line_from_files(directory_path, extension)
