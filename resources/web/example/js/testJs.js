;(function(){
	$(function(){
		setTimeout(function(){
			include('BrowserWindow');
			BrowserWindow.current.close();
		}, 4000);
		
	})
})();
