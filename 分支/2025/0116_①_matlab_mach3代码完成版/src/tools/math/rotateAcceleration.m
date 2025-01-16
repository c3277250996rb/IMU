function rotatedAcc = rotateAcceleration(input_data) % temp_x, temp_y, temp_z, roll, pitch, yaw

       is_use_uart = get_is_use_uart();

       [length_row, length_column] = size(input_data);
       rotatedAcc = zeros(length_row, 3);

       % disp(input_data(1:6,1:6));

       for i = 1:length_row
              % ȡ����������
                     if is_use_uart
                            Var2_double = (input_data(i,:));
                     else
                            Var2_double = table2array(input_data(i,:));
                     end
                     % disp(class(input_data(i, 4)));  % �����������
                     % disp(class(str2double(input_data(i, 4))));  % �����������
              
                     temp_x = (Var2_double(1));
                     temp_y = (Var2_double(2));
                     temp_z = (Var2_double(3));
                     roll   = (Var2_double(4));
                     pitch  = (Var2_double(5));
                     yaw    = (Var2_double(6));
              %

              % ���Ƕ�ת��Ϊ����
              roll  = deg2rad((roll));
              pitch = deg2rad((pitch));
              yaw   = deg2rad((yaw));


%%% old
              % % ������ת����
              % R_x = [1, 0, 0;
              %        0, cos(roll), -sin(roll);
              %        0, sin(roll), cos(roll)];
              
              % R_y = [cos(pitch), 0, sin(pitch);
              %        0, 1, 0;
              %        -sin(pitch), 0, cos(pitch)];
              
              % R_z = [cos(yaw), -sin(yaw), 0;
              %        sin(yaw), cos(yaw), 0;
              %        0, 0, 1];

              % % �ܵ���ת����Z-Y-X˳��
              % R = R_z * R_y * R_x;

              % % ��ת���ٶ�
              % acc = [temp_x; temp_y; temp_z];

              % % R = 1; % ������
              % temp = R * acc;
%%%



%%% new
              
              % ������ת�����Ԫ��
              cos_roll = cos(roll);   sin_roll = sin(roll);
              cos_pitch = cos(pitch); sin_pitch = sin(pitch);
              cos_yaw = cos(yaw);     sin_yaw = sin(yaw);

              % ��ת���� R = Rz * Ry * Rx
              R = zeros(3,3);

                     R(1,1) = cos_yaw * cos_pitch;
                     R(1,2) = cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll;
                     R(1,3) = cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll;

                     R(2,1) = sin_yaw * cos_pitch;
                     R(2,2) = sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll;
                     R(2,3) = sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll;

                     R(3,1) = -sin_pitch;
                     R(3,2) = cos_pitch * sin_roll;
                     R(3,3) = cos_pitch * cos_roll;
                     

              % ��������ϵ�µļ��ٶ� a' = R * a
              ax_world = R(1,1) * temp_x + R(1,2) * temp_y + R(1,3) * temp_z;
              ay_world = R(2,1) * temp_x + R(2,2) * temp_y + R(2,3) * temp_z;
              az_world = R(3,1) * temp_x + R(3,2) * temp_y + R(3,3) * temp_z;

              temp = [ax_world; ay_world; az_world];
%%%


              rotatedAcc(i,:) = temp';

       end
       % ѭ����� rotatedAcc Ϊ 3��N��
end
