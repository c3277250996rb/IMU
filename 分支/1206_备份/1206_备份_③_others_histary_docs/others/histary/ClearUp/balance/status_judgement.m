function output_status = status_judgement(input_all_size, input_tail_size, input_accelerate, input_velocity, input_distance)
    % ����ĩ�˾�ֵ
    mean_tail_a = mean( input_accelerate(end-input_tail_size : end) );
    mean_tail_v = mean( input_velocity(end-input_tail_size : end) );
    mean_tail_d = mean( input_distance(end-input_tail_size : end) );


    % disp(['abs(max(input_accelerate)): ', num2str(abs(max(input_accelerate)))]);
    % disp(['abs(mean_tail_v)*3: ', num2str(abs(mean_tail_v))]);
    % disp(['abs(max(input_velocity)): ', num2str(abs(max(input_velocity)))]);
    % disp(['abs(mean_tail_d)*3: ', num2str(abs(mean_tail_d))]);


    if( abs(mean_tail_d) < 0.15 )
        output_status = 2; % ����
        disp(['ĩ��d��ֵ < 0.15  =>  ����']);
        return; % �������أ���ִ�к�������
    end

    % a���ֵ < v��ֵ*3  =>  С���˶�  =>  ��
    if( (abs(max(input_accelerate)) / abs(mean_tail_v)) < 3 )
            output_status = 1; % ��

    % a���ֵ > v��ֵ*3  =>  ����˶�
    else
        % v���ֵ > d��ֵ*3  =>  ����˶�  =>  ����
        if( (abs(max(input_velocity)) / abs(mean_tail_d)) > 3 )
            disp(['(v���ֵ=', num2str(abs(max(input_velocity))), ') > (d��ֵ=', num2str(abs(mean_tail_d)), ')*3']);
            output_status = 2; % ����

        % v���ֵ < d��ֵ*3  =>  ����˶�  =>  ����
        else
            disp(['(v���ֵ=', num2str(abs(max(input_velocity))), ') < (d��ֵ=', num2str(abs(mean_tail_d)), ')*3']);
            output_status = 3; % ����

        end

    end
end

