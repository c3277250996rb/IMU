# ���ɰ��������ļ����ļ��е��б���� JSON ��ʽ
$items = Get-ChildItem -Recurse | ForEach-Object {
    $relativePath = $_.FullName.Replace((Get-Location).Path + "\", "") -replace '\\', '/'
    [PSCustomObject]@{
        Name = $_.Name
        Path = $relativePath
        IsDirectory = $_.PSIsContainer
    }
}

$json = $items | ConvertTo-Json -Depth 10

# ���� HTML �ļ�
$html = @"
<!DOCTYPE html>
<html lang='zh-CN'>
<head>
<meta charset='UTF-8'>
<title>�ļ������</title>
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
<h1 id="currentPath">������...</h1>
<ul id="fileList"></ul>
<script>
    const items = $json;

    // ��ȡ URL ?path=xxx ������Ĭ�Ͽ��ַ��������Ŀ¼
    function getQueryParam(name) {
        const urlParams = new URLSearchParams(window.location.search);
        return urlParams.get(name) || '';
    }

    function filterItemsByPath(path) {
        if (path === '') {
            // ��Ŀ¼�£�ɸѡ·��������б�ܵĶ�����
            return items.filter(item => !item.Path.includes('/'));
        } else {
            // ɸѡ�� path ��ͷ����ȥ�� path ��ǰ��б�ܺ󲻺�б�ܵ�����
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

        // ��ʾ��ǰ·��
        document.getElementById('currentPath').textContent = '��ǰ·����/' + (path || '');

        // ������һ������
        if (path) {
            const upPath = path.includes('/') ? path.substring(0, path.lastIndexOf('/')) : '';
            const upLi = document.createElement('li');
            const upA = document.createElement('a');
            upA.href = '?path=' + encodeURIComponent(upPath);

            //upA.textContent = '?? ������һ��';
            upA.textContent = '\u2B06 ������һ��';

            upLi.appendChild(upA);
            fileList.appendChild(upLi);
        }

        const list = filterItemsByPath(path);

        if (list.length === 0) {
            const emptyLi = document.createElement('li');
            emptyLi.textContent = '����Ŀ¼��';
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

Write-Host "? ���ɳɹ���index.html"
