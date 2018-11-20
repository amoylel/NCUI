# Menu

  弹出菜单.<br>工作线程:**UI线程**.
  
* **父类** 
<a href="#api/apiObject">Object</a>&nbsp;

## 事件
### **1.select** 

  菜单项被选中时触发.

* **事件参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>JsonObject </td>
	<td>被选中的菜单项.</td>
</tr>
	</tbody>
</table>

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Menu');
var menu = new Menu({
    id: '2',
    text: '222',
    menu: [
    { id: '1', text: '111' },
    { id: '2', text: '222' },
    { id: '3', text: '333' },
    { id: '4', text: '444' }
    ]
});
menu.on('select', function(){
    console.log(arguments);
});

```


<div class="adoc" id="div_select"></div>


## Menu &nbsp;<span class="label label-constructor">构造</span> 

  创建一个菜单.菜单创建的位置默认为鼠标所在位置，菜单失去焦点会自动关闭.
  
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
	<td>创建菜单所用到的参数<br>[**Int**]		lineHeight 菜单项行高，默认为32<br>[**String**]	separatorColor 分隔条颜色，默认"0xffe8e8e8"<br>[**JsonObject**] menu 所有菜单项,具体内容参考:<br>&nbsp;&nbsp;&nbsp;&nbsp;[**String**]	id 当前菜单项ID,一个菜单里面的菜单项ID应该是唯一的<br>&nbsp;&nbsp;&nbsp;&nbsp;[**String**]	text 当前菜单项所显示的文本<br>&nbsp;&nbsp;&nbsp;&nbsp;[**String**]	String 当前菜单项的预定义动作，如果有效则执且不会向页面返回菜单选择事件<br>&nbsp;&nbsp;&nbsp;&nbsp;[**Boolean**]	separator 是否显示分隔符，默认false<br>&nbsp;&nbsp;&nbsp;&nbsp;[**Boolean**]	visible 当前菜单项是否可见，默认true<br>&nbsp;&nbsp;&nbsp;&nbsp;[**Boolean**]	enabled 当前菜单项是否可用，默认true<br>&nbsp;&nbsp;&nbsp;&nbsp;[**String**]	shortcut 当前菜单项快捷键，默认无快捷键<br>&nbsp;&nbsp;&nbsp;&nbsp;[**String**]	icon 当前菜单项图标，显示在文字前面，默认无，必须为skin目录下的文件<br>&nbsp;&nbsp;&nbsp;&nbsp;[**String**]	selected 当前菜单项选中状态，支持"checkbox"和"radio",**该字段与上面的icon字段冲突，并且优先级高于icon字段**，默认无<br>&nbsp;&nbsp;&nbsp;&nbsp;[**JsonObject**] menu 子菜单，与上面的内容一样，可以不填</td>
</tr>
	</tbody>
</table>

* **返回值**
  MenuObject Menu对象 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 创建一个普通菜单
include('Menu');
var menu = new Menu({
    lineHeight: 32,
    backgroundColor: 0xFFFFFFFF,
    roundcorner: 3,
    menu: [
        { id: '1', text: '中文', 'separator': true },
        { id: '2', text: '英文' },
        { id: '3', text: '法文' },
        { id: '4', text: '德文' }
    ]
});

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 可以使用快捷键的菜单
include('Menu');
var menu = new Menu({
    lineHeight: 32,
    backgroundColor: 0xFFFFFFFF,
    roundcorner: 3,
    menu: [
        { id: '1', text: '中文', 'separator': true, icon: 'huiyuan.png' },
        { id: '2', text: '英文', 'shortcut': 'D', selected: 'checkbox' },
        { id: '3', text: '法文', 'shortcut': 'E', selected: true  },
        { id: '4', text: '德文', 'shortcut': 'F', selected: 'radio' },
        { id: '5', text: '俄文', 'shortcut': 'G', icon: 'pick.png' }
    ]
});

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 带action的菜单
include('Menu');
var menu = new Menu({
    lineHeight: 32,
    backgroundColor: 0xFFFFFFFF,
    roundcorner: 3,
    menu: [
        { id: '1', text: '中文', 'separator': true },
        { id: '2', text: '关闭窗口', 'shortcut': 'D', action:'close' },
        { id: '3', text: '法文', 'shortcut': 'E'  },
        { id: '4', text: '德文', 'shortcut': 'F'  }
    ]
});
menu.on('select', function(item) {
    alert('触发菜单:' + item.text);
})

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 多层级菜单
include('Menu');
var menu = new Menu({
   transparent: false,
   separatorColor: '0xffff0000',
   menu: [{
           id: '1',
           text: '111',
           'separator': true,
           menu: [
               { id: '1', text: '中就让这首歌', 'separator': true },
               { id: '2', text: '222' },
               { id: '3', text: '333', shortcut: 'd' },
               {
                   id: '4',
                   text: '444',
                   menu: [
                       { id: '1', text: '111', 'separator': true },
                       { id: '2', text: '222' },
                       { id: '3', text: '333' }
                   ]
               }
           ]
       },
       {
           id: '2',
           text: '222',
           menu: [
               { id: '1', text: '111' },
               { id: '2', text: '222' },
               { id: '3', text: '333' },
               { id: '4', text: '444' }
           ]
       },
       { id: '3', text: '333' },
       { id: '4', text: '444' }
   ]
});
menu.on('select', function(item) {
   console.log(item);
})

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 右键菜单
include('Menu');
document.oncontextmenu = function() {
    var menu = new Menu({
        menu: [
            { id: '1', text: '111' },
            { id: '2', text: '222' },
            { id: '3', text: '333' },
            {
                id: '4',
                text: '444',
                menu: [
                    { id: '1', text: '111' },
                    { id: '2', text: '222' },
                    { id: '3', text: '333' },
                    { id: '4', text: '444' }
                ]
            }
        ]
    });
    menu.on('select', function(item) {
        alert('触发菜单:' + item.text);
    })
    return false;
};

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 右键菜单
include('Menu');
var example = document.getElementById('example');
example.oncontextmenu = function() {
    var menu = new Menu({
        menu: [
           { id: '1', text: '撤消', action: 'undo', shortcut: 'U' },
           { id: '2', text: '重做', action: 'redo', shortcut: 'R', separator: true },
           { id: '3', text: '剪切', action: 'cut', shortcut: 'T', enabled: false },
           { id: '4', text: '复制', action: 'copy', shortcut: 'C', visible: false },
           { id: '5', text: '粘贴', action: 'paste', shortcut: 'P', visible: false },
           { id: '6', text: '删除', action: 'delete', shortcut: 'D', separator: true },
           { id: '7', text: '全选', action: 'selectAll', shortcut: 'A', separator: true },
           { id: '8', text: '放大', action: 'zoomOut', shortcut: 'Z' },
           { id: '9', text: '缩小', action: 'zoomIn', shortcut: 'X' },
           { id: '10', text: '重置', action: 'resetZoom', separator: true },
           { id: '11', text: '打印', action: 'print' },
        ]
    });
    menu.on('select', function(item) {
       alert('触发菜单:' + item.text);
    })
    return false;
};

```


<div class="adoc" id="div_Menu"><div id="example" class="example code" contenteditable="true"> 右键点我</div></div>


## close &nbsp;
  关闭菜单.
  
* **函数参数**  无

* **返回值**
   无. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Menu');
var menu = new Menu({
    menu: [
            { id: '1', text: '111' },
            { id: '2', text: '222' },
            { id: '3', text: '333' },
            {
                id: '4',
                text: '444',
                menu: [
                    { id: '1', text: '111' },
                    { id: '2', text: '222' },
                    { id: '3', text: '333' },
                    { id: '4', text: '444' }
                ]
            }
        ]
});
// 如果没有操作，5秒后菜单会自动关闭
setTimeout(function(){
console.log(123);
    menu.close();
}, 5000);

```


<div class="adoc" id="div_close"></div>


