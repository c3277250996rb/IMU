tic; % ��ʼ��ʱ
addpath('tools');
addpath('data');
addpath('scripts');

% �ɼ�����
% disp('[start]:  6axis2csv');
% run('6axis2csv');
% disp('[finish]: 6axis2csv');

% ��ת���� - ������Ʈ
disp('[start]:  rotate_accel');
run('rotate_accel');
disp('[finish]: rotate_accel');

% UKF
disp('[start]:  triple_icd');
run('triple_icd');
disp('[finish]: triple_icd');

figure; icd('y');legend('ֱ�Ӽ���V','UKF��V','ֱ�Ӽ���S','UKF��S');


elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);
