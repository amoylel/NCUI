# app

  控制程序程序的生命周期，并提供一些与程序相关的功能.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## getConfig &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取应用程序的所有配置参数,可获取的内容见<a href="#settings/settingsAppSettings">启动参数</a>.
  
* **函数参数**  无

* **返回值**
  JsonObject 所有配置参数. 

*****
  获取应用程序的指定配置参数.
  
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
	<td>需要获取的字段名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
   返回字段名所对应的配置参数。返回类型视配置参数类型而定，为JS所支持的基本数据类型. 

* **参考** 
<a href="#api/apiapp/1">setConfig</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//获取应用程序的所有配置参数
var config = app.getConfig();
console.log(config);

//获取应用程序的指定配置参数
var single_process = app.getConfig('single_process');
console.log('单进程模式:' + single_process);

```


<div class="adoc" id="div_getConfig"></div>


## setConfig &nbsp;<span class="label label-static">静态</span> 

  设置应用程序配置参数.该函数并不要求一定要输入所支持的参数，你也可以设置自定义参数，并在getConfig里面获取所设置的值与<a href="#api/apiapp/0">app.getConfig</a>相对应.
  
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
	<td>JsonObject </td>
	<td></td>
	<td>需要设置的参数，该值为一个JSON对象.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiapp/0">getConfig</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
app.setConfig({
    debugMode: true,
    custom: 'custom'
});


```


<div class="adoc" id="div_setConfig"></div>


## toAbsolutePath &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  将一个路径转为绝对路径如:%web%.
  
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
	<td>需要转换的路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  String 转换后的绝对路径. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 获取web资源文件目录
var webDir = app.toAbsolutePath('%webDir%');
console.log(webDir);
console.log(app.toAbsolutePath('%webDir%docs/index.html'));

// 获取图片文件所有目录
var picturesDir = app.toAbsolutePath('%picturesDir%');
console.log(picturesDir);


```


<div class="adoc" id="div_toAbsolutePath"></div>


## quit &nbsp;<span class="label label-static">静态</span> 

  尝试退出程序，可以用户被取消（如window.unload函数）.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip
 app.quit();

```


<div class="adoc" id="div_quit"></div>


## exit &nbsp;<span class="label label-static">静态</span> 

  强制退出程序.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip
// 直接退出程序，不会有任何提示
app.exit();

```


<div class="adoc" id="div_exit"></div>


## addUrlMapping &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  添加URL映射路径,简单来说就是将URL替换为被映射的URL或文件路径.
  
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
	<td>需要添加映射的URL.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>被映射的本地文件路径，该路径可以是一个普通的文件或文件夹，也可以是ZIP压缩包或DLL动态库.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean 成为返回true, 失败返回false. 

* **示例&nbsp;&nbsp;--&nbsp;&nbsp;映射到另一个URL**

```html
// 增加映射
app.addUrlMapping('http://www.baidu.com', 'http://www.qq.com');
window.open('http://www.baidu.com');

app.addUrlMapping('http://www.baidu.com/more', 'http://www.qq.com');
window.open('http://www.baidu.com/more');


```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;映射到文件**

```html
// 你也可以指定一个绝对路径
//    app.addUrlMapping('http://www.baidu.com/', 'E:/webDir/index.hmtl');
//    app.addUrlMapping('http://www.baidu.com/more', 'E:/webDir/more/more.html');

app.addUrlMapping('http://www.baidu.com', '%webDir%example/BrowserWindow.html');
window.open('http://www.baidu.com');

app.addUrlMapping('http://www.baidu.com/more', '%webDir%/example/FramelessWindow.html');
window.open('http://www.baidu.com/more');


```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;映射到ZIP文件**

```html
// zip文件同样可以是一个绝对路径
// app.addUrlMapping('http://www.baidu.com', 'zip:///D:/web.zip');

app.addUrlMapping('http://127.0.0.1:8088', 'zip:///%webDir%web.zip');
window.open('http://127.0.0.1:8088/example/BrowserWindow.html');

app.addUrlMapping('http://127.0.0.1:8088/test',  'zip:///%webDir%web.zip/example/FramelessWindow.html');
window.open('http://127.0.0.1:8088/test');


```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;映射到DLL文件**

```html
// demo中的dll所提供的根目录为当前程序所在目录下的web目录
app.addUrlMapping('http://www.baidu.com', 'dll:///libResource.dll');
window.open('http://www.baidu.com/example/BrowserWindow.html');

app.addUrlMapping('http://www.baidu.com/more', 'dll:///libResource.dll/example');
window.open('http://www.baidu.com/more/BrowserWindow.html');

```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;映射到RES文件**

```html
// RES本质上与ZIP是一下，只是将ZIP文件提前写入EXE的资源文件里.
// 你可以参考程序生成里面的内容，将ZIP文件写到EXE里面去
// 当前文档程序没有ID为131的资源，所以下面的方法不能正确的打开页面
app.addUrlMapping('http://www.baidu.com', 'res:///131.res');
window.open('http://www.baidu.com/example/BrowserWindow.html');


```


<div class="adoc" id="div_addUrlMapping"></div>


## removeUrlMapping &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  移除URL映射路径.
  
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
	<td>需要移除的URL.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 删除映射
app.removeUrlMapping('http://www.baidu.com');
window.open('http://www.baidu.com');

```


<div class="adoc" id="div_removeUrlMapping"></div>


## urlToNativePath &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  将一个URL转换为本地路径，这几个函数可能会被IO线程和UI线程同时调用，所以要加锁.
  
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
	<td>需要转换的URL.</td>
</tr><tr>
	<td>Boolean</td>
	<td>false </td>
	<td>是否要求所映射的文件存在 .</td>
</tr>
	</tbody>
</table>

* **返回值**
  String 转换后的路径，如果当前所给的URL没有与之映射的本地路径，将返回该URL. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
app.addUrlMapping('http://test/test.html', '%webDir%/index.html');
console.log(app.urlToNativePath('http://test/test.html', true));
console.assert(app.urlToNativePath('http://test/test.html', true) != '');
console.assert(app.urlToNativePath('http://test/test2.html', true) == '');
console.assert(app.urlToNativePath('http://test/test2.html', false)  == '');
console.assert(app.urlToNativePath('http://www.github.com', true)  == '');
console.assert(app.urlToNativePath('http://www.github.com', false)  == '');


```


<div class="adoc" id="div_urlToNativePath"></div>


## getAutoRun &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> <span class="label label-deprecated">废弃</span> 

  获取是否开机启动，使用<a href="#api/apiapp/9">isAutoRun</a>代替.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **参考** 
<a href="#api/apiapp/10">setAutoRun</a>



<div class="adoc" id="div_getAutoRun"></div>


## isAutoRun &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取是否开机启动.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **参考** 
<a href="#api/apiapp/10">setAutoRun</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('app');
app.setAutoRun(false);
console.assert(app.isAutoRun() == false);

```


<div class="adoc" id="div_isAutoRun"></div>


## setAutoRun &nbsp;<span class="label label-static">静态</span> 

  设置是否开机启动.
  
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
	<td>Boolean</td>
	<td>false </td>
	<td>true开机启动，fase取消开机启动.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiapp/9">isAutoRun</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 设置开机启动
app.setAutoRun(true);
console.assert(app.isAutoRun() == true);

// 取消开机启动
app.setAutoRun(false);
console.assert(app.isAutoRun() == false);

```


<div class="adoc" id="div_setAutoRun"></div>


## elapsed &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  当前程序从启动到现在的运行时间(毫秒).
  
* **函数参数**  无

* **返回值**
  Int . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var elapsed = app.elapsed();
console.log('程序已运行:' + elapsed + ' ms');

```


<div class="adoc" id="div_elapsed"></div>


## restart &nbsp;<span class="label label-static">静态</span> 

  重启当前程序.
  
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
	<td>Int</td>
	<td>0 </td>
	<td>重启延时(秒)，当前程序会立即关闭，新的程序会在延时时间到达后启动.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean 生成返回true, 失败返回false. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip  3秒后重启运行当前程序
app.restart(3);

```


<div class="adoc" id="div_restart"></div>


## destroy &nbsp;<span class="label label-static">静态</span> 

  销毁当前程序，(从磁盘上删除当前可执行程序).
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip 不要试！！！
app.destroy();

```


<div class="adoc" id="div_destroy"></div>


## setZipPassword &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  设置ZIP文件密码，用于打开加密的ZIP文件.
  
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
	<td>String</td>
	<td> </td>
	<td>需要设置密码的ZIP文件路径.</td>
</tr><tr>
	<td>String</td>
	<td>'' </td>
	<td>密码.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//
app.setZipPassword('%webDir%web1.zip', '123456');

```


<div class="adoc" id="div_setZipPassword"></div>


## setResPassword &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  RES密码.
  
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
	<td>String</td>
	<td> </td>
	<td>需要设置密码的RES文件ID.</td>
</tr><tr>
	<td>String</td>
	<td>'' </td>
	<td>密码.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
app.setResPassword();

```


<div class="adoc" id="div_setResPassword"></div>


