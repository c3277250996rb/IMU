@REM @echo off
@REM setlocal enabledelayedexpansion

@REM :: ���㵱ǰĿ¼���ܴ�С����λ���ֽڣ�
@REM for /f "tokens=3" %%a in ('dir /s /-c ^| find "���ļ�"') do set size=%%a

@REM :: ȥ�����ţ�����У�
@REM set size=%size:,=%

@REM :: ����200MB���ֽ���
@REM set /a limit=200*1024*1024

@REM :: �Ƚϴ�С
@REM if %size% GTR %limit% (
@REM     echo ��ǰĿ¼��С����200MB�����س��������˳���
@REM     pause
@REM     exit /b
@REM )

@REM echo ��ǰĿ¼��Сδ����200MB������ִ�С�
@REM :: �ڴ˴��������ִ�е�����


del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
:: ������ɾ��JLINK������
del *.opt /s  

del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s

del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s
exit
