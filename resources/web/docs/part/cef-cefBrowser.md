# Browser

  CEF提供的浏览器类封装.<br>工作线程:**UI线程**.
  
## GetHost &nbsp;<span class="label label-sync">同步</span> 

  获取浏览器的Host对象.
  
* **函数参数**  无

* **返回值**
  BrowserHost . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var host = browser.GetHost();
var browser2 = host.GetBrowser();
console.assert(browser.IsSame(browser2) == true);

```


<div class="adoc" id="div_GetHost"></div>


## CanGoBack &nbsp;<span class="label label-sync">同步</span> 

  判断当前浏览器是否可以后退.
  
* **函数参数**  无

* **返回值**
  Boolean true可以后退/false不能回退. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
console.log(browser.CanGoBack());

```


<div class="adoc" id="div_CanGoBack"></div>


## GoBack &nbsp;
  后退. BrowserWindowSettings.back_forword如果为false，或者<a href="#cef/cefBrowser/1">CanGoBack</a>==false该操作不会成功.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#cef/cefBrowser/0">GetHost</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
browser.GoBack();

```


<div class="adoc" id="div_GoBack"></div>


## CanGoForward &nbsp;<span class="label label-sync">同步</span> 

  判断当前浏览器是否可以前进.
  
* **函数参数**  无

* **返回值**
  Boolean true可以前进false不能前进. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
console.log(browser.CanGoForward());

```


<div class="adoc" id="div_CanGoForward"></div>


## GoForward &nbsp;
  前进. BrowserWindowSettings.back_forword如果为false，或者<a href="#cef/cefBrowser/3">CanGoForward</a>==false该操作不会成功.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#cef/cefBrowser/3">CanGoForward</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
browser.GoForward();

```


<div class="adoc" id="div_GoForward"></div>


## IsLoading &nbsp;
  是否正在加载页面.
  
* **函数参数**  无

* **返回值**
  Boolean true是/false否. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
console.log(browser.IsLoading());
console.assert(browser.IsLoading() != true);

```


<div class="adoc" id="div_IsLoading"></div>


## Reload &nbsp;
  重新加载页面.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
browser.Reload();

```


<div class="adoc" id="div_Reload"></div>


## ReloadIgnoreCache &nbsp;
  重新加载页面，无视任何缓存数据.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
browser.ReloadIgnoreCache();

```


<div class="adoc" id="div_ReloadIgnoreCache"></div>


## StopLoad &nbsp;
  停止加载页面.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
browser.StopLoad();

```


<div class="adoc" id="div_StopLoad"></div>


## GetIdentifier &nbsp;<span class="label label-sync">同步</span> 

  获取浏览器ID.
  
* **函数参数**  无

* **返回值**
  Int 浏览器ID. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
console.log(browser.GetIdentifier());

```


<div class="adoc" id="div_GetIdentifier"></div>


## IsSame &nbsp;<span class="label label-sync">同步</span> 

  与另一个Borwser对象比较是否相等.
  
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
	<td>Object </td>
	<td></td>
	<td>Browser 对象.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true相同/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var win = new BrowserWindow({url:'http://www.baidu.com'});
var browser2 = win.getBrowser();
console.assert(browser.IsSame(browser) == false);
setTimeout(function(){
   win.close();
}, 3000);

```


<div class="adoc" id="div_IsSame"></div>


## IsPopup &nbsp;<span class="label label-sync">同步</span> 

  是否为一个弹出窗口.
  
* **函数参数**  无

* **返回值**
  Boolean true弹出窗口/false非弹出窗口. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
console.log(browser.IsPopup());

```


<div class="adoc" id="div_IsPopup"></div>


## HasDocument &nbsp;<span class="label label-sync">同步</span> 

  是否在浏览器中加载了文档.
  
* **函数参数**  无

* **返回值**
  Boolean true有document/false 无document. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
console.log(browser.HasDocument());

```


<div class="adoc" id="div_HasDocument"></div>


## GetMainFrame &nbsp;<span class="label label-sync">同步</span> 

  返回浏览器窗口的主（顶层）框架Frame.
  
* **函数参数**  无

* **返回值**
  Frame Frame 对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var frame = browser.GetMainFrame();
console.log(frame.GetIdentifier());

```


<div class="adoc" id="div_GetMainFrame"></div>


## GetFocusedFrame &nbsp;<span class="label label-sync">同步</span> 

  返回浏览器窗口的焦点框架Frame.
  
* **函数参数**  无

* **返回值**
  Frame Frame 对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var frame = browser.GetFocusedFrame();
console.log(frame.GetIdentifier());

```


<div class="adoc" id="div_GetFocusedFrame"></div>


## GetFrame &nbsp;<span class="label label-sync">同步</span> 

  通过名称或ID获取Frame.
  
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
	<td>Int/String </td>
	<td></td>
	<td>Frame ID 或Frame 名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Frame Frame 对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var frame = browser.GetFocusedFrame();
var id = frame.GetIdentifier();
var frame2 = browser.GetFrame(id);
console.assert(frame.GetIdentifier() == frame2.GetIdentifier());

```


<div class="adoc" id="div_GetFrame"></div>


## GetFrameCount &nbsp;<span class="label label-sync">同步</span> 

  获取Frame个数.
  
* **函数参数**  无

* **返回值**
  Int Frame个数. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var count = browser.GetFrameCount();
console.log(count);
console.assert(count == 1);

```


<div class="adoc" id="div_GetFrameCount"></div>


## GetFrameIdentifiers &nbsp;<span class="label label-sync">同步</span> 

  获取所有Frame的ID，返回一个数组[].
  
* **函数参数**  无

* **返回值**
  Array . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var arr = browser.GetFrameIdentifiers();
console.log(arr);
console.assert(arr.length == 1);

```


<div class="adoc" id="div_GetFrameIdentifiers"></div>


## GetFrameNames &nbsp;
  获取所有Frame的名称，返回一个数组[].
  
* **函数参数**  无

* **返回值**
  Array . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var arr = browser.GetFrameNames();
console.log(arr);
console.assert(arr.length == 1);

```


<div class="adoc" id="div_GetFrameNames"></div>


## currentFrame &nbsp;
  获取当前Frame.
  
* **函数参数**  无

* **返回值**
  Frame Frame 对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'Browser');
var browser = BrowserWindow.current.getBrowser();
var frame = browser.currentFrame();
console.log(frame.GetIdentifier());

```


<div class="adoc" id="div_currentFrame"></div>


