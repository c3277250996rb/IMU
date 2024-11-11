%%%
clear;clc; close all;

N = 300;
% ��ʼ��
    data = readtable('com_data.csv');
    accelerate = data.accel;

    % N����ʾģ���ʱ�䲽�������ݵ����������
    % N = 299;
        % Q������������Э�������ģ��ϵͳ�е�����Ŷ���
        Q = 1;
        % R���۲�������Э�����ʾ���������е������
        R = 1;
    % covariation_finally��Э�������ĳ�ʼֵ����ʾ״̬���ƵĲ�ȷ���ԡ�
    covariation_finally = eye(3);

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
    % Xukf(1, :)  = accelerate();

    %%%%%%%%%%%%%%%%
    covariation_finally=eye(L);%Э�������ʼ��

    T = 0.01;
%

T = 0.039;
is_continuous = 1;
% none = close_com();
is_use_com = 1;
% V �ٶ��Ż�
Xukf = my_ukf(  N, T, L , ...
                alpha, kalpha, belta, ramda, ...
                Wm, Wc, ...
                s, Z, Xukf, covariation_finally, ...
                is_use_com, is_continuous);

for i=2:N
    Z(1) = 0;
    % Z(i) = Z(i - 1) + (accelerate(i - 1) + accelerate(i)) /2 *T;
end

%%%%%%%%%%%
%��ͼ
figure
hold on ;box on
% plot(Z,'o');
plot(s,'-k.');
plot(Xukf(2,:),'-r+');
legend('�۲��ٶ�','��ʵ�ٶ�','UKF�ٶ�')



is_use_com = 0;
% distance �����Ż�
distance = zeros(L, N);
distance = my_ukf(  N, T, L , ...
                    alpha, kalpha, belta, ramda, ...
                    Wm, Wc, ...
                    s, Xukf, distance, covariation_finally, ...
                    is_use_com, is_continuous);

Zd = zeros(N);
Xv = Xukf(2,:);
for i=2:N
    Zd(i) = Zd(i - 1) + (Xv(i) + Xv(i-1)) /2 *T;
end

%%%%%%%%%%%
%��ͼ
figure
hold on ;box on
% plot(Zd,'o');
plot(s,'-k.');
plot(distance(2,:),'-r+');
legend('�۲�켣','��ʵ�켣','UKF�켣')




%������
    % for i=1:N
    %     Err_KalmanFilter(i)=RMS(X(:,i),Xukf(:,i));
    % end
