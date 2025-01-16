function output_map = ukf_3dimensional(input_data, input_start, input_finish, input_mean, input_judge_stop_times)

    % ��ʼ��
        addpath(genpath('data'));
        addpath(genpath('main'));
        addpath(genpath('src'));

        output_map = containers.Map();

        input_is_use_uart = get_is_use_uart();
        input_is_use_rotate_accel = get_is_use_rotate_accel();
        
        a = 16.384;
        N = input_finish - input_start;
        T = 0.039;

        output_calc_data = zeros(N,5);

        % ʹ�� ��ת ��Ʈ
        if input_is_use_rotate_accel
            accelerate = input_data(input_start : input_finish);

        % �� ʹ�� ��ת ��Ʈ
        else
            
            accelerate = input_data(input_start : input_finish);

        end

        accelerate = accelerate - mean(accelerate(1:40));

        accelerate = accelerate ./ a ./ 10;


        % �޷�
            
        %


        % ��ͼ���ٶ�
            % figure
            % hold on ;box on;
            % plot(accelerate ./ 5000 .* 24 ./ 4 .* a .* 10);
            output_calc_data(:,1) = accelerate(1:end-1) ./ 5000 .* 24 ./ 4 .* a .* 10;
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
                for i=2:N
                    Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
                end
            %
        %

        % V �ٶ� ��ͼ (UKF)
            % hold on ;box on;
            % t=(0:1:N-1);
            % plot(t,Xukf(2,:));
            original_v_Xukf = Xukf;
            output_calc_data(:,2) = original_v_Xukf(2,:)';
        %

        % V �ٶ� ��ͼ (ֱ�ӻ���)
            % plot(t,Xukf(2,:));
            output_calc_data(:,3) = Zv(:,1);
            
            if 0 == get_is_use_direct_intergration
                output_calc_data(:,3) = output_calc_data(:,2);
            end
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
                Zd2 = zeros(N);
                for i=2:N
                    Zd(i) = Zd(i - 1) + (original_v_Xukf(2,i) + original_v_Xukf(2,i-1)) /2 *T;
                    Zd2(i) = Zd2(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
                end
            %

        %
        
        % D ���� ��ͼ (UKF ֱ�ӻ���)
            % plot(t, Zd); 
            output_calc_data(:,4) = Zd(:,1);
        %
        
        
        % D ���� ��ͼ (ֱ�ӻ��� ֱ�ӻ���)
            % plot(t, Zd);
            output_calc_data(:,5) = Zd2(:,1);

            if 0 == get_is_use_direct_intergration
                output_calc_data(:,5) = output_calc_data(:,4);
            end
        %
%%%


        % ��ͼ ��ע
            % legend('a', 'v', 'v', 'd', 'd');

            % ǿ��ˢ��ͼ�δ��ڣ�������ʾͼ��
            % drawnow;
        %

        % Map ��ֵ
            output_map('output_calc_data') = output_calc_data;
            % output_map('output_I_index') = output_I_index;
        %
    %
end
        