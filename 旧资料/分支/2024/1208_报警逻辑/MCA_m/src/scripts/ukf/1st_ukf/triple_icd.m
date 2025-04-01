addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));

my_char = ['x','y','z'];
sum = 0;
temp = zeros(3);

figure
for i = 1:3
    icd(my_char(i));
    temp(i) = iread('data\share.csv', 1, 1);
    sum = sum + temp(i)^2;
    disp([my_char(i), ': ', num2str(temp(i))]);
end

disp(['sum: ', num2str(sqrt(sum))]);
this_title = sprintf('distance=%.2f (x=%.2f, y=%.2f, z=%.2f)', sqrt(sum), temp(1), temp(2), temp(3));
title(this_title);
legend('x - v', 'x - s','y - v', 'y - s','z - v', 'z - s');

t = [];
t=(0:1:public_N-1);
plot(0, 'k');

saveas(gcf, 'distance.png');

