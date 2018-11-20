var formatJson = function(json, options) {
	var reg = null,
		formatted = '',
		pad = 0,
		PADDING = '    ';
	options = options || {};
	options.newlineAfterColonIfBeforeBraceOrBracket = (options.newlineAfterColonIfBeforeBraceOrBracket === true) ? true : false;
	options.spaceAfterColon = (options.spaceAfterColon === false) ? false : true;
	if(typeof json !== 'string') {
		json = JSON.stringify(json);
	} else {
		json = JSON.parse(json);
		json = JSON.stringify(json);
	}
	reg = /([\{\}])/g;
	json = json.replace(reg, '\r\n$1\r\n');
	reg = /([\[\]])/g;
	json = json.replace(reg, '\r\n$1\r\n');
	reg = /(\,")/g;
	json = json.replace(reg, ',\r\n"');
	reg = /(\r\n\r\n)/g;
	json = json.replace(reg, '\r\n');
	reg = /\r\n\,/g;
	json = json.replace(reg, ',');
	if(!options.newlineAfterColonIfBeforeBraceOrBracket) {
		reg = /\:\r\n\{/g;
		json = json.replace(reg, ':{');
		reg = /\:\r\n\[/g;
		json = json.replace(reg, ':[');
	}
	if(options.spaceAfterColon) {
		reg = /\:/g;
		json = json.replace(reg, ':');
	}
	(json.split('\r\n')).forEach(function(node, index) {
		var i = 0,
			indent = 0,
			padding = '';

		if(node.match(/\{$/) || node.match(/\[$/)) {
			indent = 1;
		} else if(node.match(/\}/) || node.match(/\]/)) {
			if(pad !== 0) {
				pad -= 1;
			}
		} else {
			indent = 0;
		}

		for(i = 0; i < pad; i++) {
			padding += PADDING;
		}

		formatted += padding + node + '\r\n';
		pad += indent;
	});
	return formatted;
};

(function() {

	include('rcedit');
	var settings = rcedit.getDefaultSettings();
	var strSettings = JSON.stringify(settings);
	var resultJson = formatJson(strSettings);
	$('#appGenExample').empty().append('<pre style="background: none; border: none; padding: 0;font-size: 14px; line-height: 20px; color:#d14; ">' + resultJson + '</pre>');
	// 右键菜单
	include('Menu');
	var example = $('#appGenExample'); //document.getElementById('appGenExample');
	console.dir(example);

	example[0].oncontextmenu = function() {
		var menu = new Menu({
			menu: [{
					id: '1',
					text: '格式化',
					shortcut: 'F'
				},
				{
					id: '2',
					text: '生成应用',
					shortcut: 'R'
				}
			]
		});
		menu.on('select', function(item) {
			if(item.id == '1') { 
				var txt = $('#appGenExample').text(); 
				var resultJson = formatJson(txt); 
				$('#appGenExample').empty().append('<pre style="background: none; border: none; padding: 0;font-size: 14px; line-height: 20px; color:#d14; ">' + resultJson + '</pre>');
				  
			} else if(item.id == '2') {
				var txt = $('#appGenExample').text();
				console.log(txt);
				var json = JSON.parse(txt);
				console.log(json);
				var fileSettings = json['fileSettings'];
				var appSettings = json['appSettings'];
				var browserWindowSettings = json['browserWindowSettings'];
				var splashWindowSettings = json['splashWindowSettings'];
				console.log(fileSettings);
				console.log(appSettings);
				console.log(browserWindowSettings);
				console.log(splashWindowSettings);
				rcedit.commit(fileSettings, appSettings, browserWindowSettings, splashWindowSettings);
			}
		})

		return false;
	};
})();