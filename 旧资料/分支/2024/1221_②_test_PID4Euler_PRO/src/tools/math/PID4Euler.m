% function    output_euler         = PID4Euler(input_data, input_kp, input_ki)
function    output_euler         = PID4Euler(input_size, input_data, input_gyro,  input_kp, input_ki, input_adjust, input_adjust_index)
% function  [roll, pitch, yaw]  = PID4Euler(ax, ay, az, gx, gy, gz, mx, my, mz)
    size = input_size;

    % Quaternion initialization
    % persistent 变量是指在函数调用之间保留其值，但不同于全局变量，它们只在函数内部可见。
    % persistent 变量不会在每次函数调用时被清除，而是维持上次的值，直到下次调用时。
    persistent q0_array q1_array q2_array q3_array
    if isempty(q0_array)
        q0_array = ones(2,size);
        q1_array = zeros(2,size);
        q2_array = zeros(2,size);
        q3_array = zeros(2,size);
    end

    if input_adjust == 9
        q0_array = ones(2,size); 
        q1_array = zeros(2,size);
        q2_array = zeros(2,size);
        q3_array = zeros(2,size);
    end

    % Integral error terms
    % persistent 变量是指在函数调用之间保留其值，但不同于全局变量，它们只在函数内部可见。
    % persistent 变量不会在每次函数调用时被清除，而是维持上次的值，直到下次调用时。
    persistent exInt_array eyInt_array ezInt_array
    if isempty(exInt_array)
        exInt_array = zeros(2,size);
        eyInt_array = zeros(2,size);
        ezInt_array = zeros(2,size);
    end
    
    cond = 0;
    % cond = 1;

    if cond
        exInt_array = zeros(2,size);
        eyInt_array = zeros(2,size);
        ezInt_array = zeros(2,size);
    end

    if input_adjust == 9
        exInt_array = zeros(2,size);
        eyInt_array = zeros(2,size);
        ezInt_array = zeros(2,size);
    end

    % 退出
    if input_adjust == 9
        return;
    end


    % array =  [p1, p2, p3, p4, p5; 
    %           i1, i2, i3, i4, i5];
    q0 = q0_array(input_adjust, input_adjust_index);
    q1 = q1_array(input_adjust, input_adjust_index);
    q2 = q2_array(input_adjust, input_adjust_index);
    q3 = q3_array(input_adjust, input_adjust_index);

    ax = input_data(1);
    ay = input_data(2);
    az = input_data(3);
    
    gx = input_gyro(1);
    gy = input_gyro(2);
    gz = input_gyro(3);

    mx = input_data(7);
    my = input_data(8);
    mz = input_data(9);


    % array
    exInt = exInt_array(input_adjust, input_adjust_index);
    eyInt = eyInt_array(input_adjust, input_adjust_index);
    ezInt = ezInt_array(input_adjust, input_adjust_index);



    output_euler = zeros(1,3);


    % Original
        % Kp = 4.50;  % Proportional gain
        % Ki = 1.0;   % Integral gain
        % halfT = 0.024;  % Time step (in seconds)
    %

    % From
        Kp = input_kp;
        Ki = input_ki;
        halfT = 0.024;  % Time step (in seconds)
    %

    % Get
        % Kp = get_Kp();  % Proportional gain
        % Ki = get_Ki();   % Integral gain
        % halfT = 0.024;  % Time step (in seconds)
    %


    % Inverse square root function
    invSqrt = @(x) 1.0 / sqrt(x);

    % Normalize accelerometer and magnetometer measurements
    norm = invSqrt(ax^2 + ay^2 + az^2);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;

    norm = invSqrt(mx^2 + my^2 + mz^2);         
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;

    % Magnetic field and gravity in world coordinates
    hx = 2 * mx * (0.5 - q2^2 - q3^2) + 2 * my * (q1 * q2 - q0 * q3) + 2 * mz * (q1 * q3 + q0 * q2);
    hy = 2 * mx * (q1 * q2 + q0 * q3) + 2 * my * (0.5 - q1^2 - q3^2) + 2 * mz * (q2 * q3 - q0 * q1);
    hz = 2 * mx * (q1 * q3 - q0 * q2) + 2 * my * (q2 * q3 + q0 * q1) + 2 * mz * (0.5 - q1^2 - q2^2);         
    bx = sqrt(hx^2 + hy^2);
    bz = hz;

    % Gravity vector in world coordinates
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0^2 - q1^2 - q2^2 + q3^2;

    % Magnetic field vector in world coordinates
    wx = 2 * bx * (0.5 - q2^2 - q3^2) + 2 * bz * (q1 * q3 - q0 * q2);
    wy = 2 * bx * (q1 * q2 - q0 * q3) + 2 * bz * (q0 * q1 + q2 * q3);
    wz = 2 * bx * (q0 * q2 + q1 * q3) + 2 * bz * (0.5 - q1^2 - q2^2);

    % Error terms (cross product)
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);



    % Correct errors if necessary
    if ex ~= 0.0 && ey ~= 0.0 && ez ~= 0.0
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;    
        ezInt = ezInt + ez * Ki * halfT;

        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    end

    % Update quaternion based on corrected gyroscope measurements
    q0 = q0 - (q1 * gx + q2 * gy + q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    % Normalize the quaternion
    norm = invSqrt(q0^2 + q1^2 + q2^2 + q3^2);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;


    roll    = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
    pitch   = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 
    yaw     = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3; 


    output_euler(1,1) = roll;
    output_euler(1,2) = pitch;
    output_euler(1,3) = yaw;

    
    % q_array =  [p1, p2, p3, p4, p5; 
    %           i1, i2, i3, i4, i5];
    q0_array(input_adjust, input_adjust_index) = q0;
    q1_array(input_adjust, input_adjust_index) = q1;
    q2_array(input_adjust, input_adjust_index) = q2;
    q3_array(input_adjust, input_adjust_index) = q3;

    % array
    exInt_array(input_adjust, input_adjust_index) = exInt;
    eyInt_array(input_adjust, input_adjust_index) = eyInt;
    ezInt_array(input_adjust, input_adjust_index) = ezInt;
end
