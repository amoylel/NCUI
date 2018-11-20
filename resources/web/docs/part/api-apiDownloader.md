# Downloader

  文件下载类，使用浏览器的下载功能下载文件.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## 事件
### **1.start** 

  开始下载时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td><br>[**Boolean**] isValid 当前下载是否有效.<br>[**Boolean**] isInProgress 是否正在下载（包括暂停）.<br>[**Boolean**] isComplete 当前下载是否已经完成.<br>[**Boolean**] isCanceled 当前下载是否已经被取消.<br><br>[**Int**] currentSpeed 当前下载速度.<br>[**Int**] percentComplete 下载完成百分比，如果返回-1，表示未获取到所下载文件的大小.<br>[**Int**] totalBytes 总字节数.<br>[**Int**] receivedBytes 已接收的字节数.<br><br>[**Double**] startTime 下载开始的时间.<br>[**Double**] endTime 下载结束的时间.<br><br>[**String**] fullPath 下载文件所保存的完整路径.<br>[**String**] id 当前下载的唯一标识符.<br>[**String**] url URL.<br>[**String**] originalUrl 重定向之前的原始URL（如果被重定向的话）.<br>[**String**] suggestedFileName 浏览器建议的文件名.<br>[**String**] contentDisposition  .<br>[**String**] mimeType MIME类型.<br></td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_start"></div>


### **2.resume** 

  下载恢复时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>与start事件中的参数相同.</td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_resume"></div>


### **3.pause** 

  暂停下载时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>与start事件中的参数相同.</td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_pause"></div>


### **4.cancel** 

  取消下载时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>与start事件中的参数相同.</td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_cancel"></div>


### **5.canceled** 

  取消下载后时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>与start事件中的参数相同.</td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_canceled"></div>


### **6.update** 

  下载数据更新时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>与start事件中的参数相同.</td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_update"></div>


### **7.complete** 

  下载完成时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>与start事件中的参数相同.</td>
</tr>
	</tbody>
</table>



<div class="adoc" id="div_complete"></div>


## Downloader &nbsp;<span class="label label-constructor">构造</span> 

  创建一下下载类，用于下载文件.
  
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
	<td>下载参数.<br>[**String**]	url 下载文件的url,如果所给的URL与之前所创建的下载对象url相同反返回之前的对象，下面的参数将被忽略.<br>[**String**]	file 文件保存路径, 默认会根据url自动命名.<br>[**Boolean**]	autoDownload 是否自动开始下载,默认为true.<br>[**Boolean**]	forceReplace 是否强制替换已经存在的文件,默认为true.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Object 返回一个Downloader实例. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要先运行该示例才能运行后面的示例
include('Downloader');
window.downloader = new Downloader({
   url: 'https://dldir1.qq.com/qqfile/qq/TIM1.2.0/21645/TIM1.2.0.exe'
});
// 监听下载开始事件
window.downloader.on('start', function(item){

   console.log('下载开始');
   console.log(item);
});

window.downloader.on('resume', function(item){
    console.log('下载恢复');
});

window.downloader.on('pause', function(item){
    console.log('下载暂停');
});

window.downloader.on('cancel', function(item){
   console.log('下载取消');
});

window.downloader.on('canceled', function(item){
    console.log('下载取消完成');
});

window.downloader.on('update', function(item){
   console.log('下载进度:' + item.percentComplete);
});

window.downloader.on('complete', function(item){
    console.log('下载完毕');
});

```


<div class="adoc" id="div_Downloader"></div>


## start &nbsp;
  开始下载.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
window.downloader.start();

```


<div class="adoc" id="div_start"></div>


## pause &nbsp;
  暂停下载.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
window.downloader.pause();

```


<div class="adoc" id="div_pause"></div>


## resume &nbsp;
  恢复当前下载.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
window.downloader.resume();

```


<div class="adoc" id="div_resume"></div>


## cancel &nbsp;
  取消下载.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
window.downloader.cancel();

```


<div class="adoc" id="div_cancel"></div>


