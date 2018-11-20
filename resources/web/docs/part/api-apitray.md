# tray

  托盘操作类.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## 事件
### **1.buttondown** <span class="label label-static">静态</span>

  鼠标左键按下时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('buttondown', function(){
    console.log('buttondown');
});

```


<div class="adoc" id="div_buttondown"></div>


### **2.buttonup** <span class="label label-static">静态</span>

  鼠标左键弹起时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('buttonup', function(){
    console.log('buttonup');
});

```


<div class="adoc" id="div_buttonup"></div>


### **3.rbuttondown** <span class="label label-static">静态</span>

  鼠标右键按下时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('rbuttondown', function(){
    console.log('rbuttondown');
});

```


<div class="adoc" id="div_rbuttondown"></div>


### **4.rbuttonup** <span class="label label-static">静态</span>

  鼠标右键弹起时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('rbuttonup', function(){
    console.log('rbuttonup');
});

```


<div class="adoc" id="div_rbuttonup"></div>


### **5.click** <span class="label label-static">静态</span>

  鼠标左键单击时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('click', function(){
    console.log('click');
});

```


<div class="adoc" id="div_click"></div>


### **6.dbclick** <span class="label label-static">静态</span>

  鼠标左键双击时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('dbclick', function(){
    console.log('dbclick');
});

```


<div class="adoc" id="div_dbclick"></div>


### **7.mousehover** <span class="label label-static">静态</span>

  鼠标悬停时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('mousehover', function(){
    console.log('mousehover');
});

```


<div class="adoc" id="div_mousehover"></div>


### **8.mouseleave** <span class="label label-static">静态</span>

  鼠标离开时触发.

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.on('mouseleave', function(){
    console.log('mouseleave');
});

```


<div class="adoc" id="div_mouseleave"></div>


## show &nbsp;<span class="label label-static">静态</span> 

  显示托盘.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.show();

```


<div class="adoc" id="div_show"></div>


## hide &nbsp;<span class="label label-static">静态</span> 

  隐藏托盘.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.hide();

```


<div class="adoc" id="div_hide"></div>


## setIcon &nbsp;<span class="label label-static">静态</span> 

  设置托盘图标.
  
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
	<td>图标本地路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.setIcon('skin/logo1.ico');

```


<div class="adoc" id="div_setIcon"></div>


## setTooltip &nbsp;<span class="label label-static">静态</span> 

  设置托盘提示文本.
  
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
	<td>需要显示的提示文本.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.setTooltip(new Date().toString());

```


<div class="adoc" id="div_setTooltip"></div>


## blink &nbsp;<span class="label label-static">静态</span> 

  闪烁托盘.
  
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
	<td>true闪烁/false停止闪烁.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **参考** 
<a href="#api/apitray/6">isBlink</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.blink(true);

```


<div class="adoc" id="div_blink"></div>


## isBlink &nbsp;<span class="label label-static">静态</span> 

  判断托盘是否闪烁.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **参考** 
<a href="#api/apitray/5">blink</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('tray');
tray.blink(true);
console.assert(tray.isBlink() == true);
tray.blink(false);
console.assert(tray.isBlink() == false);

```


<div class="adoc" id="div_isBlink"></div>


