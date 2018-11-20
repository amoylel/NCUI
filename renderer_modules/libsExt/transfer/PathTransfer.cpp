#include "stdafx.h"
#include "transfer/PathTransfer.h"
#include <amo/string.hpp>
#include <amo/path.hpp>


namespace amo {

    PathTransfer::PathTransfer()
        : RunnableTransfer("Path") {
        m_pPath.reset(new u8path());
    }
    
    PathTransfer::PathTransfer(const std::string& u8Path)
        : RunnableTransfer("Path") {
        
        m_pPath.reset(new u8path(u8Path));
        
    }
    
    PathTransfer::~PathTransfer() {
    
    }
    
    Any PathTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string u8Path = args->getString(0);
        
        std::shared_ptr<PathTransfer> pTransfer(new PathTransfer(u8Path));
        
        pTransfer->registerFunction();
        addTransfer(pTransfer);
        pTransfer->setTriggerEventFunc(getTriggerEventFunc());
        return  pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    
    
    Any PathTransfer::toString(IPCMessage::SmartType msg) {
        amo::u8string ansiPath(m_pPath->raw_string(), true);
        //TransferEventInfo info(amo::u8string("success", true), false, true);
        //info.data = "fffff";
        //broadcastEvent(info);
        //triggerEvent(info);
        return ansiPath.to_utf8();
    }
    
    Any PathTransfer::normalize(IPCMessage::SmartType msg) {
        m_pPath->normalize();
        // 返回this
        return getFuncMgr().toSimplifiedJson();
    }
    
    Any PathTransfer::isExsit(IPCMessage::SmartType msg) {
        return m_pPath->file_exists();
    }
    
    Any PathTransfer::append(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string ansiPath(args->getString(0), true);
        
        amo::u8path p(ansiPath);
        m_pPath->append(p);
        
        // 返回this
        return getFuncMgr().toSimplifiedJson();
    }
    
    Any PathTransfer::toAbsolute(IPCMessage::SmartType msg) {
        m_pPath->to_absolute();
        return getFuncMgr().toSimplifiedJson();
    }
    
    Any PathTransfer::removeFileSpec(IPCMessage::SmartType msg) {
        m_pPath->remove_file_spec();
        return getFuncMgr().toSimplifiedJson();
    }
    
    Any PathTransfer::getFullAppDir(IPCMessage::SmartType msg) {
        amo::u8string u8Path(amo::u8path::getExeDir(), true);
        
        IPCMessage::SmartType cloneMsg = msg->clone();
        cloneMsg->getArgumentList()->setValue(0, u8Path.to_utf8());
        return onCreateClass(cloneMsg);
    }
    
    Any PathTransfer::getFullAppName(IPCMessage::SmartType msg) {
        amo::u8string ansiPath(amo::u8path::getFullExeName(), true);
        IPCMessage::SmartType cloneMsg = msg->clone();
        cloneMsg->getArgumentList()->setValue(0, ansiPath.to_utf8());
        return onCreateClass(cloneMsg);
    }
    
    
    Any PathTransfer::fileExsit(IPCMessage::SmartType msg) {
        amo::u8string strPath(msg->getArgumentList()->getString(0), true);
        return amo::u8path(strPath).file_exists();
    }
    
    Any PathTransfer::Remove(IPCMessage::SmartType msg) {
        amo::u8string strPath(msg->getArgumentList()->getString(0), true);
        return amo::u8path(strPath).remove();
    }
    
    Any PathTransfer::RemoveAll(IPCMessage::SmartType msg) {
        amo::u8string strPath(msg->getArgumentList()->getString(0), true);
        return amo::u8path(strPath).remove_all();
    }
    
    Any PathTransfer::copyTo(IPCMessage::SmartType msg) {
        if (!m_pPath) {
            return false;
        }
        
        amo::u8string p(msg->getArgumentList()->getString(0), true);
        
        if (p.empty()) {
            return false;
        }
        
        amo::u8path dstPath(p);
        return m_pPath->copy_to(dstPath);
    }
    
    Any PathTransfer::moveTo(IPCMessage::SmartType msg) {
        if (!m_pPath) {
            return false;
        }
        
        amo::u8string p(msg->getArgumentList()->getString(0), true);
        
        if (p.empty()) {
            return false;
        }
        
        amo::u8path dstPath(p);
        return m_pPath->move_to(dstPath.generic_wstring());
    }
    
    Any PathTransfer::remove(IPCMessage::SmartType msg) {
        return m_pPath->remove();
    }
    
    Any PathTransfer::removeAll(IPCMessage::SmartType msg) {
        return m_pPath->remove_all();
    }
    
    std::string PathTransfer::getClass() const {
        return ("Path");
    }
    
    Transfer* PathTransfer::getInterface(const std::string& name) {
        if (name == PathTransfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
}

