function s = open_uart(com)
    if com == 'auto'
        % ��ȡ���õĴ�����Ϣ
        ports = instrhwinfo('serial');
        % ��ӡ���õĴ��ڶ˿��б�
        disp('��ǰ���õĴ��ڶ˿�:');
        disp(ports.SerialPorts);
        com = cell2mat(ports.SerialPorts(end));
    else
        com = ['COM', num2str(com)];
    end

    s = serial(com, 'BaudRate', 115200); % ����ʵ���������COM�˿ںͲ�����
%     disp(s);
    fopen(s);
end
