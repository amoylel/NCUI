// Created by amoylel on 12/01/2017.
// Copyright (c) 2017 amoylel All rights reserved.


#ifndef LIBSEXT_FILETRANSFER_7B3B1E43_5470_4945_A0D5_0AFA9312172F_HPP__
#define LIBSEXT_FILETRANSFER_7B3B1E43_5470_4945_A0D5_0AFA9312172F_HPP__






#include <string>
#include <memory>

#include <amo/singleton.hpp>
#include <transfer/RunnableTransfer.hpp>

#include <amo/file.hpp>
#include <amo/filestream.hpp>
#include <amo/path.hpp>

namespace amo {

    /**
     * @class	FileStream
     *
     * @chapter extend
     *
     * @brief	文件读写类.
     *
     * @extend	Runnable
     */
    
    class FileTransfer
        : public RunnableTransfer
        , public amo::singleton<FileTransfer> {
    public:
        FileTransfer()
            : RunnableTransfer("FileStream") {
            
        }
        
        /**
         * @fn	FileTransfer::FileStream(const amo::string& fileName, int mode = 3)
         * @tag constructor
         * @index 1
         * @brief	创建或打开一个文件.
         *
         * @param	#String 需要操作文件的路径.
         * @param	#Int=3	文件打开方式，可以选择以下值，组合使用.<br>
         * 					&nbsp;&nbsp;&nbsp;&nbsp; 1 以只读方式打开，如果文件不存在，那么将打开失败<br>
         * 					&nbsp;&nbsp;&nbsp;&nbsp; 2 以写方式打开，如果文件不存在则会创建文件<br>
         * 					&nbsp;&nbsp;&nbsp;&nbsp; 8 以追加方式打开<br>
         * 					&nbsp;&nbsp;&nbsp;&nbsp; 32 以二进制方式打开文件<br>
         * @return #FileStream
         */
        
        FileTransfer(const amo::u8string& fileName, int mode = 3)
            : RunnableTransfer("FileStream") {
            
            
            m_filename = fileName;
            m_mode = mode;
            open(IPCMessage::Empty());
        }
        
        ~FileTransfer() {
            if (m_filestream) {
                m_filestream->close();
            }
            
            m_filestream.reset();
            m_file.reset();
        }
        
        virtual std::string getClass() const override {
            return "FileStream";
        }
        
        virtual amo::Transfer * getInterface(const std::string& name) override {
            if (name == FileTransfer::getClass()) {
                return this;
            }
            
            return RunnableTransfer::getInterface(name);
        }
        
        virtual  Any onCreateClass(IPCMessage::SmartType msg) override {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            amo::u8string fileName(args->getString(0), true);
            int nMode = 3;
            Any& val = args->getValue(1);
            
            if (val.is<int>()) {
                nMode = args->getInt(1);
            }
            
            auto pTransfer = ClassTransfer::createTransfer<FileTransfer>(fileName, nMode);
            pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());
            pTransfer->setDefaultTriggerEventFunc(this->getDefaultTriggerEventFunc());
            return pTransfer->getFuncMgr().toSimplifiedJson();
        }
        
        /**
         * @fn	Any FileTransfer::open(IPCMessage::SmartType msg)
         * @tag sync
         * @brief	打开文件，创建对象时会默认打开文件，不需要重复调用.
         * 			如果你调用了{@link close=FileStream.close}函数，那么可以调用该函数重新打开文件
         *
         * @param	#int=0 文件打开方式，参见{@link 构造函数=FileStream.FileStream}的内容.
         * 				   如果不输入该参数，程序会使用创建该对象时指定的打开方式
         *
         *
         * @return	#Boolean.
         */
        
        Any open(IPCMessage::SmartType msg) {
        
            if (m_filestream) {
                m_filestream->close();
            }
            
            
            
            
            m_filestream.reset();
            m_file.reset();
            Any& val = msg->getArgumentList()->getValue(0);
            int mode = m_mode;
            
            if (val.is<int>()) {
                mode = val.As<int>();
            }
            
            if (mode <= 0) {
                return false;
            }
            
            amo::u8path p(m_filename);
            
            if (!p.exists() && mode & std::ios::out) {
                std::ofstream ofs(p.generic_wstring());
                ofs.close();
            }
            
            
            m_file.reset(new amo::u8file(p));
            m_filestream = m_file->get_filestream(mode);
            
            return true;
        }
        
        /**
         * @fn	Any FileTransfer::read(IPCMessage::SmartType msg)
         *
         * @tag sync deprecated
         *
         * @brief	读取所有文件内容.
         *
         * @return	#String 读取到的字符串.
         *
         * @see readSome=FileStream.readSome
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json');
        	console.log(filestream.read());
        	filestream.close();
         ```
         */
        
        Any read(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return "";
            }
            
            return m_filestream->read_all();
        }
        
        /**
         * @fn	Any FileTransfer::readAll(IPCMessage::SmartType msg)
         * @tag sync
         * @brief	获取所有字符串, readAll采用不同的机制读取文件，该操作与当前流无关，
         * 			所以调用该函数不会影响其他状态函数如{@link eof=FileStream.eof}.
         *
         * @return	#String
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json');
        	console.log(filestream.readAll());
        	filestream.close();
         ```
         */
        
        Any readAll(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return "";
            }
            
            return m_filestream->read_all();
        }
        
        /**
         * @fn	Any FileTransfer::readSome(IPCMessage::SmartType msg)
         * @tag sync
         * @brief	读取指定数量.
         *
         * @param	#Int 最多读取多少个字符.
         *
         * @return	#String.
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json');
        	while(!filestream.eof()){
        		console.log(filestream.readSome(100));
        	}
        	filestream.close();
         ```
         */
        
        Any readSome(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return "";
            }
            
            int count = msg->getArgumentList()->getInt(0);
            
            if (count <= 0) {
                return "";
            }
            
            std::vector<char> buffer(count, 0);
            
            int bytes = m_filestream->read_some(buffer.data(), count);
            return std::string(buffer.data(), bytes);
        }
        
        /**
         * @fn	Any FileTransfer::readLine(IPCMessage::SmartType msg)
         *
         * @tag sync
         *
         * @brief	读取一行,一行最多4096个字符，如果多于这个值则不能获取完整数据，参考
         * 			{@link readSome=FileStream.readSome}.
         *
         * @return	#String.
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json');
        	while(!filestream.eof()){
        		console.log(filestream.readLine());
        	}
        	filestream.close();
         ```
         */
        
        Any readLine(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return "";
            }
            
            return m_filestream->read_line();
        }
        
        /**
         * @fn	Any FileTransfer::write(IPCMessage::SmartType msg)
         * @tag sync
         * @brief	向文件中写入字符串.
         *
         * @param	#String 需要写入的数据.
         *
         * @return	#Boolean true 成功/false 失败.
         * @example
         *
         ```
        	 include('FileStream');
        	 var filestream = new FileStream('manifest2.json', FileStream.out | FileStream.binary);
        	 console.assert(filestream.write('1234567890') == true);
        	 filestream.remove();
         ```
         */
        
        Any write(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return false;
            }
            
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            std::string str = args->getString(0);
            
            if (str.empty()) {
                return false;
            }
            
            m_filestream->write(str);
            return true;
        }
        
        /**
         * @fn	Any FileTransfer::append(IPCMessage::SmartType msg)
         *
         * @tag sync
         *
         * @brief	将一个文件的内容追加到当前文件中.
         *
         * @param	#String 文件路径
         *
         * @return	#Boolean true 成功/false 失败.
         *
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest2.json', 2);
        	filestream.append('manifest.json');
        	filestream.close();
        	console.log(filestream.size());
        	filestream.remove();
         ```
         */
        
        Any append(IPCMessage::SmartType msg) {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            amo::u8string str(args->getString(0), true);
            amo::u8path p(str);
            
            if (!p.file_exists()) {
                return false;
            }
            
            amo::u8filestream file(p, std::ios::in | std::ios::binary);
            int nTotal = 0;
            
            while (true) {
                char buffer[4096] = { 0 };
                int nBytes = file.read_some(buffer, 4095);
                nTotal += nBytes;
                
                if (nBytes == 0) {
                    break;
                }
                
                m_filestream->write(buffer, nBytes);
                
            }
            
            return true;
        }
        
        
        // 将当前文件追加到另一个文件中去
        Any appendTo(IPCMessage::SmartType msg) {
            return Undefined();
        }
        
        /**
         * @fn	Any FileTransfer::close(IPCMessage::SmartType msg)
         * @tag sync
         * @brief	关闭文件，关闭后不能再调用其他API操作文件.
         *
         *
         * @return	无.
         *
         * @see open=FileStream.open
         *
         * @example
         *
         ```
        		include('FileStream');
        		var filestream = new FileStream('manifest.json');
        		filestream.close();
        
        		console.assert(filestream.isOpened() == false);
         ```
         */
        
        Any close(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return Undefined();
            }
            
            m_filestream->close();
            m_filestream.reset();
            m_file.reset();
            return Undefined();
        }
        
        /**
         * @fn	Any FileTransfer::isOpened(IPCMessage::SmartType msg)
         *
         * @tag sync
         *
         * @brief	判断文件是否打开.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json');
        	console.assert(filestream.isOpened() == true);
        
        	filestream.close();
        	console.assert(filestream.isOpened() == false);
         ```
         */
        
        Any isOpened(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return false;
            }
            
            return m_filestream->is_open();
            
        }
        
        /**
         * @fn	Any FileTransfer::size(IPCMessage::SmartType msg)
         * @tag sync
         * @brief	获取文件大小.
         *
         * @return	#Int.
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json', FileStream.in);
        	console.log(filestream.size());
        
        	var filestream2 = new FileStream('manifest2.json',FileStream.out);
        	console.assert(filestream2.size() == 0);
        
        	filestream2.write('1234567890');
        	filestream2.close();
        	filestream2.open(1);
        	console.assert(filestream2.size() == 10);
        
        	filestream2.remove();
         ```
         */
        
        Any size(IPCMessage::SmartType msg) {
            if (!m_file) {
                amo::u8path p(m_filename);
                m_file.reset(new amo::u8file(p));
            }
            
            return (int)m_file->size();
        }
        
        /**
         * @fn	Any FileTransfer::Remove(IPCMessage::SmartType msg)
         * @tag sync static
         * @brief 删除文件.
         *
         * @param #String 文件路径.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest2.json', FileStream.out + FileStream.binary);
        	filestream.write('33');
        	// 文件被占用，无法删除
        	console.assert(FileStream.Remove('manifest2.json') == false);
        	filestream.close();
        	// 解除文件占用，可以删除
        	console.assert(FileStream.Remove('manifest2.json') == true);
         ```
         */
        
        Any Remove(IPCMessage::SmartType msg) {
            amo::u8string strPath(msg->getArgumentList()->getString(0), true);
            amo::u8path p(strPath);
            return p.remove();
        }
        
        
        /**
        * @fn	Any FileTransfer::remove(IPCMessage::SmartType msg)
        * @tag sync
        * @brief 删除当前文件.
        *
        * @return	#Boolean.
        * @example
        *
        ```
        	include('FileStream');
        	// 以读写方式打开，如果文件不存在，会创建文件
        	var filestream = new FileStream('manifest2.json');
        	console.assert(filestream.remove() == true);
        	// 以只读方式打开，如果文件不存在，则不会创建文件
        	var filestream2 = new FileStream('manifest2.json', 1);
        	console.assert(filestream.remove() == true);
        ```
        */
        
        Any remove(IPCMessage::SmartType msg) {
            close(msg);
            amo::u8path p(m_filename);
            return  p.remove();
        }
        
        /**
         * @fn	Any FileTransfer::eof(IPCMessage::SmartType msg)
         *
         * @brief	判断是否已读取到文件末尾.
         *
         *
         * @return	#Boolean true 末尾/ false 非末尾.
         * @example
         *
         ```
        	include('FileStream');
        	var filestream = new FileStream('manifest.json', FileStream.in);
        	//filestream.readAll(); // readAll不会影响内部文件指针位置
        	console.log(filestream.eof());
        	console.assert(filestream.eof() ==false);
        	while(!filestream.eof()){
        		console.log(filestream.readSome(100));
        	}
        	console.assert(filestream.eof() == true);
        
         ```
         */
        
        Any eof(IPCMessage::SmartType msg) {
            if (!m_filestream) {
                return true;
            }
            
            bool bRetval =  m_filestream->eof();
            return bRetval;
        }
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(FileTransfer, RunnableTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Remove, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(read, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(readAll, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(readSome, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(readLine, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(write, TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(append, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(appendTo, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(close, TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(size, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(remove, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(eof, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(open, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isOpened, TransferFuncNormal | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(in, (int)std::ios::in)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(out, (int)std::ios::out)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(ate, (int)std::ios::ate)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(app, (int)std::ios::app)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(trunc, (int)std::ios::trunc)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(binary, (int)std::ios::binary)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    public:
    
        std::shared_ptr<amo::u8file> m_file;
        amo::u8string m_filename;
        std::shared_ptr<amo::u8filestream> m_filestream;
        int m_mode;
    };
}

#endif //LIBSEXT_FILETRANSFER_7B3B1E43_5470_4945_A0D5_0AFA9312172F_HPP__
