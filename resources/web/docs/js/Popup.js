var dialogHtml = `<div id="ddddd" class="" style="width: 100%; padding: 0px;">

			<body>
				<div class="header  ">
					<div class="flex-row">
						<div class="flex-column flex-top" style="height: 40px;">
							<img class="flex-middle" src="images/logo01.png" style="width: 40px; height: 40px; padding: 8px; box-sizing: border-box;" />
							<p style="line-height: 40px; color: white;">{title}</p>
							<div class="flex-item-6 flex-right " style="width: 40px;">
								<input type="button" id="closedialog" class="closedialog no-drag" />
							</div>
						</div>
					</div>
				</div>
				<div class="container " style="margin-top: 20px;">
					<form class="form-horizontal">
						<div class="form-group">
							<div class="col-xs-2">
							</div>
							<div class="col-xs-10">
								<h4 class="text-left">{text}</h4>
							</div>

						</div>

						<div class="form-group">
							<div class="col-xs-2">
							</div>
							<div class="col-xs-2 div-left hidden">
							</div>
							<div class=" col-xs-4">
								<button type="button" class="btn btn-primary btn-block no-drag" id="btnFirst">{btnFirst}</button>
							</div>
							<div class=" col-xs-4">
								<button type="button" class="btn btn-outline-primary  btn-block no-drag" id="btnSecond">{btnSecond}</button>
							</div>
							<div class="col-xs-2 div-right hidden">
							</div>
							<div class="col-xs-2">
							</div>
						</div>
						<p id="msg" style="color: red;"></p>
					</form>
				</div>
			</body>
		</div>
`;

var Popup = function() {

}

Popup.dialog = null;

Popup.show = function(options) {
	Popup.close();
	if(!options) options = {};

	// 默认的显示参数
	var defaultOptions = {
		width: 330,
		className: 'modal-ex',
		backdrop: false,
		showHeader: false,
		btnFirst: '确定',
		btnSecond: '取消',
		text: '关闭窗口?',
		title: '提示',
		showFirstBtn: true,
		showSecondBtn: true,
		keyboard: false,
		fnBtnFirst: Popup.close,
		fnBtnSecond: Popup.close,
		fnBtnClose: Popup.close

	};
	options = $.extend({}, defaultOptions, options);
	var html = dialogHtml.format(options);
	options.custom = html;

	//	console.log(dialogHtml);
	this.dialog = (new $.zui.ModalTrigger(options));

	this.dialog.show({
		onShow: function() {
			//  alert('对话框已显示。');
			
			if(options.fnBtnSecond == Popup.close){
				options.showSecondBtn = false;
			}
			
			if(!options.showFirstBtn) {
				$('#btnFirst').hide();
				$('.div-left').removeClass('hidden');
				$('.div-right').removeClass('hidden');
			}
			if(!options.showSecondBtn) {
				$('#btnSecond').hide();
				$('.div-left').removeClass('hidden');
				$('.div-right').removeClass('hidden');
			}

			// 清理原有的事件绑定
			$('#btnFirst, #btnSecond, .closedialog').unbind();

			if(options.fnBtnFirst) {
				$('#btnFirst').on('click', options.fnBtnFirst);
			}
			if(options.fnBtnSecond) {
				$('#btnSecond').on('click', options.fnBtnSecond);
			}
			if(options.fnBtnClose) {
				$('#closedialog').on('click', options.fnBtnClose);
			}
 
		}
	});
	return this.dialog;
}

Popup.close = function() {
	if(Popup.dialog != null) Popup.dialog.close();
	Popup.dialog = null;
}

function onAjaxError(retval) {
	console.dir(arguments);
	if(!retval || retval.status == 404) {
		Popup.show({
			text: '网络发生故障，请重试或联系管理员！'
		});
		return;
	}

	if(retval.status == 400) {
		Popup.show({
			text: '请求失败，参数输入错误:' + retval.responseJSON.path
		});
		return;
	} else if(retval.status == 500) {
		Popup.show({
			text: '500错误！'
		});
		return;
	}
}

function alertMessage(msg) {
	Popup.show({
		text: msg
	});
}

