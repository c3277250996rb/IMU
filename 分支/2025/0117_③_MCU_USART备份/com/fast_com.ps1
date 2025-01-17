# $host.ui.RawUI.WindowTitle = "你的窗口标题"
$host.ui.RawUI.WindowTitle = "zat_com"



# 获取当前所有可用的串口号
$ports = [System.IO.Ports.SerialPort]::GetPortNames()

# 输出串口号数组
Write-Output "所有串口号: $ports"

# 输出数组的最后一个元素
if ($ports.Length -gt 0) {
    $lastPort = $ports[-1]
    Write-Output "最后一个串口号: $lastPort"
} else {
    Write-Output "没有找到可用的串口号。"
    pause
    return  # 如果没有串口号，退出脚本
}







# $comment = Read-Host "(输入0则放日志文件入垃圾桶) 请输入备注"
$comment = 0
$is_save2TrashCan = if ($comment -eq 0) {1} else {0}

$log_date = Get-Date -Format "yyyy-MM-dd_日志_HH-mm-ss_备注_$comment"
$log_year_month = Get-Date -Format "yyyy-MM"
$log_year_month_date = Get-Date -Format "yyyy-MM-dd"

if ($is_save2TrashCan) {
    Write-Host "放入垃圾桶"
    $log_file_name = "D:\垃圾桶\LOG_垃圾桶\LOG_$log_year_month\LOG_$log_year_month_date\$log_date.log"
} else {
    $log_file_name = "C:\ZAT\LOG\LOG_$log_year_month\LOG_$log_year_month_date\$log_date.log"
}


# 获取文件夹路径（不包括文件名）
$folderPath = [System.IO.Path]::GetDirectoryName($log_file_name)

# 检查文件夹是否存在，如果不存在，则创建文件夹
if (-not (Test-Path -Path $folderPath)) {
    New-Item -Path $folderPath -ItemType Directory -Force
}

# 创建文件并写入内容（如果文件已存在，会覆盖）
New-Item -Path $log_file_name -ItemType File -Force


# StreamWriter 无法实时追加
# 打开 StreamWriter（如果文件不存在，则会创建）
# $streamWriter = [System.IO.StreamWriter]::new($log_file_name, $true)










# 设置串口参数
$portName = $lastPort  # 串口名称
$baudRate = 115200     # 波特率
$parity = "None"       # 校验位
$dataBits = 8          # 数据位
$stopBits = "One"      # 停止位

# 创建串口对象
$serialPort = New-Object System.IO.Ports.SerialPort($portName, $baudRate, $parity, $dataBits, $stopBits)

# 保险
$serialPort.Close()

# 打开串口
$serialPort.Open()

# 记录开始时间
$startTime = Get-Date

# 用于计算刷新率的时间间隔和计数器
$lastTime = $startTime
$readCount = 0
$outputCount = 0

# 设置等待最大时长（秒）
$timeout = 60
$lastReceivedTime = Get-Date

# clear
while ($serialPort.IsOpen) {

    # 如果串口有数据可读取
    if ($serialPort.BytesToRead -gt 0) {
        $data = $serialPort.ReadLine()  # 读取一行数据

        $currentTime = Get-Date
        $lastReceivedTime = Get-Date

        # 判断是否进入10秒之后的阶段
        if ($currentTime - $startTime -gt (New-TimeSpan -Seconds 10)) {
            $interval = 2 # 10秒后每隔2秒计算一次刷新率
        } else {
            $interval = 1 # 10秒内每隔1秒计算一次刷新率
        }

        # 每隔设定时间计算刷新率
        if (($currentTime - $lastTime).TotalSeconds -ge $interval) {
            if ($readCount -gt 0) {
                $timeElapsed = ($currentTime - $lastTime).TotalSeconds
                # $timeElapsed = ($currentTime - $startTime).TotalSeconds
                $refreshRate = $readCount / $timeElapsed
                # Write-Host "刷新率: $refreshRate 次/秒"
            }

            # 重置计数器
            $readCount = 0
            $lastTime = $currentTime
        }
        # 增加读取计数
        $readCount++
        $outputCount++

        $output_date = Get-Date -Format "yyyy/MM/dd HH;mm;ss"
        # $output_fps  = "{0:00.00}" -f $refreshRate

        # 格式化输出，整数部分补空格到三位，小数保留两位
        # "{0,6}" 表示总速度至少 6 位，前面用空格补齐
        # "F2" 表示保留两位小数
        $output_fps = "{0,6:F2}" -f $refreshRate
        $output_count = "{0,8}" -f $outputCount
        $output_data = "[$output_date][$output_fps][$output_count]:     $data"

        # Write-Host "接收到数据: $data"  # 显示接收到的数据
        Write-Host $output_data  # 显示接收到的数据
        

        # 移除字符串两端的所有空白字符，包括换行符。
        $output_data = $output_data.Trim()

        # 将数据追加到日志文件，使用 Out-File 来追加字符串，自动添加换行符
        $output_data | Out-File -Append -FilePath $log_file_name
        
        # 写入内容，不自动换行
        # $streamWriter.Write($output_data)


    }

    # 检查是否超过超时时间没有接收到数据
    $judgeTimeout_currentTime = Get-Date
    if (($judgeTimeout_currentTime - $lastReceivedTime).TotalSeconds -ge $timeout) {
        
        $output_date = Get-Date -Format "yyyy/MM/dd HH;mm;ss"
        $output_count = "{0,8}" -f $outputCount
        $output_data = "[$output_date][      ][$output_count]:     standby."
        # Write-Host "提示: 在 $timeout 秒内未接收到数据！"
        Write-Host $output_data
        $lastReceivedTime = $judgeTimeout_currentTime  # 重置超时时间
    }

}

# 关闭串口
$serialPort.Close()
Write-Host "串口已关闭。"

# 关闭 StreamWriter
# $streamWriter.Close()