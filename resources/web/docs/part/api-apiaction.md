# action

  内置事件.
  
## execute &nbsp;<span class="label label-static">静态</span> 

  执行一个内置动作，可以支持事件参考<a href="#settings/settingsActionPreprocessor">预置事件</a>.
  
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
	<td>动作.</td>
</tr>
	</tbody>
</table>

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('action');
// 最小化当前窗口
action.execute('minimize');
// 最大化当前窗口
action.execute('maximize');

```


<div class="adoc" id="div_execute"></div>


