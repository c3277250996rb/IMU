function [Xukf, Z] = my_ukf( N, T, L , ...
                        alpha, kalpha, belta, ramda, ...
                        Wm, Wc, ...
                        s, Z, Xukf, covariation_finally);

is_wm_wc = 1;
if is_wm_wc
    % Q������������Э�������ģ��ϵͳ�е�����Ŷ���
    Q = 1;
    % R���۲�������Э�����ʾ���������е������
    R = 1;
    % Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
    % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        Z(1) = 0;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    for t=2:N

        % 1 ��� 2L + 1 ��Sigma�㼰��Ȩֵ:
            X0 = Xukf(:, t-1); % ��ȡ��һ����UKF��
            C0 = covariation_finally; % Э������
            cho = (chol(C0 * (L + ramda)))';

            for k=1:L
                Xsigma_1_I (:, k)=X0+cho(:,k);
                Xsigma_1_II(:, k)=X0-cho(:,k);
            end

            Xsigma_1  = [X0, Xsigma_1_I, Xsigma_1_II]; % X0����һ���ĵ㣬�൱�ھ�ֵ��
            % sx = size(Xsigma_1);
            % disp(['sx size: ', num2str(sx)]);


        % 2 ��Sigma��ͨ��״̬ת�ƺ��� gfun(x,t) Ԥ��õ�:
            Xsigma_1_predict = my_gfun(Xsigma_1, Z(t), T);


        % 3 ����Ԥ���ֵ��Э����
            Xs_1_p_weight=zeros(1);

            for k=1:2*L+1
                % ��� Xsigma_1_predict �Ĵ�С
                % size_Xsigma_1_predict = size(Xsigma_1_predict);
                % disp(['Xsigma_1_predict size: ', num2str(size_Xsigma_1_predict)]);

                Xs_1_p_weight = Xs_1_p_weight + Wm(k) * Xsigma_1_predict(:,k);
            end

            Xs_1_p_covariation = zeros(L,L);

            for k = 1:2*L+1
                Xs_1_p_covariation =    Xs_1_p_covariation + ...
                                        Wc(k) * ...
                                        (Xsigma_1_predict(:,k) - Xs_1_p_weight) * ...
                                        (Xsigma_1_predict(:,k) - Xs_1_p_weight)';
            end

            if is_wm_wc
                Xs_1_p_covariation = Xs_1_p_covariation + Q;
            end


        % 4 ʹ��UT�任�����µ�Sigma�㼯
            % disp((L + ramda) * Xs_1_p_covariation);
            chor = chol((L + ramda) * Xs_1_p_covariation)';

            for k=1:L
                Xsigma_UT_I (:,k) = Xs_1_p_weight + chor(:,k);
                Xsigma_UT_II(:,k) = Xs_1_p_weight - chor(:,k);
            end

            Xsigma_UT = [Xs_1_p_weight, Xsigma_UT_I, Xsigma_UT_II];
            

        % 5 ��Sigma�����۲⺯�� hfun(x,t) ����Ԥ��۲�ֵ
            for k=1:2*L+1
                % Zsigma_UT_predict(1,k)=hfun(Xsigma_UT(:,k),k);
                %                        a0              , v0              , velocity
                Zsigma_UT_predict(1,k) = my_hfun(Xsigma_UT(:,k), Xukf(2, t-1), T);
            end


        % 6 ����Ԥ��۲��ֵ��Э����
            % 6.1 ����۲�Ԥ���Ȩ��ֵ
                Zs_UT_p_weight = 0;
                for k=1:2*L+1
                    Zs_UT_p_weight = Zs_UT_p_weight + Wm(k) * Zsigma_UT_predict(1,k);
                end

            % 6.2 ����۲��Ȩ����
                Zs_UT_p_covariation = 0;

                for k=1:2*L+1
                    Zs_UT_p_covariation =   Zs_UT_p_covariation + ...
                                            Wc(k) * ...
                                            (Zsigma_UT_predict(1,k) - Zs_UT_p_weight) * ...
                                            (Zsigma_UT_predict(1,k) - Zs_UT_p_weight)'; 
                end
                
                if is_wm_wc
                    Zs_UT_p_covariation = Zs_UT_p_covariation + R;
                end

            % 6.3 ����״̬��۲�֮���Э����
                XZs_UT_p_covariation = zeros(L, 1);
                for k = 1:2*L + 1  
                    XZs_UT_p_covariation =  XZs_UT_p_covariation + ...
                                            Wc(k) * ...
                                            (Xsigma_UT(:,k) - Xs_1_p_weight) * ...
                                            (Zsigma_UT_predict(1,k) - Zs_UT_p_weight)';
                end
                

        % 7 ���㿨��������
            K = XZs_UT_p_covariation * Zs_UT_p_covariation^-1;
                

        % 8 ����״̬��Э����
            % 8.1 ״̬����
                Xukf(:,t) = Xs_1_p_weight + K * (Z(t) - Zs_UT_p_weight);
                % Xukf(1,t) = Z(t);
            % 8.2 �������
                covariation_finally = Xs_1_p_covariation - K * Zs_UT_p_covariation * K';

        %   ���ڸ�λЭ�������
            if mod(N , 100) == 0
                % disp(N);
                % covariation_finally = eye(L);
            end
        %

    end

end
