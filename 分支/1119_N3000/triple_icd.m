my_char = ['x','y','z'];
sum = 0;

figure
for i = 1:3
    icd(my_char(i));
    temp = iread('share.csv', 1, 1);
    sum = sum + temp^2;
    disp([my_char(i), ': ', num2str(temp)]);
end

disp(['sum: ', num2str(sqrt(sum))]);
title('distance (x,y,z)');
% legend('x','y','z');
legend('x - v', 'x - s','y - v', 'y - s','z - v', 'z - s');

t = [];
t=(0:1:public_N-1);
plot(0, 'k');

saveas(gcf, 'distance.png');

