# ipc

  进程间通信类，你可以通过该类发其他页面发送消息.<br>但要注意的是不能在消息处理函数中调用任何导致UI阻塞的函数如(弹出窗口 alert dialog)<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('ipc');
ipc.unique('ipc.exec', function(){
    console.log(arguments);
    return 'hello';
});
ipc.unique('ipc.sync', function(){
    console.log(arguments);
    return 'ipc.sync';
});
ipc.unique('ipc.async', function(){
    console.log(arguments)
    return 'ipc.async';
});
ipc.unique('ipc.test', function(){
    console.log(arguments);
    return 666;
});


```
## exec &nbsp;<span class="label label-static">静态</span> 

  发送一条消息.
  
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
	<td>Args... </td>
	<td></td>
	<td>要发送的内容.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('ipc');
ipc.exec('ipc.exec', 12, '88', 12.33);

```


<div class="adoc" id="div_exec"></div>


## sync &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  发送一条同步消息.
  
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
	<td>Args... </td>
	<td></td>
	<td>要发送的内容.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Any 消息返回的结果,只会返回第一个监听函数的返回值. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('ipc');
var cc = ipc.sync('ipc.sync', 12);
console.log(cc);
console.assert(ipc.sync('ipc.sync', 12) == 'ipc.sync');
console.assert(ipc.sync('ipc.async', 'test') == 'ipc.async');


```


<div class="adoc" id="div_sync"></div>


## async &nbsp;<span class="label label-static">静态</span> 

  发送一条异步消息，可以通过回调函数获取结果.
  
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
	<td>Args... </td>
	<td></td>
	<td>要发送的内容.</td>
</tr><tr>
	<td>Function </td>
	<td></td>
	<td>要接收消息返回值的回调函数,任意位置都可以，但只能有一个, 只会返回第一个监听函数的返回值.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('ipc');
ipc.async('ipc.sync', function(val){console.assert(val == 'ipc.sync');});
ipc.async('ipc.async', function(val){console.assert(val == 'ipc.async');}, '2382934', 888);

```


<div class="adoc" id="div_async"></div>


## dispatchEvent &nbsp;
  向所有页面的document发送自定义事件.
  
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
	<td>事件名.</td>
</tr><tr>
	<td>JsonObject </td>
	<td></td>
	<td>事件内容，CustomEvent的detail字段</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 



<div class="adoc" id="div_dispatchEvent"></div>


