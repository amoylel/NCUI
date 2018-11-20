# shell

  运行一个外部程序（或者是打开一个已注册的文件、打开一个目录、打印一个文件等等）.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## 属性列表

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">名称</th>
			<th class="col-xs-1">取值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>Int</td>
	<td>SW_HIDE</td>
	<td>SW_HIDE </td>
	<td>打开时不显示界面.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_MINIMIZE</td>
	<td>SW_MINIMIZE </td>
	<td>以最小化的方式打开.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_RESTORE</td>
	<td>SW_RESTORE </td>
	<td>以还原显示的方式打开.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOW</td>
	<td>SW_SHOW </td>
	<td>打开时显示界面.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOWMAXIMIZED</td>
	<td>SW_SHOWMAXIMIZED </td>
	<td>激活窗口并将其最大化.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOWMINIMIZED</td>
	<td>SW_SHOWMINIMIZED </td>
	<td>激活窗口并将其最小化.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOWMINNOACTIVE</td>
	<td>SW_SHOWMINNOACTIVE </td>
	<td>窗口最小化，激活窗口仍然维持激活状态.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOWNA</td>
	<td>SW_SHOWNA </td>
	<td>以窗口原来的状态显示窗口。激活窗口仍然维持激活状态.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOWNOACTIVATE</td>
	<td>SW_SHOWNOACTIVATE </td>
	<td>以窗口最近一次的大小和状态显示窗口。激活窗口仍然维持激活状态.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>SW_SHOWNORMAL</td>
	<td>SW_SHOWNORMAL </td>
	<td>激活并显示一个窗口。如果窗口被最小化或最大化，系统将其恢复到原来的尺寸和大小。.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr>
	</tbody>
</table>


## exec &nbsp;<span class="label label-static">静态</span> 

  执行一下shell命令.
  
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
	<td>要执行的shell命令 open print 等.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>要调用的程序或打开的文件路径.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>启动参数,默认为"".</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>工作目录,默认为当前路径.</td>
</tr><tr>
	<td>Int</td>
	<td>1 </td>
	<td>显示参数</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('shell');
shell.exec("open", "explorer", "/e, /select, skin\\close_p.png", "", shell.SW_SHOWNORMAL);

```


<div class="adoc" id="div_exec"></div>


## open &nbsp;<span class="label label-static">静态</span> 

  打开文件或程序.
  
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
	<td>String	</td>
	<td></td>
	<td>文件或程序路径.</td>
</tr><tr>
	<td>String	</td>
	<td></td>
	<td>启动参数,默认为"".</td>
</tr><tr>
	<td>String	</td>
	<td></td>
	<td>工作目录,默认为当前路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('shell');
shell.open("www.baidu.com");


```


<div class="adoc" id="div_open"></div>


## print &nbsp;<span class="label label-static">静态</span> 

  打印文件.
  
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
	<td>文件路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('shell');
shell.print("skin\\close_p.png");

```


<div class="adoc" id="div_print"></div>


## showItemInFolder &nbsp;<span class="label label-static">静态</span> 

  在文件夹中显示文件（定位文件位置）.
  
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
	<td>文件路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('shell');
shell.showItemInFolder('skin\\close_p.png');

```


<div class="adoc" id="div_showItemInFolder"></div>


