function none = close_uart()
    % 获取当前系统中的所有串口
    ports = instrfind;

    % 循环释放每个串口
    for i = 1:length(ports)
        s = ports(i);
        disp(['端口: ', num2str(s.Port), ', 波特率: ', num2str(s.BaudRate)]);
        fclose(s);
    end

    disp('close_uart finished!');
    none = 0;
end
