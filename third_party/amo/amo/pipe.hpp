#ifndef AMO_PIPE_HPP__
#define AMO_PIPE_HPP__


#include <string>
#include <cstdint>
#include <iostream>
#include <amo/config.hpp>
#include <amo/object.hpp>

#ifndef PipeNamePrefix
#define PipeNamePrefix "\\\\.\\pipe\\"
#endif


namespace amo {

    enum pipe_type {
        server,
        client,
    };
    
    class pipe_base : public object {
    public:
        int64_t read(void* buffer, int64_t buffer_size) {
            DWORD cbBytesRead = 0;
            BOOL fSuccess = ReadFile(hPipe, buffer, static_cast<DWORD>(buffer_size), &cbBytesRead, NULL);
            
            if (!fSuccess || cbBytesRead == 0) {
                cbBytesRead = 0;
            }
            
            return static_cast<int64_t>(cbBytesRead);
            
        }
        
        bool is_ready() {
            DWORD cbBytesRead = 0;
            BOOL fSuccess = PeekNamedPipe(hPipe, NULL, 0, NULL, &cbBytesRead, NULL);
            
            if (!fSuccess || cbBytesRead == 0) {
                return FALSE;
            }
            
            return TRUE;
        }
        
        int64_t write(const void* buffer, int64_t buffer_size) {
            DWORD cbBytesWrite = 0;
            BOOL fSuccess = WriteFile(hPipe, buffer, static_cast<DWORD>(buffer_size), &cbBytesWrite, NULL);
            
            if (!fSuccess || cbBytesWrite == 0) {
                cbBytesWrite = 0;
            }
            
            return static_cast<int64_t>(cbBytesWrite);
        }
        
        virtual bool connect() {
            return false;
        }
    protected:
        pipe_base() {
            hPipe = INVALID_HANDLE_VALUE;
        }
    protected:
        HANDLE hPipe;				//管道句柄
        std::string pipe_name;		//管道名称
    };
    
    template<amo::pipe_type> class pipe;
    
    
    template<> class pipe<amo::server> : public pipe_base {
    public:
        pipe(std::string pipe_name_, int buffer_size = 10000000) {
            pipe_name = std::string(PipeNamePrefix) + pipe_name_;
            connected = false;
            //开始创建管道
            hPipe = CreateNamedPipeA(pipe_name.c_str(),     // 管道名
                                     PIPE_ACCESS_DUPLEX,							// 双向读写访问
                                     PIPE_TYPE_BYTE |							// 管道类型，字节流
                                     PIPE_READMODE_BYTE |						// 字节读取模型
                                     PIPE_WAIT,									// blocking mode
                                     PIPE_UNLIMITED_INSTANCES,					// max. instances
                                     buffer_size,								// output buffer size
                                     buffer_size,								// input buffer size
                                     1000,										// client time-out
                                     NULL);										// no security attribute
                                     
            if (hPipe == INVALID_HANDLE_VALUE) {
                set_last_error(1);
                set_valid(true);
            }
            
            return;
        }
        
        ~pipe() {
            //后处理
            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
        }
        
        // 连接管道，阻塞
        virtual bool connect() {
            if (connected) {
                return true;
            }
            
            //连接管道
            BOOL fConnected = (ConnectNamedPipe(hPipe, NULL)) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            
            if (fConnected == TRUE) {
                connected = true;
            } else {
                set_last_error(2);
                set_valid(true);
                connected = false;
            }
            
            return connected;
        }
        
    private:
        bool connected;
    };
    
    
    
    //管道客户端
    
    template<> class pipe<amo::client> : public pipe_base {
    public:
        pipe(std::string pipe_name_) {
            pipe_name = std::string(PipeNamePrefix) + pipe_name_;
        }
        
        // 连接pipe,服务端未开连接失败
        virtual bool connect() {
            if (hPipe != INVALID_HANDLE_VALUE) {
                return true;
            }
            
            BOOL bOK = WaitNamedPipeA(pipe_name.c_str(), NMPWAIT_WAIT_FOREVER);
            
            if (bOK == FALSE) {
                return false;
            }
            
            hPipe = CreateFileA(pipe_name.c_str(), GENERIC_READ | GENERIC_WRITE, \
                                0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                                
            if (hPipe == INVALID_HANDLE_VALUE) {
                set_last_error(3);
                set_valid(true);
                return false;
            }
            
            return true;
        }
        
        
        ~pipe() {
            CloseHandle(hPipe);
        }
    };
    
}

#endif // AMO_PIPE_HPP__
