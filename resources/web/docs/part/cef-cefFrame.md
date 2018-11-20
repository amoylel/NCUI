# Frame

  CEF Frame 封装.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## injectScript &nbsp;
  向页面注入JS代码.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>JS代码.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
var frame = Frame.current;
frame.injectScript('@file:///example/js/testJs.js');

```


<div class="adoc" id="div_injectScript"></div>


## injectCSS &nbsp;
  向页面注入CSS代码.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>CSS代码.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
var frame = Frame.current;
frame.injectCSS('@file:///example/css/zui-theme.css');

```


<div class="adoc" id="div_injectCSS"></div>


## IsValid &nbsp;<span class="label label-sync">同步</span> 

  当前页面是否有效.
  
* **函数参数**  无

* **返回值**
  Boolean true有效/false无效. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Frame');
var frame = Frame.current;
console.log(frame.IsValid());


```


<div class="adoc" id="div_IsValid"></div>


## Undo &nbsp;
  撤消，编辑框内有效.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_Undo"></div>


## Redo &nbsp;
  重做，编辑框内有效.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_Redo"></div>


## Cut &nbsp;
  剪切，编辑框内有效.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_Cut"></div>


## Copy &nbsp;
  复制，编辑框内有效.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_Copy"></div>


## Paste &nbsp;
  粘贴，编辑框内有效.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_Paste"></div>


## Delete &nbsp;
  删除，编辑框内有效.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_Delete"></div>


## SelectAll &nbsp;
  全选.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Frame');
Frame.current.SelectAll();


```


<div class="adoc" id="div_SelectAll"></div>


## LoadURL &nbsp;
  加载URL.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>URL.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
Frame.current.LoadURL('http://www.baidu.com');

```


<div class="adoc" id="div_LoadURL"></div>


## LoadString &nbsp;
  加载HTML代码.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>HTML代码.</td>
</tr><tr>
	<td>String</td>
	<td>"" </td>
	<td>URL,选填</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip vConsole转换存在问题，程序中可以正常使用
include('Frame');
Frame.current.LoadString(`
    <!DOCTYPE html>
    <html lang="zh-CN">

        <head>
            <meta charset='utf-8'>
            <script type="text/javascript">
                includes('app', 'BrowserWindow');
            </script>
            <style>
                html,
                body {
                    width: 100%;
                    height: 100%;
                    overflow: hidden;
                }
                .container {
                    position: absolute;
                    top: 0;
                    left: 0;
                    right: 0;
                    bottom: 0;
                }

                .drag {
                    -webkit-app-region: drag;
                }

                .no-drag {
                    -webkit-app-region: no-drag;
                }
            </style>
        </head>

        <body>
            <div class="container ">
                <h1>我是一个普通浏览器窗口<a href="#" onclick="recoveryManifest();">点我恢复manifest.json</a></h1>
                <h1>我是一个普通浏览器窗口<a href="#" onclick="app.restart();">点我重启APP</a></h1>
            </div>
        </body>
        <script type="text/javascript">


            function recoveryManifest() {
                include('Path');
                var path = new Path('manifest2.json');
                if(!path.isExsit()) return;
                path.copyTo('manifest.json');
                path.remove();
            }
        </script>

    </html>
`);

```


<div class="adoc" id="div_LoadString"></div>


## IsMain &nbsp;<span class="label label-sync">同步</span> 

  当前页面是否为主页面.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.assert(Frame.current.IsMain() == true);

```


<div class="adoc" id="div_IsMain"></div>


## IsFocused &nbsp;<span class="label label-sync">同步</span> 

  当前页面是否获得焦点.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.assert(Frame.current.IsFocused() == true);

```


<div class="adoc" id="div_IsFocused"></div>


## GetName &nbsp;<span class="label label-sync">同步</span> 

  获取当前页面的名称.
  
* **函数参数**  无

* **返回值**
  String 页面名称. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.assert(Frame.current.GetName() == '');

```


<div class="adoc" id="div_GetName"></div>


## GetIdentifier &nbsp;<span class="label label-sync">同步</span> 

  获取当前页面的ID.
  
* **函数参数**  无

* **返回值**
  Int identifier. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.log(Frame.current.GetIdentifier());

```


<div class="adoc" id="div_GetIdentifier"></div>


## GetParent &nbsp;<span class="label label-sync">同步</span> 

  获取当前页面的父页面.
  
* **函数参数**  无

* **返回值**
  Frame 如果不存在返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.log(Frame.current.GetParent());

```


<div class="adoc" id="div_GetParent"></div>


## GetURL &nbsp;<span class="label label-sync">同步</span> 

  获取当前页面的URL.
  
* **函数参数**  无

* **返回值**
  String url. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.log(Frame.current.GetURL());

```


<div class="adoc" id="div_GetURL"></div>


## GetBrowser &nbsp;<span class="label label-sync">同步</span> 

  获取当前页面所属的Browser对象.
  
* **函数参数**  无

* **返回值**
  Browser . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Frame');
console.log(Frame.current.GetBrowser().GetIdentifier());

```


<div class="adoc" id="div_GetBrowser"></div>


