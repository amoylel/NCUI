# Object

  所有JS扩展对象所对应的C++类基类；该类不能被直接使用,需要编写C++扩展时继承该类，并映射成JS类使用.
  
## CLASS &nbsp;<span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> <span class="label label-single">单线程</span> 

  获取当前类所对象的对象，调用该对象的函数将会作用到类上面去.
  
* **函数参数**  无

* **返回值**
  Object 类所对象的CLASS对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Demo');
console.assert(Demo.CLASS.staticTestID == 2, true);
Demo.CLASS.staticTestID = 32;
console.log(Demo.CLASS.staticTestID);
console.assert(Demo.CLASS.staticTestID != 2, true);


```


<div class="adoc" id="div_CLASS"></div>


## fromObjectName &nbsp;<span class="label label-static">静态</span> <span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  通过对象名称查找对象.
  
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
	<td>对象名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Object 如果对象存在，返回该对象; 如果不存在，返回Undefined. 

* **参考** 
<a href="#api/apiObject/2">fromObjectID</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Demo');
var demo = new Demo();
demo.setObjectName('demo2');
var name = demo.getObjectName();
var demo2 = Demo.fromObjectName(name);
console.assert(demo.getObjectID() == demo2.getObjectID(), true);

```


<div class="adoc" id="div_fromObjectName"></div>


## fromObjectID &nbsp;<span class="label label-static">静态</span> <span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  通过对象ID查找对象.
  
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
	<td>对象ID.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Object 如果对象存在，返回该对象; 如果不存在，返回Undefined. 

*****
  通过对象ID查找对象.
  
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
	<td>对象ID.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Object 如果对象存在，返回该对象; 如果不存在，返回Undefined. 

* **参考** 
<a href="#api/apiObject/1">fromObjectName</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Demo');
var demo = new Demo();
demo.setObjectName('demo1');
var id = demo.getObjectID();
var demo2 = Demo.fromObjectID(id);
console.assert(demo.getObjectName() == demo2.getObjectName(), true);

```


<div class="adoc" id="div_fromObjectID"></div>


## All &nbsp;<span class="label label-static">静态</span> <span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  获取当前类的所有对象.
  
* **函数参数**  无

* **返回值**
  Array 包含当前类的所有对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Demo');
var arr = Demo.All();
console.log(arr);

```


<div class="adoc" id="div_All"></div>


## setObjectName &nbsp;
  设置当前对象名称.
  
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
	<td>对象名称.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include ('Demo');
var demo = new Demo();
demo.setObjectName('demo3');
console.log(demo.getObjectName());

```


<div class="adoc" id="div_setObjectName"></div>


## onGetObjectID &nbsp;
  获取当前对象名称ID.ID为一个字符串
  
* **函数参数**  无

* **返回值**
  String . 

* **参考** 
<a href="#api/apiObject/6">getObjectName</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include ('Demo');
var demo = new Demo();
demo.setObjectName('demo4');
console.log(demo.getObjectName());

```


<div class="adoc" id="div_onGetObjectID"></div>


## getObjectName &nbsp;<span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  获取当前对象名称.
  
* **函数参数**  无

* **返回值**
  String 对象名称. 

* **参考** 
<a href="#api/apiObject/4">setObjectName</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include ('Demo');
var demo = new Demo();
demo.setObjectName('demo4');
console.log(demo.getObjectName());

```


<div class="adoc" id="div_getObjectName"></div>


## notify &nbsp;
  向自己发送一个通知，可以监听这个通知得到返回结果(当transfer在单独的线程上执行时，可以通过自己定义消息来监听执行进度).
  
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
	<td>通知名称</td>
</tr><tr>
	<td>Object </td>
	<td></td>
	<td>任务Javascript所支持的基本类型（Int Double String JsonObject Array），可以不填.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include ('Demo');
var demo = new Demo();
demo.on('Demo.notifyTest', function(){
   console.log(arguments);
});
demo.notify('Demo.notifyTest');

```


<div class="adoc" id="div_notify"></div>


## relase &nbsp;<span class="label label-single">单线程</span> 

  释放当前对象所对应的C++对象，该函数调用后页面上对应的对象将无法使用
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_relase"></div>


## getUserData &nbsp;<span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  获取用户保存的自定义数据.
  
* **函数参数**  无

* **返回值**
  JsonObject 所有配置参数. 

*****
  根据输入字段返回用户保存的自定义数据.
  
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
   返回字段名所对应的数据。返回类型视配置参数类型而定，为JS所支持的基本数据类型. 

* **参考** 
<a href="#api/apiObject/10">setUserData</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Demo');
var demo = new Demo();
demo.setUserData({aaa: 123, bbb: 344});
console.assert(demo.getUserData('aaa')== 123, true);
console.log(demo.getUserData());

```


<div class="adoc" id="div_getUserData"></div>


## setUserData &nbsp;<span class="label label-single">单线程</span> 

  保存自定义数据.该函数并不要求一定要输入所支持的参数，你也可以设置自定义参数，并在getUserData里面获取所设置的值与<a href="#api/apiObject/9">getUserData</a>相对应.
  
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
<a href="#api/apiObject/9">getUserData</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Demo');
var demo = new Demo();
demo.setUserData({aaa: 123, bbb: 344});
demo.setUserData({aaa:222});
demo.setUserData({ccc:444});
console.assert(demo.getUserData('aaa')== 222, true);
console.log(demo.getUserData());

```


<div class="adoc" id="div_setUserData"></div>


