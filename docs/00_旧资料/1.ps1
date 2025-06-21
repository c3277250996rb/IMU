# 输出文件
$outputHtml = "index.html"

# 生成文件列表
$files = Get-ChildItem -Recurse -File | ForEach-Object {
    $_.FullName.Replace((Get-Location).Path + "\", "") -replace '\\', '/'
}

# 开始生成 HTML 内容
$html = @"
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>文件列表</title>
    <style>
        body { font-family: sans-serif; padding: 20px; }
        ul { list-style: none; padding-left: 0; }
        li { margin: 5px 0; }
        a { color: #0a5; text-decoration: none; }
        a:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <h1>当前目录文件列表</h1>
    <ul id="file-list">加载中...</ul>

    <script>
        const files = [
"@

# 添加文件数组
$html += ($files | ForEach-Object { "            `"$($_)`"," }) -join "`n"

# 结尾去掉最后逗号
$html = $html.TrimEnd(",`n") + @"
        ];
        const list = document.getElementById('file-list');
        list.innerHTML = "";
        files.forEach(path => {
            const li = document.createElement("li");
            const a = document.createElement("a");
            a.href = path;
            a.textContent = path;
            a.target = "_blank";
            li.appendChild(a);
            list.appendChild(li);
        });
    </script>
</body>
</html>
"@

# 写入 HTML 文件
Set-Content -Path $outputHtml -Value $html -Encoding UTF8

Write-Host "? 文件已生成：$outputHtml"
