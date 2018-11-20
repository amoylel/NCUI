# BrowserWindow

  浏览器窗口创建及控制.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## addBrowserWindowSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  为一个URL预设置窗口参数，当这个URL对应及其子路径的浏览器窗口由浏览器触发创建时，将使用该方法预设置的参数.<br>说明:**该方法并不能影响new BrowserWindow() 创建的浏览器窗口**
  
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
	<td>需要预设置窗口参数的url.</td>
</tr><tr>
	<td>JsonObject </td>
	<td></td>
	<td>可以支持的<a href="#settings/settingsBrowserWindowSettings">浏览器窗口参数</a></td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#settings/settingsBrowserWindowSettings">浏览器窗口参数</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'app');
app.removeUrlMapping( "http://www.baidu.com/more");
BrowserWindow.addBrowserWindowSettings('https://www.baidu.com',{titleBar: true});
window.open('https://www.baidu.com');
window.open('https://www.baidu.com/more');


```


<div class="adoc" id="div_addBrowserWindowSettings"></div>


## removeBrowserWindowSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  移除一个URL的默认窗口参数.
  
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
	<td>需要移除浏览器窗口参数的URL.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 


* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
BrowserWindow.removeBrowserWindowSettings('https://www.baidu.com');
window.open('https://www.baidu.com');
window.open('https://www.baidu.com/more');

```


<div class="adoc" id="div_removeBrowserWindowSettings"></div>


## getBrowserWindowSettings &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取指定URL的默认窗口参数.
  
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
  JsonObject . 



<div class="adoc" id="div_getBrowserWindowSettings"></div>


## BrowserWindow &nbsp;<span class="label label-constructor">构造</span> <span class="label label-sync">同步</span> 

  创建一个浏览器窗口.
  
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
	<td>参见<a href="#settings/settingsBrowserWindowSettings">浏览器窗口参数</a>.</td>
</tr>
	</tbody>
</table>

* **返回值**
  BrowserWindow 一个浏览器窗口对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
// 创建一个普通窗口
var win = new BrowserWindow({titleBar:true, url: 'http://127.0.0.1:8030/example/BrowserWindow.html'});
// 创建一个离屏窗口
new BrowserWindow({
        titleBar:false,
        url: 'http://127.0.0.1:8030/example/FramelessWindow.html',
        offscreen: true
    });
// 创建一个透明窗口
new BrowserWindow({
        titleBar:false,
        hasShadow:false,
        url: 'http://127.0.0.1:8030/example/TransparentWindow.html',
        offscreen: true,
        transparent: true
    });

```


<div class="adoc" id="div_BrowserWindow"></div>


## destroy &nbsp;
  强制关闭窗口,用户不能取消该操作, 但是它保证了 closed 触发.<br>当<a href="#api/apiBrowserWindow/38">isClosable</a>为true,该函数无效.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/74">close</a>, <a href="#api/apiBrowserWindow/37">setClosable</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip
// 窗口将被强制关闭，用户不能取消该操作
var win = BrowserWindow.current;
win.destroy();

```


<div class="adoc" id="div_destroy"></div>


## focus &nbsp;
  窗口获得焦点.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.focus();

```


<div class="adoc" id="div_focus"></div>


## isFocused &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否获得焦点.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
console.assert(win.isFocused() == true);

win.minimize();
console.assert(win.isFocused() == false);

win.restore();
console.assert(win.isFocused() == true);

```


<div class="adoc" id="div_isFocused"></div>


## show &nbsp;
  显示并且使窗口获得焦点.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/9">hide</a>, <a href="#api/apiBrowserWindow/10">isVisible</a>, <a href="#api/apiBrowserWindow/8">showInactive</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.show();
console.assert(win.isFocused() == true);


```


<div class="adoc" id="div_show"></div>


## showInactive &nbsp;
  显示窗口但是不获得焦点.但如果窗口本身就已经获得焦点，调用该函数后窗口依然有焦点
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/9">hide</a>, <a href="#api/apiBrowserWindow/10">isVisible</a>, <a href="#api/apiBrowserWindow/7">show</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.show();
console.assert(win.isFocused() == true);

win.showInactive();
console.assert(win.isFocused() == true);

```


<div class="adoc" id="div_showInactive"></div>


## hide &nbsp;
  隐藏窗口.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/10">isVisible</a>, <a href="#api/apiBrowserWindow/7">show</a>, <a href="#api/apiBrowserWindow/8">showInactive</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.hide();
console.assert(win.isVisible() == false);
console.assert(win.isFocused() == false);

// 显示但不激活窗口
win.showInactive();
console.assert(win.isFocused() == false);

```


<div class="adoc" id="div_hide"></div>


## isVisible &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否可见.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **参考** 
<a href="#api/apiBrowserWindow/9">hide</a>, <a href="#api/apiBrowserWindow/7">show</a>, <a href="#api/apiBrowserWindow/8">showInactive</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
 win.hide();
console.assert(win.isVisible() == false);

 win.showInactive();
 console.assert(win.isVisible() == true);
 console.assert(win.isFocused() == false);

 win.show();
 console.assert(win.isVisible() == true);
 console.assert(win.isFocused() == true);

```


<div class="adoc" id="div_isVisible"></div>


## maximize &nbsp;
  窗口最大化.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/12">unmaximize</a>, <a href="#api/apiBrowserWindow/15">restore</a>, <a href="#api/apiBrowserWindow/13">isMaximized</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.maximize();
console.assert(win.isMaximized()==true);

```


<div class="adoc" id="div_maximize"></div>


## unmaximize &nbsp;
  取消窗口最大化，建议使用<a href="#api/apiBrowserWindow/15">restore</a>代替
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/15">restore</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.unmaximize();
console.assert(win.isMaximized() == false);

```


<div class="adoc" id="div_unmaximize"></div>


## isMaximized &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否最大化.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.maximize();
console.assert(win.isMaximized() == true);

win.unmaximize();
console.assert(win.isMaximized() == false);

```


<div class="adoc" id="div_isMaximized"></div>


## minimize &nbsp;
  窗口最小化.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/16">isMinimized</a>, <a href="#api/apiBrowserWindow/15">restore</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;

win.minimize();
console.assert(win.isMinimized() == true);

// 还原窗口
win.restore();
console.assert(win.isMinimized() == false);

```


<div class="adoc" id="div_minimize"></div>


## restore &nbsp;
  将窗口恢复为之前的状态,不管窗口之前是最小化还是最大化.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/14">minimize</a>, <a href="#api/apiBrowserWindow/11">maximize</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;

win.minimize();
console.assert(win.isMinimized() == true);
win.maximize();
console.assert(win.isMaximized() == true);
// 还原窗口
win.restore();
console.assert(win.isMinimized() == false);

```


<div class="adoc" id="div_restore"></div>


## isMinimized &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否最小化.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **参考** 
<a href="#api/apiBrowserWindow/14">minimize</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;

win.minimize();
console.assert(win.isMinimized() == true);
win.maximize();
console.assert(win.isMaximized() == true);
// 还原窗口
win.restore();
console.assert(win.isMinimized() == false);

```


<div class="adoc" id="div_isMinimized"></div>


## setFullScreen &nbsp;
  设置是否全屏.
  
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
	<td>全屏/非全屏.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/18">isFullScreen</a>, <a href="#api/apiBrowserWindow/52">toggleFullScreen</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setFullScreen(true);
console.assert(win.isFullScreen() == true);


```


<div class="adoc" id="div_setFullScreen"></div>


## isFullScreen &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否处于全屏状态.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setFullScreen(true);
console.assert(win.isFullScreen() == true);

win.setFullScreen(false);
console.assert(win.isFullScreen() == false);


```


<div class="adoc" id="div_isFullScreen"></div>


## setWindowRect &nbsp;
  重新设置窗口的宽高值，并且移动到指定的 x, y 位置,<br>当窗口处于全屏或者最大化状态时，该函数无效.<br>当所给宽高小于限制宽高时，以最小宽高为准<br>当所给宽高大于限制宽高时，以最大宽高为准
  
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
	<td>.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/20">getWindowRect</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;

win.setWindowRect({
    x: 100,
    y: 100,
    width: 1000,
    height: 500,
});


```


<div class="adoc" id="div_setWindowRect"></div>


## getWindowRect &nbsp;<span class="label label-sync">同步</span> 

  返回一个对象，它包含了窗口的宽，高，x坐标，y坐标.
  
* **函数参数**  无

* **返回值**
  JsonObject 窗口的坐标参数.
	* **Int** x  窗口左上角X坐标.
	* **Int** y  窗口左上角Y坐标.
	* **Int** r  窗口右下角X坐标.
	* **Int** b  窗口右下角Y坐标.
	* **Int** width  窗口宽度.
	* **Int** height  窗口高度.
 

* **参考** 
<a href="#api/apiBrowserWindow/19">setWindowRect</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;

win.setWindowRect({
    x: 100,
    y: 100,
    width: 1000,
    height: 500,
});

// 获取窗体坐标
var winInfo = win.getWindowRect();
console.assert(winInfo.x == 100);
console.assert(winInfo.y == 100);
console.assert(winInfo.width == 1000);
console.assert(winInfo.height == 500);


```


<div class="adoc" id="div_getWindowRect"></div>


## setSize &nbsp;
  重新设置窗口的宽高值,窗口的X坐标和Y坐标保持不变,<br>当窗口处于全屏或者最大化状态时，该函数无效<br>当所给宽高小于限制宽高时，以最小宽高为准<br>当所给宽高大于限制宽高时，以最大宽高为准.
  
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
	<td>宽.</td>
</tr><tr>
	<td>Int</td>
	<td>0 </td>
	<td>高</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/22">getSize</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setSize(1300,800);

```


<div class="adoc" id="div_setSize"></div>


## getSize &nbsp;
  获取窗口宽高.
  
* **函数参数**  无

* **返回值**
  JsonObject 窗口的宽高数据. 

* **参考** 
<a href="#api/apiBrowserWindow/21">setSize</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
// 设置窗口宽高
win.setSize(1000,500);

// 获取窗口宽高
var info = win.getSize();
console.log(info);
console.assert(info.width == 1000);
console.assert(info.height == 500);


```


<div class="adoc" id="div_getSize"></div>


## setMinimumSize &nbsp;
  设置窗口最小化的宽高值，,当输入宽高为0时，将取消最小宽高限制.
  
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
	<td>Int	</td>
	<td></td>
	<td>最小宽度.</td>
</tr><tr>
	<td>Int	</td>
	<td></td>
	<td>最小高度.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 设置最小宽高
var win = BrowserWindow.current;
win.setMinimumSize(1280,720);

```


<div class="adoc" id="div_setMinimumSize"></div>


## getMinimumSize &nbsp;<span class="label label-sync">同步</span> 

  返回一个对象，它包含了窗口最小化的宽，高.{width:100, height:200}
  
* **函数参数**  无

* **返回值**
  JsonObject 窗口的最小宽高信息. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setMinimumSize(1280,720);

// 获取最小宽高
var info = win.getMinimumSize();
console.assert(info.width == 1280);
console.assert(info.height == 720);

// 取消限制
win.setMinimumSize(0,0);

```


<div class="adoc" id="div_getMinimumSize"></div>


## setMaximumSize &nbsp;
  设置窗口最大宽高值,当输入宽高为0时，将取消最大宽高限制<br>如果设置了最大宽高，那么窗口不能被最大化，不管所给的值是不是比屏幕窗口大<br>窗口全屏不受该函数影响.
  
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
	<td>Int	</td>
	<td></td>
	<td>最大宽度.</td>
</tr><tr>
	<td>Int	</td>
	<td></td>
	<td>最大高度</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 设置最小宽高
var win = BrowserWindow.current;
win.setMaximumSize(1280,720);

```


<div class="adoc" id="div_setMaximumSize"></div>


## getMaximumSize &nbsp;<span class="label label-sync">同步</span> 

  返回一个对象，它包含了窗口最大化的宽，高.{width:100, height:200}
  
* **函数参数**  无

* **返回值**
  JsonObject 窗口的最大宽高信息. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setMaximumSize(1280,720);

// 获取最大宽高
var info = win.getMaximumSize();
console.assert(info.width == 1280);
console.assert(info.height == 720);

// 取消最大化宽高限制,DEBUG模块下无效
win.setMaximumSize(0,0);



```


<div class="adoc" id="div_getMaximumSize"></div>


## setResizable &nbsp;
  设置窗口是否可以被用户改变窗口大小.<br>该函数优先级高于setMaximizable，只要调用了这个函数，都相关于调用了一次<a href="#api/apiBrowserWindow/33">setMaximizable(true)</a>,maximizable会被重置为true
  
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
	<td>true允许 false不允许 .</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 不允许窗口改变大小
var win = BrowserWindow.current;
win.setResizable(false);
console.assert(win.isResizable() == false);

```


<div class="adoc" id="div_setResizable"></div>


## isResizable &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否可以被用户改变大小.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setResizable(false);
console.assert(win.isResizable()== false);
// 允许窗口改变大小
win.setResizable(true);
console.assert(win.isResizable() == true);

```


<div class="adoc" id="div_isResizable"></div>


## setMovable &nbsp;
  设置窗口是否可以被用户拖动.
  
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
	<td>true可以移动，false不能移动窗口位置.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 拖动窗口试试
var win = BrowserWindow.current;
win.setMovable(false);


```


<div class="adoc" id="div_setMovable"></div>


## isMovable &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否可以被用户拖动.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setMovable(false);
console.assert(win.isMovable() == false);
// 允许窗口被改变位置
win.setMovable(true);
console.assert(win.isMovable() == true);


```


<div class="adoc" id="div_isMovable"></div>


## setMinimizable &nbsp;
  设置窗口是否可以最小化.
  
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
	<td>Boolean </td>
	<td></td>
	<td>true/false.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 设置窗口不能被最小化
var win = BrowserWindow.current;
win.setMinimizable(false);
console.assert(win.isMinimizable()== false);

```


<div class="adoc" id="div_setMinimizable"></div>


## isMinimizable &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否可以最小化.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 设置窗口不能被最小化
var win = BrowserWindow.current;
win.setMinimizable(false);
console.assert(win.isMinimizable() == false);

// 设置窗口可以被最小化
win.setMinimizable(true);
console.assert(win.isMinimizable() == true);

```


<div class="adoc" id="div_isMinimizable"></div>


## setMaximizable &nbsp;
  设置窗口是否可以最大化. 如果<a href="#api/apiBrowserWindow/28">isResizable</a>==false，此函数无效
  
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
	<td>Boolean </td>
	<td></td>
	<td>true/false.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/27">setResizable</a>, <a href="#api/apiBrowserWindow/34">isMaximizable</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 不允许窗口最大化
var win = BrowserWindow.current;
win.setMaximizable(false);
console.assert(win.isMaximizable() == false);


```


<div class="adoc" id="div_setMaximizable"></div>


## isMaximizable &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否可以最大化.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 不允许窗口最大化
var win = BrowserWindow.current;
win.setMaximizable(false);
console.assert(win.isMaximizable() == false);
// 允许窗口最大化
win.setMaximizable(true);
console.assert(win.isMaximizable() == true);


```


<div class="adoc" id="div_isMaximizable"></div>


## setFullScreenable &nbsp;
  设置否可以全屏.
  
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
	<td>true/false.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 不允许窗口全屏
var win = BrowserWindow.current;
win.setFullScreenable(false);
console.assert(win.isFullScreenable() == false);
// 无法全屏
win.setFullScreen(true);
console.assert(win.isFullScreen() == false);

```


<div class="adoc" id="div_setFullScreenable"></div>


## isFullScreenable &nbsp;<span class="label label-sync">同步</span> 

  判断是否允许全屏.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 不允许窗口全屏
var win = BrowserWindow.current;
win.setFullScreenable(false);
console.assert(win.isFullScreenable() == false);
// 无法全屏
win.setFullScreen(true);
console.assert(win.isFullScreen() == false);

// 允许窗口全屏
win.setFullScreenable(true);
console.assert(win.isFullScreenable() == true);
// 可以全屏
win.setFullScreen(true);
console.assert(win.isFullScreen() == true);


```


<div class="adoc" id="div_isFullScreenable"></div>


## setClosable &nbsp;
  设置窗口是否可以人为关闭.
  
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
	<td>true可以关闭，false不能关闭.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setClosable(false);
// 现在窗口关不掉了
console.assert(win.isClosable() == false);


```


<div class="adoc" id="div_setClosable"></div>


## isClosable &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否可以关闭.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setClosable(false);
console.assert(win.isClosable() == false);

win.setClosable(true);
console.assert(win.isClosable() == true);


```


<div class="adoc" id="div_isClosable"></div>


## setAlwaysOnTop &nbsp;
  是否设置这个窗口始终在其他窗口之上.
  
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
	<td>Boolean </td>
	<td></td>
	<td>true置顶，false取消置顶.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 使窗口置顶
var win = BrowserWindow.current;
win.setAlwaysOnTop(true);
console.assert(win.isAlwaysOnTop() == true);

```


<div class="adoc" id="div_setAlwaysOnTop"></div>


## isAlwaysOnTop &nbsp;<span class="label label-sync">同步</span> 

  返回 boolean,当前窗口是否始终在其它窗口之前.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 使窗口置顶
var win = BrowserWindow.current;
win.setAlwaysOnTop(true);
console.assert(win.isAlwaysOnTop() == true);

// 取消置顶
win.setAlwaysOnTop(false);
console.assert(win.isAlwaysOnTop() == false);


```


<div class="adoc" id="div_isAlwaysOnTop"></div>


## center &nbsp;
  窗口居中.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.center();

```


<div class="adoc" id="div_center"></div>


## setPosition &nbsp;
  移动窗口到对应的 x and y 坐标.
  
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
	<td>x坐标.</td>
</tr><tr>
	<td>Int</td>
	<td>0 </td>
	<td>y坐标.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setPosition( 300,200);

```


<div class="adoc" id="div_setPosition"></div>


## getPosition &nbsp;
  返回一个包含当前窗口位置的对象,返回内容与<a href="#api/apiBrowserWindow/20">getWindowRect</a>相同.
  
* **函数参数**  无

* **返回值**
  JsonObject . 

* **参考** 
<a href="#api/apiBrowserWindow/20">getWindowRect</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;

win.setPosition( 100,200);

// 获取窗体坐标
var winInfo = win.getPosition();

console.assert(winInfo.x == 100);
console.assert(winInfo.y == 200);

```


<div class="adoc" id="div_getPosition"></div>


## setTitle &nbsp;
  改变原窗口的title.
  
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
	<td>标题栏名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setTitle("NCUI测试窗口");

```


<div class="adoc" id="div_setTitle"></div>


## getTitle &nbsp;<span class="label label-sync">同步</span> 

  返回原窗口的title.
  
* **函数参数**  无

* **返回值**
  String . 

* **参考** 
<a href="#api/apiBrowserWindow/44">setTitle</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setTitle("NCUI测试窗口");
console.assert(win.getTitle() == 'NCUI测试窗口');

```


<div class="adoc" id="div_getTitle"></div>


## flashFrame &nbsp;
  开始或停止闪烁窗口来获得用户的关注.
  
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
	<td>true闪烁窗口，false停止闪烁.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.flashFrame(true);
// 停止
win.flashFrame(false);

```


<div class="adoc" id="div_flashFrame"></div>


## setHasShadow &nbsp;
  设置是否显示窗口阴影
  
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
	<td>true显示窗口阴影，false不显示窗口阴影.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setHasShadow(false);

```


<div class="adoc" id="div_setHasShadow"></div>


## hasShadow &nbsp;<span class="label label-sync">同步</span> 

  返回 boolean,是否显示窗口阴影
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
// 不显示阴影
win.setHasShadow(false);
console.assert(win.hasShadow() == false);

// 显示阴影
win.setHasShadow(true);
console.assert(win.hasShadow() == true);


```


<div class="adoc" id="div_hasShadow"></div>


## topmost &nbsp;
  设置窗口是否置顶.
  
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
	<td>Boolean </td>
	<td></td>
	<td>true置顶/false取消置顶.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/39">setAlwaysOnTop</a>, <a href="#api/apiBrowserWindow/50">isTopmost</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 使窗口置顶
var win = BrowserWindow.current;
win.topmost(true);
console.assert(win.isTopmost() == true);

```


<div class="adoc" id="div_topmost"></div>


## isTopmost &nbsp;<span class="label label-sync">同步</span> 

  判断窗口是否置顶.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 使窗口置顶
var win = BrowserWindow.current;
win.topmost(true);
console.assert(win.isTopmost() == true);

// 取消置顶
win.topmost(false);
console.assert(win.isTopmost() == false);

```


<div class="adoc" id="div_isTopmost"></div>


## toggleVisible &nbsp;
  显示/隐藏窗口.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win= BrowserWindow.current;
win.toggleVisible();
console.assert(win.isVisible() == false);
win.toggleVisible();
console.assert(win.isVisible() == true);

```


<div class="adoc" id="div_toggleVisible"></div>


## toggleFullScreen &nbsp;
  全屏/非全屏窗口.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
// 设置窗口可以全屏显示
win.setFullScreenable(true);
// 退出全屏
win.setFullScreen(false);

// 全屏
win.toggleFullScreen();
console.assert(win.isFullScreen() == true);

// 退出全屏
win.toggleFullScreen();
console.assert(win.isFullScreen() == false);


```


<div class="adoc" id="div_toggleFullScreen"></div>


## setIcon &nbsp;
  设置窗口图标.
  
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
	<td>图标路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
var icon = app.toAbsolutePath('%skinDir%logo1.ico');
console.log(icon);
win.setIcon(icon);

```


<div class="adoc" id="div_setIcon"></div>


## setConfig &nbsp;
  设置窗口配置参数.该函数并不要求一定要输入所支持的参数，该函数可以影响一部分窗口行为，具体是哪些还要再确认<br>一般用来设置自定义参数，并在getConfig里面获取所设置的值与<a href="#api/apiBrowserWindow/55">BrowserWindow.getConfig</a>相对应.
  
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
<a href="#api/apiBrowserWindow/55">getConfig</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setConfig({
    debugMode: true,
    custom: 'custom'
});


```


<div class="adoc" id="div_setConfig"></div>


## getConfig &nbsp;<span class="label label-sync">同步</span> 

  获取应用程序的所有配置参数,可获取的内容见<a href="#settings/settingsBrowserWindowSettings">浏览器窗口参数</a>.
  
* **函数参数**  无

* **返回值**
  JsonObject 所有配置参数. 

*****
  获取窗口的指定配置参数.
  
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
<a href="#api/apiBrowserWindow/54">setConfig</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//获取浏览器窗口的所有配置参数
var win = BrowserWindow.current;
var config = win.getConfig();
console.log(config);

//获取浏览器窗口的指定配置参数
var titleBar = win.getConfig('titleBar');
console.assert(titleBar == false);

```


<div class="adoc" id="div_getConfig"></div>


## setOpacity &nbsp;
  设置窗口不透明度, 该方法会作用在整个窗口窗口上，如果想部分透明，那么应当使用离屏渲染功能来实现.
  
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
	<td>255 </td>
	<td>不透明度，取值范围[0,255],值越大越不透明 0表示完全透明，255表示完全不透明.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/57">getOpacity</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
BrowserWindow.current.setOpacity(200);

```


<div class="adoc" id="div_setOpacity"></div>


## getOpacity &nbsp;<span class="label label-sync">同步</span> 

  获取当前窗口的不透明度.
  
* **函数参数**  无

* **返回值**
  Int 不透明度. 

* **参考** 
<a href="#api/apiBrowserWindow/56">setOpacity</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
BrowserWindow.current.setOpacity(100);
console.assert(BrowserWindow.current.getOpacity() == 100);

BrowserWindow.current.setOpacity(255);
console.assert(BrowserWindow.current.getOpacity() == 255);

```


<div class="adoc" id="div_getOpacity"></div>


## disableIME &nbsp;
  禁用输入法.
  
* **函数参数**  无

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
win.disableIME();

```


<div class="adoc" id="div_disableIME"></div>


## enableIME &nbsp;
  开启输入法.
  
* **函数参数**  无

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
win.enableIME();

```


<div class="adoc" id="div_enableIME"></div>


## computeCursor &nbsp;
  设置是否计算光标位置，可以用来定位输入法窗口，离屏模式（offscreen=true）下有效.
  
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
	<td>true计算光标位置/false不计算光标光标位置.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
// 禁用后将不能定位输入法
win.computeCursor(false);

```


<div class="adoc" id="div_computeCursor"></div>


## isComputeCursor &nbsp;<span class="label label-sync">同步</span> 

  判断是否计算光标位置.
  
* **函数参数**  无

* **返回值**
  Boolean true/false. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
win.computeCursor(true);
console.assert(win.isComputeCursor() == true);

```


<div class="adoc" id="div_isComputeCursor"></div>


## setTheme &nbsp;
  设置窗口主题.
  
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
	<td>JSON对象.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
BrowserWindow.current.setTheme({
    primaryColor: '#ff8666b8',
    primaryColor2: '#ff8666b8'
});

include('Frame');
var frame = Frame.current;
frame.injectCSS('@file:///example/css/zui-theme.css');


```


<div class="adoc" id="div_setTheme"></div>


## getName &nbsp;<span class="label label-sync">同步</span> 

  获取窗口名.
  
* **函数参数**  无

* **返回值**
  String . 

* **参考** 
<a href="#api/apiBrowserWindow/72">setName</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
console.log(win.getName());

```


<div class="adoc" id="div_getName"></div>


## currentWindow &nbsp;<span class="label label-deprecated">废弃</span> 

  获取当前页面所对应的窗口对象
  
* **函数参数**  无

* **返回值**
  BrowserWindow 如果不存在，返回Undefined. 

* **参考** 
<a href="#api/apiBrowserWindow/65">current</a>



<div class="adoc" id="div_currentWindow"></div>


## current &nbsp;
  获取当前页面所对应的窗口对象.
  
* **函数参数**  无

* **返回值**
  BrowserWindow 如果不存在，返回Undefined. 



<div class="adoc" id="div_current"></div>


## getAllWindows &nbsp;
  获取所有的浏览器窗口.
  
* **函数参数**  无

* **返回值**
  Array 所有的浏览器窗口. 

* **参考** 




<div class="adoc" id="div_getAllWindows"></div>


## repaint &nbsp;
  强制页面重绘，或停止之前的强制重绘
  
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
	<td>Boolean </td>
	<td></td>
	<td>是否重绘界面,如果为false，将移除之前的计时器.</td>
</tr><tr>
	<td>Int </td>
	<td></td>
	<td>重绘界面的间隔时间(毫秒)，程序每隔(delay)对页面进行一次重绘。如果=0，将不会添加计时器进行重绘.如果<30,delay会提升到30ms.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 



<div class="adoc" id="div_repaint"></div>


## addOverlap &nbsp;
  添加一个渲染图层，在进行页面重绘时会将该图片合并到页面原始的Bitmap中
  
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
	<td>图层名称 （该名称为一个共享内存的名称，页面重绘时将读取共享内存中的数据进行合并）.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 



<div class="adoc" id="div_addOverlap"></div>


## removeOverlap &nbsp;
  移除一个渲染图层
  
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
	<td>图层名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 



<div class="adoc" id="div_removeOverlap"></div>


## setDragBlackList &nbsp;
  设置拖拽黑名单.
  
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
	<td>Int </td>
	<td></td>
	<td>参数.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 



<div class="adoc" id="div_setDragBlackList"></div>


## getDragBlackList &nbsp;
  获取拖拽黑名单.
  
* **函数参数**  无

* **返回值**
  Int 不允许拖拽的类型. 



<div class="adoc" id="div_getDragBlackList"></div>


## setName &nbsp;
  设置窗口名,这个函数暂时还没有什么用，只是把名称保留下来了.
  
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
	<td>名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
win.setName('NCUI测试窗口');
console.assert(win.getName() == 'NCUI测试窗口');

```


<div class="adoc" id="div_setName"></div>


## showTitleBar &nbsp;
  显示/隐藏标题栏.
  
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
	<td>true显示/false隐藏.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var win = BrowserWindow.current;
var titleBar = win.getConfig('titleBar');
win.showTitleBar(!titleBar);

```


<div class="adoc" id="div_showTitleBar"></div>


## close &nbsp;
  尝试关闭窗口，可以被用户取消如:window.onload.<br>当<a href="#api/apiBrowserWindow/38">isClosable</a>为true,该函数无效.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#api/apiBrowserWindow/4">destroy</a>, <a href="#api/apiBrowserWindow/37">setClosable</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip
 var win = BrowserWindow.current;
 win.close();

```


<div class="adoc" id="div_close"></div>


## saveImageToFile &nbsp;
  保存页面到文件， 离屏模式下(offscreen== true)有效.支持.png, .bmp, .gif, .jpeg, .jpg, .tiff
  
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
	<td>图片保存路径.</td>
</tr><tr>
	<td>Boolean</td>
	<td>false </td>
	<td>是否保存标题栏（标题栏存在时有效）</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
// 如果当前程序是以离屏模式运行才能成功
win.saveImageToFile('1.png', true);




```


<div class="adoc" id="div_saveImageToFile"></div>


## saveGifToFile &nbsp;
  录制GIF到文件,离屏模式下(offscreen== true)有效(不建议使用).
  
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
	<td>录制参数.<br>| #String filename gif文件保存目录<br>| #Int=10	delay 帧间隔（注意单位为:百分一秒）<br>| #Int=20	total 最多录制帧数<br></td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
win.recordGifToFile({
    filename: '1.gif',
    delay: 10,
    total:100
});

```


<div class="adoc" id="div_saveGifToFile"></div>


## stopRecordGif &nbsp;
  停止录制GIF,离屏模式下(offscreen== true)有效(不建议使用).
  
* **函数参数**  无

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('BrowserWindow');
var win = BrowserWindow.current;
win.stopRecordGif();

```


<div class="adoc" id="div_stopRecordGif"></div>


## setNextDialogFiles &nbsp;
  设置下一次用户点击文件选择对话框时自动选择的文件.如果文件列表存在，那么不会弹出文件选择对话框，已是直接返回当前列表给页面.
  
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
	<td>Array </td>
	<td></td>
	<td>文件列表（也可以是单独的String）.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'app');
// 可以
BrowserWindow.current.setNextDialogFiles('%appDir%manifest.json');
var splashPath = app.toAbsolutePath('%appDir%images/splash.jpg');
console.log(splashPath);
BrowserWindow.current.setNextDialogFiles([splashPath, '%appDir%manifest.json']);

```


<div class="adoc" id="div_setNextDialogFiles"></div>


## getNextDialogFiles &nbsp;<span class="label label-sync">同步</span> 

  获取<a href="#api/apiBrowserWindow/78">setNextDialogFiles</a>中设置的数据.
  
* **函数参数**  无

* **返回值**
  Array . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'BrowserHost');
BrowserWindow.current.setNextDialogFiles('%appDir%manifest.json');
var arr = BrowserWindow.current.getNextDialogFiles();
console.log(arr);
console.assert(arr.length == 1);
var x = $('#ncui_getNextDialogFiles').offset().left;
var y = $('#ncui_getNextDialogFiles').offset().top;
// 可以自己模拟一个点击事件
BrowserHost.current.click(x + 10, y + 10);


```


<div class="adoc" id="div_getNextDialogFiles"><div id="" class="example code" contenteditable="true"><input id="ncui_getNextDialogFiles" type="file"  style="background:#f9f;"/></div></div>


## dropFiles &nbsp;
  在所给位置依次触发 dragenter dragover drop 事件(offscreen == true)是有效.
  
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
	<td>Array </td>
	<td></td>
	<td>需要送入的文件列表，也可以是一个单独String.</td>
</tr><tr>
	<td>Int </td>
	<td></td>
	<td>x坐标</td>
</tr><tr>
	<td>Int </td>
	<td></td>
	<td>y坐标</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要保证$('#ncui_divDropFiles') 在可视区域内
window.readBlobAsDataURL = function(blob, callback) {
    var reader = new FileReader();
    reader.onload = function(e) {
        callback(e.target.result);
    };
    reader.readAsDataURL(blob);
}
window.ncui_divDropFiles = document.querySelector('#ncui_divDropFiles');
window.ncui_divDropFiles.addEventListener("dragenter", function(e) {  e.preventDefault(); e.stopPropagation(); console.log('dragenter'); }, false);
window.ncui_divDropFiles.addEventListener("dragover", function(e) { e.preventDefault(); e.stopPropagation(); console.log('dragover'); }, false);
window.ncui_divDropFiles.addEventListener("drop", function(e) {
    e.preventDefault(); e.stopPropagation();console.log('drop');
    var item = e.dataTransfer.items[0];
    var file = item.getAsFile();
    window.readBlobAsDataURL(file, function(dataurl) {
        $('#ncui_dropFiles').attr('src', dataurl);
    });
}, false);

include('BrowserWindow');
var x = $('#ncui_divDropFiles').offset().left + 10;
var y = $('#ncui_divDropFiles').offset().top + 10;
BrowserWindow.current.dropFiles(['%appDir%images/splash.jpg'], x, y);

```


<div class="adoc" id="div_dropFiles"><div id="ncui_divDropFiles" class="example code" contenteditable="true" draggable="true" ><img id="ncui_dropFiles" src=""></img> </div></div>


