function xlabel_csv()

    xlabel(['当前使用: ', public_csv()], 'Interpreter', 'none');

end



% 在 MATLAB 中，下划线 `_` 默认会被解释为 **LaTeX 子脚本符号**，导致 `2` 被缩小并显示为下标。如果你希望正常显示 `1_2`，可以通过以下方法解决：

% ---

% ### **方法 1: 使用反斜杠 `\` 转义下划线**
% 在字符串中的下划线前添加 `\`，MATLAB 会将其视为普通字符而不是 LaTeX 符号：
% ```matlab
% xlabel('1\_2');
% ```

% ---

% ### **方法 2: 禁用 LaTeX 解释**
% 使用 `Interpreter` 属性将文本解释器设为 `'none'`，MATLAB 不会将 `_` 解析为子脚本符号：
% ```matlab
% xlabel('1_2', 'Interpreter', 'none');
% ```

% ---

% ### **方法 3: 使用 `tex` 模式**
% 虽然 MATLAB 默认使用 LaTeX 解释器，但也可以明确切换到 `tex` 模式并转义 `_`：
% ```matlab
% xlabel('1\_2', 'Interpreter', 'tex');
% ```

% ---

% ### **方法 4: 完全自定义格式**
% 如果需要更多控制，可以使用 LaTeX 的完整能力（如字体、大小等），但记得正确转义 `_`：
% ```matlab
% xlabel('$1\_2$', 'Interpreter', 'latex');
% ```

% ---

% ### 总结
% - **快速解决**：使用转义字符 `\_`。
% - **更灵活设置**：设置 `Interpreter` 为 `'none'`。

% 以下代码可供参考：
% ```matlab
% figure;
% plot(1:10);
% xlabel('1\_2');                 % 转义下划线
% xlabel('1_2', 'Interpreter', 'none');  % 关闭 LaTeX 解释
% xlabel('$1\_2$', 'Interpreter', 'latex');  % 使用 LaTeX
% ```