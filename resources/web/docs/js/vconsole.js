(function(){
	$(function(){
		window.console.assert = function(condition, outstr){
			if(!condition){
				if(window.assertArr && window.assertArr.length >0){
					var str = window.assertArr.shift();
					str = str.replace('console.assert', '');
					str= str.replace(';', '');
					console.error(window.assertFuncName + ': 失败[ ' + str +  ' ]');
					
				}
				else{
					console.error(window.assertFuncName + ': 失败[ ]');
				}
				
			}else{
				if(window.assertArr && window.assertArr.length >0){
					var str = window.assertArr.shift();
//					console.log(str);
					str = str.replace('console.assert', '');
					str= str.replace(';', '');
					console.info(window.assertFuncName + ': 通过[ ' + str +  ' ]'); 
				}
				else{
					console.info(window.assertFuncName + ': 通过[ ]');
				}
				 
			}
		}
	});
})();
