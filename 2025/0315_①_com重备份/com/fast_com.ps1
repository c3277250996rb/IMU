# $host.ui.RawUI.WindowTitle = "��Ĵ��ڱ���"
$host.ui.RawUI.WindowTitle = "zat_com"



# ��ȡ��ǰ���п��õĴ��ں�
$ports = [System.IO.Ports.SerialPort]::GetPortNames()

# ������ں�����
Write-Output "���д��ں�: $ports"

# �����������һ��Ԫ��
if ($ports.Length -gt 0) {
    $lastPort = $ports[-1]
    Write-Output "���һ�����ں�: $lastPort"
} else {
    Write-Output "û���ҵ����õĴ��ںš�"
    pause
    return  # ���û�д��ںţ��˳��ű�
}







# $comment = Read-Host "(����0�����־�ļ�������Ͱ) �����뱸ע"
$comment = 0
$is_save2TrashCan = if ($comment -eq 0) {1} else {0}

$log_date = Get-Date -Format "yyyy-MM-dd_��־_HH-mm-ss_��ע_$comment"
$log_year_month = Get-Date -Format "yyyy-MM"
$log_year_month_date = Get-Date -Format "yyyy-MM-dd"

if ($is_save2TrashCan) {
    Write-Host "��������Ͱ"
    $log_file_name = "D:\����Ͱ\LOG_����Ͱ\LOG_$log_year_month\LOG_$log_year_month_date\$log_date.log"
} else {
    $log_file_name = "C:\ZAT\LOG\LOG_$log_year_month\LOG_$log_year_month_date\$log_date.log"
}


# ��ȡ�ļ���·�����������ļ�����
$folderPath = [System.IO.Path]::GetDirectoryName($log_file_name)

# ����ļ����Ƿ���ڣ���������ڣ��򴴽��ļ���
if (-not (Test-Path -Path $folderPath)) {
    New-Item -Path $folderPath -ItemType Directory -Force
}

# �����ļ���д�����ݣ�����ļ��Ѵ��ڣ��Ḳ�ǣ�
New-Item -Path $log_file_name -ItemType File -Force


# StreamWriter �޷�ʵʱ׷��
# �� StreamWriter������ļ������ڣ���ᴴ����
# $streamWriter = [System.IO.StreamWriter]::new($log_file_name, $true)










# ���ô��ڲ���
$portName = $lastPort  # ��������
$baudRate = 115200     # ������
$parity = "None"       # У��λ
$dataBits = 8          # ����λ
$stopBits = "One"      # ֹͣλ

# �������ڶ���
$serialPort = New-Object System.IO.Ports.SerialPort($portName, $baudRate, $parity, $dataBits, $stopBits)

# ����
$serialPort.Close()

# �򿪴���
$serialPort.Open()

# ��¼��ʼʱ��
$startTime = Get-Date

# ���ڼ���ˢ���ʵ�ʱ�����ͼ�����
$lastTime = $startTime
$readCount = 0
$outputCount = 0

# ���õȴ����ʱ�����룩
$timeout = 60
$lastReceivedTime = Get-Date

# clear
while ($serialPort.IsOpen) {

    # ������������ݿɶ�ȡ
    if ($serialPort.BytesToRead -gt 0) {
        $data = $serialPort.ReadLine()  # ��ȡһ������

        $currentTime = Get-Date
        $lastReceivedTime = Get-Date

        # �ж��Ƿ����10��֮��Ľ׶�
        if ($currentTime - $startTime -gt (New-TimeSpan -Seconds 10)) {
            $interval = 2 # 10���ÿ��2�����һ��ˢ����
        } else {
            $interval = 1 # 10����ÿ��1�����һ��ˢ����
        }

        # ÿ���趨ʱ�����ˢ����
        if (($currentTime - $lastTime).TotalSeconds -ge $interval) {
            if ($readCount -gt 0) {
                $timeElapsed = ($currentTime - $lastTime).TotalSeconds
                # $timeElapsed = ($currentTime - $startTime).TotalSeconds
                $refreshRate = $readCount / $timeElapsed
                # Write-Host "ˢ����: $refreshRate ��/��"
            }

            # ���ü�����
            $readCount = 0
            $lastTime = $currentTime
        }
        # ���Ӷ�ȡ����
        $readCount++
        $outputCount++

        $output_date = Get-Date -Format "yyyy/MM/dd HH;mm;ss"
        # $output_fps  = "{0:00.00}" -f $refreshRate

        # ��ʽ��������������ֲ��ո���λ��С��������λ
        # "{0,6}" ��ʾ���ٶ����� 6 λ��ǰ���ÿո���
        # "F2" ��ʾ������λС��
        $output_fps = "{0,6:F2}" -f $refreshRate
        $output_count = "{0,8}" -f $outputCount
        $output_data = "[$output_date][$output_fps][$output_count]:     $data"

        # Write-Host "���յ�����: $data"  # ��ʾ���յ�������
        Write-Host $output_data  # ��ʾ���յ�������
        

        # �Ƴ��ַ������˵����пհ��ַ����������з���
        $output_data = $output_data.Trim()

        # ������׷�ӵ���־�ļ���ʹ�� Out-File ��׷���ַ������Զ���ӻ��з�
        $output_data | Out-File -Append -FilePath $log_file_name
        
        # д�����ݣ����Զ�����
        # $streamWriter.Write($output_data)


    }

    # ����Ƿ񳬹���ʱʱ��û�н��յ�����
    $judgeTimeout_currentTime = Get-Date
    if (($judgeTimeout_currentTime - $lastReceivedTime).TotalSeconds -ge $timeout) {
        
        $output_date = Get-Date -Format "yyyy/MM/dd HH;mm;ss"
        $output_count = "{0,8}" -f $outputCount
        $output_data = "[$output_date][      ][$output_count]:     standby."
        # Write-Host "��ʾ: �� $timeout ����δ���յ����ݣ�"
        Write-Host $output_data
        $lastReceivedTime = $judgeTimeout_currentTime  # ���ó�ʱʱ��
    }

}

# �رմ���
$serialPort.Close()
Write-Host "�����ѹرա�"

# �ر� StreamWriter
# $streamWriter.Close()