// Created by amoylel on 08/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_KEYBOARDTRANSFER_H__
#define AMO_KEYBOARDTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
     * @class	keyboard
     *
     * @script	../../example/js/keyboard.js
     *
     * @extend Object
     *
     * @brief	键盘操作类.<br>
     * 			工作线程:**UI线程**.
     */
    
    class KeyboardTransfer
        : public ClassTransfer
        , public amo::singleton < KeyboardTransfer > {
    public:
    
        KeyboardTransfer();
        ~KeyboardTransfer();
        
        virtual std::string getClass() const override {
            return "keyboard";
        }
        
        virtual amo::Transfer * getInterface(const std::string& name) override {
            if (name == getClass()) {
                return this;
            }
            
            return ClassTransfer::getInterface(name);
        }
        
        /*!
         * @fn	Any KeyboardTransfer::sayString(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	输入一段文本.
         *
         * @param	#String 要发送的字符串.
         * @param	#Int=5 每输入一个字符后的暂停时间.默认为5ms，输入太快有可能导致字符错乱.
         * @param	#Boolean=false ,是否异步输入字符串，如果为false，那么会阻塞
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        				include('keyboard');
        				$('#div_sayString').find('.example').focus();
        				keyboard.sayString('NCUI测试字符串');
        			 ```
         */
        
        Any sayString(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any KeyboardTransfer::keyPress(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	单击按键.
         *
         * @param	#Args... 按键, 输入参数个数不定，单个参数可以是一个字符串，也可以是一个特殊按键..
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        				include('keyboard');
        				$('#div_keyPress').find('.example').focus();
        				keyboard.keyPress('1','2','3', '5');
        				// 关闭当前窗口
        				keyboard.keyPress(keyboard.VK_MENU, keyboard.VK_F4);
        			 ```
         */
        
        Any keyPress(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any KeyboardTransfer::keyDown(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	按下按键.
         *
         * @param	#Args... 按键, 输入参数个数不定，单个参数可以是一个字符串，也可以是一个特殊按键.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        			include('keyboard');
        			$('#div_keyDown').find('.example').focus();
        			keyboard.keyDown('1');
        			keyboard.keyUp('1');
        		 ```
         */
        
        Any keyDown(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any KeyboardTransfer::keyUp(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	弹起按键.
         *
         * @param	#Args... 按键, 输入参数个数不定，单个参数可以是一个字符串，也可以是一个特殊按键.
         *
         * @return	无.
         * @example
         *
        			 ```
        				include('keyboard');
        				$('#div_keyUp').find('.example').focus();
        				keyboard.keyDown('2');
        				keyboard.keyUp('2');
        			 ```
         */
        
        Any keyUp(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any KeyboardTransfer::waitKey(IPCMessage::SmartType msg);
         * @ignore
         * @tag sync static
         *
         * @brief	等待按任意键.
         *
         * @return	无.
         */
        
        Any waitKey(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any KeyboardTransfer::getLastKey(IPCMessage::SmartType msg);
         * @ignore
         * @tag sync static
         *
         * @brief	检测上次按键.
         *
         * @return	#String.
         */
        
        Any getLastKey(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(KeyboardTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sayString, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(keyPress, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(keyDown, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(keyUp, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(waitKey, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getLastKey, TransferFuncStatic | TransferExecSync)
        
        /*! @attr #Int=CONTROL VK_CONTROL 键盘按键CONTROL. {@tag const static property}*/
        /*! @attr #Int=SHIFT VK_SHIFT 键盘按键SHIFT.{@tag const static property} */
        /*! @attr #Int=ALT VK_MENU 键盘按键ALT.{@tag const static property} */
        
        /*! @attr #Int=F1 VK_F1 键盘按键F1. {@tag const static property}*/
        /*! @attr #Int=F2 VK_F2 键盘按键F2. {@tag const static property}*/
        /*! @attr #Int=F3 VK_F3 键盘按键F3. {@tag const static property}*/
        /*! @attr #Int=F4 VK_F4 键盘按键F4. {@tag const static property}*/
        /*! @attr #Int=F5 VK_F5 键盘按键F5. {@tag const static property}*/
        /*! @attr #Int=F6 VK_F6 键盘按键F6. {@tag const static property}*/
        /*! @attr #Int=F7 VK_F7 键盘按键F7. {@tag const static property}*/
        /*! @attr #Int=F8 VK_F8 键盘按键F8. {@tag const static property}*/
        /*! @attr #Int=F9 VK_F9 键盘按键F9. {@tag const static property}*/
        /*! @attr #Int=F10 VK_F10 键盘按键F10. {@tag const static property}*/
        /*! @attr #Int=F11 VK_F11 键盘按键F11. {@tag const static property}*/
        /*! @attr #Int=F12 VK_F12 键盘按键F12. {@tag const static property}*/
        
        /*! @attr #Int=UP VK_UP 键盘按键UP. {@tag const static property}*/
        /*! @attr #Int=DOWN VK_DOWN 键盘按键DOWN. {@tag const static property}*/
        /*! @attr #Int=LEFT VK_LEFT 键盘按键LEFT. {@tag const static property}*/
        /*! @attr #Int=RIGHT VK_RIGHT 键盘按键RIGHT. {@tag const static property}*/
        /*! @attr #Int=RETURN VK_RETURN 键盘按键RETURN. {@tag const static property}*/
        /*! @attr #Int=ESCAPE VK_ESCAPE 键盘按键ESCAPE. {@tag const static property}*/
        /*! @attr #Int=BACK VK_BACK 键盘按键BACK. {@tag const static property}*/
        /*! @attr #Int=HOME VK_HOME 键盘按键HOME. {@tag const static property}*/
        /*! @attr #Int=DELETE VK_DELETE 键盘按键DELETE. {@tag const static property}*/
        /*! @attr #Int=INSERT VK_INSERT 键盘按键INSERT. {@tag const static property}*/
        
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_CONTROL)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_SHIFT)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_MENU)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F1)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F2)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F3)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F4)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F5)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F6)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F7)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F8)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F9)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F10)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F11)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_F12)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_UP)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_DOWN)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_LEFT)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_RIGHT)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_RETURN)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_ESCAPE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_BACK)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_HOME)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_DELETE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(VK_INSERT)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
    private:
        void SendKeyDown(char data);
        void SendKeyUp(char data);
        void SendUnicode(wchar_t data);
        void SendKeys(const amo::u8string& msg, int interval = 5);
        
        void AsyncSendKeys(IPCMessage::SmartType msg, const std::string& eventName,
                           int interval = 5);
                           
        std::vector<char> getKeys(IPCMessage::SmartType msg);
    private:
    
        std::shared_ptr<amo::looper_executor> m_executor;
    };
}

#endif // AMO_KEYBOARDTRANSFER_H__

