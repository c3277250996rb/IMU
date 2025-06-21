# 生成包含所有文件和文件夹的列表，存成 JSON 格式
$items = Get-ChildItem -Recurse | ForEach-Object {
    $relativePath = $_.FullName.Replace((Get-Location).Path + "\", "") -replace '\\', '/'
    [PSCustomObject]@{
        Name = $_.Name
        Path = $relativePath
        IsDirectory = $_.PSIsContainer
    }
}

$json = $items | ConvertTo-Json -Depth 10

# 生成 HTML 文件
$html = @"
<!DOCTYPE html>
<html lang='zh-CN'>
<head>
<meta charset='UTF-8'>
<title>文件浏览器</title>
<style>
    body { font-family: sans-serif; padding: 20px; }
    ul { list-style: none; padding-left: 0; }
    li { margin: 5px 0; }
    a { color: #06c; text-decoration: none; }
    a:hover { text-decoration: underline; }

    <!-- .folder::before { content: '? '; } -->
    .folder::before { content: '\1F4C1 '; }

    <!-- .file::before { content: '? '; } -->
    .file::before { content: '\1F4C4 '; }

</style>
</head>
<body>
<h1 id="currentPath">加载中...</h1>
<ul id="fileList"></ul>
<script>
    const items = $json;

    // 读取 URL ?path=xxx 参数，默认空字符串代表根目录
    function getQueryParam(name) {
        const urlParams = new URLSearchParams(window.location.search);
        return urlParams.get(name) || '';
    }

    function filterItemsByPath(path) {
        if (path === '') {
            // 根目录下，筛选路径不包含斜杠的顶层项
            return items.filter(item => !item.Path.includes('/'));
        } else {
            // 筛选以 path 开头，且去除 path 和前面斜杠后不含斜杠的子项
            return items.filter(item => {
                if (!item.Path.startsWith(path + '/')) return false;
                const rest = item.Path.substring(path.length + 1);
                return rest.indexOf('/') === -1;
            });
        }
    }

    function renderList(path) {
        const fileList = document.getElementById('fileList');
        fileList.innerHTML = '';

        // 显示当前路径
        document.getElementById('currentPath').textContent = '当前路径：/' + (path || '');

        // 返回上一级链接
        if (path) {
            const upPath = path.includes('/') ? path.substring(0, path.lastIndexOf('/')) : '';
            const upLi = document.createElement('li');
            const upA = document.createElement('a');
            upA.href = '?path=' + encodeURIComponent(upPath);

            //upA.textContent = '?? 返回上一级';
            upA.textContent = '\u2B06 返回上一级';

            upLi.appendChild(upA);
            fileList.appendChild(upLi);
        }

        const list = filterItemsByPath(path);

        if (list.length === 0) {
            const emptyLi = document.createElement('li');
            emptyLi.textContent = '（空目录）';
            fileList.appendChild(emptyLi);
            return;
        }

        list.forEach(item => {
            const li = document.createElement('li');
            const a = document.createElement('a');
            a.textContent = item.Name;
            if (item.IsDirectory) {
                a.href = '?path=' + encodeURIComponent(item.Path);
                a.className = 'folder';
            } else {
                a.href = item.Path;
                a.target = '_blank';
                a.className = 'file';
            }
            li.appendChild(a);
            fileList.appendChild(li);
        });
    }

    const currentPath = getQueryParam('path');
    renderList(currentPath);
</script>
</body>
</html>
"@

Set-Content -Encoding UTF8 -Path "index.html" -Value $html

Write-Host "? 生成成功：index.html"
