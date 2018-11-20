# HTTP请求参数

  手动HTTP请求时需要用到的参数.
  
## 变量列表

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">名称</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String</td>
	<td>type</td>
	<td>GET </td>
	<td>HTPP请求类型，POST 或者 GET </td>
</tr><tr>
	<td>String </td>
	<td>url</td>
	<td></td>
	<td>HTTP请求地址. </td>
</tr><tr>
	<td>JsonObject </td>
	<td>data</td>
	<td></td>
	<td>要发送的数据. </td>
</tr><tr>
	<td>JsonObject </td>
	<td>header</td>
	<td></td>
	<td>HTTP请求的头部参数. </td>
</tr><tr>
	<td>Array </td>
	<td>files</td>
	<td></td>
	<td>要上传的文件列表，文件以字符串表示. </td>
</tr>
	</tbody>
</table>


