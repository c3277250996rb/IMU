tic; % ��ʼ��ʱ
addpath('tools');
addpath('data');
addpath('scripts');

% % �ɼ����� - �ű�������ͷ����ʹ������
disp('[start]:  axis2csv');
run('axis2csv');
disp('[finish]: axis2csv');

% % ��ת���� - ������Ʈ
disp('[start]:  rotate_accel');
run('rotate_accel');
disp('[finish]: rotate_accel');

% % UKF
% disp('[start]:  triple_icd');
% run('triple_icd');
% disp('[finish]: triple_icd');

% run('show');
% figure; icd('x'); legend('ֱ�Ӽ���V','UKF��V','ֱ�Ӽ���S','UKF��S');
% icd('x'); title(['x = ', num2str(iread('data\share.csv', 1, 1))]);
icd('x'); title('+/-0.4��'); legend('���ٶ�', '�ٶ�', 'λ��');
% figure; icd('y'); title(['y = ', num2str(iread('data\share.csv', 1, 1))]);


elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);
