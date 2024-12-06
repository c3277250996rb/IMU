function output_status = ukf_balance(input_data, input_start, input_finish, input_mean, input_judge_stop_times)

    % ��ʼ��
        addpath(genpath('data'));
        addpath(genpath('main'));
        addpath(genpath('src'));

        input_is_use_uart = get_is_use_uart();
        input_is_use_rotate_accel = get_is_use_rotate_accel();
        
        a = 16.384;
        N = input_finish - input_start;
        T = 0.039;


        % ʹ�� ��ת ��Ʈ
        if input_is_use_rotate_accel
            accelerate = input_data(input_start : input_finish) - mean(input_data(1:30));
            accelerate = accelerate ./ a ./ 10;

        % �� ʹ�� ��ת ��Ʈ
        else
            accelerate = input_data(input_start : input_finish) - input_mean;
            accelerate = accelerate ./ a ./ 10;

        end

        % ��ͼ���ٶ�
            figure
            hold on ;box on;
            plot(accelerate ./ 5000 .* 24 ./ 4 .* a .* 10);
        %

        % UKF������ʼ��
            % Q������������Э�������ģ��ϵͳ�е�����Ŷ���
            Q = 1;
            % R���۲�������Э�����ʾ���������е������
            R = 1;
            % L��״̬������ά�ȡ�
            L = 2;    %?������
            % alpha��kalpha��belta��UKF�ĳ����������ڵ���Sigma������ɺ�Ȩ�ء�
            alpha=1;
            kalpha=0;
            belta=2;
            % ramda��UKF����չ������Ӱ��Sigma���ɢ���̶ȡ�
            ramda=3-L;

            for j=1:2*L+1
                Wm(j)=1/(2*(L+ramda));
                Wc(j)=1/(2*(L+ramda));
            end
            % Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
            % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
            s = zeros(N);
            Z = accelerate;
            Xukf = zeros(L,N);
            covariation_finally=eye(L);%Э�������ʼ��
        %
    %


%%% V �ٶȴ���
        % V �ٶ� ����
            % a -> v (UKF)
                [Xukf, Z] = my_ukf(  N, T, L , ...
                                alpha, kalpha, belta, ramda, ...
                                Wm, Wc, ...
                                s, Z, Xukf, covariation_finally);
            %

            % a -> v (ֱ�ӻ���)
                Zv = zeros(N);
                % disp(Z);
                for i=2:N
                    Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
                end
            %
        %

        % V �ٶ� ��ͼ (UKF ԭʼ)
            hold on ;box on;
            t=(0:1:N-1);
            plot(t,Xukf(2,:));
            original_v_Xukf = Xukf;
        %

        % V �ٶ� ����
            input_tail_size = input_judge_stop_times - 5;
            input_near_size = 4;
            input_accelerate = Z;
            input_velocity = Xukf(2,:);
            % �� I ��: �� a �� v �����һ������ (��Ҫ a �� v ����һ����ֵ�ĵ�)
            output_I_index = I_av_intersection(input_accelerate, input_velocity, input_judge_stop_times);
            disp(['output_I_index: ', num2str(output_I_index)]);
            % �� II ��: �� I ����ǰ��һ�� a ����� (��Ҫ near_size �����ĵ�)
            output_velocity = velocity_compensation(output_I_index, input_tail_size, input_near_size, input_accelerate, input_velocity);
            Xukf(2,:) = output_velocity;
        %

        % V �ٶ� ��ͼ (UKF ����)
            plot(t,Xukf(2,:));
        %
%%%


%%% D ���봦��
        % D ���� ����
            % v -> d (UKF)
                distance = zeros(L, N);
                [distance, original_v_Xukf] = my_ukf(  N, T, L , ...
                                    alpha, kalpha, belta, ramda, ...
                                    Wm, Wc, ...
                                    s, original_v_Xukf, distance, covariation_finally);
            %

            % v -> d (ֱ�ӻ���) (ԭʼ UKF)
                Zd = zeros(N);
                for i=2:N
                    Zd(i) = Zd(i - 1) + (original_v_Xukf(2,i) + original_v_Xukf(2,i-1)) /2 *T;
                    % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
                end
            %

        %
        
        % D ���� ��ͼ (ֱ�ӻ��� ԭʼ)
            plot(t, Zd);
        %
        
        % D ���� ����
            Zd = zeros(N);
            for i=2:N
                Zd(i) = Zd(i - 1) + (Xukf(2,i) + Xukf(2,i-1)) /2 *T;
                % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
            end
        %
        
        % D ���� ��ͼ (ֱ�ӻ��� ����)
            plot(t, Zd);
        %
%%%


        % ��ͼ ��ע
            legend('a', 'v', 'v', 'd', 'd');

            % ǿ��ˢ��ͼ�δ��ڣ�������ʾͼ��
            drawnow;
        %
        
        % Status ״̬ �ж�
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
        