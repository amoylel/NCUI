// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Windows 头文件:
#include <windows.h>
#include <objbase.h>
#include <Commdlg.h>
#include <MmSystem.h>
#include <commctrl.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Comctl32.lib")

#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <string>
#include <tchar.h>
#include <unordered_map>
#include <cstdint>

#include <UIlib.h>
#ifdef _DEBUG
#pragma comment(lib, "libcef_dll_wrapper.lib")
#pragma comment(lib, "libcef.lib")
#else
#pragma comment(lib, "libcef_dll_wrapper.lib")
#pragma comment(lib, "libcef.lib")
#endif
using namespace DuiLib;

#ifdef GetNextSibling
#undef GetNextSibling
#endif

#ifdef GetFirstChild
#undef GetFirstChild
#endif


#include <amo/json.hpp>
#include <amo/loader.hpp>
#include <amo/path.hpp>
#include <amo/string.hpp>
#include <amo/logger.hpp>



#include "utility/utility.hpp"
//#include "utility/libzippp.h"

#include "ipc/Any.hpp"
#include "ipc/AsyncFunctionManager.hpp"
#include "ipc/IPCMessage.hpp"
#include "ipc/MessageLauncher.hpp"
#include "ipc/ProcessExchanger.hpp"
#include "ipc/RenderMessageEmitter.hpp"
#include "ipc/UIMessageBroadcaster.hpp"
#include "ipc/UIMessageEmitter.hpp"
#include "ipc/IPCEmitter.hpp"


#include "handler/BrowserManager.hpp"
#include "handler/BrowserProcessHandler.h"
#include "handler/CefHeader.hpp"
#include "handler/CefProcessExchanger.hpp"
#include "handler/ClientApp.h"
#include "handler/ClientHandler.h"
#include "handler/ContextMenuHandler.h"
#include "handler/DialogHandler.h"
#include "handler/DisplayHandler.h"
#include "handler/DownloadHandler.h"
#include "handler/DragHandler.h"
#include "handler/DummyClientHandler.h"
#include "handler/FocusHandler.h"
#include "handler/GeolocationHandler.h"
#include "handler/HandlerDelegate.hpp"
#include "handler/JSDialogHandler.h"
#include "handler/KeyboardHandler.h"
#include "handler/LifeSpanHandler.h"
#include "handler/LoadHandler.h"
#include "handler/MessageHandlerDelegate.hpp"
#include "handler/MessageRouterBrowserSide.h"
#include "handler/RenderHandler.h"
#include "handler/RenderProcessHandler.h"
#include "handler/RequestHandler.h"
#include "handler/ResourceBundleHandler.h"
#include "handler/ResourceHandler.h"
#include "handler/RunFileDialogCallback.hpp"
#include "handler/URLRequestClient.h"
#include "handler/UtilityV8Handler.h"
#include "handler/V8ExtentionHandler.h"


#include "scheme/LocalSchemeHandler.h"
#include "scheme/NativeFileHandler.h"
#include "scheme/UrlResourceHandlerFactory.h"
//#include "scheme/ZipFileHandler.h"
//#include "scheme/ZipFileMnager.h"

#include "module/basic/ClassMethodMgr.hpp"
#include "module/basic/EventFunctionManager.h"
#include "module/basic/TypeConvertor.h"
#include "module/basic/V8HandlerManager.h"

#include "module/dll/DllManager.h"
#include "module/dll/DllRunner.h"
#include "module/dll/DllV8Handler.h"
#include "module/dll/DllValueAccessor.h"
#include "module/dll/DllValueHandler.h"
#include "module/dll/JsTypeMapHandler.h"
#include "module/dll/TypeMapManager.h"

#include "module/ConstantAttributeV8Handler.h"
#include "module/DragWindowV8Handler.h"
#include "module/EventEmitterV8Handler.h"
#include "module/IPCRendererV8Handler.h"
#include "module/JsClassObjectV8Handler.h"
#include "module/JsClassV8Handler.h"
#include "module/JSV8Handler.h"

#include "node/CefCallbackHandler.h"
#include "node/NodeContextMenuHandler.h"
#include "node/NodeDialogHandler.h"
#include "node/NodeDisplayHandler.h"
#include "node/NodeDownloadHandler.h"
#include "node/NodeDragHandler.h"
#include "node/NodeFocusHandler.h"
#include "node/NodeGeolocationHandler.h"
#include "node/NodeHandlerHelper.h"
#include "node/NodeJSDialogHandler.h"
#include "node/NodeKeyboardHandler.h"
#include "node/NodeLifeSpanHandler.h"
#include "node/NodeLoadHandler.h"
#include "node/NodeMessageHandler.h"
#include "node/NodeRenderHandler.h"
#include "node/NodeRequestHandler.h"

#include "settings/AppSettings.h"
#include "settings/BasicSettings.h"
#include "settings/BrowserWindowSettings.h"
#include "settings/DowloaderSettings.h"
#include "settings/GlobalShortcutSettings.h"
#include "settings/MenuWindowSettings.h"
#include "settings/NativeWindowSettings.h"
#include "settings/RequestSettings.h"
#include "settings/SplashWindowSettings.h"

#include "transfer/AppTransfer.h"
#include "transfer/BrowserHostTransfer.h"
#include "transfer/BrowserTransfer.h"
#include "transfer/BrowserTransferMgr.h"
#include "transfer/BrowserWindowTransfer.h"
#include "transfer/ClassTransfer.hpp"
#include "transfer/CommandLineTransfer.h"
#include "transfer/ControlTransfer.h"
#include "transfer/DesktopTransfer.h"
#include "transfer/DialogTransfer.h"
#include "transfer/DownloaderTransfer.h"
#include "transfer/EventTransfer.h"
#include "transfer/FrameTransfer.h"
#include "transfer/FunctionWrapper.hpp"
#include "transfer/GlobalShortcutTransfer.h"
#include "transfer/IPCTransfer.h"
#include "transfer/KeyboardTransfer.h"
#include "transfer/MenuTransfer.h"
#include "transfer/MouseTransfer.h"
#include "transfer/NativeWindowTransfer.h"
#include "transfer/NetTransfer.h"
#include "transfer/RendererThreadTransfer.h"
#include "transfer/RequestTransfer.h"
#include "transfer/RunnableTransfer.hpp"
#include "transfer/ShellTransfer.h"
#include "transfer/SplashTransfer.h"
#include "transfer/SystemInfo.h"
#include "transfer/SystemTransfer.h"
#include "transfer/ThreadTransfer.hpp"
#include "transfer/Transfer.hpp"
#include "transfer/TransferEventEmitter.hpp"
#include "transfer/TransferEventInfo.hpp"
#include "transfer/TransferMapping.hpp"
#include "transfer/TransferMappingMgr.hpp"
#include "transfer/TransferMgr.h"
#include "transfer/TrayTransfer.h"
#include "transfer/UIThreadTransfer.h"
#include "transfer/URLRequestClientTransfer.h"
#include "transfer/URLRequestTransfer.h"

#include "ui/win/ActionPreprocessor.h"
#include "ui/win/BrowserWindow.h"
#include "ui/win/BrowserWindowManager.h"
#include "ui/win/BrowserWindowSkin.h"
#include "ui/win/BytesWriteHandler.h"
#include "ui/win/clipboard/Clipboard.h"
#include "ui/win/clipboard/ClipWatcher.h"
#include "ui/win/Imm32Manager.h"
#include "ui/win/LayeredWindow.h"
#include "ui/win/LocalWindow.h"
#include "ui/win/menu/ContextMenuWindow.h"
#include "ui/win/menu/MenuElementUI.h"
#include "ui/win/menu/MenuUI.h"
#include "ui/win/menu/MenuWindow.h"
#include "ui/win/menu/ObserverImplBase.hpp"
#include "ui/win/MessageWindow.h"
#include "ui/win/NativeWindow.h"
#include "ui/win/NativeWindowProxy.h"
#include "ui/win/OffscreenDragdrop.h"
#include "ui/win/OffscreenDragdropEvents.h"
#include "ui/win/OffScreenRenderView.h"
#include "ui/win/PictureView.h"
#include "ui/win/RenderView.h"
#include "ui/win/ShadowWindow.h"
#include "ui/win/SharedMemory.h"
#include "ui/win/SplashWindow.h"
#include "ui/win/tray/NotifyWindow.h"
#include "ui/win/tray/Tray.h"
#include "ui/win/tray/TrayBase.h"
#include "ui/win/ViewRenderer.h"
#include "ui/win/WebkitView.h"



#include "context/AppContext.h"


// TODO:  在此处引用程序需要的其他头文件
