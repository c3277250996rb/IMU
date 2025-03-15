SetWorkingDir, %A_ScriptDir%

Gui, +AlwaysOnTop +ToolWindow -MaximizeBox -MinimizeBox  ; 设置窗口置顶，去除最大化和最小化按钮
Gui, Add, Button, x50 y50 w100 h30 gOpen, Open  ; 创建 Open 按钮，点击时执行 Open 标签的代码
Gui, Add, Button, x50 y100 w100 h30 gClose, Close  ; 创建 Close 按钮，点击时执行 Close 标签的代码
Gui, Show, w200 h200, MyWindow  ; 显示窗口，宽度 200，高度 200，标题为 MyWindow
return


^F5::
    function_show()
return

F6::
    ExitApp  ; 退出应用程序，关闭窗口
return

F11::
    function_open()
return

F12::
    function_close()
return



Open:
    ; MsgBox, Open button clicked!  ; 可以在这里执行你的“打开”脚本操作
    function_open()
return

Close:
    ; ExitApp  ; 关闭应用程序
    function_close()
return




function_open(){
    ; 获取操作系统版本
    osVersion := A_OSVersion
    StringSplit, parts, osVersion, .

    ; Windows 10 从内部版本 10240 开始，以内部版本 19044 结束。
    ; Windows 11 从内部版本 22000 开始

    ; 检查操作系统版本
    if (parts3 >= 22000){
        ; MsgBox, 操作系统版本: %parts3%, 这是 Windows 11
        Run, powershell.exe -ExecutionPolicy Bypass -File ".\com.ps1"
    }else if (parts3 >= 10240){
        ; MsgBox, 操作系统版本: %parts3%, 这是 Windows 10
        Run, open.cmd
    }else{
        MsgBox, 操作系统版本未知: %parts3%
    }

}

function_close(){
    Run, close.cmd, , Hide
}

function_show(){
    Gui, Show  ; 通过快捷键显示窗口
}

function_hide(){
    Gui, Hide  ; 隐藏 GUI 
}







