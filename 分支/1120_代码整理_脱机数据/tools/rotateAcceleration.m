function rotatedAcc = rotateAcceleration(temp_x, temp_y, temp_z, roll, pitch, yaw)
    % rotateAcceleration - 根据欧拉角旋转加速度
    %
    % 输入参数:
    % acc - 3x1向量, 包含x, y, z方向的加速度
    % eulerAngles - 3x1向量, 欧拉角 (roll, pitch, yaw) [单位: 角度]
    %
    % 输出:
    % rotatedAcc - 3x1向量, 旋转后的加速度

    % 将角度转换为弧度
    roll = deg2rad(roll);
    pitch = deg2rad(pitch);
    yaw = deg2rad(yaw);
    
    % 提取欧拉角
%     roll = eulerAngles(1);   % 绕x轴的旋转
%     pitch = eulerAngles(2);  % 绕y轴的旋转
%     yaw = eulerAngles(3);    % 绕z轴的旋转

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
%     rotatedAcc = R * acc;
    rotatedAcc = acc;
end
