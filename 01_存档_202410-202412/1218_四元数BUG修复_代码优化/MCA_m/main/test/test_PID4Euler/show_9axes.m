nine_axes_data = csv2array();

N = get_public_N();
time=(0:1:N-1);

L = 9;

% accel
figure_hold_box();
for i = 1:3
    plot(time, nine_axes_data(:,i));
end
legend( ...
    'a x' , ...
    'a y' , ...
    'a z'   ...
);
title('accel');


% gyro
figure_hold_box();
for i = 1:3
    plot(time, nine_axes_data(:,i+3));
end
legend( ...
    'g x' , ...
    'g y' , ...
    'g z'   ...
);
title('gyro');


% magnetic
figure_hold_box();
for i = 1:3
    plot(time, nine_axes_data(:,i+6));
end
legend( ...
    'm x' , ...
    'm y' , ...
    'm z'   ...
);
title('magnetic');
