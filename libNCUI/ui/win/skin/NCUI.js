'use strict'
Object.defineProperty(this, "cef", {
    get: function() {
        native function init();
        delete this.cef;
        return this.cef = init();
    },
    configurable: true
});

function include(str) {
    native function include(str);
    return include(str);
};

function includes() {
    native function includes(arguments);
    return includes(arguments);
};

function renderer_modules() {
    native function renderer_modules(arguments);
    return renderer_modules(arguments);
};

function browser_modules() {
    native function browser_modules(arguments);
    return browser_modules(arguments);
};
 
function sleep(ms) {
    native function sleep(ms);
    return sleep(ms);
};

function usleep(us) {
    native function usleep(us);
    return usleep(us);
};

function updateBlobData(blobName, blobMime, blobData) {
    native function updateBlobData(blobName, blobMime, blobData);
    return updateBlobData(blobName, blobMime, blobData);
};

function ncuiTriggerCustomEvent(name, val){
	document.dispatchEvent(new CustomEvent(name, {detail: val}));
}

function objectToString(json) {
    return JSON.stringify(json);
};

function stringToObject(str) {
    return JSON.parse(str);
};

function createBlobByString(str, opt) {
	var arr =[];
	arr.push(str);
    return new Blob(arr, opt);
};

function objectIsBlob(ptr){
	if(!ptr) return 0;
	if(!ptr.constructor) return 0;
	if( ptr.constructor != Blob) return 0; 
	return ptr.size;
}

function createBlobByArray(arr, opt) { 
    return new Blob(arr, opt);
};

function readDataFromBlob(blobName, blob){
	var reader = new FileReader();
	reader.readAsBinaryString(blob);

	//  当读取操作成功完成时调用.
	reader.onload = function(evt) {
		if(evt.target.readyState == FileReader.DONE) {
			console.dir(evt);
			var blobData = evt.target.result;
			console.log(blobName);
			console.log(blob.type);
			updateBlobData(blobName, blob.type, blobData);
		 
		}

	}
};

function getWebkitAppRegion(obj){
	if(obj == document){ 
		return '';
	}

	if(!obj) { 
		return '';
	}
	//console.dir(window.getComputedStyle(obj));
	//return window.getComputedStyle(obj)['-webkit-app-region']; 
	var retval = window.getComputedStyle(obj)['-webkit-app-region']; 
	console.log(retval);
	return retval;
	 
}

function getCssText(obj){
	if(obj == document){ 
		return "no-drag";
	} 
	if(!obj) { 
		return 'no-drag';
	}
	
	return window.getComputedStyle(obj).cssText; 
}

function getParentWindowPosition(parent) {
    var result = {};
    result.objectType = "f622feb3-0b69-4b28-8cc6-57da93b7eb6a";
    result.left = 0;
    result.top = 0;
    if(parent == undefined || parent == null) return result;
    if(parent.parent != null && parent.parent != parent) {
        var point = getParentWindowPosition(parent.parent);
        result.left += point.left;
        result.top += point.top;
        return result;
    }

    var element = parent.document.activeElement;
    if(element == undefined || element == null) return result;
    var rect = element.getBoundingClientRect();
    result.left += rect.left - element.scrollLeft;
    result.top += rect.top - element.scrollTop;
    return result;
}

function getActiveElementPosition() {
    var element = document.activeElement;
    var result = {};
    result.objectType = "f622feb3-0b69-4b28-8cc6-57da93b7eb6a";
    result.left = 0;
    result.top = 0;
    if(window.parent != null && window.parent != window)
        result = getParentWindowPosition(window.parent);

    result.width = 0;
    result.height = 0;
    if(element == undefined || element == null) return result;
    var rect = element.getBoundingClientRect();
    var style = getComputedStyle(element); 
    
    var marginLeft = parseInt(style.marginLeft);
    var marginTop = parseInt(style.marginTop);
    var marginRight = parseInt(style.marginRight);
    var marginBottom = parseInt(style.marginBottom);
    
    var paddingLeft = parseInt(style.paddingLeft);
    var paddingTop = parseInt(style.paddingTop);
    var paddingRight = parseInt(style.paddingRight);
    var paddingBottom = parseInt(style.paddingBottom);
    result.left += parseInt(rect.left + paddingLeft );
    result.top += parseInt(rect.top + paddingTop );
    
    result.width = parseInt(rect.width - paddingLeft - paddingRight );
    result.height = parseInt(rect.height - paddingTop - paddingBottom );
    result.right = parseInt(result.left + result.width);
    result.bottom = parseInt(result.top + result.height);
    result.marginLeft = marginLeft;
    result.marginTop = marginTop;
    result.marginRight = marginRight;
    result.marginBottom = marginBottom;
    result.paddingTop = paddingTop;
    result.paddingLeft = paddingLeft;
    result.paddingRight = paddingRight;
    result.paddingBottom = paddingBottom;
    result.rectLeft = rect.left;
    result.rectTop = rect.top;
    result.rectRight= rect.right;
    result.rectBottom = rect.bottom;
    
 
    return result;
}

function getActiveElementAttributes(id, newstr, selection_start) {
    var result = {
        objectType: "f622feb3-0b69-4b28-8cc6-57da93b7eb6a",
        selectionStart: 0,
        scrollHeight: 0,
        scrollWidth: 0,
        scrollTop: 0,
        scrollLeft: 0,
        newString: newstr,
        needUpdate: false,
        value: ""
    };

    var obj = document.activeElement;
    if(obj == null) return result;
    result.selectionStart = obj.selectionStart;
    result.scrollHeight = obj.scrollHeight;
    result.scrollWidth = obj.scrollWidth;
    result.scrollTop = obj.scrollTop;
    result.scrollLeft = obj.scrollLeft;
    result.needUpdate = false;
    
    if(selection_start != obj.selectionStart) {
        result.needUpdate = true;
        result.value = obj.value;

    }
    return result;
}

function getActiveElementInfo() {
    var element = document.activeElement; 
    var result = getActiveElementPosition();
    result.type = element.type;
    result.scrollWidth = element.scrollWidth;
    result.scrollHeight = element.scrollHeight;
    result.fontFamily = getComputedStyle(element).fontFamily;
    result.fontSize = parseInt(getComputedStyle(element).fontSize);
    result.lineHeight = getComputedStyle(element).lineHeight;
    result.color = getComputedStyle(element).color;
//  console.dir(result); 
    return result;
}

function loadScript(url, callback) { 
    var script = document.createElement("script");
    script.type = "text/javascript";
    if(typeof(callback) != "undefined"){
        if (script.readyState) {
            script.onreadystatechange = function () {
                if (script.readyState == "loaded" || script.readyState == "complete") {
                    script.onreadystatechange = null;
                    callback();
                }
            };
        } else {
            script.onload = function () {
                callback();
            };
        }
    }
    script.src = url;
    document.body.appendChild(script); 
}

function loadCSS(url){
    if(!url || url.length === 0){
        throw new Error('argument "path" is required !');
    }
    var head = document.getElementsByTagName('head')[0];
    var link = document.createElement('link');
    link.href = url;
    link.rel = 'stylesheet';
    link.type = 'text/css';
    head.appendChild(link);
}

function listenDocumentEvent(){
	document.addEventListener('ipc.exec', function(event) { 
		console.dir(event);
		include('ipc');
		var name = event.detail.eventName;
		var data = event.detail;
		ipc.exec(name, data); 
	});
	
	document.addEventListener('ipc.dispatchEvent', function(event) { 
		console.dir(event);
		include('ipc');
		var name = event.detail.eventName;
		var data = event.detail;
		ipc.dispatchEvent(name, data); 
	});
	
	document.dispatchEvent(new CustomEvent('ipc.ready', {}));
}