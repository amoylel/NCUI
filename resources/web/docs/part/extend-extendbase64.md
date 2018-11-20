# base64

  Base64 编解码.<br>工作线程**Renderer线程**
  
* **父类** 
<a href="#api/apiRunnable">Runnable</a>&nbsp;

## decode &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  Base64 解码.
  
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
	<td>需要解码的字符串.</td>
</tr>
	</tbody>
</table>

* **返回值**
  String 解码后的字符串. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('base64');
console.assert(base64.decode('TkNVSQ==') == 'NCUI');

```


<div class="adoc" id="div_decode"></div>


## encode &nbsp;<span class="label label-static">静态</span> <span class="label label-sync">同步</span> 

  Base64 编码.
  
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
	<td>需要编码的字符串.</td>
</tr>
	</tbody>
</table>

* **返回值**
  String 编码后的字符串. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('base64');
console.assert(base64.encode('NCUI') == 'TkNVSQ==');

```


<div class="adoc" id="div_encode"></div>


