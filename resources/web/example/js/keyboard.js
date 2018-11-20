(function() {
 

	$(function() {
	  
		include('keyboard', 'mouse');

		console.dir(keyboard);
		$('.adoc').append(`<div class="example code" contenteditable="true">
 
		</div>`);

		
		$('#sayString').on('click', function() {
//			mouse.click(900, 500);
			mouse.moveR(0, -60);
			mouse.click();
			keyboard.sayString(`msdn对此方式的说明为：input_keyboard支持非键盘的输入方式，
		    	例如手写识别或语音识别，通过KEYEVENTF_UNICODE标识，这些方式与键盘（文本）输入别无二致。
		    	如果指定了KEYEVENTF_UNICODE，SendInput发送一个WM_KEYDOWN或WM_KEYUP消息给活动窗口的线程消息队列，
		    	消息的wParam参数为VK_PACKET。GetMessage或PeedMessage一旦获得此消息，
		    	就把它传递给TranslateMessage，TranslateMessage根据wScan中指定的Unicode字符产生一个WM_CHAR消息。
		    	若窗口是ANSI窗口，则Unicode字符会自动转换为相应的ANSI字符。`);

		});
		$('#keyPress').on('click', function() {
			mouse.moveR(0, -60);
			mouse.click();
			keyboard.keyPress(keyboard.VK_MENU, keyboard.VK_F4);

		});
		$('#keyDown').on('click', function() {
			mouse.moveR(0, -60);
			mouse.click();
			keyboard.keyDown('1');

		});
		$('#keyUp').on('click', function() {
			mouse.moveR(0, -60);
			mouse.click();
			keyboard.keyUp('1');

		});
	})
})();