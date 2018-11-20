# Path

  路径库.<br>工作线程**Renderer线程**
  
* **父类** 
<a href="#api/apiRunnable">Runnable</a>&nbsp;

## getFullAppDir &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  返回当前程序所在目录.
  
* **函数参数**  无

* **返回值**
  Path 返回一个Path对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = Path.getFullAppDir();
console.log(p);
console.log(p.toString());

```


<div class="adoc" id="div_getFullAppDir"></div>


## getFullAppName &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取当前程序的完整路径.
  
* **函数参数**  无

* **返回值**
  Path 返回一个Path对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = Path.getFullAppName();
console.log(p);
console.log(p.toString());

```


<div class="adoc" id="div_getFullAppName"></div>


## fileExsit &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  判断文件或目录是否存在.
  
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
	<td>路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 存在/false 不存在. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
p.copyTo('manifest2.json');
console.assert(Path.fileExsit('manifest2.json') == true);

```


<div class="adoc" id="div_fileExsit"></div>


## Remove &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  删除一个文件，或空目录.
  
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
	<td>路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 成功/false 失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
p.copyTo('manifest2.json');
Path.Remove('manifest2.json')
console.assert(Path.fileExsit('manifest2.json') == false);

```


<div class="adoc" id="div_Remove"></div>


## RemoveAll &nbsp;<span class="label label-static">静态</span> 

  删除一个文件或文件夹，文件目录可以不为空.
  
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
	<td>路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 成功/false 失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
p.copyTo('manifest2.json');
Path.RemoveAll('manifest2.json')
console.assert(Path.fileExsit('manifest2.json') == false);

```


<div class="adoc" id="div_RemoveAll"></div>


## toString &nbsp;<span class="label label-sync">同步</span> 

  将当前Path对象转换为String.
  
* **函数参数**  无

* **返回值**
  String . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
console.log(p.toString());

```


<div class="adoc" id="div_toString"></div>


## normalize &nbsp;
  标准化路径.
  
* **函数参数**  无

* **返回值**
  Path 当前对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('./sddf/../../233/../234/../../');
console.log(p.normalize().toString());

```


<div class="adoc" id="div_normalize"></div>


## isExsit &nbsp;<span class="label label-sync">同步</span> 

  判断当前路径是否存在.
  
* **函数参数**  无

* **返回值**
  Boolean true 存在/ false 不存在. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
console.assert(p.isExsit() == true);

```


<div class="adoc" id="div_isExsit"></div>


## append &nbsp;<span class="label label-sync">同步</span> 

  往当前路径中追加子路径.
  
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
	<td>需要追加了路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Path 当前对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('./');
p.append('manifest.json');
console.log(p.toString());
console.assert(p.isExsit() == true);

```


<div class="adoc" id="div_append"></div>


## toAbsolute &nbsp;
  将当前路径转换为一个绝对路径.
  
* **函数参数**  无

* **返回值**
  Path 当前对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('./');
p.append('manifest.json');
p.toAbsolute();
console.log(p.toString());
console.assert(p.isExsit() == true);

```


<div class="adoc" id="div_toAbsolute"></div>


## removeFileSpec &nbsp;<span class="label label-sync">同步</span> 

  去除文件名，得到目录.
  
* **函数参数**  无

* **返回值**
  Path 当前对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('./');
p.append('manifest.json');
p.toAbsolute().removeFileSpec();
console.log(p.toString());
console.assert(p.normalize().toString() == Path.getFullAppDir().normalize().toString());

```


<div class="adoc" id="div_removeFileSpec"></div>


## copyTo &nbsp;<span class="label label-sync">同步</span> 

  复制文件到指定位置.
  
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
	<td>目标位置.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 存在/ false 不存在. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
p.copyTo('manifest2.json');

console.assert(Path.fileExsit('manifest2.json') == true);

```


<div class="adoc" id="div_copyTo"></div>


## moveTo &nbsp;<span class="label label-sync">同步</span> 

  移动文件到指定位置.
  
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
	<td>目标位置.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 存在/ false 不存在. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest.json');
p.copyTo('manifest2.json');
console.assert(Path.fileExsit('manifest2.json') == true);

p = new Path('manifest2.json');
p.moveTo('manifest3.json');

console.assert(Path.fileExsit('manifest2.json') == false);
console.assert(Path.fileExsit('manifest3.json') == true);


```


<div class="adoc" id="div_moveTo"></div>


## remove &nbsp;
  删除当前路径，该路径只能是一个文件或空目录删除一个文件，或空目录.
  
* **函数参数**  无

* **返回值**
  Boolean true 成功/false 失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('manifest3.json');
p.remove();

console.assert(Path.fileExsit('manifest3.json') == false);


```


<div class="adoc" id="div_remove"></div>


## removeAll &nbsp;
  删除当前路径，可以删除子目录，.
  
* **函数参数**  无

* **返回值**
  Boolean true 成功/false 失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Path');
var p = new Path('web');
p.copyTo('web2');

console.assert(Path.fileExsit('web2') == true);
p = new Path('web2');
p.removeAll();
console.assert(p.isExsit('web2') == false);

```


<div class="adoc" id="div_removeAll"></div>


