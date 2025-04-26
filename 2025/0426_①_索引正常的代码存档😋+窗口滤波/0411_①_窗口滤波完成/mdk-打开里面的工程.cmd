@REM cmd 打开当前目录以及子目录下的第一个指定扩展名.uvprojx的文件
for /r %%i in (*.uvprojx) do (start "" "%%i" & exit)
