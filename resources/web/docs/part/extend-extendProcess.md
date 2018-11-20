# Process

  子进程类，本模块的大部分函数都比较慢，尽可能的放到线程里面去执行<br>工作线程**Renderer线程**
  
* **父类** 
<a href="#api/apiRunnable">Runnable</a>&nbsp;

## findPIDByName &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  通过程序名称查找进程.
  
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
	<td>程序名.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Array 与程序名匹配的进程ID列表. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Process');
console.log(Process.findPIDByName('NCUI.exe'));

// 该函数通常需要1秒以上的执行时间，实际上应该把这个函数放到线程上支执行
include('Thread');
Thread.Exec(Process.findPIDByName, 'NCUI.exe');
Process.unique('Process.findPIDByName', function(){
    console.log(arguments);
});

```


<div class="adoc" id="div_findPIDByName"></div>


## Process &nbsp;<span class="label label-constructor">构造</span> <span class="label label-sync">同步</span> 

  创建一个进程类.
  
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
	<td>进程路径 .</td>
</tr>
	</tbody>
</table>

* **返回值**
  Process 进程对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include ('Process');
var process = new Process('NCUI.exe');
process.start();

```


<div class="adoc" id="div_Process"></div>


## terminateProcessByName &nbsp;
  通过程序名称结束进程.
  
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
	<td>程序名.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;--&nbsp;&nbsp;终止程序**

```html
//@skip 注意，运行该示例将结束当前程序
include('Process');

Process.terminateProcessByName('NCUI.exe');

```


<div class="adoc" id="div_terminateProcessByName"></div>


## killProcessByName &nbsp;
  通过程序名杀死进程.
  
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
	<td>程序名.</td>
</tr><tr>
	<td>Boolean</td>
	<td>true </td>
	<td>是否杀死子进程.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;--&nbsp;&nbsp;终止程序**

```html
//@skip 注意，运行该示例将结束当前程序
include('Process');

Process.killProcessByName('NCUI.exe');

```


<div class="adoc" id="div_killProcessByName"></div>


## start &nbsp;
  运行子程序.
  
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
	<td>启动参数.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;--&nbsp;&nbsp;终止程序**

```html
include('Process');
var process = new Process('C:/Windows/System32/notepad.exe');

process.start('manifest.json');

```


<div class="adoc" id="div_start"></div>


## getResult &nbsp;
  获取子进程执行结果，该函数应该放到线程里面去执行，不然会将渲染进程阻塞.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Process', 'Thread');
var process = new Process('NCUI.exe');

process.start('');
Thread.Exec(process.getResult);
process.unique('process.result', function(){
   console.log(arguments);
});
process.unique('Process.getResult', function(){
   console.log(arguments);
});

```


<div class="adoc" id="div_getResult"></div>


