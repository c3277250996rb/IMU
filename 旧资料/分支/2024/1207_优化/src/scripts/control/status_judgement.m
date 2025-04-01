function output_status = status_judgement(input_all_size, input_tail_size, input_accelerate, input_original_v_Xukf, input_velocity, input_distance)

    a_v_times = 3;
    disp(['max(abs(input_accelerate)): ', num2str(max(abs(input_accelerate)))]);
    if 0 < max(abs(input_accelerate)) < 2
        v_d_times = 3;
    end
    if 2 < max(abs(input_accelerate)) < 6
        v_d_times = 4;
    end
    if 6 < max(abs(input_accelerate))
        v_d_times = 6;
    end
    
    % 计算末端均值
        mean_tail_a = mean( input_accelerate(end-input_tail_size : end) );
        mean_tail_o_v = mean( input_original_v_Xukf(end-input_tail_size : end) );
        mean_tail_v = mean( input_velocity(end-input_tail_size : end) );
        mean_tail_d = mean( input_distance(end-input_tail_size : end) );
    %

    % DEBUG INFO
        % disp(['abs(max(input_accelerate)): ', num2str(abs(max(input_accelerate)))]);
        % disp(['abs(mean_tail_v)*3: ', num2str(abs(mean_tail_v))]);
        % disp(['abs(max(input_velocity)): ', num2str(abs(max(input_velocity)))]);
        % disp(['abs(mean_tail_d)*3: ', num2str(abs(mean_tail_d))]);
    %

    if( abs(mean_tail_d) < 0.15 )
        output_status = 1; % 震动
        disp(['末端d均值 < 0.15  =>  震动']);
        return; % 立即返回，不执行后续代码
    end

    % a最大值 < v末端均值*3  =>  小幅运动  =>  震动
    if( ( (max(abs(input_accelerate))/abs(mean_tail_v)) < a_v_times ) )
            disp(['(a最大值=', num2str(abs(max(input_accelerate))), ') > (v末端均值=', num2str(abs(mean_tail_v)), ')*', num2str(a_v_times)]);
            output_status = 1; % 震动

    % a最大值 > v末端均值*3  =>  大幅运动
    else
        % v最大值 > d末端均值*3  =>  大幅运动  =>  往返
        if( (max(abs(input_velocity)) / abs(mean_tail_d)) > v_d_times )
            disp(['(v最大值=', num2str(max(abs(input_velocity))), ') > (d末端均值=', num2str(abs(mean_tail_d)), ')*', num2str(v_d_times)]);
            output_status = 2; % 往返

        % v最大值 < d末端均值*3  =>  大幅运动  =>  单程
        else
            disp(['(v最大值=', num2str(max(abs(input_velocity))), ') < (d末端均值=', num2str(abs(mean_tail_d)), ')*', num2str(v_d_times)]);
            output_status = 3; % 单程

        end

    end
end

