data = readtable('6axis_data.csv');
% data = readtable('adjusted_data.csv');


figure
hold on ;box on

plot(data.roll);
plot(data.pitch);
plot(data.yaw);

legend('roll', 'pitch', 'yaw');









