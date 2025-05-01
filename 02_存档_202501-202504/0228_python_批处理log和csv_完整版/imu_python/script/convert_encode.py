# convert_encode.py
# 转换encode目录下的编码

import os
import chardet
import codecs

def convert_file_encoding(file_path, from_encoding, to_encoding):
    try:
        # 读取文件并指定原编码
        with codecs.open(file_path, 'r', encoding=from_encoding) as f:
            content = f.read()
        
        # 使用新编码写入文件
        with codecs.open(file_path, 'w', encoding=to_encoding) as f:
            f.write(content)
        print(f"Successfully converted {file_path} from {from_encoding} to {to_encoding}")
    
    except Exception as e:
        print(f"Error converting file {file_path}: {e}")

def get_file_encoding(file_path):
    with open(file_path, 'rb') as f:
        raw_data = f.read()
    result = chardet.detect(raw_data)
    return result['encoding']

def convert_files_in_directory(directory, from_encoding, to_encoding):
    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            # 检测文件的当前编码
            current_encoding = get_file_encoding(file_path)
            if current_encoding.lower() != from_encoding.lower():
                print(f"Skipping {file_path}, expected encoding {from_encoding}, found {current_encoding}.")
                continue
            convert_file_encoding(file_path, from_encoding, to_encoding)

# 设置需要转换的目录和编码格式
directory = 'encode'  # 当前目录
# from_encoding = 'utf-8'  # 源编码
from_encoding = 'utf-16'  # 源编码
# from_encoding = 'ascii'  # 源编码
to_encoding = 'gbk'  # 目标编码

convert_files_in_directory(directory, from_encoding, to_encoding)
