function s = open_uart(com)
    com = ['COM', num2str(com)];
    s = serial(com, 'BaudRate', 115200); % ����ʵ���������COM�˿ںͲ�����
    fopen(s);
end