# desktop

  用于获取桌面的基本信息.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## getScreenInfo &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取屏幕信息,包括屏幕大小和工作区域大小.
  
* **函数参数**  无

* **返回值**
  JsonObject .
	* **Int** width   桌面宽度
	* **Int** height  桌面高度
	* **JsonObject** rcWork  桌面工作区域信息
		* **Int** left  工作区域左上角坐标X
		* **Int** top  工作区域左上角坐标Y
		* **Int** right  工作区域右下角坐标X
		* **Int** bottom  工作区域右下角坐标Y
		* **Int** width  工作区域宽度
		* **Int** height  工作区域高度
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('desktop');
var desktopInfo = desktop.getScreenInfo();
console.log(desktopInfo);

```


<div class="adoc" id="div_getScreenInfo"></div>


## screenshot &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  截屏.
  
* **函数参数**  无

* **返回值**
  String base64编码的图片数据（JPG格式）. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('desktop');
var img = desktop.screenshot();
console.log(img.length);
$('#ncuiImg2').attr("src", img);


```
*****
  截屏.
  
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
  String base64编码的图片数据（JPG格式）. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
includes('desktop', 'BrowserWindow');
var win = BrowserWindow.current;
var winInfo = win.getWindowRect();
console.log(winInfo);
var img = desktop.screenshot([winInfo.x, winInfo.y, winInfo.width, winInfo.height]);
$('#ncuiImg2').attr("src", img);



```


<div class="adoc" id="div_screenshot"><div id="example" class="example code" contenteditable="true"><img id="ncuiImg2" src=""></img> </div></div>


