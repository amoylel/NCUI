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

app.addUrlMapping('http://127.0.0.1:8020/', 'web/');
tray.show(true);

function fooo() {
	var sb = JSON.stringify(cc);
	return sb;
}
console.log(shell.SW_HIDE);
console.log(shell.SW_RESTORE);
//shell.open("www.baidu.com");

const win = new BrowserWindow({
	show: true,
	hasShadow: true,
	titleBar: false,
	offscreen: false,
	transparent: false,
	width: 1280,
	height: 720,
	roundcorner: 5,
	main:true,
	sizebox: "4,4,6,6",
	url: "http://127.0.0.1:8020/index.html",
	titleBar: true
});

win.on('maximize', function() {
	tray.blink(true);
	console.log(333);
});
win.on('OnAfterCreated', function(browser) {
	//  tray.blink(true);
	var bc = browser.CanGoBack();
});

tray.on('rbuttondown', function() {
	console.log('rbuttondown');

	var menu = new Menu({
		lineHeight: 32,
		roundcorner: 0,
		menu: [
			{ id: '1', text: '全屏', action: 'fullscreen', separator: true },
			{ id: '2', text: '退出全屏', action: 'nonFullscreen' },
			{ id: '3', text: '全选', action: 'selectAll' },
			{ id: '3', text: '重新加载', action: 'forceReload' },
			{ id: '4', text: '调试', action: 'showDevtools' },
			{ id: '5', text: '退出程序', action: 'quit' }
		]
	});
	menu.on('close', function() {
		//      dialog.alert(333);
	});
	menu.on('click', function() {
		//      dialog.alert(0904);
	});
	menu.prop2;
	tray.blink(!tray.isBlink());
	console.log(1);
	//  console.log(menu);
	console.log(2);
});

//console.log(33);
//tray.blink(true);
//globalShortcut.on('hotkeydown', function(id, str){
////          alert(id +', ' + str);  
//      });
//       globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+O');
//      $('#registerShortcut').on('click', function() {
//         
//      });
//
//      $('#unregisterShortcut').on('click', function() {
//         globalShortcut.unregisterShortcut('MOD_CONTROL+MOD_ALT+O');
//      });
//shell.open('http://www.baidu.com');
//win.maximize();

var urlRequest = new URLRequest();
urlRequest.on('RequestComplete', function() {
	//              shell.open('http://www.baidu.com');
})

win.on('OnLoadEnd', function() {
	win.send('abcd', 66);

})

// ipc
ipc.on('message', function() {
	tray.blink(!tray.isBlink());
	return 3;
})

// system
console.dir(system.screen);
console.dir(system.mouse);

//mouse.moveTo(100, 100);