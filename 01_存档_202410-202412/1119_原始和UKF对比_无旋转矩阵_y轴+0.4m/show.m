% clear;clc; close all;

% data = readtable('com_data.csv');
% accelerate = data.accel;
% N = 299;
% T = 0.01;

% velocity = zeros(1,N);

% for i = 2:N
%     velocity(i) = my_gfun(velocity(i - 1), accelerate(i - 1), accelerate(i), T);
% end

% figure
% hold on ;box on
% t=(0:1:N-1);
% plot(t, velocity);
% plot(t, accelerate);
% xlabel('Time');
% ylabel('Value');
% title('Real-time Data Plot');



clear;clc; close all;
% N = 3000-1;
N = public_N;

% data = readtable('3axis_data.csv');
data = readtable('adjusted_data.csv');


figure
hold on ;box on
t=(0:1:N-1);
plot(t, data.x);
% xlabel('Time');
% ylabel('Value');
title('x-a');

% 保存图像为PNG文件
% saveas(gcf, 'x-a.png');


% figure
hold on ;box on
% t=(0:1:N);
plot(t, data.y);
% xlabel('Time');
% ylabel('Value');
title('y-a');

% 保存图像为PNG文件
% saveas(gcf, 'y-a.png');


% figure
hold on ;box on
% t=(0:1:N);
plot(t, data.z);
% xlabel('Time');
% ylabel('Value');
% title('z-a');


% 保存图像为PNG文件
% saveas(gcf, 'z-a.png');  % 也可以保存为 'figure.jpg'、'figure.tif' 等其他格式

title('accelerate (x,y,z)');
legend('x-a','y-a','z-a');
saveas(gcf, 'accelerate.png');  % 也可以保存为 'figure.jpg'、'figure.tif' 等其他格式





