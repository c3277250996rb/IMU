clear;clc; close all;
% N = 3000-1;
N = public_N;

% data = readtable('3axis_data.csv');
data = readtable('adjusted_data.csv');


figure;
hold on ;box on
t=(0:1:N-1);
data.x = data.x ./ 5000 .* 24 ./ 4;
plot(t, data.x);

% ����ͼ��ΪPNG�ļ�
% saveas(gcf, 'x-a.png');


% figure
hold on ;box on
% t=(0:1:N);
% plot(t, data.y);

% ����ͼ��ΪPNG�ļ�
% saveas(gcf, 'y-a.png');


% figure
hold on ;box on
% t=(0:1:N);
% plot(t, data.z);

% ����ͼ��ΪPNG�ļ�
% saveas(gcf, 'z-a.png');  % Ҳ���Ա���Ϊ 'figure.jpg'��'figure.tif' ��������ʽ


% title('accelerate (x,y,z)');
% legend('x-a','y-a','z-a');
% saveas(gcf, 'accelerate.png');  % Ҳ���Ա���Ϊ 'figure.jpg'��'figure.tif' ��������ʽ





