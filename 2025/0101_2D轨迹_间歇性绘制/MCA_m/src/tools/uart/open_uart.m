function s = open_uart(com)
    if com == 'auto'
        % 获取可用的串口信息
        ports = instrhwinfo('serial');
        % 打印可用的串口端口列表
        disp('当前可用的串口端口:');
        disp(ports.SerialPorts);
        com = cell2mat(ports.SerialPorts(end));
    else
        com = ['COM', num2str(com)];
    end

    s = serial(com, 'BaudRate', 115200); % 根据实际情况调整COM端口和波特率
%     disp(s);
    fopen(s);
end
