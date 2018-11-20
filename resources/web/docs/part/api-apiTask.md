# Task

  线程类，该类构建的实例会在UI进程上执行,可以使一个由C++编写的扩展在线程上运行.<br>工作线程:**UI线程**.<br>说明:**虽然没有限制，但还是不建议将内置接口放到线程上去执行**
  
## Exec &nbsp;<span class="label label-static">静态</span> <span class="label label-single">单线程</span> 

  在默认线程中执行一个函数.
  
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
	<td>Function </td>
	<td></td>
	<td>要执行的函数.</td>
</tr><tr>
	<td>Args...	</td>
	<td></td>
	<td>该函数需要传入的参数，具体参数及个数以需要调用的函数为准.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 



<div class="adoc" id="div_Exec"></div>


## Sync &nbsp;<span class="label label-static">静态</span> <span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  在默认线程中同步执行一个函数.
  
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
	<td>Function </td>
	<td></td>
	<td>需要执行的函数.</td>
</tr><tr>
	<td>Args</td>
	<td>...	</td>
	<td>该函数需要传入的参数，具体参数及个数以需要调用的函数为准.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Any 该函数的执行结果. 



<div class="adoc" id="div_Sync"></div>


## weakup &nbsp;<span class="label label-single">单线程</span> 

  唤醒线程，只能在浏览器线程（UI/Renderer）上执行.
  
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
	<td>Any </td>
	<td></td>
	<td>传递唤醒参数，该值可以做为suppend的返回值，可用于线程上的扩展与页面交换数据</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 



<div class="adoc" id="div_weakup"></div>


## suspend &nbsp;
  暂停线程，不能在浏览器线程（UI/Renderer）上执行.
  
* **函数参数**  无

* **返回值**
  Any weakup函数的第一个参数. 



<div class="adoc" id="div_suspend"></div>


## exec &nbsp;<span class="label label-single">单线程</span> 

  执行一个函数.
  
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
	<td>Function </td>
	<td></td>
	<td>要执行的函数.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 



<div class="adoc" id="div_exec"></div>


## sync &nbsp;<span class="label label-single">单线程</span> <span class="label label-sync">同步</span> 

  同步调用一个函数，并返回该函数的执行结果.
  
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
	<td>Function </td>
	<td></td>
	<td>要同步调用的函数.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Any . 



<div class="adoc" id="div_sync"></div>


## kill &nbsp;<span class="label label-single">单线程</span> 

  杀死当前线程.
  
* **函数参数**  无

* **返回值**
   无. 



<div class="adoc" id="div_kill"></div>


## start &nbsp;<span class="label label-single">单线程</span> 

  启动线程，该函数一般不需要调用，线程会在创建时自动启动，除非你调用了stop.
  
* **函数参数**  无

* **返回值**
  Boolean 是否启动成功. 



<div class="adoc" id="div_start"></div>


## stop &nbsp;<span class="label label-single">单线程</span> 

  停止当前线程.
  
* **函数参数**  无

* **返回值**
  Boolean 是否停止成功. 



<div class="adoc" id="div_stop"></div>


## Task演示 &nbsp;
  Task演示
  
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Task', 'TaskTest');
var taskTest = new TaskTest();

taskTest.unique('TaskTest.add', function(retval){
    console.log(arguments);
    console.log(retval.data);
});

taskTest.unique('TaskTest.sub', function(retval){
    console.log(arguments);
    console.log(retval.data);
});

// 使用Task类提供的线程
Task.Exec(taskTest.add, 1,1);


console.assert(Task.Sync(taskTest.add, 1,2) == 3);

var thread = new Task();
thread.exec(taskTest.add, 1,3);


console.assert(thread.sync(taskTest.add, 1,4) == 5);

 // 附加到线程
taskTest.attach(thread);
// 输出undefined
console.log(taskTest.add(1,5));
taskTest.sub(3,1);
taskTest.detach();
console.assert(taskTest.add(1,6) == 7);

taskTest.attach(thread);
// 在扩展中挂起线程
taskTest.sleepForWeakup();
// add 不能得到结果
taskTest.add(1,8);
setTimeout(function(){
    // 唤醒线程后，继续执行add(1,8);
    taskTest.weakup();

    // 挂起线程

    taskTest.suspend();
    taskTest.add(1,9);
    setTimeout(function(){
        // 线程已经被杀死，add(1,9)不会被执行;
        taskTest.weakup();
    }, 5000);

    taskTest.detach();
    thread.kill(); // 杀死线程

    taskTest.add(1,10);
    console.assert(taskTest.add(1,10) == 11);

}, 5000);




```


<div class="adoc" id="div_Task演示"></div>


