tic; % ��ʼ��ʱ

% �ɼ�����
disp('[start]:  triple_axis2csv');
run('triple_axis2csv');
disp('[finish]: triple_axis2csv');

% ��ת���� - ������Ʈ
disp('[start]:  te');
run('te');
disp('[finish]: te');

% UKF
disp('[start]:  triple_icd');
run('triple_icd');
disp('[finish]: triple_icd');




elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);
