# FileStream

  文件读写类.
  
* **父类** 
<a href="#api/apiRunnable">Runnable</a>&nbsp;

## Remove &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  删除文件.
  
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
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest2.json', FileStream.out + FileStream.binary);
filestream.write('33');
// 文件被占用，无法删除
console.assert(FileStream.Remove('manifest2.json') == false);
filestream.close();
// 解除文件占用，可以删除
console.assert(FileStream.Remove('manifest2.json') == true);

```


<div class="adoc" id="div_Remove"></div>


## FileStream &nbsp;<span class="label label-constructor">构造</span> 

  创建或打开一个文件.
  
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
	<td>需要操作文件的路径.</td>
</tr><tr>
	<td>Int</td>
	<td>3	</td>
	<td>文件打开方式，可以选择以下值，组合使用.<br>&nbsp;&nbsp;&nbsp;&nbsp; 1 以只读方式打开，如果文件不存在，那么将打开失败<br>&nbsp;&nbsp;&nbsp;&nbsp; 2 以写方式打开，如果文件不存在则会创建文件<br>&nbsp;&nbsp;&nbsp;&nbsp; 8 以追加方式打开<br>&nbsp;&nbsp;&nbsp;&nbsp; 32 以二进制方式打开文件<br></td>
</tr>
	</tbody>
</table>

* **返回值**
  FileStream  



<div class="adoc" id="div_FileStream"></div>


## open &nbsp;<span class="label label-sync">同步</span> 

  打开文件，创建对象时会默认打开文件，不需要重复调用.如果你调用了<a href="#extend/extendFileStream/9">close</a>函数，那么可以调用该函数重新打开文件
  
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
	<td>int</td>
	<td>0 </td>
	<td>文件打开方式，参见<a href="#extend/extendFileStream/1">构造函数</a>的内容.如果不输入该参数，程序会使用创建该对象时指定的打开方式</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 



<div class="adoc" id="div_open"></div>


## read &nbsp;<span class="label label-sync">同步</span> <span class="label label-deprecated">废弃</span> 

  读取所有文件内容.
  
* **函数参数**  无

* **返回值**
  String 读取到的字符串. 

* **参考** 
<a href="#extend/extendFileStream/5">readSome</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json');
console.log(filestream.read());
filestream.close();

```


<div class="adoc" id="div_read"></div>


## readAll &nbsp;<span class="label label-sync">同步</span> 

  获取所有字符串, readAll采用不同的机制读取文件，该操作与当前流无关，所以调用该函数不会影响其他状态函数如<a href="#extend/extendFileStream/13">eof</a>.
  
* **函数参数**  无

* **返回值**
  String  

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json');
console.log(filestream.readAll());
filestream.close();

```


<div class="adoc" id="div_readAll"></div>


## readSome &nbsp;<span class="label label-sync">同步</span> 

  读取指定数量.
  
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
	<td>最多读取多少个字符.</td>
</tr>
	</tbody>
</table>

* **返回值**
  String . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json');
while(!filestream.eof()){
    console.log(filestream.readSome(100));
}
filestream.close();

```


<div class="adoc" id="div_readSome"></div>


## readLine &nbsp;<span class="label label-sync">同步</span> 

  读取一行,一行最多4096个字符，如果多于这个值则不能获取完整数据，参考<a href="#extend/extendFileStream/5">readSome</a>.
  
* **函数参数**  无

* **返回值**
  String . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json');
while(!filestream.eof()){
    console.log(filestream.readLine());
}
filestream.close();

```


<div class="adoc" id="div_readLine"></div>


## write &nbsp;<span class="label label-sync">同步</span> 

  向文件中写入字符串.
  
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
	<td>需要写入的数据.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 成功/false 失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest2.json', FileStream.out | FileStream.binary);
console.assert(filestream.write('1234567890') == true);
filestream.remove();

```


<div class="adoc" id="div_write"></div>


## append &nbsp;<span class="label label-sync">同步</span> 

  将一个文件的内容追加到当前文件中.
  
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
	<td>文件路径</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true 成功/false 失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest2.json', 2);
filestream.append('manifest.json');
filestream.close();
console.log(filestream.size());
filestream.remove();

```


<div class="adoc" id="div_append"></div>


## close &nbsp;<span class="label label-sync">同步</span> 

  关闭文件，关闭后不能再调用其他API操作文件.
  
* **函数参数**  无

* **返回值**
   无. 

* **参考** 
<a href="#extend/extendFileStream/2">open</a>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json');
filestream.close();

console.assert(filestream.isOpened() == false);

```


<div class="adoc" id="div_close"></div>


## isOpened &nbsp;<span class="label label-sync">同步</span> 

  判断文件是否打开.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json');
console.assert(filestream.isOpened() == true);

filestream.close();
console.assert(filestream.isOpened() == false);

```


<div class="adoc" id="div_isOpened"></div>


## size &nbsp;<span class="label label-sync">同步</span> 

  获取文件大小.
  
* **函数参数**  无

* **返回值**
  Int . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json', FileStream.in);
console.log(filestream.size());

var filestream2 = new FileStream('manifest2.json',FileStream.out);
console.assert(filestream2.size() == 0);

filestream2.write('1234567890');
filestream2.close();
filestream2.open(1);
console.assert(filestream2.size() == 10);

filestream2.remove();

```


<div class="adoc" id="div_size"></div>


## remove &nbsp;<span class="label label-sync">同步</span> 

  删除当前文件.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
// 以读写方式打开，如果文件不存在，会创建文件
var filestream = new FileStream('manifest2.json');
console.assert(filestream.remove() == true);
// 以只读方式打开，如果文件不存在，则不会创建文件
var filestream2 = new FileStream('manifest2.json', 1);
console.assert(filestream.remove() == true);

```


<div class="adoc" id="div_remove"></div>


## eof &nbsp;
  判断是否已读取到文件末尾.
  
* **函数参数**  无

* **返回值**
  Boolean true 末尾/ false 非末尾. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('FileStream');
var filestream = new FileStream('manifest.json', FileStream.in);
//filestream.readAll(); // readAll不会影响内部文件指针位置
console.log(filestream.eof());
console.assert(filestream.eof() ==false);
while(!filestream.eof()){
    console.log(filestream.readSome(100));
}
console.assert(filestream.eof() == true);


```


<div class="adoc" id="div_eof"></div>


