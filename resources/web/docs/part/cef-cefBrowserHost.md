# BrowserHost

  CEF BrowserHost 对象封装.<br>工作线程:**UI线程**.
  
## click &nbsp;
  向页面发送单击事件.
  
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
	<td>X轴坐标.</td>
</tr><tr>
	<td>Int</td>
	<td>0 </td>
	<td>Y轴坐标</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost');
var host = BrowserHost.current;
var x = $('.closebtn').offset().left;
var y = $('.closebtn').offset().top;

console.log(x);
console.log(y);
host.click(x + 20, y + 20);

```


<div class="adoc" id="div_click"></div>


## GetBrowser &nbsp;<span class="label label-sync">同步</span> 

  获取Host所对应的Browser对象.
  
* **函数参数**  无

* **返回值**
  Browser . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost', 'Browser');
var host = BrowserHost.current;
console.log(host.GetBrowser().GetIdentifier());


```


<div class="adoc" id="div_GetBrowser"></div>


## CloseBrowser &nbsp;
  关闭浏览器.
  
* **函数参数**  无

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost', 'Browser');
var host = BrowserHost.current;
host.CloseBrowser();


```


<div class="adoc" id="div_CloseBrowser"></div>


## SetFocus &nbsp;
  使浏览器获得焦点.
  
* **函数参数**  无

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost', 'Browser');
var host = BrowserHost.current;
host.SetFocus();


```


<div class="adoc" id="div_SetFocus"></div>


## GetZoomLevel &nbsp;<span class="label label-sync">同步</span> 

  获取页面缩放等级.
  
* **函数参数**  无

* **返回值**
  Double . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost', 'Browser');
var host = BrowserHost.current;
console.log(host.GetZoomLevel());
host.SetZoomLevel(1.0);
console.assert(host.GetZoomLevel() == 1.0);
host.SetZoomLevel(0.0);

```


<div class="adoc" id="div_GetZoomLevel"></div>


## SetZoomLevel &nbsp;
  设备页面缩放等级 .
  
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
	<td>Double </td>
	<td></td>
	<td>缩放等级.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost', 'Browser');
var host = BrowserHost.current;
host.SetZoomLevel(-0.6);

```


<div class="adoc" id="div_SetZoomLevel"></div>


