// Created by amoylel on 08/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_USENODEJS_A7E11AE8_5D40_48BA_BDBD_BD276B84F3E0_HPP__
#define AMO_USENODEJS_A7E11AE8_5D40_48BA_BDBD_BD276B84F3E0_HPP__

namespace amo {

    /*!
     * @class	使用Node
     *
     * @id basicUserNodeJs
     *
     * @chapter	basic
     *
     * @brief	NCUI支持使用Node.js控制程序，但目前只针对4.7.1进行过开发，更高或更低的版本可能并不支持.
     * 			你可以在Node.js中使用文档api部分所提供的绝大部分接口，其他类型的接口不被支持.
     */
    
    class UserNodeJs {
    public:
    
        /*!
         * @section 编辑配置文件
         *
         * @brief	通过编辑manifest.json可以使程序获得Node.js能力.<br>
         * 			你可以编辑manifest.json文件(useNode=true)来获取Node.js支持.默认情况下Node.js会注入到NCUI中，并在单独的线程中运行.<br>
         * 			你可以编辑manifest.json文件(useNodeProcess=true)使Node.js在单独的进程中执行.<br>
         *
         * @example
         *
        	```
        			//@skip 直接在NCUI中使用Node.js
        			{
        				"appSettings":{
        					useNode: true,
        					useNodeProcess: false,
        					"main": "main.js"
        				}
        			}
        
        			//@skip 在单独的进程中使用Node.js
        			{
        				"appSettings":{
        					"useNode": true,
        					"useNodeProcess": true,
        					"main": "main.js"
        				}
        			}
        	```
         */
        
        void foo1() {
        
        }
        
        /*!
         * @section	编写Node.js代码
         *
         * @brief	编写manifest.json中"appSettings"中"main"字段所指定的文件，为Node.js提供入口.<br>
         * 			NCUI通过Node 扩展的方式提供文档中的所有的内置接口，只需要require(process.cwd() + '/ncui')即可使用，不需要使用include之类的函数加载模块.
         * 			同Node.js代码不能使用renderer_modules或者browser_modules等文件夹下面的外部扩展.<br>
         * 			注意:**图形界面下NCUI需要重定向Node.js的控制台输出，否则将无法加载ncui.node插件**
         *
         * @example main.js
         *
         ```
        	'use strict';
        	var console = {};
        	console.log = function() {}
        	console.info = function() {}
        	console.warn = function() {}
        	console.error = function() {}
        	console.dir = function() {}
        	console.time = function() {}
        	console.timeEnd = function() {}
        	console.trace = function() {}
        	console.assert = function() {}
        	require(process.cwd() + '/ncui');
        	app.addUrlMapping('http://127.0.0.1:8030/', 'web/');
        	tray.show(true);
        	const win = new BrowserWindow({
        		show: true,
        		hasShadow: true,
        		titleBar: false,
        		offscreen: false,
        		transparent: false,
        		width: 1280,
        		height: 720,
        		roundcorner: 5,
        		sizebox: "4,4,6,6",
        		url: "http://127.0.0.1:8030/index.html",
        		titleBar: false
        	});
         ```
         */
        
        void foo2() {
        
        }
        
        /*!
         * @section 调试Node.js
         *
         * @brief	正式的NCUI程序不支持调试Node.js，但你可以通过以下方式来调试Node.js代码<br>
         * 			&nbsp;&nbsp;&nbsp;&nbsp;1.编辑manifest.json，使Node.js在单独的进程中运行:{"useNode": true, "useNodeProcess": true}<br>
         * 			&nbsp;&nbsp;&nbsp;&nbsp;2.编编Node.js的入口文件，加载C++扩展 require(process.cwd() + '/ncui-dev').<br>
         * 			&nbsp;&nbsp;&nbsp;&nbsp;3.开启node-insepector服务.<br>
         * 			&nbsp;&nbsp;&nbsp;&nbsp;3.运行NCUI-DEV.exe即可在浏览器中调试Node.js代码.<br>
         * 			说明:**NCUI-DEV.exe为控制台程序，不需要重定向Node.js的控制台输出.**
         * 			说明:**NCUI-DEV.exe启动时任务栏图标不能正确显示.**
         * @example
         *
         ```
        	'use strict';
        	require(process.cwd() + '/ncui-dev');
        	app.addUrlMapping('http://127.0.0.1:8030/', 'web/');
        	tray.show(true);
        	const win = new BrowserWindow({
        		show: true,
        		hasShadow: true,
        		titleBar: false,
        		offscreen: false,
        		transparent: false,
        		width: 1280,
        		height: 720,
        		roundcorner: 5,
        		sizebox: "4,4,6,6",
        		url: "http://127.0.0.1:8030/index.html",
        		titleBar: false
        	});
         ```
         *
         */
        
        void foo3() {
        
        }
        
        /*!
         * @section	调试标志
         *
         * @brief	调试模式下的Node.js会加入命令行参数“--ncui-debug”，你可以通过该参数判断Node.js是否处于调试状态，来加载正确的c++扩展.
         * @example
         *
         ```
        	    'use strict';
        
        		var debugMode = false;
        		process.argv.forEach((val, index) => {
        			if(val == '--ncui-debug') {
        				debugMode = true;
        			}
        		});
        
        		var console = {};
        		if(!debugMode) {
        			console.log = function() {}
        			console.info = function() {}
        			console.warn = function() {}
        			console.error = function() {}
        			console.dir = function() {}
        			console.time = function() {}
        			console.timeEnd = function() {}
        			console.trace = function() {}
        			console.assert = function() {}
        			require(process.cwd() + '/ncui');
        		} else {
        			console = global.console;
        			require(process.cwd() + '/ncui-dev');
        		}
        
        		app.addUrlMapping('http://127.0.0.1:8030/', 'web/');
        		tray.show(true);
        		const win = new BrowserWindow({
        			show: true,
        			hasShadow: true,
        			titleBar: false,
        			offscreen: false,
        			transparent: false,
        			width: 1280,
        			height: 720,
        			roundcorner: 5,
        			sizebox: "4,4,6,6",
        			url: "http://127.0.0.1:8030/index.html",
        			titleBar: false
        		});
         ```
         */
        
        void foo4() {
        
        }
    };
}

#endif // AMO_USENODEJS_A7E11AE8_5D40_48BA_BDBD_BD276B84F3E0_HPP__

