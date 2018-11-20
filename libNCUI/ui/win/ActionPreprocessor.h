// Created by amoylel on 08/06/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_ACTIONPREPROCESSOR_H__
#define AMO_ACTIONPREPROCESSOR_H__

#include <amo/stdint.hpp>

namespace amo {

    /*!
     * @class	预置事件
     *
     * @id settingsActionPreprocessor
     *
     * @desc 预置事件
     *
     * @chapter settings
     *
     * @brief	预置事件,用于处理菜单，和全局快捷键的action所描述的事件，如果action为以下所描述的事件，那么程序会处理这些事件，并阻止事件向页面传递.
     */
    
    class ActionPreprocessor {
    public:
        ActionPreprocessor(int32_t nBrowserID = -9999, int64_t nFrameID = -1);
        
        /*!
         * @fn	bool ActionPreprocessor::process(const std::string& action);
         * @ignore
         * @brief	Process the given action.
         *
         * @param	action	The action.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool process(const std::string& action);
        
        /*! @var	#String undo  撤消，作用于Frame*/
        /*! @var	#String redo  重做，作用于Frame*/
        /*! @var	#String cut  剪切，作用于Frame*/
        /*! @var	#String copy  复制，作用于Frame*/
        /*! @var	#String paste  粘贴，作用于Frame*/
        /*! @var	#String selectAll  全选，作用于Frame*/
        /*! @var	#String delete  删除，作用于Frame*/
        
        /*! @var	#String goBack  后退，作用于Browser*/
        /*! @var	#String goForward  前进，作用于Browser*/
        /*! @var	#String reload  正常重新加载当前窗口，作用于Browser*/
        /*! @var	#String forceReload  忽略缓存并重新加载当前窗口，作用于Browser*/
        /*! @var	#String stopLoad  停止加载，作用于Browser*/
        
        
        /*! @var	#String print  打印页面，作用于HOST*/
        /*! @var	#String printToPDF  打印页面为PDF，作用于HOST*/
        /*! @var	#String resetZoom  重置页面缩放等级，作用于HOST*/
        /*! @var	#String zoomIn  缩小页面，作用于HOST*/
        /*! @var	#String zoomOut  放大页面，作用于HOST*/
        
        /*! @var	#String show  显示窗口*/
        /*! @var	#String hide  隐藏窗口*/
        /*! @var	#String toggleVisible  显示/隐藏窗口*/
        /*! @var	#String minimize  最小化窗口*/
        /*! @var	#String maximize  最大化窗口*/
        /*! @var	#String close  关闭窗口*/
        /*! @var	#String showDevtools  打开调试窗口*/
        /*! @var	#String closeDevtools  关闭调试窗口*/
        /*! @var	#String fullscreen  全屏*/
        /*! @var	#String nonFullscreen  退出全屏*/
        /*! @var	#String toggleFullScreen  全屏/退出全屏*/
        
        /*! @var	#String quit  尝试退出程序*/
        /*! @var	#String exit  强制退出程序*/
        
        
        
    private:
        /*! @brief	Identifier for the browser. */
        int32_t m_nBrowserID;
        /*! @brief	Identifier for the frame. */
        int64_t m_nFrameID;
    };
}

#endif // AMO_ACTIONPREPROCESSOR_H__

