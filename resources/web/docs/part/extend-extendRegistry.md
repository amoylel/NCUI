# Registry

  注册表操作类.<br>工作线程**Renderer线程**
  
* **父类** 
<a href="#api/apiRunnable">Runnable</a>&nbsp;

## RegistryTransfer &nbsp;<span class="label label-constructor">构造</span> <span class="label label-sync">同步</span> 

  创建一个注册表操作对象.
  
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
	<td>根键 .</td>
</tr>
	</tbody>
</table>

* **返回值**
  Registy 注册表操作对象 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Registry');
window.registry = new Registry(Registry.HKEY_CURRENT_USER);


```


<div class="adoc" id="div_RegistryTransfer"></div>


## create &nbsp;<span class="label label-sync">同步</span> 

  创建一个子键.
  
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
	<td>子键路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要先运行构造函数里面的例子
// 创建一个子键
console.assert(registry.create('Software\\NCUI') == true);

```


<div class="adoc" id="div_create"></div>


## open &nbsp;<span class="label label-sync">同步</span> 

  打开子键.
  
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
	<td>子键路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要先运行构造函数里面的例子
// 打开一个子键
console.assert(registry.create('Software\\NCUI') == true);

```


<div class="adoc" id="div_open"></div>


## read &nbsp;<span class="label label-sync">同步</span> 

  读取键值.
  
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
	<td>子键.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>键名.</td>
</tr>
	</tbody>
</table>

* **返回值**
  String 读取到的值. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要先运行构造函数里面的例子
// 写入值
console.assert(registry.write('Software\\NCUI', 'version', '1.0.0.0', true) == true);
// 读取值
console.assert(registry.read('Software\\NCUI', 'version') ==  '1.0.0.0');

```


<div class="adoc" id="div_read"></div>


## write &nbsp;<span class="label label-sync">同步</span> 

  写入键值.
  
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
	<td>子键.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>键名.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>键值.</td>
</tr><tr>
	<td>Boolean</td>
	<td>false </td>
	<td>如果子键不存在，是否自己创建子键.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要先运行构造函数里面的例子
// 写入值
console.assert(registry.write('Software\\NCUI', 'remark', 'Chromium Embedded Framework (CEF)', true) == true);
// 读取值
console.assert(registry.read('Software\\NCUI', 'remark') ==  'Chromium Embedded Framework (CEF)');

```


<div class="adoc" id="div_write"></div>


## remove &nbsp;<span class="label label-sync">同步</span> 

  删除子键或键值.
  
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
	<td>子键.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>键名，如果不输入该值，那么将删除整个子键.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 需要先运行构造函数里面的例子
// 删除单个键值
console.assert(registry.remove('Software\\NCUI', 'version') ==true);
registry.create('Software\\NCUI\\NCUI');
// 删除整个子键，
console.assert(registry.remove('Software\\NCUI') == true);


```


<div class="adoc" id="div_remove"></div>


