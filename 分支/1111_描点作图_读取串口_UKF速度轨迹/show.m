clear;clc; close all;

data = readtable('com_data.csv');
accelerate = data.accel;
N = 299;
T = 0.01;

velocity = zeros(1,N);

for i = 2:N
    velocity(i) = my_gfun(velocity(i - 1), accelerate(i - 1), accelerate(i), T);
end

figure
hold on ;box on
t=(0:1:N-1);
plot(t, velocity);
plot(t, accelerate);
xlabel('Time');
ylabel('Value');
title('Real-time Data Plot');

