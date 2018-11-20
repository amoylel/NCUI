+ function($) {
	'use strict';

	// PAGER PUBLIC CLASS DEFINITION
	// ===============================

	var Tester = function(options) {
		this.type = null;
		this.options = null;
		this.enabled = null;
		this.arr = [];
		this.init('tester', options);

	}

	Tester.DEFAULTS = {

	}

	// 初始化控件
	Tester.prototype.init = function(type, options) {
		this.enabled = true;
		this.type = type;
		this.arr = [];

		$(document).on('tester.run', $.proxy(this.run, this));
		$(document).on('tester.next', $.proxy(this.next, this));
		$(document).on('tester.finish', $.proxy(this.finish, this));
		$(document).on('tester.continue', $.proxy(this.finish, this));
		$(document).on('tester.skip', $.proxy(this.finish, this));

		var self = this;
		$('code.lang-html').each(function(index, element) {
			var $element = $(element);
			var src = $element.find('span').html();
			var index = src.indexOf('@skip');
			var strFuncName = $element.parents('article').find('.adoc').attr('id');
			if(!strFuncName) strFuncName = '';
			strFuncName = strFuncName.replace('div_', '');
			var info = {
				skip: false,
				src: src,
				funcName: strFuncName
			}
			if(index != -1) {
				info.skip = true;
			}

			self.arr.push(info);
		})

	}

	Tester.prototype.run = function() {

		if(!this.arr || this.arr.length == 0) {
			$(document).trigger('tester.finish');
			return;
		}

		var info = this.arr[0];
		//		if(info.skip) {
		//			Popup.show({
		//				title: '是否跳过当前测试?',
		//				text: '是否跳过当前测试',
		//				fnBtnFirst: $.proxy(this.skip, this),
		//				fnBtnSecond: $.proxy(this.runImpl, this),
		//			});
		//			return;
		//		} else {
		//			this.runImpl();
		//		}
		this.runImpl();
	}

	Tester.prototype.runImpl = function() {
		Popup.close();
		if(!this.arr || this.arr.length == 0) {
			$(document).trigger('tester.finish');
			return;
		}
		var info = this.arr.shift();
		if(info.skip) {
			console.log('skip: --->\n' + info.src);
		} else {
			var src = info.src;

			window.assertArr = src.match(/console.assert\(.+?\);/g);
			window.assertFuncName = info.funcName;
			if(!window.assertArr) window.assertArr = [];
			if(!window.assertFuncName) window.assertFuncName = '';
			var js = '(function(){' + src + '\n})();';
			eval(js);
		}

		this.triggerNext();

	}

	Tester.prototype.triggerNext = function() {
		setTimeout(function() {
			$(document).trigger('tester.next');
		}, 300);
	}

	Tester.prototype.next = function() {
		this.run();
	}

	Tester.prototype.skip = function() {
		Popup.close();
		this.arr.shift();
		this.triggerNext();
	}

	Tester.prototype.finish = function() {
		Popup.close();
	}

	// 获取默认参数
	Tester.prototype.getDefaults = function() {
		return Tester.DEFAULTS
	}

	// 合并参数
	Tester.prototype.getOptions = function(options) {
		options = $.extend({}, this.getDefaults(), this.$element.data(), options);
		return options;
	}

	// 导出为全局变量
	window.Tester = Tester;

}(window.jQuery);