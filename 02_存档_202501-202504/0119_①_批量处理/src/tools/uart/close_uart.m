function none = close_uart()
    % ��ȡ��ǰϵͳ�е����д���
    ports = instrfind;

    % ѭ���ͷ�ÿ������
    for i = 1:length(ports)
        s = ports(i);
        disp(['�˿�: ', num2str(s.Port), ', ������: ', num2str(s.BaudRate)]);
        fclose(s);
    end

    disp('close_uart finished!');
    none = 0;
end
