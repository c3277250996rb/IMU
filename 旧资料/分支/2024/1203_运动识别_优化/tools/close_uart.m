function none = close_com()
    % 获取当前系统中的所有串口
    ports = instrfind;

    % 循环释放每个串口
    for i = 1:length(ports)
        disp(ports(i));
        fclose(ports(i));
    end

    disp('close_uart finished!');
    none = 0;
end
