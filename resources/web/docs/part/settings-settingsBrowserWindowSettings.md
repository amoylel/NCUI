# 浏览器窗口参数

  创建浏览器窗口时可用的属性.<br>说明:**配置参数不能在程序运行过程中修改，只有程序内部函数可以修改这些值。**
  
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
	<td>id</td>
	<td>0 </td>
	<td>窗口ID，由程序设置该值，你不应该修改该值，当通过ID 查找窗口时可以使用该值 . <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>name</td>
	<td>Chromium Embedded Framework (CEF) </td>
	<td>窗口名，当通过名称查找窗口时可以使用该值，如果想要通过名称查找窗口，那么这个值应该被设置成唯一的，默认为Chromium Embedded Framework (CEF) </td>
</tr><tr>
	<td>String</td>
	<td>parent</td>
	<td>NULL </td>
	<td>当前窗口的父窗口，创建窗口时设置.  <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>title</td>
	<td>Chromium Embedded Framework (CEF) </td>
	<td>窗口标题（你可以在任何窗口预览的地方看到该值，如任务栏，任务管理器），默认为Chromium Embedded Framework (CEF),程序会根据页面的title标签内容自己修改该值. </td>
</tr><tr>
	<td>String</td>
	<td>icon</td>
	<td>"" </td>
	<td>窗口图标，默认为空. </td>
</tr><tr>
	<td>Boolean</td>
	<td>titleBar</td>
	<td>false </td>
	<td>是否显示标题栏. </td>
</tr><tr>
	<td>String</td>
	<td>url</td>
	<td>chrome://version </td>
	<td>需要打开的网页地址. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>sizebox</td>
	<td>2,2,2,2 </td>
	<td>可以被拖动改变窗口大小的边框距. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>roundcorner</td>
	<td>0 </td>
	<td>窗口圆角. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>windowClass</td>
	<td>BrowserWindow </td>
	<td>窗口类名. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>modal</td>
	<td>false </td>
	<td>当前窗口是否为模态窗口，如果是那么在当前窗口之后所创建的窗口都是模态窗口，不管有没有指定modal=true . <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>show</td>
	<td>true </td>
	<td>创建时是否显示窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>center</td>
	<td>true </td>
	<td>创建时窗口是否居中. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>adjustPos</td>
	<td>true </td>
	<td>是否允许调整窗口位置. </td>
</tr><tr>
	<td>Int</td>
	<td>x</td>
	<td>0 </td>
	<td>窗口创建时的X轴坐标, adjustPos = true && center = false时有效. </td>
</tr><tr>
	<td>Int</td>
	<td>y</td>
	<td>0 </td>
	<td>窗口创建时的Y轴坐标, adjustPos = true && center = false时有效. </td>
</tr><tr>
	<td>Int</td>
	<td>width</td>
	<td>1280 </td>
	<td>窗口宽度. </td>
</tr><tr>
	<td>Int</td>
	<td>height</td>
	<td>720 </td>
	<td>窗口高度. </td>
</tr><tr>
	<td>Int</td>
	<td>minWidth</td>
	<td>0 </td>
	<td>窗口最小宽度，为0时不限制. </td>
</tr><tr>
	<td>Int</td>
	<td>minHeight</td>
	<td>0 </td>
	<td>窗口最小宽度，为0时不限制. </td>
</tr><tr>
	<td>Int</td>
	<td>maxWidth</td>
	<td>0 </td>
	<td>窗口最小宽度，为0时不限制. </td>
</tr><tr>
	<td>Int</td>
	<td>maxHeight</td>
	<td>0 </td>
	<td>窗口最小宽度，为0时不限制. </td>
</tr><tr>
	<td>Boolean</td>
	<td>resizable</td>
	<td>true </td>
	<td>是否可以改变窗口大小. </td>
</tr><tr>
	<td>Boolean</td>
	<td>moveable</td>
	<td>true </td>
	<td>是否可以移动窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>minimizable</td>
	<td>true </td>
	<td>是否可以最小化窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>maximizable</td>
	<td>true </td>
	<td>是否要以最大化窗口，如果resizable==false, maximizable会被重置为true, 不管之前传入的是什么值. </td>
</tr><tr>
	<td>Boolean</td>
	<td>fullscreenable</td>
	<td>true </td>
	<td>是否要以全屏窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>closable</td>
	<td>true </td>
	<td>是否要以关闭窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>fullscreen</td>
	<td>false </td>
	<td>是否以全屏的方式启动窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>alwaysOnTop</td>
	<td>false </td>
	<td>是否保持窗口前端显示. </td>
</tr><tr>
	<td>Boolean</td>
	<td>offscreen</td>
	<td>false </td>
	<td>是否使用离屏模式创建窗口. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>accelerator</td>
	<td>false </td>
	<td>是否使用GPU渲染窗口，offscreen=true是有效，需要Drect2D支持. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>transparent</td>
	<td>false </td>
	<td>是否使用透明窗口，offscreen=true时有效. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>windowColor</td>
	<td>0xffffffff </td>
	<td>浏览器控件背景颜色. </td>
</tr><tr>
	<td>Boolean</td>
	<td>hasShadow</td>
	<td>false </td>
	<td>是否显示窗口阴影. </td>
</tr><tr>
	<td>Boolean</td>
	<td>focusable</td>
	<td>true </td>
	<td>是否允许窗口获得焦点. </td>
</tr><tr>
	<td>Boolean</td>
	<td>esc</td>
	<td>false </td>
	<td>是否允许ESC关闭窗口. </td>
</tr><tr>
	<td>Boolean</td>
	<td>back_forword</td>
	<td>false </td>
	<td>是否允许前进后退. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>disable_ime</td>
	<td>false </td>
	<td>是否禁用输入法. </td>
</tr><tr>
	<td>Boolean</td>
	<td>compute_cursor</td>
	<td>false </td>
	<td>计算光标位置，offscreen=true时有效. </td>
</tr><tr>
	<td>String</td>
	<td>primaryColor</td>
	<td>#ff3280fc </td>
	<td>窗口主要颜色（一般为标题栏背景颜色）. </td>
</tr><tr>
	<td>String</td>
	<td>primaryColor2</td>
	<td>#ff3280fc </td>
	<td>窗口主要颜色（一般为标题栏背景颜色2，与bkcolr配合可实现标题栏渐变色）. </td>
</tr><tr>
	<td>Boolean</td>
	<td>main</td>
	<td>false </td>
	<td>当前窗口是否为主窗口.主窗口只能有一个，如果设置为主窗口将取消之前的主窗口设定，新的主窗口会替代原来的主窗口.你可以在程序运行过程中修改该值，但要保证任何时候都最多只能有一个浏览器窗口被标记为main </td>
</tr><tr>
	<td>Boolean</td>
	<td>relad</td>
	<td>false </td>
	<td>是否允许页面刷新. 默认false </td>
</tr><tr>
	<td>Int</td>
	<td>dragBlackList</td>
	<td>0 </td>
	<td>不允许的拖拽操作黑名单,设定后对应类型的拖拽将不被允许。以下是可以使用的值,可以组合使用<br>DRAG_OPERATION_NONE    = 0,<br>DRAG_OPERATION_COPY    = 1,<br>DRAG_OPERATION_LINK    = 2,<br>DRAG_OPERATION_GENERIC = 4,<br>DRAG_OPERATION_PRIVATE = 8,<br>DRAG_OPERATION_MOVE    = 16,<br>DRAG_OPERATION_DELETE  = 32,<br>DRAG_OPERATION_EVERY   = 0xffffffff </td>
</tr><tr>
	<td>JsonArray</td>
	<td>cssList</td>
	<td>[] </td>
	<td>需要注入到页面的样式列表，必须是一个URL。 程序会在主页面加载完成后将里面的文件注入到页面中. </td>
</tr><tr>
	<td>JsonArray</td>
	<td>javascriptList</td>
	<td>[] </td>
	<td>需要注入到页面的Javascript列表，必须是一个URL。 程序会在主页面加载完成后将里面的文件注入到页面中. </td>
</tr>
	</tbody>
</table>


