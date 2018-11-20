# 事件驱动编程

  你可以使用事件驱动编程防止NCUI污染你的页面.
  
## 监听事件 &nbsp;
  使用document.addEventListener监听一个自定义事件.NCUI会将ipc.dispatchEvent发送的消息重定向到document上
  
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
document.addEventListener('NCUI.TEST', function(event){
    console.log(arguments);
});
// 下面的代码可以写到Node.js里面去，这里只做演示
include('ipc');
ipc.dispatchEvent('NCUI.TEST', {name:3, value:'2323'});


```


<div class="adoc" id="div_监听事件"></div>


## 发送事件 &nbsp;
  NCUI允许你通过document.dispatchEvent向Node.js或其他页面发送自定消息.<br>1. "ipc.exec", 通过<a href="#api/apiipc/0">ipc.exec</a>向Node.js或其他页面发送消息，使用ipc.on或ipc.unique可以监听该消息<br>2. "ipc.dispatchEvent", 通过<a href="#api/apiipc/3">ipc.dispatchEvent</a>他页面的document对象发送消息，使用document.addEventListener可以监听该消息，Node.js代码无法接收该消息<br>说明:**必须在CustomEvent的detail对象中增加eventName字段表明需要触发的事件名称**
  
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
// 你可以将ipc.unique写到Node.js代码中,或编辑manifest.json，开启Node.js支持，并以调试模块运行NCUI-DEV.exe查看效果
include('ipc');
ipc.unique('NCUI.TEST2', function(){
    console.log('NCUI.TEST2');
    console.log(arguments);
});

document.addEventListener('NCUI.TEST3', function(event){
    console.log('NCUI.TEST3');
    console.log(arguments);
});

document.dispatchEvent(new CustomEvent('ipc.exec', {
        detail: {
            eventName: 'NCUI.TEST2',
            value: 3232
        }
    }));

document.dispatchEvent(new CustomEvent('ipc.dispatchEvent', {
        detail: {
            eventName: 'NCUI.TEST3',
            value: 3232,
            path: 'D:/'
        }
    }));


```


<div class="adoc" id="div_发送事件"></div>


