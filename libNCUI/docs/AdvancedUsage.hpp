// Created by amoylel on 07/21/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_ADVANCEDUSAGE_E1DEC618_F70E_496C_A726_0D629A243048_HPP__
#define AMO_ADVANCEDUSAGE_E1DEC618_F70E_496C_A726_0D629A243048_HPP__

namespace amo {

    /*!
     * @class	高级用法
     * @desc	高级用法
     * @id basicAdvancedUsage
     * @chapter basic
     * @brief	NCUI进阶教程.
     */
    
    class AdvancedUsage {
    public:
    
    
    
        /*!
         * @section	local协议
         *
         * @brief	NCUI注册了自定义协议local://file来支持本地文件的访问.具体为"local://file/文件路径",文件路径如果为相对路径的话，将以%webDir%所在目录进行查找.
         *
         * @example
         *
         ```
        	includes('BrowserWindow', 'app');
        
        	var win = new BrowserWindow({
        		titleBar: true,
        		url: 'local://file/example/BrowserWindow.html'
        	})
        
        	var url = app.toAbsolutePath("%webDir%example/FramelessWindow.html");
        	console.log(url);
        
        	new BrowserWindow({
        		titleBar: false,
        		url: 'local://file/' + url
        	})
         ```
         */
        
        void foo2() {
        
        }
        
        /*!
        * @section	@file协议
        *
        * @brief	NCUI支持自定义协议@file:///来支持本地文件的访问.具体为"@file:///文件路径",文件路径如果为相对路径的话，将以%webDir%所在目录进行查找.<br>
        * 			@file协议更加强大,你可以在任意url中注入@file协议将url指向本地文件，且不存在跨域等问题.
        *
        * @example
        *
        ```
        		includes('BrowserWindow', 'app');
        
        		var win = new BrowserWindow({
        			titleBar: true,
        			url: 'file:///@file:///example/BrowserWindow.html'
        		})
        
        		var url = app.toAbsolutePath("%webDir%example/FramelessWindow.html");
        		console.log(url);
        
        		new BrowserWindow({
        			titleBar: false,
        			url: 'http://www.baidu.com/ananana/@file:///'+url
        		})
        ```
        */
        
        void foo3() {
        
        }
        
        /*!
         * @section 地址映射
         *
         * @brief	该功能可以将一个URL映射到本地文件.目前支持的映射方式有文件、目录、ZIP格式压缩文件、DLL动态库、RES资源文件<br>
         * 			可以编辑manifest.json指定映射路径，也可以在程序运行后{@link 手动指定=app.addUrlMapping}映射路径.<br>
         * 			本文档程序就是采用URL到文件夹的方式访问页面的.<br>
         *
         *
         * @see app.addUrlMapping
         * @example 修改manifest.json
         *
         ```
        	//@skip 通过修改manifest.json映射文件
        	{
        		"appSettings": {
        			"urlMappings":[
        				{
        					"url":"http://127.0.0.1:8030",
        					"path": "%webDir%"
        				},
        				{
        					"url":"http://127.0.0.1:8030",
        					"path": "zip:///%webDir%web.zip"
        				},
        				{
        					"url":"http://127.0.0.1:8030",
        					"path": "dll:///libResource.dll"
        				},
        				{
        					"url":"http://127.0.0.1:8030",
        					"path": "res:///131.res"
        				}
        			]
        		}
        	}
         ```
         *
         */
        
        void foo4() {
        
        }
        
        /*!
         * @section	源代码管理
         *
         * @brief	NCUI支持读取本地文件、ZIP格式压缩文件、DLL动态库中的HTML源代码，你可以通过这些方式为NCUI提供源代码.<br>
         * 			1. **从本地目录中读取HTML:** 直接指定文件路径或文件夹路径，如D:/web<br>
         * 			2. **从ZIP文件中读取HTML:** 以zip:///开头<br>
         * 			3. **从DLL中读取HTML:** 以dll:///开头<br>
         * 			4. **从程序的资源文件中读取HTML:**以res:///开头<br>
         * 			NCUI不直接提供源代码加密功能，但支持读取加密的ZIP文件、RES，你可以在使用这些加密文件前通过{@link app.setResPassword} 或{@link app.setZipPassword}设置文件密码.
         *
         */
        
        void foo5() {
        
        }
        
        
        
        /*!
         * @section	C++扩展
         *
         * @brief	NCUI提供更加简单的扩展编写方式，只需要将扩展项目生成的动态库文件放到renderer_modules或者browser_modules目录即可在页面中调用.<br>
         * 			如果你是C++程序员，请参考libExt/libTaskTest项目的源代码.
         */
        
        void foo6() {
        
        }
        
        
        /*!
        * @section	外部渲染层
        *
        * @brief	仅离屏模式下支持使用外部数据渲染界面.
        *
        * @see BrowserWindow.addOverlap
        *
        */
        
        void foo7() {
        
        }
        
        
        /*!
        * @section	DLL调用
        * @ignore
        * @brief	NCUI直接调用C导出的DLL接口,但限制比较多，还不如直接写C++扩展快，暂未开放.
        *
        */
        
        void foo8() {
        
        }
        
        /**
         * @section	使用Flash
         *
         * @brief
         * 你可以按照下下步骤加载ppapi类型的flash到程序中：<br> 1.找到一个可用的flash的动态库,将dll文件名为pepflashplayer.dll;<br> 2.和manifest.json文件一起放到plugins目录下即可.<br>
         * 使用NPAPI类型的flash:直接将dll放到plugins目录下即可<br>
         * <b>2704及以上版本不支持npapi插件，只能使用ppapi类型的flash</b>
         */
        
        
        void foo9() {
        
        }
    };
}

#endif // AMO_ADVANCEDUSAGE_E1DEC618_F70E_496C_A726_0D629A243048_HPP__

