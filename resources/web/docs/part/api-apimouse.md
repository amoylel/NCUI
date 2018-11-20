# mouse

  鼠标控制类,所有函数均为静态函数.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## leftClick &nbsp;<span class="label label-static">静态</span> 

  单击左键.
  
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

* **参考** 
<a href="#api/apimouse/14">click</a>



<div class="adoc" id="div_leftClick"></div>


## leftDown &nbsp;<span class="label label-static">静态</span> 

  按下左键.
  
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
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;


var winRect = win.getWindowRect();
var x = winRect.r - winRect.width / 2;
var y = winRect.y + 20;

// 移动鼠标试试
mouse.moveTo(x, y);
mouse.leftDown(x,y);

```


<div class="adoc" id="div_leftDown"></div>


## leftUp &nbsp;<span class="label label-static">静态</span> 

  弹起左键.
  
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



<div class="adoc" id="div_leftUp"></div>


## leftDoubleClick &nbsp;<span class="label label-static">静态</span> 

  双击左键.
  
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
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;


var winRect = win.getWindowRect();
var x = winRect.r - winRect.width / 2;
var y = winRect.y + 20;

// 双击标题栏最大化窗口
mouse.moveTo(x, y)
mouse.leftDoubleClick(x, y);

```


<div class="adoc" id="div_leftDoubleClick"></div>


## rightClick &nbsp;<span class="label label-static">静态</span> 

  单击右键.
  
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
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;

// 获取最大化按钮的位置
var winRect = win.getWindowRect();
var x = winRect.x + 150;
var y = winRect.y + 20;

// 右键
mouse.rightClick(x,y);

```


<div class="adoc" id="div_rightClick"></div>


## rightDown &nbsp;<span class="label label-static">静态</span> 

  按下右键.
  
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



<div class="adoc" id="div_rightDown"></div>


## rightUp &nbsp;<span class="label label-static">静态</span> 

  弹起右键.
  
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



<div class="adoc" id="div_rightUp"></div>


## middleClick &nbsp;<span class="label label-static">静态</span> 

  单击中键.
  
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
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;


var winRect = win.getWindowRect();
var x = winRect.r - 500;
var y = winRect.y + winRect.height / 2;

//
mouse.moveTo(x, y);
mouse.middleClick(x, y);

```


<div class="adoc" id="div_middleClick"></div>


## moveTo &nbsp;<span class="label label-static">静态</span> 

  移动鼠标到指定位置.
  
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
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;


var winRect = win.getWindowRect();
var x = winRect.r - 50;
var y = winRect.y + 20;

//
mouse.moveTo(x, y);

```


<div class="adoc" id="div_moveTo"></div>


## moveR &nbsp;<span class="label label-static">静态</span> 

  移动鼠标到当前位置的相对位置.
  
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
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;


var winRect = win.getWindowRect();
var x = 100;
var y = 0;

//
mouse.moveR(x, y);

```


<div class="adoc" id="div_moveR"></div>


## mouseWheel &nbsp;<span class="label label-static">静态</span> 

  鼠标滚轮.
  
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
	<td>偏移量 >0 时向上滚动，<0时向下滚动.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
mouse.mouseWheel(100);

```


<div class="adoc" id="div_mouseWheel"></div>


## saveMousePos &nbsp;<span class="label label-static">静态</span> 

  保存当前鼠标位置.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
mouse.saveMousePos();

```
*****
  保存指定鼠标位置.
  
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
include('mouse');
mouse.saveMousePos(100, 100);

```


<div class="adoc" id="div_saveMousePos"></div>


## restoreMousePos &nbsp;<span class="label label-static">静态</span> 

  移动鼠标到上次保存的鼠标位置.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
mouse.restoreMousePos();

```


<div class="adoc" id="div_restoreMousePos"></div>


## unlockMouse &nbsp;<span class="label label-static">静态</span> 

  解锁鼠标（未实现）.s
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_unlockMouse"></div>


## click &nbsp;
  单击鼠标左键.
  
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
	<td>x坐标</td>
</tr><tr>
	<td>Int</td>
	<td>0 </td>
	<td>y坐标</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apimouse/0">leftClick</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow','mouse');
var win = BrowserWindow.current;
// 获取最大化按钮的位置
var winRect = win.getWindowRect();
var x = winRect.r - 50;
var y = winRect.y + 10;
// 点击最大化按钮
mouse.click(x,y);

```


<div class="adoc" id="div_click"></div>


## lockMouse &nbsp;
  锁定鼠标，禁止所有鼠标功能（未实现）.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_lockMouse"></div>


## waitClick &nbsp;
  等待一下鼠标事件（未实现）.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_waitClick"></div>


## getLastClick &nbsp;
  获取上一次按过的鼠标按键（未实现）.
  
* **函数参数**  无

* **返回值**
   The last click. 



<div class="adoc" id="div_getLastClick"></div>


## getCursorPos &nbsp;
  获取当前鼠标在屏幕上的位置.
  
* **函数参数**  无

* **返回值**
  JsonObject .
	* **Int** x  鼠标x轴坐标.
	* **Int** y  鼠标y轴坐标.
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
var pos = mouse.getCursorPos();
console.log(pos);

```


<div class="adoc" id="div_getCursorPos"></div>


## setCursorPos &nbsp;
  设置鼠标位置.
  
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
	<td>x x轴坐标.</td>
</tr><tr>
	<td>Int</td>
	<td>0 </td>
	<td>y y轴坐标</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apimouse/8">moveTo</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
mouse.setCursorPos(100,100);

```


<div class="adoc" id="div_setCursorPos"></div>


## screenToClient &nbsp;
  将一个屏幕坐标转换为当前窗口坐标.
  
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
  JsonObject .
	* **Int** x  转换后的x坐标
	* **Int** y  转换后的y坐标
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
var pos = mouse.screenToClient(100,100);
console.log(pos);

```
*****
  将一个屏幕坐标转换为指定窗口坐标.
  
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
	<td>BrowserWindow </td>
	<td></td>
	<td>窗口对象</td>
</tr><tr>
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
  JsonObject .
	* **Int** x  转换后的x坐标
	* **Int** y  转换后的y坐标
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'mouse');
var win = BrowserWindow.current;
var pos = mouse.screenToClient(win, 100, 100);
console.log(pos);

```


<div class="adoc" id="div_screenToClient"></div>


## clientToScreen &nbsp;
  将当前窗口坐标转换为屏幕坐标.
  
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
  JsonObject .
	* **Int** x  转换后的x坐标
	* **Int** y  转换后的y坐标
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('mouse');
var pos = mouse.clientToScreen(100,100);
console.log(pos);

```
*****
  将一个指定窗口坐标转换为屏幕坐标.
  
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
	<td>BrowserWindow </td>
	<td></td>
	<td>窗口对象</td>
</tr><tr>
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
  JsonObject .
	* **Int** x  转换后的x坐标
	* **Int** y  转换后的y坐标
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('BrowserWindow', 'mouse');
var win = BrowserWindow.current;
var pos = mouse.clientToScreen(win, 100, 100);
console.log(pos);

```


<div class="adoc" id="div_clientToScreen"></div>


## hideCursor &nbsp;
  隐藏鼠标，只对当前程序窗口有效.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('mouse');
mouse.hideCursor();
setTimeout(function(){
   mouse.showCursor();
}, 5000);

```


<div class="adoc" id="div_hideCursor"></div>


## showCursor &nbsp;
  显示鼠标.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('mouse');
mouse.showCursor();

```


<div class="adoc" id="div_showCursor"></div>


