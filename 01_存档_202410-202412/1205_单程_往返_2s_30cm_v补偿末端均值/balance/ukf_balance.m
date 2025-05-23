function output_status = ukf_balance(input_data, input_start, input_finish, input_mean, input_is_use_rotate_accel, input_judge_stop_times, input_is_use_uart)

    addpath('tools');
    addpath('data');
    addpath('scripts');
    addpath('balance');
    
    a = 16.384;
    % N = 3000-1;
    % N = public_N;
    N = input_finish - input_start;
    % 初始化
        % 使用 旋转 零飘
        if input_is_use_rotate_accel


        % 不 使用 旋转 零飘
        else
            accelerate = input_data(input_start : input_finish) - input_mean;
            accelerate = accelerate ./ a ./ 10;

        end

        figure
        hold on ;box on;
        plot(accelerate ./ 5000 .* 24 ./ 4 .* a .* 10);

        % Q：过程噪声的协方差，用于模拟系统中的随机扰动。
        Q = 1;
        % R：观测噪声的协方差，表示测量过程中的随机误差。
        R = 1;
        % L：状态变量的维度。
        L = 2;    %?个变量
        % alpha、kalpha、belta：UKF的超参数，用于调整Sigma点的生成和权重。
        alpha=1;
        kalpha=0;
        belta=2;
        % ramda：UKF的扩展参数，影响Sigma点的散布程度。
        ramda=3-L;

        % 如果在UKF中不设置第一个`Wm`和`Wc`的特殊权重，而是将所有权重设置为相同值，你发现效果反而更好，这可能与以下原因有关：

        % 1. **状态模型和观测模型的特性**：UKF的默认权重设置（即将第一个Sigma点的权重设置为更高）是基于无味变换的一般假设，假设系统的非线性程度适中。然而在一些实际系统中（特别是当模型较为线性，或者噪声较小、初始状态较准确时），均匀的权重分布可能会更有效，避免系统逐步偏向某个方向。
        
        % 2. **模型的非线性程度**：UKF默认设置的特殊权重设计是为了增强在高度非线性系统下的状态估计能力。对于非线性程度较低的系统，特殊权重可能会引入过多的偏移，导致估计值偏离真实值。简单均匀的权重反而更适合此类系统。
        
        % 3. **过程噪声和观测噪声的相对影响**：如果系统的过程噪声或观测噪声较低，使用均匀权重可以减少噪声对状态估计的偏移。此时权重的特殊设置（偏向于第一个Sigma点）可能会导致过度信任初始状态，从而引入偏差。
        
        % 4. **初始状态和协方差设定的影响**：初始状态和协方差的选择会影响UKF权重的适用性。不同初始设定下，某些权重组合可能会使系统更快地收敛，而特殊权重可能使状态在某一方向上有偏移。
        
        % ### 建议的实验方法
        % 可以进一步实验来优化UKF权重参数，比如调整 `alpha`、`beta` 或 `kappa`（也即代码中的 `ramda`）参数。这样可能会找到一种适合你系统的权重配置，既能提高估计精度，又能避免偏移。

        for j=1:2*L+1
            Wm(j)=1/(2*(L+ramda));
            Wc(j)=1/(2*(L+ramda));
        end
        % Wm(1)=ramda/(L+ramda);%权值Wm的初值需要另外定
        % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%权值Wc的初值需要另外定
        s = zeros(N);
        Z = accelerate;
        Xukf = zeros(L,N);
        covariation_finally=eye(L);%协方差阵初始化
    %

    % UKF
        T = 0.039;
        % T = 0.0388295;
        is_continuous = 0;
        is_use_com = 0;

        % tic; % 开始计时

        % V 速度优化
        [Xukf, Z] = my_ukf(  N, T, L , ...
                        alpha, kalpha, belta, ramda, ...
                        Wm, Wc, ...
                        s, Z, Xukf, covariation_finally, ...
                        is_use_com, is_continuous);


        
        elapsedTime = toc; % 结束计时并获取所用时间
        % fprintf('代码块执行时间为 %.8f 秒\n', elapsedTime);

        Zv = zeros(N);
        % disp(Z);
        for i=2:N
            Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
        end

        %%%%%%%%%%%
        %画图
        % figure
        hold on ;box on;
        t=(0:1:N-1);
        plot(t,Xukf(2,:));


        differential = zeros(2,N);
        input_data = Xukf(2,:);
        input_count_size = N;
        input_window_size = 5;
        % differential = calc_differential(input_data, input_count_size, input_window_size);



        input_tail_size = input_judge_stop_times - 5;
        input_near_size = 4;
        input_accelerate = Z;
        input_velocity = Xukf(2,:);
        output_I_index = I_av_intersection(input_accelerate, input_velocity, input_judge_stop_times, input_is_use_uart);
        disp(['output_I_index: ', num2str(output_I_index)]);
        output_velocity = velocity_compensation(output_I_index, input_tail_size, input_near_size, input_accelerate, input_velocity);
        Xukf(2,:) = output_velocity;
        plot(t,Xukf(2,:));


        is_use_com = 0;
        % distance 距离优化
        distance = zeros(L, N);
        [distance, Xukf] = my_ukf(  N, T, L , ...
                            alpha, kalpha, belta, ramda, ...
                            Wm, Wc, ...
                            s, Xukf, distance, covariation_finally, ...
                            is_use_com, is_continuous);


        tic; % 开始计时

        Zd = zeros(N);
        for i=2:N
            Zd(i) = Zd(i - 1) + (Xukf(2,i) + Xukf(2,i-1)) /2 *T;
            % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
        end
        % disp(Zd(1:9));

        % elapsedTime = toc; % 结束计时并获取所用时间
        % fprintf('代码块执行时间为 %.8f 秒\n', elapsedTime);


        
        % 判断
            input_accelerate = zeros(N);
            input_velocity = zeros(N);
            input_distance = zeros(N);

            input_tail_size = 30;
            input_accelerate = accelerate ./ 5000 .* 24 ./ 4 .* a .* 10;
            input_velocity = Xukf(2,:);
            input_distance = Zd(1:N);
            input_all_size = N;
            output_status = status_judgement(input_all_size, input_tail_size, input_accelerate, input_velocity, input_distance);
        %
        % disp(Zd(1:9));

        %%%%%%%%%%%
        %画图
        % figure
        hold on ;box on;
        t=(0:1:N-1);
        plot(t, Zd);

        legend('a', 'v', 'v', 'd');

        % 强制刷新图形窗口，立即显示图形
        drawnow;
    %


end
        