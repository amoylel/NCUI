# UUID

  UUID 生成类,如果使用频繁，请直接调用静态函数返回UUID字符串，纯UUID对象也没什么意义.<br>工作线程**Renderer线程**
  
* **父类** 
<a href="#api/apiRunnable">Runnable</a>&nbsp;

## fromString &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  用一个字符串构建UUID.
  
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
  UUID 返回一个UUID对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('UUID');
var uuid = UUID.fromString('f1adcc5b-6943-4929-8a2f-2b64691c3867');
console.log(uuid);
console.log(uuid.toString());

```


<div class="adoc" id="div_fromString"></div>


## getString &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  直接获取一个UUID的字符串，不创建新的对象.
  
* **函数参数**  无

* **返回值**
  String UUID字符串. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('UUID');
console.log(UUID.getString());

```


<div class="adoc" id="div_getString"></div>


## nil &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取一个空UUID对象.
  
* **函数参数**  无

* **返回值**
  UUID . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('UUID');
console.log(UUID.nil().toString());

```


<div class="adoc" id="div_nil"></div>


## nilString &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取一个空UUID的字符串.
  
* **函数参数**  无

* **返回值**
  String 一个表示空UUID的字符串. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('UUID');
console.log(UUID.nilString());

```


<div class="adoc" id="div_nilString"></div>


## UUID &nbsp;<span class="label label-constructor">构造</span> 

  创建一个UUID 对象.
  
* **函数参数**  无

* **返回值**
  UUID 实例 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('UUID');
var uuid = new UUID();

```


<div class="adoc" id="div_UUID"></div>


## toString &nbsp;<span class="label label-sync">同步</span> 

  将当前UUID转换为字符串.
  
* **函数参数**  无

* **返回值**
  String UUID字符串. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('UUID');
var uuid = new UUID();
console.log(uuid.toString());

```


<div class="adoc" id="div_toString"></div>


