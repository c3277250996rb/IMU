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

% 保存图像为PNG文件
% saveas(gcf, 'x-a.png');


% figure
hold on ;box on
% t=(0:1:N);
% plot(t, data.y);

% 保存图像为PNG文件
% saveas(gcf, 'y-a.png');


% figure
hold on ;box on
% t=(0:1:N);
% plot(t, data.z);

% 保存图像为PNG文件
% saveas(gcf, 'z-a.png');  % 也可以保存为 'figure.jpg'、'figure.tif' 等其他格式


% title('accelerate (x,y,z)');
% legend('x-a','y-a','z-a');
% saveas(gcf, 'accelerate.png');  % 也可以保存为 'figure.jpg'、'figure.tif' 等其他格式





