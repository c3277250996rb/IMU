function output_status = ukf_balance(input_data, input_start, input_finish, input_mean, input_judge_stop_times)

    % 初始化
        addpath(genpath('data'));
        addpath(genpath('main'));
        addpath(genpath('src'));

        input_is_use_uart = get_is_use_uart();
        input_is_use_rotate_accel = get_is_use_rotate_accel();
        
        a = 16.384;
        N = input_finish - input_start;
        T = 0.039;


        % 使用 旋转 零飘
        if input_is_use_rotate_accel
            accelerate = input_data(input_start : input_finish) - mean(input_data(1:30));
            accelerate = accelerate ./ a ./ 10;

        % 不 使用 旋转 零飘
        else
            accelerate = input_data(input_start : input_finish) - input_mean;
            accelerate = accelerate ./ a ./ 10;

        end

        % 绘图加速度
            figure
            hold on ;box on;
            plot(accelerate ./ 5000 .* 24 ./ 4 .* a .* 10);
        %

        % UKF参数初始化
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
    %


%%% V 速度处理
        % V 速度 计算
            % a -> v (UKF)
                [Xukf, Z] = my_ukf(  N, T, L , ...
                                alpha, kalpha, belta, ramda, ...
                                Wm, Wc, ...
                                s, Z, Xukf, covariation_finally);
            %

            % a -> v (直接积分)
                Zv = zeros(N);
                % disp(Z);
                for i=2:N
                    Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
                end
            %
        %

        % V 速度 画图 (UKF 原始)
            hold on ;box on;
            t=(0:1:N-1);
            plot(t,Xukf(2,:));
            original_v_Xukf = Xukf;
        %

        % V 速度 补偿
            input_tail_size = input_judge_stop_times - 5;
            input_near_size = 4;
            input_accelerate = Z;
            input_velocity = Xukf(2,:);
            % 求 I 点: 求 a 和 v 的最后一个交点 (不要 a 和 v 低于一定阈值的点)
            output_I_index = I_av_intersection(input_accelerate, input_velocity, input_judge_stop_times);
            disp(['output_I_index: ', num2str(output_I_index)]);
            % 求 II 点: 从 I 点往前第一个 a 的零点 (不要 near_size 附近的点)
            output_velocity = velocity_compensation(output_I_index, input_tail_size, input_near_size, input_accelerate, input_velocity);
            Xukf(2,:) = output_velocity;
        %

        % V 速度 画图 (UKF 补偿)
            plot(t,Xukf(2,:));
        %
%%%


%%% D 距离处理
        % D 距离 计算
            % v -> d (UKF)
                distance = zeros(L, N);
                [distance, original_v_Xukf] = my_ukf(  N, T, L , ...
                                    alpha, kalpha, belta, ramda, ...
                                    Wm, Wc, ...
                                    s, original_v_Xukf, distance, covariation_finally);
            %

            % v -> d (直接积分) (原始 UKF)
                Zd = zeros(N);
                for i=2:N
                    Zd(i) = Zd(i - 1) + (original_v_Xukf(2,i) + original_v_Xukf(2,i-1)) /2 *T;
                    % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
                end
            %

        %
        
        % D 距离 画图 (直接积分 原始)
            plot(t, Zd);
        %
        
        % D 距离 补偿
            Zd = zeros(N);
            for i=2:N
                Zd(i) = Zd(i - 1) + (Xukf(2,i) + Xukf(2,i-1)) /2 *T;
                % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
            end
        %
        
        % D 距离 画图 (直接积分 补偿)
            plot(t, Zd);
        %
%%%


        % 画图 标注
            legend('a', 'v', 'v', 'd', 'd');

            % 强制刷新图形窗口，立即显示图形
            drawnow;
        %
        
        % Status 状态 判断
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
    %
end
        