function s = open_uart(com)
    com = ['COM', num2str(com)];
    s = serial(com, 'BaudRate', 115200); % 根据实际情况调整COM端口和波特率
    fopen(s);
end