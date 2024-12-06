function output_status = status_judgement(input_all_size, input_tail_size, input_accelerate, input_velocity, input_distance)
    % 计算末端均值
    mean_tail_a = mean( input_accelerate(end-input_tail_size : end) );
    mean_tail_v = mean( input_velocity(end-input_tail_size : end) );
    mean_tail_d = mean( input_distance(end-input_tail_size : end) );


    % disp(['abs(max(input_accelerate)): ', num2str(abs(max(input_accelerate)))]);
    % disp(['abs(mean_tail_v)*3: ', num2str(abs(mean_tail_v))]);
    % disp(['abs(max(input_velocity)): ', num2str(abs(max(input_velocity)))]);
    % disp(['abs(mean_tail_d)*3: ', num2str(abs(mean_tail_d))]);


    if( abs(mean_tail_d) < 0.15 )
        output_status = 2; % 往返
        disp(['末端d均值 < 0.15  =>  往返']);
        return; % 立即返回，不执行后续代码
    end

    % a最大值 < v均值*3  =>  小幅运动  =>  震动
    if( (abs(max(input_accelerate)) / abs(mean_tail_v)) < 3 )
            output_status = 1; % 震动

    % a最大值 > v均值*3  =>  大幅运动
    else
        % v最大值 > d均值*3  =>  大幅运动  =>  往返
        if( (abs(max(input_velocity)) / abs(mean_tail_d)) > 3 )
            disp(['(v最大值=', num2str(abs(max(input_velocity))), ') > (d均值=', num2str(abs(mean_tail_d)), ')*3']);
            output_status = 2; % 往返

        % v最大值 < d均值*3  =>  大幅运动  =>  单程
        else
            disp(['(v最大值=', num2str(abs(max(input_velocity))), ') < (d均值=', num2str(abs(mean_tail_d)), ')*3']);
            output_status = 3; % 单程

        end

    end
end

