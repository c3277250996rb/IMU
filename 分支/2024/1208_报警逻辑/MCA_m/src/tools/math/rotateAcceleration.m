function rotatedAcc = rotateAcceleration(input_data) % temp_x, temp_y, temp_z, roll, pitch, yaw

       is_use_uart = get_is_use_uart();

       [length_row, length_column] = size(input_data);
       rotatedAcc = zeros(length_row, 3);

       disp(input_data(1:6,1:6));

       for i = 1:length_row
              % 取出单个数据
                     if is_use_uart
                            Var2_double = (input_data(i,:));
                     else
                            Var2_double = table2array(input_data(i,:));
                     end
                     % disp(class(input_data(i, 4)));  % 检查数据类型
                     % disp(class(str2double(input_data(i, 4))));  % 检查数据类型
              
                     temp_x = (Var2_double(1));
                     temp_y = (Var2_double(2));
                     temp_z = (Var2_double(3));
                     roll   = (Var2_double(4));
                     pitch  = (Var2_double(5));
                     yaw    = (Var2_double(6));
              %

              % 将角度转换为弧度
              roll  = deg2rad((roll));
              pitch = deg2rad((pitch));
              yaw   = deg2rad((yaw));

              % 构造旋转矩阵
              R_x = [1, 0, 0;
                     0, cos(roll), -sin(roll);
                     0, sin(roll), cos(roll)];
              
              R_y = [cos(pitch), 0, sin(pitch);
                     0, 1, 0;
                     -sin(pitch), 0, cos(pitch)];
              
              R_z = [cos(yaw), -sin(yaw), 0;
                     sin(yaw), cos(yaw), 0;
                     0, 0, 1];

              % 总的旋转矩阵（Z-Y-X顺序）
              R = R_z * R_y * R_x;

              % 旋转加速度
              acc = [temp_x; temp_y; temp_z];

              % R = 1; % 测试用
              temp = R * acc;
              rotatedAcc(i,:) = temp';

       end
       % 循环完后 rotatedAcc 为 3行N列
end
