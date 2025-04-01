import subprocess

def fun_run_now(run_now):
    dir = 'script'
    run_now = f'{dir}\\{run_now}'
    print(f'--- start {run_now} ---')
    subprocess.run(['python', run_now])
    print(f'--- end {run_now} ---')


# 1.转换encode目录下编码
fun_run_now('convert_encode.py')

# 2.转换将log格式化至csv
fun_run_now('log2database.py')

# 3.转换修改列备注
fun_run_now('alter_csv_colume.py')

# 4.转换修改列备注
fun_run_now('remove_first_line.py')








