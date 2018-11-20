# 快速开始

  开始使用NCUI.
  
## 编辑配置文件 &nbsp;
  编辑程序所在目录下的manifest.json文件可以自定义设置程序的启动方式。<br>完整的manifest包含三个段，分别为 appSettings， browserWindowSettings， splashWindowSettings;<br>对应可以设置<a href="#settings/settingsAppSettings">启动参数</a>，<a href="#settings/settingsBrowserWindowSettings">启动浏览器窗口参数</a>，<a href="#settings/settingsSplashWindowSettings">程序启动画面</a><br>manifest不是必须的，如果不存在，那么程序将使用默认配置.<br>NCUI共有三个地方保存程序的启动参数，优先级从1到3，分别为:<br>&nbsp;&nbsp;&nbsp;&nbsp;1. manifest.json，可以通过编辑该文件修改启动参数<br>&nbsp;&nbsp;&nbsp;&nbsp;2. exe程序中的资源文件，可以通过编辑exe程序修改启动参数,参考<a href="#extend/extendrcedit">资源编辑</a>、<a href="#basic/basicAppGen">应用生成</a><br>&nbsp;&nbsp;&nbsp;&nbsp;3. 源代码中的默认启动参数，只能通过修改源代码来更改启动参数<br>注意:**manifest.json只支持ANSI编码，其他类型的编码可能不会被正确识别**
  
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip 一个简单的配置文件,只包含启动窗口的参数配置段， 设置程序启时要打开的URL
{
    "browserWindowSettings":{
    "titleBar": true,
    "url": "http://127.0.0.1:8030/example/ManifestWindow.html"
    }
}

```
* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
//@skip 一个包含所有段的的配置文件
{
   "appSettings":{
       "single_process": true,
       "useNode":false
   },
   "browserWindowSettings":{
        "titleBar": true,
        "url": "http://127.0.0.1:8030/example/ManifestWindow.html"
    },
    "splashWindowSettings":{
       "images":"%webDir%images/splash.png"
    }
}

```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;修改manifest.json**

```html
//@skip
includes('Path','FileStream', 'app');
var path = new Path('manifest.json');
path.copyTo('manifest2.json');// 备份manifest.json，你可以再查看效果后手动恢复该文件
var filestream = new FileStream('manifest.json', 2 + 32); // 2 以写入方式打开，32 以二进制方式打开
filestream.write(`
    {
        "appSettings":{
            "single_process": true,
            "useNode":false,
            "urlMappings":[
               {
                   "url":"http://127.0.0.1:8030",
                   "path":"%webDir%"
               }
            ]
        },
        "browserWindowSettings":{
            "titleBar": true,
            "url": "http://127.0.0.1:8030/example/ManifestWindow.html"
        },
        "splashWindowSettings":{
           "images":"%webDir%images/splash.png"
        }
   }`);
filestream.close();
app.restart();

```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;修改manifest.json**

```html
//@skip
include('Path');
var path = new Path('manifest.json');
path.copyTo('manifest2.json');// 备份manifest.json，你可以再查看效果后手动恢复该文件
include('shell');
shell.exec('open', 'notepad', 'manifest.json');
sleep(1000);
include('keyboard');
keyboard.keyPress(keyboard.VK_CONTROL, 'A');
keyboard.sayString(`
{
    "appSettings":{
        "single_process": true,
        "useNode":false,
        "urlMappings":[
            {
                "url":"http://127.0.0.1:8030",
                "path":"%webDir%"
            }
        ]
    },
    "browserWindowSettings":{
            "titleBar": true,
            "url": "http://127.0.0.1:8030/example/ManifestWindow.html"
        },
        "splashWindowSettings":{
        "images":"%webDir%images/splash.png"
        }
}`,10, true);
keyboard.unique('keyboard.sayString', function(){
    sleep(1000);
    keyboard.keyPress(keyboard.VK_CONTROL, 'S');
    keyboard.keyPress(keyboard.VK_MENU, keyboard.VK_F4);
    include('app');
    app.restart();
});


```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;修改manifest.json**

```html
//@skip
include('Path');
var path = new Path('manifest.json');
path.copyTo('manifest2.json');// 备份manifest.json，你可以再查看效果后手动恢复该文件
include('shell');
shell.exec('open', 'notepad', 'manifest.json');
sleep(1000);
includes('keyboard', 'clipboard');
keyboard.keyPress(keyboard.VK_CONTROL, 'A');
clipboard.writeText(`
{
    "appSettings":{
    "single_process": true,
    "useNode":false,
    "urlMappings":[
           {
                "url":"http://127.0.0.1:8030",
                "path":"%webDir%"
           }
        ]
    },
    "browserWindowSettings":{
       "titleBar": true,
       "url": "http://127.0.0.1:8030/example/ManifestWindow.html"
    },
    "splashWindowSettings":{
       "images":"%webDir%images/splash.png"
    }
}`);

   keyboard.keyPress(keyboard.VK_CONTROL, 'V');
sleep(1000);
keyboard.keyPress(keyboard.VK_CONTROL, 'S');
keyboard.keyPress(keyboard.VK_MENU, keyboard.VK_F4);
include('app');
app.restart();



```


<div class="adoc" id="div_编辑配置文件"></div>


## 创建浏览器窗口 &nbsp;
  程序启动时会创建一个默认的浏览器窗口，该窗口的参数可以通过编辑manifest.json文件进行配置.<br>如果需要在页面上创建窗口，可以通过<a href="#api/apiBrowserWindow">BrowserWindow</a>的<a href="#api/apiBrowserWindow/3">构造函数</a>创建一个新的浏览器窗口。<br>除此之外，浏览器默认的窗口创建动作（如:**a标签**、**window.open**等）出来的窗口将使用默认的<a href="#settings/settingsBrowserWindowSettings">浏览器窗口参数</a>,这可能会影响你的正常使用（比如没有标题栏，不能拖动窗口），可以<a href="#api/apiBrowserWindow/0">预设置浏览器窗口参数</a>来避免该问题
  
* **参考** 
<a href="#api/apiBrowserWindow/0">BrowserWindow.addBrowserWindowSettings</a>

* **示例&nbsp;&nbsp;--&nbsp;&nbsp;创建浏览器窗口**

```html
include('BrowserWindow');
var win = new BrowserWindow({
                titleBar: true,
                url: 'http://127.0.0.1:8030/example/BrowserWindow.html'
            });

```


<div class="adoc" id="div_创建浏览器窗口"></div>


## 导入模块 &nbsp;
  如果要使用NCUI提供的模块，那么必须在使用之间前将需要的模块导入到页面中，你可以通过以下方式导入需要的模块。**所有被导入的模块都会被注入到页面的全局变量中，可能会与页面中的变量冲突**<br>使用**include**导入单个模块.<br>使用**includes**导入多个模块.<br>如果内置模块不存在，将会导入renderer_modules文件夹下模块，如果还是不存在会导入browser_modules文件夹下模块.renderer_modules文件夹下的扩展在Renderer线程上运行，browser_modules文件夹下的扩展在UI线程上运行.<br>使用**renderer_modules**导入renderer_modules文件夹下的模块，如果模块存在依赖那么，会使用includes加载依赖模块<br>使用**browser_modules**可以跳过renderer_modules文件夹下的模块，直接导入browser_modules文件夹下的模块，如果模块存在依赖那么，会使用includes加载依赖模块<br>说明:<br>&nbsp;&nbsp;&nbsp;&nbsp;**一个iframe中同一个模块只需要加载一次即可在任意地方使用**<br>&nbsp;&nbsp;&nbsp;&nbsp;**如无特殊需求，都应该使用include 或 inclcudes 加载模块**<br>&nbsp;&nbsp;&nbsp;&nbsp;**尽量不要编写与其他模块名相同的C++扩展模块**<br>&nbsp;&nbsp;&nbsp;&nbsp;**renderer_modules和browser_modules同样可以导入内置模块，但不建议用这两个函数来导入内置模块**<br>
  
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;导入单个模块**

```html
// 导入单个模块
include('BrowserWindow');
BrowserWindow.current.maximize();

```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;导入多个个模块**

```html
// 导入多个个模块
includes('BrowserWindow', 'app');
BrowserWindow.current.maximize();
BrowserWindow.current.restore();
// 尝试退出程序
app.quit();

```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;renderer_modules**

```html
// 导入多个个模块
renderer_modules('BrowserWindow', 'md5');


```
* **示例&nbsp;&nbsp;--&nbsp;&nbsp;browser_modules**

```html
// 导入多个个模块
browser_modules('BrowserWindow', 'TaskTest');
var taskTest = new TaskTest();
console.assert(taskTest.add(1,2) == 3);


```


<div class="adoc" id="div_导入模块"></div>


