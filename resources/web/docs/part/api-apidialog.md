# dialog

  弹出框，文件选择框.<br>工作线程:**UI线程**.
  
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
	<td>FILE_DIALOG_OPEN</td>
	<td>FILE_DIALOG_OPEN </td>
	<td>允许选择一个存在的文件.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>FILE_DIALOG_OPEN_MULTIPLE</td>
	<td>FILE_DIALOG_OPEN_MULTIPLE </td>
	<td>可以选择多个存在的文件.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>FILE_DIALOG_OPEN_FOLDER</td>
	<td>FILE_DIALOG_OPEN_FOLDER </td>
	<td>选择文件夹.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>FILE_DIALOG_SAVE</td>
	<td>FILE_DIALOG_SAVE </td>
	<td>允许选择一个不存在的文件，如果所选择的文件存在，那么会提示是否覆盖该文件，一般用来做文件保存选择.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>FILE_DIALOG_TYPE_MASK</td>
	<td>FILE_DIALOG_TYPE_MASK </td>
	<td>General mask defining the bits used for the type values.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>FILE_DIALOG_OVERWRITEPROMPT_FLAG</td>
	<td>FILE_DIALOG_OVERWRITEPROMPT_FLAG </td>
	<td>Prompt to overwrite if the user selects an existing file with the Save dialog.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>FILE_DIALOG_HIDEREADONLY_FLAG</td>
	<td>FILE_DIALOG_HIDEREADONLY_FLAG </td>
	<td>不显示只读文件.  <span class="label label-static">静态</span> <span class="label label-const">常量</span> <span class="label label-property">属性</span> 
</td>
</tr>
	</tbody>
</table>


## runFileDialog &nbsp;<span class="label label-static">静态</span> <span class="label label-async">异步</span> 

  打开文件选择对话框.
  
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
	<td>文件打开参数.<br>[**String**]  title 标题.<br>[**String**]	model 对话框类型，可用的<a href="#api/apidialog/0">对话框模式</a><br>[**String**]	defaultFilePath 默认开始目录<br>[**Array**]	acceptFilters, 可接受的文件类型<br>[**String**]	selectedAcceptFilter 过滤器</td>
</tr><tr>
	<td>Function </td>
	<td></td>
	<td>选择完成后的回调函数, 用于传递用户的选择结果，结果为一个Array.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('dialog');
// 单文件选择
dialog.runFileDialog({
    title: 'NCUI文件选择对话框',
    model: dialog.FILE_DIALOG_OPEN,
    defaultFilePath: '',
    acceptFilters: ['image/*'],
    selectedAcceptFilter: 0
}, function(arr) {
    console.log(arr);
});

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('dialog');
// 从文件选择
dialog.runFileDialog({
    title: 'NCUI文件选择对话框',
    model: dialog.FILE_DIALOG_OPEN_MULTIPLE,
    defaultFilePath: '',
    acceptFilters: ['image/*'],
    selectedAcceptFilter: 0
}, function(arr) {
   console.log(arr);
});

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('dialog');
// 从文件选择
dialog.runFileDialog({
title: 'NCUI文件选择对话框',
model: dialog.FILE_DIALOG_OPEN_MULTIPLE,
defaultFilePath: '',
acceptFilters: ['image/*'],
selectedAcceptFilter: 0
}, function(arr) {
console.log(arr);
});

```


<div class="adoc" id="div_runFileDialog"></div>


## alert &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  弹出提示框，相关于js的alert函数.
  
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
	<td>需要显示的文本.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('dialog');
dialog.alert('我是一个弹出框');

```


<div class="adoc" id="div_alert"></div>


## confirm &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  选择对话框，相当于js的confirm函数.
  
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
	<td>需要显示的文本.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('dialog');
var retval = dialog.confirm('是否需要退出程序');
console.log(retval);

```


<div class="adoc" id="div_confirm"></div>


## prompt &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  获取用户输入值，相当于js的prompt函数.
  
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
	<td>提示文本.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>默认输入文本，可选.</td>
</tr>
	</tbody>
</table>

* **返回值**
  JsonObject .
	* **Boolean** code  用户的选择结果,true /false.
	* **String** text  用户的输入文本.
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('dialog');
var retval = dialog.prompt('请输入你的姓名', '张三');
console.log(retval);

```


<div class="adoc" id="div_prompt"></div>


