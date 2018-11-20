# clipboard

  剪贴板.
  
## 事件
### **1.copy** 

  监听到复制事件时触发 .

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.unique('copy', function(){
    console.log('trigger copy event');
});

```


<div class="adoc" id="div_copy"></div>


### **2.cut** 

  监听到剪切事件时触发（只在文件剪切时触发） .

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.unique('cut', function(){
    console.log('trigger cut event');
});

```


<div class="adoc" id="div_cut"></div>


### **3.empty** 

  清空剪贴板时触发 .

* **事件参数**  无

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.unique('empty', function(){
    console.log('trigger empty event');
});

```


<div class="adoc" id="div_empty"></div>


## startWatch &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  监听剪贴板事件.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
console.assert(clipboard.startWatch() == true);

```


<div class="adoc" id="div_startWatch"></div>


## stopWatch &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  停止监听剪贴板事件.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
console.assert(clipboard.stopWatch() == true);

```


<div class="adoc" id="div_stopWatch"></div>


## isWatching &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  是否正在监听剪贴板事件.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.startWatch();
console.assert(clipboard.isWatching() == true);
clipboard.stopWatch();
console.assert(clipboard.isWatching() == false);
clipboard.startWatch();

```


<div class="adoc" id="div_isWatching"></div>


## writeText &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  向剪贴板中写入字符串.
  
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
	<td>要写入的字符串.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Undefined . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
console.assert(clipboard.writeText('NCUI Clipboard Text 中文') == true);

```


<div class="adoc" id="div_writeText"></div>


## writeImage &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  写入图片到剪贴板.
  
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
	<td>图片数据 base64编码的图片.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

*****
  写入图片到剪贴板.
  
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
	<td>图片路径（本地文件路径）,(也可以是一个BigStr封装的对象，如果图片太大，最好使用BigStr封装，进程间通信的管道只有2M，超过这个大小的数据不能被正确送达).</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.writeImage('');
//将图片转化成base64
function convertImgToBase64(url, callback, outputFormat) {
    var canvas = document.createElement('CANVAS'),
　  ctx = canvas.getContext('2d'),
  　img = new Image;　　
    img.crossOrigin = 'Anonymous';　　
    img.onload = function() {　　
        canvas.height = img.height;　　
        canvas.width = img.width;　　
        ctx.drawImage(img, 0, 0);　　
        var dataURL = canvas.toDataURL(outputFormat || 'image/jpeg');　　
        callback.call(this, dataURL);　　
        canvas = null;
    };
    img.src = url;
}

var url = "@file:///%appDir%images/splash.jpg"; //这是站内的一张图片资源，采用的相对路径
convertImgToBase64(url, function(base64Img) {
    //转化后的base64
    clipboard.writeImage(base64Img);

    include('BigStr');
    // 也可以使用BigStr传递数据
    clipboard.writeImage(new BigStr(base64Img));
});




```


<div class="adoc" id="div_writeImage"><div id="example" class="example code" contenteditable="true"><img id="ncuiImg_writeImage" src="@file:///%appDir%images/splash.jpg"></img> </div></div>


## writeImageFromFile &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  将一个本地图片文件内容写入到剪贴板.
  
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
include('clipboard');
console.assert(clipboard.writeImageFromFile('%appDir%images/splash.jpg') == true);

```


<div class="adoc" id="div_writeImageFromFile"></div>


## writeFiles &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  写入文件到剪贴板.
  
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
include('clipboard');
console.assert(clipboard.writeFiles('manifest.json') == true);

```
*****
  写入文件到剪贴板.
  
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
	<td>Array </td>
	<td></td>
	<td>文件路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
console.assert(clipboard.writeFiles(['skin', 'manifest.json']) == true);

```


<div class="adoc" id="div_writeFiles"></div>


## readText &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  读取剪贴板中的字符串.
  
* **函数参数**  无

* **返回值**
  String . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
console.assert(clipboard.writeText('NCUI Clipboard Text 中文') == true);
var str = clipboard.readText();
console.log(str);
console.assert(str == 'NCUI Clipboard Text 中文');

```


<div class="adoc" id="div_readText"></div>


## readImage &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  读取图片,读取到的图片以Base64编码返回
  
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
	<td>String</td>
	<td>"jpeg" </td>
	<td>图片格式.</td>
</tr>
	</tbody>
</table>

* **返回值**
  String 图片数据. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.writeImageFromFile('%appDir%images/splash.jpg');
var str = clipboard.readImage('.jpeg');
$('#ncuiImg').attr("src", str);

```


<div class="adoc" id="div_readImage"><div id="example" class="example code" contenteditable="true"><img id="ncuiImg" src="http:://127.0.0.1:8030/@file:///%skinDir%logo.ico"></img> </div></div>


## readFiles &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  读取文件.
  
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
	<td>Boolean</td>
	<td>false </td>
	<td>是否包含目录.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Array 返回一个包含文件路径的字符串. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
var arr = clipboard.readFiles();
console.log(arr);
console.log(arr.length);

```


<div class="adoc" id="div_readFiles"></div>


## hasFormat &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  查看剪贴板中是否存在指定格式的数据.
  
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
	<td>需要判断的格式.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.hasFormat(clipboard.CF_TEXT);

```


<div class="adoc" id="div_hasFormat"></div>


## empty &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  清空剪贴板.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.empty();
console.assert(clipboard.isEmpty() == true);
clipboard.writeText('1232');
console.assert(clipboard.isEmpty() == false);

```


<div class="adoc" id="div_empty"></div>


## isEmpty &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  判断剪贴板是否为空.
  
* **函数参数**  无

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.empty();
console.assert(clipboard.isEmpty() == true);
clipboard.writeText('1232');
console.assert(clipboard.isEmpty() == false);

```


<div class="adoc" id="div_isEmpty"></div>


## getFormats &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取剪贴板中所有的数据格式.
  
* **函数参数**  无

* **返回值**
  Array . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.empty();
var arr = clipboard.getFormats();
var len = arr.length;
console.log(arr);
console.log(len);
console.assert(len == 0);
clipboard.writeText('1232');
arr = clipboard.getFormats();
len = arr.length;
console.log(arr);
console.log(len);

```


<div class="adoc" id="div_getFormats"></div>


## effect &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  判断剪贴板中的数据是复制的还是粘贴的(只对资源管理器中的文件操作有效).
  
* **函数参数**  无

* **返回值**
  Int 未知(0) 复制(1) 剪切(2) 连接(4). 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.empty();
console.assert(clipboard.effect() == 0);
clipboard.writeFiles('manifest.json');
console.assert(clipboard.effect() == 1);

```


<div class="adoc" id="div_effect"></div>


## screenshot &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  截屏,将数据保存到剪贴板中（JPG格式）.
  
* **函数参数**  无

* **返回值**
  Boolean  

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
console.assert(clipboard.screenshot() == true);
var img = clipboard.readImage();
$('#ncuiImg3').attr("src", img);


```
*****
  截屏,将数据保存到剪贴板中（JPG格式）.
  
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
	<td>Array </td>
	<td></td>
	<td>指定截屏区域Int类型 [left,top, width, height].</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('clipboard', 'BrowserWindow');
var win = BrowserWindow.current;
var winInfo = win.getWindowRect();
console.log(winInfo);
console.assert(clipboard.screenshot([winInfo.x, winInfo.y, winInfo.width, winInfo.height]) == true);
var img = clipboard.readImage();
$('#ncuiImg3').attr("src", img);



```


<div class="adoc" id="div_screenshot"><div id="example" class="example code" contenteditable="true"><img id="ncuiImg3" src=""></img> </div></div>


## saveText &nbsp;
  保存剪贴板中的文本数据到磁盘.
  
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
include('clipboard');
clipboard.writeText('1111111111111111111111中文');
console.assert(clipboard.saveText('test3.json') == true);
include('Path');
console.assert(Path.Remove('test3.json') == true);

```


<div class="adoc" id="div_saveText"></div>


## saveImage &nbsp;
  保存剪贴板中的文本数据到磁盘.
  
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
includes( 'clipboard');
console.assert(clipboard.screenshot() == true);
console.assert(clipboard.saveImage('test2/screen.jpg') == true);
console.assert(clipboard.saveImage('test2/screen.bmp') == true);
console.assert(clipboard.saveImage('test2/screen.png') == true);
$('#ncuiImg4').attr('src', '');
$('#ncuiImg4').attr('src', '@file:///%appDir%test2/screen.jpg');

```


<div class="adoc" id="div_saveImage"><div id="example" class="example code" contenteditable="true"><img id="ncuiImg4" src=""></img> </div></div>


## saveFiles &nbsp;
  保存剪贴板中的文件数据到磁盘（单个文件）.
  
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
include('clipboard');
clipboard.empty();
console.assert(clipboard.saveFiles('%appDir%test2.json') == false);
clipboard.writeFiles('manifest.json');
console.assert(clipboard.saveFiles('%appDir%test2.json') == true);
include('Path');
console.assert(Path.Remove('test2.json') == true);

```
*****
  保存剪贴板中的文本数据到磁盘.
  
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
	<td>JsonArray </td>
	<td></td>
	<td>文件路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('clipboard');
clipboard.empty();
console.assert(clipboard.saveFiles('%appDir%test2.json') == false);
clipboard.writeFiles(['manifest.json', 'libcef.dll', 'C:/windows/notepad.exe']);
console.assert(clipboard.saveFiles([
    {src: 'manifest.json', dst:'%appDir%test2/test2.json'},
    {src: 'libcef.dll', dst:'%appDir%test2/libcef.dll'},
    {src: 'C:/windows/notepad.exe', dst:'%appDir%test2/nodepad.exe'}
]) == true);
include('Path');
Path.Remove('test2.json');
Path.RemoveAll('test2');

```


<div class="adoc" id="div_saveFiles"></div>


