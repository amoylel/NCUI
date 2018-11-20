# globalShortcut

  全局快捷键.<br>工作线程 **UI线程**
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## 事件
### **1.hotkeydown** <span class="label label-static">静态</span>

  快捷键事件监听，当按下注册的快捷键时，该事件会被触发.但是如果注册快捷键时指定了action，且该action能被程序处理，那么该快捷键的事件不会被触发.

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
	<td>Int </td>
	<td>快捷键ID</td>
</tr><tr>
	<td>JsonObject </td>
	<td>快捷键相关信息[**Boolean**] ctrl		键盘上的CTRL按键， 对应字符串MOD_CONTROL.<br>[**Boolean**] win		键盘上的WIN按键， 对应字符串MOD_WIN.<br>[**Boolean**] alt		键盘上的ALT按键， 对应字符串MOD_ALT.<br>[**Boolean**] shift		键盘上的SHIFT按键, 对应字符串MOD_SHIFT.<br>[**String**]  key		键盘上的字母或数字按键，一个字符.<br>[**String**]  action	当前快捷键所触发的<a href="#settings/settingsActionPreprocessor">预定义事件</a>.<br></td>
</tr>
	</tbody>
</table>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
globalShortcut.on('hotkeydown', function(hotkey){
    console.log(hotkey);
});

```


<div class="adoc" id="div_hotkeydown"></div>


## registerShortcut &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  通过组合键注册快捷键.
  
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
	<td>字符串.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int 快捷键ID ， 当返回值为0时表示注册失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+D');
console.log(hotkey);
console.assert(hotkey != 0);


```
*****
  注册快捷键.
  
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
	<td>字符串.<br>[**Boolean**] ctrl		键盘上的CTRL按键，默认为false, 对应字符串MOD_CONTROL.<br>[**Boolean**] win		键盘上的WIN按键，默认为false, 对应字符串MOD_WIN.<br>[**Boolean**] alt		键盘上的ALT按键，默认为false, 对应字符串MOD_ALT.<br>[**Boolean**] shift		键盘上的SHIFT按键，默认为false, 对应字符串MOD_SHIFT.<br>[**String**]  key		键盘上的字母或数字按键，一个字符.<br>[**String**]  action	当前快捷键所触发的<a href="#settings/settingsActionPreprocessor">预定义事件</a>.<br></td>
</tr>
	</tbody>
</table>

* **返回值**
  Int 快捷键ID， 当返回值为0时表示注册失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
var hotkey = globalShortcut.registerShortcut({ ctrl: true, alt: true, key: 'O', action: 'toggleVisible' });
console.log(hotkey);
// 按下Ctrl+Atl+O可以显示或隐藏当前窗口
console.assert(hotkey != 0);


```


<div class="adoc" id="div_registerShortcut"></div>


## isRegistered &nbsp;<span class="label label-static">静态</span> 

  通过ID判断快捷键是否注册.
  
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
	<td>快捷键ID.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+M');
console.assert(globalShortcut.isRegistered(hotkey) == true);


```
*****
  通过组合键判断快捷键是否注册.
  
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
	<td>组合键.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+M');
console.assert(globalShortcut.isRegistered('MOD_CONTROL+MOD_ALT+M') == true);

```


<div class="adoc" id="div_isRegistered"></div>


## unregisterShortcut &nbsp;<span class="label label-static">静态</span> 

  通过ID注销快捷键.
  
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
	<td>快捷键ID.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

*****
  通过组合键注销快捷键.
  
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
	<td>组合键所组成的字符串.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+N');
console.assert(globalShortcut.isRegistered('MOD_CONTROL+MOD_ALT+N') == true);

globalShortcut.unregisterShortcut('MOD_CONTROL+MOD_ALT+N');
console.assert(globalShortcut.isRegistered('MOD_CONTROL+MOD_ALT+N') == false);


```


<div class="adoc" id="div_unregisterShortcut"></div>


## unregisterAll &nbsp;<span class="label label-static">静态</span> 

  注销所有快捷键.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('globalShortcut');
globalShortcut.unregisterAll();

```


<div class="adoc" id="div_unregisterAll"></div>


