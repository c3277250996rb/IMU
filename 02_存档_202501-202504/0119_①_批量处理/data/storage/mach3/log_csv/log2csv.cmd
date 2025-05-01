@echo off
setlocal enabledelayedexpansion

:: ������ǰĿ¼����Ŀ¼�е�����.log�ļ�
for /r %%F in (*.log) do (
    :: ��ȡ��ǰ�����log�ļ�·��
    set "logfile=%%F"
    echo ���ڴ���: !logfile!
    
    :: ����csv�ļ��У���������ڣ�
    set "csvfolder=%%~dpFlog_csv"
    if not exist "!csvfolder!" (
        mkdir "!csvfolder!"
    )
    
    :: �������ɵ�CSV�ļ�·��
    set "csvfile=!csvfolder!\%%~nxF.csv"
    
    :: ���Ŀ��CSV�ļ��Ƿ��Ѵ��ڣ������������������
    if exist "!csvfile!" (
        echo �����ļ�
        @REM goto :skip
    ) else (
        
        :: ���csv�ļ�
        > "!csvfile!" echo.
        
        :: ����log�ļ���ÿһ��
        for /f "tokens=1,* delims=:" %%a in ('type "!logfile!"') do (
            set "line=%%b"
            
            :: ȥ�����еĿո�
            set "line=!line: =!"
            
            :: ������������д��csv�ļ�
            if defined line (
                echo !line! >> "!csvfile!"
            )
        )

    )


    @REM :skip
)

echo ��ɣ�
pause
