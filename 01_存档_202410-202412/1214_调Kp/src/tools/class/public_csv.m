function output_csv = public_csv()

    ext = '.csv';
    %
        % folder_1st = 'data\';
        % if get_is_use_uart()
        %     folder_2nd = 'common\';
        % else
        %     folder_2nd = 'storage\';
        % end
    %




%%% common

    % output_csv = ['6axis_data', ext];

%%%




%%% base

    % output_csv = ['return_fast_3cm', ext];
    % output_csv = ['return_fast_20cm', ext];

    % output_csv = ['return_slow_3cm', ext];
    % output_csv = ['return_slow_20cm', ext];

    % output_csv = ['shake_1cm', ext];
    % output_csv = ['shake_2mm', ext];
    % output_csv = ['shake_double', ext];

    % output_csv = ['single_fast_3cm', ext];
    % output_csv = ['single_fast_20cm', ext];

    % output_csv = ['single_slow_3cm', ext];
    % output_csv = ['single_slow_20cm', ext];

%%%




%%% euler

    % output_csv = ['9axes_data', ext];
    output_csv = ['test_PID_pitch_+-90degree_return_slow', ext];
    % output_csv = ['test_PID_pitch_+-90degree_return_fast', ext];

%%%




%%% robot

    % output_csv = ['484', ext];

    % output_csv = ['0.8mps_0.8m_return_2', ext];

    % output_csv = ['0.8mps_0.8m_single_1', ext];
    % output_csv = ['0.8mps_0.8m_single_2', ext];

    % output_csv = ['0.8mps_2m_single_1', ext];
    % output_csv = ['0.8mps_2m_single_2', ext];
    % output_csv = ['0.8mps_2m_single_3', ext];

%%%




%%% rotate

    % output_csv = ['test_rotate_x+_single_slow', ext]; % 实验用数据 验证旋转
    % output_csv = ['test_rotate_y+_single_slow', ext]; % 实验用数据 验证旋转

%%%




    disp(['[当前使用]: ', output_csv]);
end