#Persistent  ; 保持脚本运行

; 文件路径
iniFile := "C:\ProgramData\zat_com.ini"

; 检查文件是否存在，如果不存在则创建
if !FileExist(iniFile)
{
    FileAppend, zat_com, %iniFile%  ; 创建文件并写入默认的字符串
}

; 读取第一行作为窗口匹配字符串
FileRead, fileContent, %iniFile%
Loop, parse, fileContent, `n, `r
{
    ; 获取第一行的内容作为窗口匹配字符串
    matchString := A_LoopField
    break
}

; 设置窗口匹配模式为包含
SetTitleMatchMode, 2

; 设置一个计时器，在10秒后退出脚本
SetTimer, ForceExit, -10000  ; -10000表示10秒后触发ForceExit标签

; 下面的部分是查找并关闭符合条件的窗口
Loop  ; 无限循环直到所有符合条件的窗口都被关闭
{
    WinGet, windows, List  ; 获取所有窗口句柄
    found := false  ; 标记是否找到符合条件的窗口
    
    Loop, %windows%  ; 遍历所有窗口
    {
        thisWindow := windows%A_Index%
        WinGetTitle, title, ahk_id %thisWindow%
        if (InStr(title, matchString))  ; 如果窗口标题包含读取的字符串
        {
            found := true
            WinClose, ahk_id %thisWindow%  ; 关闭窗口
        }
    }
    
    if (!found)  ; 如果没有找到符合条件的窗口
        break  ; 退出循环
}

ExitApp  ; 退出脚本

; 强制退出的函数
ForceExit:
    MsgBox, 10秒到，脚本强制退出！
    ExitApp  ; 退出脚本
