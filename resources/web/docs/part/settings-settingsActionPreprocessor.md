# 预置事件

  预置事件,用于处理菜单，和全局快捷键的action所描述的事件，如果action为以下所描述的事件，那么程序会处理这些事件，并阻止事件向页面传递.
  
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
	<td>String </td>
	<td>undo</td>
	<td></td>
	<td>撤消，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>redo</td>
	<td></td>
	<td>重做，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>cut</td>
	<td></td>
	<td>剪切，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>copy</td>
	<td></td>
	<td>复制，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>paste</td>
	<td></td>
	<td>粘贴，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>selectAll</td>
	<td></td>
	<td>全选，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>delete</td>
	<td></td>
	<td>删除，作用于Frame </td>
</tr><tr>
	<td>String </td>
	<td>goBack</td>
	<td></td>
	<td>后退，作用于Browser </td>
</tr><tr>
	<td>String </td>
	<td>goForward</td>
	<td></td>
	<td>前进，作用于Browser </td>
</tr><tr>
	<td>String </td>
	<td>reload</td>
	<td></td>
	<td>正常重新加载当前窗口，作用于Browser </td>
</tr><tr>
	<td>String </td>
	<td>forceReload</td>
	<td></td>
	<td>忽略缓存并重新加载当前窗口，作用于Browser </td>
</tr><tr>
	<td>String </td>
	<td>stopLoad</td>
	<td></td>
	<td>停止加载，作用于Browser </td>
</tr><tr>
	<td>String </td>
	<td>print</td>
	<td></td>
	<td>打印页面，作用于HOST </td>
</tr><tr>
	<td>String </td>
	<td>printToPDF</td>
	<td></td>
	<td>打印页面为PDF，作用于HOST </td>
</tr><tr>
	<td>String </td>
	<td>resetZoom</td>
	<td></td>
	<td>重置页面缩放等级，作用于HOST </td>
</tr><tr>
	<td>String </td>
	<td>zoomIn</td>
	<td></td>
	<td>缩小页面，作用于HOST </td>
</tr><tr>
	<td>String </td>
	<td>zoomOut</td>
	<td></td>
	<td>放大页面，作用于HOST </td>
</tr><tr>
	<td>String </td>
	<td>show</td>
	<td></td>
	<td>显示窗口 </td>
</tr><tr>
	<td>String </td>
	<td>hide</td>
	<td></td>
	<td>隐藏窗口 </td>
</tr><tr>
	<td>String </td>
	<td>toggleVisible</td>
	<td></td>
	<td>显示/隐藏窗口 </td>
</tr><tr>
	<td>String </td>
	<td>minimize</td>
	<td></td>
	<td>最小化窗口 </td>
</tr><tr>
	<td>String </td>
	<td>maximize</td>
	<td></td>
	<td>最大化窗口 </td>
</tr><tr>
	<td>String </td>
	<td>close</td>
	<td></td>
	<td>关闭窗口 </td>
</tr><tr>
	<td>String </td>
	<td>showDevtools</td>
	<td></td>
	<td>打开调试窗口 </td>
</tr><tr>
	<td>String </td>
	<td>closeDevtools</td>
	<td></td>
	<td>关闭调试窗口 </td>
</tr><tr>
	<td>String </td>
	<td>fullscreen</td>
	<td></td>
	<td>全屏 </td>
</tr><tr>
	<td>String </td>
	<td>nonFullscreen</td>
	<td></td>
	<td>退出全屏 </td>
</tr><tr>
	<td>String </td>
	<td>toggleFullScreen</td>
	<td></td>
	<td>全屏/退出全屏 </td>
</tr><tr>
	<td>String </td>
	<td>quit</td>
	<td></td>
	<td>尝试退出程序 </td>
</tr><tr>
	<td>String </td>
	<td>exit</td>
	<td></td>
	<td>强制退出程序 </td>
</tr>
	</tbody>
</table>


