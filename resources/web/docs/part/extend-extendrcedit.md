# rcedit

  资源编辑器，用来生成exe程序.
  
## loadDiskSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  从磁盘中加载配置文件，配置文件包含四个数据段，其中三个段内容与manifest.json相同，只增加了一个文件段描述EXE文件的基本信息，段名"fileSettings",可用值如下:<br>&nbsp; **CompanyName**: 公司名称，默认值"NCUI"<br>&nbsp; **FileDescription**: 文件描述，默认值"NCUI演示程序"<br>&nbsp; **FileVersion**:文件版本，默认值"1.0.0.0"<br>&nbsp; **InternalName**:内部名称，默认值"NCUIDemo.exe"<br>&nbsp; **LegalCopyright**:法律著作权，默认值"Copyright (C) 2017"<br>&nbsp; **OriginalFileName**:原始文件名称，默认值"NCUIDemo.exe"<br>&nbsp; **ProductName**:产品名称，默认值"NCUI演示程序"<br>&nbsp; **ProductVersion**:产品版本，默认值"1.0.0.0"<br>&nbsp; **Icon**:图标路径，默认使用当前程序的图标<br>
  
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
	<td>配置文件路径，文件内容必须为JSON格式.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 成功/ false 失败. 



<div class="adoc" id="div_loadDiskSettings"></div>


## getDefaultFileSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取默认的文件参数.
  
* **函数参数**  无

* **返回值**
  JsonObject . 



<div class="adoc" id="div_getDefaultFileSettings"></div>


## getDefaultAppSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取默认的APP启动参数.
  
* **函数参数**  无

* **返回值**
  JsonObject . 



<div class="adoc" id="div_getDefaultAppSettings"></div>


## getDefaultBrowserSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取默认的浏览器窗口启动参数.
  
* **函数参数**  无

* **返回值**
  JsonObject . 



<div class="adoc" id="div_getDefaultBrowserSettings"></div>


## getDefaultSplashSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取默认的启动画面.
  
* **函数参数**  无

* **返回值**
  JsonObject . 



<div class="adoc" id="div_getDefaultSplashSettings"></div>


## getDefaultSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取默认的程序配置.
  
* **函数参数**  无

* **返回值**
  JsonObject . 



<div class="adoc" id="div_getDefaultSettings"></div>


## commit &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  生成EXE文件，这个函数将把配置好的参数写入EXE文件中.注意:**输入的参数都是JSON格式的字符串，不是JSON对象**
  
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
	<td>EXE文件信息，JSON字符串，可用见<a href="#extend/extendrcedit/0">loadDiskSettings</a>.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>启动参数,JSON字符串，可用值见<a href="#settings/settingsAppSettings">启动参数</a>.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>浏览器窗口参数,JSON字符串，可用值见<a href="#settings/settingsBrowserWindowSettings">浏览器窗口参数</a>.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>启动画面参数,JSON字符串，可用值见<a href="#settings/settingsSplashWindowSettings">启动画面</a>.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true/false. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('rcedit');
var fileSettings = rcedit.getDefaultFileSettings();
var appSettings = rcedit.getDefaultAppSettings();
var browserWindowSettings = rcedit.getDefaultBrowserSettings();
var splashWindowSettings = rcedit.getDefaultSplashSettings();
console.log(fileSettings);
console.log(appSettings);
console.log(browserWindowSettings);
console.log(splashWindowSettings);
rcedit.commit(fileSettings, appSettings, browserWindowSettings, splashWindowSettings);


```
*****
  生成EXE文件，这个函数将把配置好的参数写入EXE文件中.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('rcedit');
rcedit.commit();

```


<div class="adoc" id="div_commit"></div>


