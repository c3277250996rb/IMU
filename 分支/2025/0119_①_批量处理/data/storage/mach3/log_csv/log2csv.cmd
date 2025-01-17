@echo off
setlocal enabledelayedexpansion

:: 遍历当前目录及子目录中的所有.log文件
for /r %%F in (*.log) do (
    :: 获取当前处理的log文件路径
    set "logfile=%%F"
    echo 正在处理: !logfile!
    
    :: 创建csv文件夹（如果不存在）
    set "csvfolder=%%~dpFlog_csv"
    if not exist "!csvfolder!" (
        mkdir "!csvfolder!"
    )
    
    :: 设置生成的CSV文件路径
    set "csvfile=!csvfolder!\%%~nxF.csv"
    
    :: 检查目标CSV文件是否已存在，如果存在则跳过处理
    if exist "!csvfile!" (
        echo 跳过文件
        @REM goto :skip
    ) else (
        
        :: 清空csv文件
        > "!csvfile!" echo.
        
        :: 处理log文件的每一行
        for /f "tokens=1,* delims=:" %%a in ('type "!logfile!"') do (
            set "line=%%b"
            
            :: 去掉行中的空格
            set "line=!line: =!"
            
            :: 将处理后的内容写入csv文件
            if defined line (
                echo !line! >> "!csvfile!"
            )
        )

    )


    @REM :skip
)

echo 完成！
pause
