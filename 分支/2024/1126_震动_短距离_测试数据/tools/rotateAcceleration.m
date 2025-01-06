function rotatedAcc = rotateAcceleration(temp_x, temp_y, temp_z, roll, pitch, yaw)
    % rotateAcceleration - ����ŷ������ת���ٶ�
    %
    % �������:
    % acc - 3x1����, ����x, y, z����ļ��ٶ�
    % eulerAngles - 3x1����, ŷ���� (roll, pitch, yaw) [��λ: �Ƕ�]
    %
    % ���:
    % rotatedAcc - 3x1����, ��ת��ļ��ٶ�

    % ���Ƕ�ת��Ϊ����
    roll = deg2rad(roll);
    pitch = deg2rad(pitch);
    yaw = deg2rad(yaw);
    
    % ��ȡŷ����
%     roll = eulerAngles(1);   % ��x�����ת
%     pitch = eulerAngles(2);  % ��y�����ת
%     yaw = eulerAngles(3);    % ��z�����ת

    % ������ת����
    R_x = [1, 0, 0;
           0, cos(roll), -sin(roll);
           0, sin(roll), cos(roll)];
    
    R_y = [cos(pitch), 0, sin(pitch);
           0, 1, 0;
          -sin(pitch), 0, cos(pitch)];
    
    R_z = [cos(yaw), -sin(yaw), 0;
           sin(yaw), cos(yaw), 0;
           0, 0, 1];
    
    % �ܵ���ת����Z-Y-X˳��
    R = R_z * R_y * R_x;

    % ��ת���ٶ�
    acc = [temp_x; temp_y; temp_z];
%     rotatedAcc = R * acc;
    rotatedAcc = acc;
end
