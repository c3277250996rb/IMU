@echo off
setlocal enabledelayedexpansion

:: ���㵱ǰĿ¼���ܴ�С����λ���ֽڣ�
for /f "tokens=3" %%a in ('dir /s /-c ^| find "���ļ�"') do set size=%%a

:: ȥ�����ţ�����У�
set size=%size:,=%

:: ����200MB���ֽ���
set /a limit=200*1024*1024

:: �Ƚϴ�С
if %size% GTR %limit% (
    echo ��ǰĿ¼��С����200MB�����س��������˳���
    pause
    exit /b
)

echo ��ǰĿ¼��Сδ����200MB������ִ�С�
:: �ڴ˴��������ִ�е�����


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
