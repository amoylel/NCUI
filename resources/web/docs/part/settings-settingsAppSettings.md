# 启动参数

  应用程序启动参数，这些参数大部分不可更改，且只在程序启动前设置才能生效.<br>如果使用NodeJS可以在JS代码里面调用<a href="#api/apiapp/1">app.setConfig</a>设置启动参数.<br>如果未使用NodeJS可以通过程序目录下的manifest.json文件达到同样的效果.<br>说明:**配置参数不能在程序运行过程中修改，只有程序内部函数可以修改这些值。**
  
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
	<td>locales</td>
	<td>%appDir%locales </td>
	<td>语言文件所在目录   <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>single_process</td>
	<td>true </td>
	<td>是否使用单进程模式，如果程序比较复杂，应该尽量使用多进程模式,cef3440及以上不再支持单进程模式，该字段失效 <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>resources_dir_path</td>
	<td>%appDir% </td>
	<td>资源文件所在目录 <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>cache_path</td>
	<td>%APPDATA%appName/cache </td>
	<td>CEF缓存文件所在目录 <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>locale</td>
	<td>zh-CN </td>
	<td>CEF语言环境 <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>manifest</td>
	<td>true </td>
	<td>是否允许外部配置文件manifest.json；如果要使用，该文件必须与应用程序所在目录相同且只能使用ANSI编码， 不能用UTF8, 程序会将编码转换为UTF8 <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>appID</td>
	<td>783a02fd-b493-45ad-aa7f-ddbefeec1122 </td>
	<td>程序唯一ID，默认为NCUI的程序ID. 其他程序应该修改该值，否则会影响单例模式的使用（appID相同的程序会被判定为同一个程序程序）. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>singleInstance</td>
	<td>false </td>
	<td>单例模式,该模式下只允许一个实例运行，如果尝试启动多个程序将不会成功 </td>
</tr><tr>
	<td>Boolean</td>
	<td>showSplash</td>
	<td>false </td>
	<td>是否显示启动画面.如果需要启动画面，需要修改配置参数<a href="#settings/settingsSplashWindowSettings">启动画面</a> </td>
</tr><tr>
	<td>Boolean</td>
	<td>useNode</td>
	<td>false </td>
	<td>是否使用NodeJs. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>useNodeProcess</td>
	<td>false </td>
	<td>是否在单独的进程中使用NodeJs.当useNode=true时该字段生效，如果在单独的进程中使用NodeJs,那么NCUI和Node将会在不同地进程运行，并通过通道进行消息同步  <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Int</td>
	<td>ipcTimeout</td>
	<td>0 </td>
	<td>进程通信的超时时间，如果超过这个时间没有得到返回结果，该消息将被丢弃。可以用来解决部分死锁问题。默认为0（永不超时）. </td>
</tr><tr>
	<td>String</td>
	<td>main</td>
	<td>main.js </td>
	<td>NodeJs所要调用的JS文件所在位置. useNode为true 时，该值有效 <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Boolean</td>
	<td>debugNode</td>
	<td>false </td>
	<td>是否允许调试NodeJs.如果未在单进行上运行Node时要调试NodeJs 需要使用NCUI-DEV.exe,即必须在命令行模式下运行NCUI(会相对于正常的图形界面多出一个命令行窗口) <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String </td>
	<td>appPath</td>
	<td></td>
	<td>程序在磁盘上的完整路径. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String </td>
	<td>appDir</td>
	<td></td>
	<td>程序所在目录. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String </td>
	<td>appName</td>
	<td></td>
	<td>程序名称，不包含目录，不包含扩展名. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>skinDir</td>
	<td>%appDir%skin </td>
	<td>Duilib skin 目录. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>workDir</td>
	<td>%appDir% </td>
	<td>工作目录,默认与程序所有目录相同. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String</td>
	<td>webDir</td>
	<td>%appDir%web </td>
	<td>html 源代码目录. <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String </td>
	<td>homeDir</td>
	<td></td>
	<td>储存应用程序设置文件的文件夹，默认是 "我的文档" 文件夹附加应用的名称. </td>
</tr><tr>
	<td>String</td>
	<td>downloadsDir</td>
	<td>%homeDir%downloads </td>
	<td>用户下载目录的路径. </td>
</tr><tr>
	<td>String</td>
	<td>musicsDir</td>
	<td>%homeDir%musics </td>
	<td>用户音乐目录的路径. </td>
</tr><tr>
	<td>String</td>
	<td>picturesDir</td>
	<td>%homeDir%pictures </td>
	<td>用户图片目录的路径. </td>
</tr><tr>
	<td>String</td>
	<td>videosDir</td>
	<td>%homeDir%videos </td>
	<td>用户视频目录的路径. </td>
</tr><tr>
	<td>String</td>
	<td>dragClassName</td>
	<td>drag </td>
	<td>拖拽移动窗口的元素类名. </td>
</tr><tr>
	<td>String</td>
	<td>noDragClassName</td>
	<td>no-drag </td>
	<td>禁止窗口元素类名. </td>
</tr><tr>
	<td>String</td>
	<td>appDataDir</td>
	<td>%APPDATA% </td>
	<td>当前用户的应用数据文件夹. </td>
</tr><tr>
	<td>String</td>
	<td>temp</td>
	<td>%TEMP% </td>
	<td>临时文件夹.  <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String </td>
	<td>desktop</td>
	<td></td>
	<td>当前用户的桌面文件夹.  <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>String </td>
	<td>documents</td>
	<td></td>
	<td>用户文档目录的路径.  <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>Int </td>
	<td>startTime</td>
	<td></td>
	<td>启动时间（本地时间戳ms）. </td>
</tr><tr>
	<td>Boolean</td>
	<td>debugMode</td>
	<td>true </td>
	<td>是否以调试模式运行,默认true.该模式下F12可以打开调试窗口，F5可以刷新页面，并会输出日志 </td>
</tr><tr>
	<td>Boolean</td>
	<td>clearCache</td>
	<td>false </td>
	<td>是否在应用程序启动时清除浏览器缓存（调试用）.  <span class="label label-const">常量</span> 
</td>
</tr><tr>
	<td>JsonArray</td>
	<td>nonGlobalModules</td>
	<td>[] </td>
	<td>禁止被导出到全局变量的类列表. </td>
</tr><tr>
	<td>Boolean</td>
	<td>dump</td>
	<td>false </td>
	<td>是否在崩溃时创建dump文件. </td>
</tr>
	</tbody>
</table>


