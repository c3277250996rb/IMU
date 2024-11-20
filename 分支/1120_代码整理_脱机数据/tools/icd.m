function icd(my_char)

    a = 16.384;
    % N = 3000-1;
    N = public_N;
    % ��ʼ��
        data = readtable('adjusted_data.csv');
        
        picture_name_v = [my_char, '-v.png']; picture_name_s = [my_char , '-s.png'];
        if my_char == 'x'
            accelerate = data.x;
        end
        if my_char == 'y'
            accelerate = data.y;
        end
        if my_char == 'z'
            accelerate = data.z;
        end 

        accelerate = accelerate ./ a ./ 10;

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

        % �����UKF�в����õ�һ��`Wm`��`Wc`������Ȩ�أ����ǽ�����Ȩ������Ϊ��ֵͬ���㷢��Ч���������ã������������ԭ���йأ�

        % 1. **״̬ģ�ͺ͹۲�ģ�͵�����**��UKF��Ĭ��Ȩ�����ã�������һ��Sigma���Ȩ������Ϊ���ߣ��ǻ�����ζ�任��һ����裬����ϵͳ�ķ����Գ̶����С�Ȼ����һЩʵ��ϵͳ�У��ر��ǵ�ģ�ͽ�Ϊ���ԣ�����������С����ʼ״̬��׼ȷʱ�������ȵ�Ȩ�طֲ����ܻ����Ч������ϵͳ��ƫ��ĳ������
        
        % 2. **ģ�͵ķ����Գ̶�**��UKFĬ�����õ�����Ȩ�������Ϊ����ǿ�ڸ߶ȷ�����ϵͳ�µ�״̬�������������ڷ����Գ̶Ƚϵ͵�ϵͳ������Ȩ�ؿ��ܻ���������ƫ�ƣ����¹���ֵƫ����ʵֵ���򵥾��ȵ�Ȩ�ط������ʺϴ���ϵͳ��
        
        % 3. **���������͹۲����������Ӱ��**�����ϵͳ�Ĺ���������۲������ϵͣ�ʹ�þ���Ȩ�ؿ��Լ���������״̬���Ƶ�ƫ�ơ���ʱȨ�ص��������ã�ƫ���ڵ�һ��Sigma�㣩���ܻᵼ�¹������γ�ʼ״̬���Ӷ�����ƫ�
        
        % 4. **��ʼ״̬��Э�����趨��Ӱ��**����ʼ״̬��Э�����ѡ���Ӱ��UKFȨ�ص������ԡ���ͬ��ʼ�趨�£�ĳЩȨ����Ͽ��ܻ�ʹϵͳ�����������������Ȩ�ؿ���ʹ״̬��ĳһ��������ƫ�ơ�
        
        % ### �����ʵ�鷽��
        % ���Խ�һ��ʵ�����Ż�UKFȨ�ز������������ `alpha`��`beta` �� `kappa`��Ҳ�������е� `ramda`���������������ܻ��ҵ�һ���ʺ���ϵͳ��Ȩ�����ã�������߹��ƾ��ȣ����ܱ���ƫ�ơ�

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

    T = 0.039;
    % T = 0.0388295;
    is_continuous = 0;
    is_use_com = 0;

    % V �ٶ��Ż�
    [Xukf, Z] = my_ukf(  N, T, L , ...
                    alpha, kalpha, belta, ramda, ...
                    Wm, Wc, ...
                    s, Z, Xukf, covariation_finally, ...
                    is_use_com, is_continuous);

    Zv = zeros(N);
    % disp(Z);
    for i=2:N
        Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
    end

    %%%%%%%%%%%
    %��ͼ
    % figure
    hold on ;box on;;
    t=(0:1:N-1);
    plot(t,Zv);
    plot(t,Xukf(2,:));

    % ����ͼ��ΪPNG�ļ�
    % title(picture_name_v);
    % saveas(gcf, picture_name_v);



    is_use_com = 0;
    % distance �����Ż�
    distance = zeros(L, N);
    [distance, Xukf] = my_ukf(  N, T, L , ...
                        alpha, kalpha, belta, ramda, ...
                        Wm, Wc, ...
                        s, Xukf, distance, covariation_finally, ...
                        is_use_com, is_continuous);

    Zd = zeros(N);
    for i=2:N
        Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
    end

    %%%%%%%%%%%
    %��ͼ
    % figure
    hold on ;box on;
    t=(0:1:N-1);
    plot(t, Zd);
    plot(t, distance(2,:));

    % ����ͼ��ΪPNG�ļ�
    title(picture_name_s);
    % saveas(gcf, picture_name_s);

    iwrite('data\share.csv', 1, 1, distance(2,N-1));

    %������
        % for i=1:N
        %     Err_KalmanFilter(i)=RMS(X(:,i),Xukf(:,i));
        % end

end
        