# ����ļ�
$outputHtml = "index.html"

# �����ļ��б�
$files = Get-ChildItem -Recurse -File | ForEach-Object {
    $_.FullName.Replace((Get-Location).Path + "\", "") -replace '\\', '/'
}

# ��ʼ���� HTML ����
$html = @"
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>�ļ��б�</title>
    <style>
        body { font-family: sans-serif; padding: 20px; }
        ul { list-style: none; padding-left: 0; }
        li { margin: 5px 0; }
        a { color: #0a5; text-decoration: none; }
        a:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <h1>��ǰĿ¼�ļ��б�</h1>
    <ul id="file-list">������...</ul>

    <script>
        const files = [
"@

# ����ļ�����
$html += ($files | ForEach-Object { "            `"$($_)`"," }) -join "`n"

# ��βȥ����󶺺�
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

# д�� HTML �ļ�
Set-Content -Path $outputHtml -Value $html -Encoding UTF8

Write-Host "? �ļ������ɣ�$outputHtml"
