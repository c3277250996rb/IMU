function none = close_com()
    % ��ȡ��ǰϵͳ�е����д���
    ports = instrfind;

    % ѭ���ͷ�ÿ������
    for i = 1:length(ports)
        disp(ports(i));
        fclose(ports(i));
    end

    disp('close_uart finished!');
    none = 0;
end
