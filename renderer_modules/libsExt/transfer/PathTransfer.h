// Created by amoylel on 17/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBSEXT_PATHTRANSFER_F5089CC4_51A8_4448_BB3D_04242220BFA0_H__
#define LIBSEXT_PATHTRANSFER_F5089CC4_51A8_4448_BB3D_04242220BFA0_H__







#include <transfer/RunnableTransfer.hpp>
#include <amo/path.hpp>

namespace amo {



    /*!
     * @class	Path
     * @chapter extend
     *
     * @extend Runnable
     *
     * @chapter extend
     *
     * @brief	路径库.<br>
     * 			工作线程**Renderer线程**
     */
    
    class PathTransfer
        : public RunnableTransfer
        , public amo::singleton<PathTransfer> {
    public:
    
        PathTransfer();
        PathTransfer(const std::string& ansiPath);
        ~PathTransfer();
        
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        
        
        /*!
         * @fn	Any PathTransfer::getFullAppDir(IPCMessage::SmartType msg);
         *  @tag sync static
         * @brief	返回当前程序所在目录.
         *
         *
         * @return	#Path 返回一个Path对象.
         * @example
         *
         ```
        	include('Path');
        	var p = Path.getFullAppDir();
        	console.log(p);
        	console.log(p.toString());
         ```
         */
        
        Any getFullAppDir(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::getFullAppName(IPCMessage::SmartType msg);
         *  @tag sync static
         * @brief	获取当前程序的完整路径.
         *
         * @return	#Path 返回一个Path对象.
         * @example
         *
         ```
        	include('Path');
        	var p = Path.getFullAppName();
        	console.log(p);
        	console.log(p.toString());
         ```
         */
        
        Any getFullAppName(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::fileExsit(IPCMessage::SmartType msg);
         *  @tag sync static
         * @brief	判断文件或目录是否存在.
         *
         * @param	#String 路径.
         *
         * @return	#Boolean true 存在/false 不存在.
         *
         * @example
         *
         ```
        	include('Path');
        	var p = new Path('manifest.json');
        	p.copyTo('manifest2.json');
        	console.assert(Path.fileExsit('manifest2.json') == true);
         ```
         *
         */
        
        Any fileExsit(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::Remove(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	删除一个文件，或空目录.
         *
         * @param	#String 路径.
         *
         * @return	#Boolean true 成功/false 失败.
         *
         * @example
         *
         ```
        	include('Path');
        	var p = new Path('manifest.json');
        	p.copyTo('manifest2.json');
        	Path.Remove('manifest2.json')
        	console.assert(Path.fileExsit('manifest2.json') == false);
         ```
         */
        
        Any Remove(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::RemoveAll(IPCMessage::SmartType msg);
         * @tag static
         * @brief	删除一个文件或文件夹，文件目录可以不为空.
         *
         * @param	#String 路径.
         *
         * @return	#Boolean true 成功/false 失败.
         *
         * @example
         *
         ```
        	 include('Path');
        	 var p = new Path('manifest.json');
        	 p.copyTo('manifest2.json');
        	 Path.RemoveAll('manifest2.json')
        	 console.assert(Path.fileExsit('manifest2.json') == false);
         ```
         */
        
        Any RemoveAll(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	Any PathTransfer::toString(IPCMessage::SmartType msg);
        * @tag sync
        * @brief	将当前Path对象转换为String.
        *
        *
        * @return	#String .
        *
        * @example
        *
        ```
        	include('Path');
        	var p = new Path('manifest.json');
        	console.log(p.toString());
        ```
        */
        
        Any toString(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any PathTransfer::normalize(IPCMessage::SmartType msg);
         *
         * @brief	标准化路径.
         *
         * @return	#Path 当前对象.
         * @example
         *
        	```
        		include('Path');
        		var p = new Path('./sddf/../../233/../234/../../');
        		console.log(p.normalize().toString());
        	```
         */
        
        Any normalize(IPCMessage::SmartType msg);
        
        /*!
        * @fn	Any PathTransfer::isExsit(IPCMessage::SmartType msg);
        * @tag sync
        * @brief	判断当前路径是否存在.
        *
        *
        * @return	#Boolean true 存在/ false 不存在.
        *
        * @example
        *
        ```
        	include('Path');
        	var p = new Path('manifest.json');
        	console.assert(p.isExsit() == true);
        ```
        */
        
        Any isExsit(IPCMessage::SmartType msg);
        
        /*!
        * @fn	Any PathTransfer::append(IPCMessage::SmartType msg);
        * @tag sync
        * @brief	往当前路径中追加子路径.
        *
        * @param	#String 需要追加了路径.
        *
        * @return	#Path 当前对象.
        *
        * @example
        *
        ```
        	include('Path');
        	var p = new Path('./');
        	p.append('manifest.json');
        	console.log(p.toString());
        	console.assert(p.isExsit() == true);
        ```
        */
        
        Any append(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::toAbsolute(IPCMessage::SmartType msg);
         *
         * @brief	将当前路径转换为一个绝对路径.
         *
         *
         * @return	#Path 当前对象.
         *
         * @example
         *
         ```
        	 include('Path');
        	 var p = new Path('./');
        	 p.append('manifest.json');
        	 p.toAbsolute();
        	 console.log(p.toString());
        	 console.assert(p.isExsit() == true);
         ```
         */
        
        Any toAbsolute(IPCMessage::SmartType msg);
        
        /*!
        * @fn	Any PathTransfer::removeFileSpec(IPCMessage::SmartType msg);
        * @tag sync
        * @brief	去除文件名，得到目录.
        *
        * @return	#Path 当前对象.
        *
        * @example
        *
        ```
        	include('Path');
        	var p = new Path('./');
        	p.append('manifest.json');
        	p.toAbsolute().removeFileSpec();
        	console.log(p.toString());
        	console.assert(p.normalize().toString() == Path.getFullAppDir().normalize().toString());
        ```
        */
        
        Any removeFileSpec(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::copyTo(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	复制文件到指定位置.
         *
         * @param	#String 目标位置.
         *
         * @return	#Boolean true 存在/ false 不存在.
         *
         * @example
         *
         ```
        	 include('Path');
        	 var p = new Path('manifest.json');
        	 p.copyTo('manifest2.json');
        
        	 console.assert(Path.fileExsit('manifest2.json') == true);
         ```
         */
        
        Any copyTo(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any PathTransfer::moveTo(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	移动文件到指定位置.
         *
         * @param	#String 目标位置.
         *
         * @return	#Boolean true 存在/ false 不存在.
         *
         * @example
         *
         ```
        	 include('Path');
        	 var p = new Path('manifest.json');
        	 p.copyTo('manifest2.json');
        	 console.assert(Path.fileExsit('manifest2.json') == true);
        
        	 p = new Path('manifest2.json');
        	 p.moveTo('manifest3.json');
        
        	 console.assert(Path.fileExsit('manifest2.json') == false);
        	 console.assert(Path.fileExsit('manifest3.json') == true);
        
         ```
         *
         */
        
        Any moveTo(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	Any PathTransfer::remove(IPCMessage::SmartType msg);
        * @tag
        * @brief	删除当前路径，该路径只能是一个文件或空目录删除一个文件，或空目录.
        *
        * @return	#Boolean true 成功/false 失败.
        *
        * @example
        *
        ```
        	include('Path');
        	var p = new Path('manifest3.json');
        	p.remove();
        
        	console.assert(Path.fileExsit('manifest3.json') == false);
        
        ```
        */
        
        Any remove(IPCMessage::SmartType msg);
        
        /*!
        * @fn	Any PathTransfer::removeAll(IPCMessage::SmartType msg);
        * @tag
        * @brief	删除当前路径，可以删除子目录，.
        *
        *
        * @return	#Boolean true 成功/false 失败.
        *
        * @example
        *
        ```
        	include('Path');
        	var p = new Path('web');
        	p.copyTo('web2');
        
        	console.assert(Path.fileExsit('web2') == true);
        	p = new Path('web2');
        	p.removeAll();
        	console.assert(p.isExsit('web2') == false);
        ```
        */
        
        Any removeAll(IPCMessage::SmartType msg);
    public:
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(PathTransfer, RunnableTransfer)
        
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getFullAppDir,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getFullAppName,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(fileExsit, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Remove, TransferFuncStatic | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(RemoveAll,
                                      TransferFuncStatic | TransferExecNormal)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(toString, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isExsit, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(append, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(normalize, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(removeFileSpec,
                                      TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(toAbsolute,
                                      TransferFuncNormal | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(remove,   TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(removeAll,
                                      TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(copyTo,
                                      TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(moveTo,
                                      TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        std::shared_ptr<amo::u8path>  m_pPath;
        
    };
}

#endif //LIBSEXT_PATHTRANSFER_F5089CC4_51A8_4448_BB3D_04242220BFA0_H__