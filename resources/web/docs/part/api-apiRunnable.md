# Runnable

  如果一个类想要在线程中运行的话，需要继承自Runable.<br>工作线程:**UI线程** 或 **Renderer线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## attach &nbsp;<span class="label label-single">单线程</span> 

  将当前对象附加到一个线程中.
  
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
	<td>需要附加的线程</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Demo', 'Thread');
var demo = new Demo();
var thread = new Thread();
console.log(demo.add(1,2));
demo.attach(thread);
demo.unique('Demo.add',function(info){
    console.log(info);
    console.log('在线程中执行函数的返回结果:' + info.data);
});
console.log(demo.add(1,2));

```


<div class="adoc" id="div_attach"></div>


## detach &nbsp;<span class="label label-single">单线程</span> 

  将当前对象从一个线程中分离.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Demo', 'Thread');
var demo = new Demo();
var thread = new Thread();
console.log(demo.add(1,2));
demo.attach(thread);
demo.unique('Demo.add',function(info){
    console.log(info);
    console.log('在线程中执行函数的返回结果:' + info.data);
});
console.log(demo.add(1,2));
demo.detach();
console.assert(demo.add(1,3) == 4, true);

```


<div class="adoc" id="div_detach"></div>


## weakup &nbsp;<span class="label label-single">单线程</span> 

  唤醒当前对象所附加的线程.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Demo', 'Thread');
var demo = new Demo();
var thread = new Thread();
console.log(demo.add(1,2));
demo.attach(thread);
demo.sleepForWeakup();
setTimeout(function(){
   demo.weakup();
}, 3000);
demo.unique('Demo.add',function(info){
    console.log(info);
    console.log('在线程中执行函数的返回结果:' + info.data);
});
console.log(demo.add(1,3));


```


<div class="adoc" id="div_weakup"></div>


## suspend &nbsp;
  挂起所附加线程，不能在浏览器线程（UI/Renderer）上执行.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('Demo', 'Thread');
 var demo = new Demo();
 var thread = new Thread();
 console.log(demo.add(1,2));
 demo.attach(thread);
 demo.suspend();   // 如果没有attach，不能在渲染线程上调用该函数，否则会阻塞渲染线程
 setTimeout(function(){
    demo.weakup();
 }, 3000);
 demo.unique('Demo.add',function(info){
    // 3秒后才能得到结果
     console.log(info);
     console.log('在线程中执行函数的返回结果:' + info.data);
 });

 demo.add(1,4);


```


<div class="adoc" id="div_suspend"></div>


