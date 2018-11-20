// Created by amoylel on 06/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_HTTP_A29D8D02_DB5E_4810_89A2_554057E28584_HPP__
#define AMO_HTTP_A29D8D02_DB5E_4810_89A2_554057E28584_HPP__



#include <algorithm>
#include <fstream>
#include <vector>
#include <functional>
#include <memory>

#include <amo/looper_executor.hpp>

#include <amo/asio/http/server_http.hpp>
#include <amo/asio/http/client_http.hpp>

#include <amo/uid.hpp>


// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <boost/regex.hpp>
// Added for the default_resource example

#include <boost/filesystem.hpp>
#ifdef HAVE_OPENSSL
#include <amo/asio/http/crypto.hpp>
#endif

#ifndef CALLBACK_SERVER_FUNC_PARAMS
#define CALLBACK_SERVER_FUNC_PARAMS amo::http::server::SmartResponse response, amo::http::server::SmartRequest request
#endif

#ifndef CALLBACK_CLIENT_FUNC_PARAMS
#define CALLBACK_CLIENT_FUNC_PARAMS amo::http::client::SmartResponse response, const amo::http::client::ErrorCodeType & ec, int64_t uid
#endif



#ifndef REGISTER_EXT_HTTP_RESOURCE_BEGIN
#define REGISTER_EXT_HTTP_RESOURCE_BEGIN(ClassName, ptr2, http_ptr2)\
		{\
			typedef ClassName ClassType;\
			const auto& ptr = ptr2;\
			const auto& http_ptr = http_ptr2;
#endif

#ifndef REGISTER_EXT_HTTP_RESOURCE_END
#define REGISTER_EXT_HTTP_RESOURCE_END()\
		}
#endif


#ifndef REGISTER_HTTP_RESOURCE_BEGIN
#define REGISTER_HTTP_RESOURCE_BEGIN(ClassName, ptr)\
	    REGISTER_EXT_HTTP_RESOURCE_BEGIN(ClassName, ptr, this)

#endif

#ifndef REGISTER_HTTP_RESOURCE_END
#define REGISTER_HTTP_RESOURCE_END()\
		}
#endif


#ifndef REGISTER_DEFAULT_RESOURCE
#define REGISTER_DEFAULT_RESOURCE(func)\
http_ptr->set_default_resouce("GET", std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));\
http_ptr->set_default_resouce("POST", std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_DEFAULT_POST_RESOURCE
#define REGISTER_DEFAULT_POST_RESOURCE(func)\
http_ptr->set_default_resouce("POST", std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif


#ifndef REGISTER_DEFAULT_GET_RESOURCE
#define REGISTER_DEFAULT_GET_RESOURCE(func)\
http_ptr->set_default_resouce("GET", std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif


#ifndef REGISTER_HTTP_RESOURCE
#define REGISTER_HTTP_RESOURCE(func)\
http_ptr->add_post_resource(std::string("^/") + #func + "$", std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));\
http_ptr->add_get_resource(std::string("^/") + #func + "$", std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_POST_RESOURCE
#define REGISTER_POST_RESOURCE(func)\
http_ptr->add_post_resource(std::string("^/") + #func + "$",  std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif


#ifndef REGISTER_GET_RESOURCE
#define REGISTER_GET_RESOURCE(func)\
http_ptr->add_get_resource(std::string("^/") + #func + "$",std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif


#ifndef REGISTER_REG_HTTP_RESOURCE
#define REGISTER_REG_HTTP_RESOURCE(reg, func)\
http_ptr->add_post_resource( reg, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));\
http_ptr->add_get_resource( reg, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_REG_POST_RESOURCE
#define REGISTER_REG_POST_RESOURCE(reg, func)\
http_ptr->add_post_resource( reg,  std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif


#ifndef REGISTER_REG_GET_RESOURCE
#define REGISTER_REG_GET_RESOURCE(reg, func)\
http_ptr->add_get_resource( reg,std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_BEFORE_HANDLE_MSG_RESOURCE
#define REGISTER_BEFORE_HANDLE_MSG_RESOURCE(func)\
http_ptr->set_before_handle_msg_callback(std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_AFTER_HANDLE_MSG_RESOURCE
#define REGISTER_AFTER_HANDLE_MSG_RESOURCE(func)\
http_ptr->set_after_handle_msg_callback(std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
#endif

namespace amo {
    namespace http {
        class server {
        public:
            typedef  SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
            typedef std::function
            < void(std::shared_ptr<HttpServer::Response>, std::shared_ptr<HttpServer::Request>) >
            ResourceType;
            typedef std::function<std::shared_ptr<boost::any>(std::shared_ptr<HttpServer::Response>, std::shared_ptr<HttpServer::Request>)>
            BeforeHandleMsgFunc;
            typedef std::function
            < void(std::shared_ptr<boost::any>, std::shared_ptr<HttpServer::Response>, std::shared_ptr<HttpServer::Request>) >
            AfterHandleMsgFunc;
            typedef HttpServer::Response Response;
            typedef HttpServer::Request Request;
            typedef std::shared_ptr<Response> SmartResponse;
            typedef std::shared_ptr<Request> SmartRequest;
            
        public:
            server() {
                m_is_running = false;
                m_http_server.config.port = 8080;
                
                REGISTER_HTTP_RESOURCE_BEGIN(server, this)
                REGISTER_DEFAULT_POST_RESOURCE(default_post_method)
                REGISTER_DEFAULT_GET_RESOURCE(default_get_method)
                REGISTER_HTTP_RESOURCE_END()
                /*set_default_resouce("GET", std::bind(&server::default_get_method, this,
                                                     std::placeholders::_1, std::placeholders::_2));
                set_default_resouce("POST", std::bind(&server::default_post_method, this,
                                                      std::placeholders::_1, std::placeholders::_2));*/
            }
            
            
            ~server() {
                stop_service();
            }
            HttpServer::Config& config() {
                return m_http_server.config;
            }
            
            const HttpServer::Config& config() const {
                return m_http_server.config;
            }
            
            bool add_get_resource(const std::string& reg, ResourceType fn) {
                return add_resource(reg, "GET", fn);
            }
            
            bool add_post_resource(const std::string& reg, ResourceType fn) {
                return add_resource(reg, "POST", fn);
            }
            
            bool add_resource(const std::string& reg, const std::string& method,
                              ResourceType fn) {
                              
                              
                ResourceType callback = std::bind(&server::on_handle_http_method,
                                                  this,
                                                  fn,
                                                  std::placeholders::_1,
                                                  std::placeholders::_2);
                m_http_server.resource[reg][method] = callback;
                return true;
            }
            
            void remove_resouce(const std::string& reg, const std::string& method) {
                auto iter = m_http_server.resource.find(reg);
                
                if (iter == m_http_server.resource.end()) {
                    return;
                }
                
                iter->second.erase(method);
                
                return;
            }
            
            void set_default_resouce(const std::string& method, ResourceType fn) {
                ResourceType callback = std::bind(&server::on_handle_http_method,
                                                  this,
                                                  fn,
                                                  std::placeholders::_1,
                                                  std::placeholders::_2);
                m_http_server.default_resource[method] = callback;
            }
            
            void set_on_error(std::function<void(std::shared_ptr<HttpServer::Request>,
                                                 const SimpleWeb::error_code)> fn) {
                m_http_server.on_error = fn;
            }
            
            
            
            void start_service() {
                if (m_is_running) {
                    return;
                }
                
                if (!m_executor) {
                    m_executor.reset(new amo::looper_executor());
                }
                
                m_executor->execute(std::bind(&server::run_service, this));
                
            }
            
            void run_service() {
                if (m_is_running) {
                    return;
                }
                
                m_is_running = true;
                
                m_http_server.start();
                
            }
            
            void stop_service() {
                if (!m_is_running) {
                    return;
                }
                
                m_http_server.stop();
                m_is_running = false;
                
                if (m_executor) {
                    m_executor->stop();
                }
            }
            
            bool is_running() const {
                return  m_is_running;
            }
            
            
            // //////////////////////////////////////////////////////////////////////////
            //
            
            // 处理消息之前先调用该函数
            virtual std::shared_ptr<boost::any> before_handle_msg(
                std::shared_ptr<HttpServer::Response> response,
                std::shared_ptr<HttpServer::Request> request) {
                if (m_fn_before_handle_msg) {
                    return m_fn_before_handle_msg(response, request);
                }
                
                return {};
            }
            
            // 处理完消息后调用该函数
            virtual void after_handle_msg(std::shared_ptr<boost::any> any,
                                          std::shared_ptr<HttpServer::Response> response,
                                          std::shared_ptr<HttpServer::Request> request) {
                if (m_fn_after_handle_msg) {
                    return m_fn_after_handle_msg(any, response, request);
                }
                
                return ;
            }
            
            
            void set_before_handle_msg_callback(BeforeHandleMsgFunc fn) {
                m_fn_before_handle_msg = fn;
            }
            
            void set_after_handle_msg_callback(AfterHandleMsgFunc fn) {
                m_fn_after_handle_msg = fn;
            }
        protected:
            void default_get_method(std::shared_ptr<HttpServer::Response> response,
                                    std::shared_ptr<HttpServer::Request> request) {
                response->write("invalid request");
            }
            void default_post_method(std::shared_ptr<HttpServer::Response> response,
                                     std::shared_ptr<HttpServer::Request> request) {
                response->write("invalid request");
            }
            
            void on_handle_http_method(ResourceType fn,
                                       std::shared_ptr<HttpServer::Response> response,
                                       std::shared_ptr<HttpServer::Request> request) {
                std::shared_ptr<boost::any> val;
                val = before_handle_msg(response, request);
                
                if (fn) {
                    fn(response, request);
                }
                
                return after_handle_msg(val, response, request);
            }
            
            
        public:
            HttpServer m_http_server;
            std::atomic<bool> m_is_running;
            std::shared_ptr<amo::looper_executor> m_executor;
            BeforeHandleMsgFunc m_fn_before_handle_msg;
            AfterHandleMsgFunc  m_fn_after_handle_msg;
        };
        
        
        class client : public log_object {
        public:
            using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;
            typedef std::function<void(std::shared_ptr<HttpClient::Response>,
                                       const SimpleWeb::error_code &, int64_t)> ResponeType;
                                       
            typedef std::shared_ptr<HttpClient::Response> SmartResponse;
            typedef SimpleWeb::error_code ErrorCodeType;
            
            class HttpViewBase {
            public:
                HttpViewBase() {
                    uid = amo::uid();
                    retry_count = 1;
                }
                
                std::unordered_map<std::string, std::string> parse_url() {
                    auto map = parse_url(url);
                    protocol = map["protocol"];
                    host = map["host"];
                    port = map["port"];
                    path = map["path"];
                    file = map["file"];
                    parameters = map["parameters"];
                    
                    amo::string_utils::trim_left(path, " \t/");
                    amo::string_utils::trim_right(path, " \t/");
                    
                    amo::string_utils::trim_left(parameters, " \t?");
                    
                    return map;
                    
                }
                
                std::unordered_map<std::string, std::string> parse_url(const std::string & url)
                const {
                    std::string src = url;
                    bool sub_url  = false;
                    
                    if (src.find("/") == 0) {
                        src = "http://localhost" + src;
                        sub_url = true;
                    }
                    
                    std::unordered_map<std::string, std::string> retval;
                    retval.insert({"protocol", ""});
                    retval.insert({ "host", "" });
                    retval.insert({ "port", "" });
                    retval.insert({ "path", "" });
                    retval.insert({ "file", "" });
                    retval.insert({ "parameters", "" });
                    
                    if (src.empty()) {
                        return retval;
                    }
                    
                    std::vector<std::string> values;
                    boost::regex expression(
                        //   proto                 host               port
                        "^(\?:([^:/\?#]+)://)\?(\\w+[^/\?#:]*)(\?::(\\d+))\?"
                        //   path                  file       parameters
                        "(/\?(\?:[^\?#/]*/)*)\?([^\?#]*)\?(\\\?(.*))\?"
                    );
                    
                    // http://127.0.0.1:8080/path/file?id=2323&name=haha
                    // proto://ip:port/path/file?parameters
                    // 如果port后面直接就是file的话，那么path为空
                    
                    if (boost::regex_split(std::back_inserter(values), src, expression)) {
                        const char* names[] = { "protocol", "host", "port", "path", "file",
                                                "parameters", NULL
                                              };
                                              
                        for (int i = 0; names[i]; i++) {
                            retval[names[i]] = values[i];
                            //::printf("%s: %s\n", names[i], values[i].c_str());
                        }
                    }
                    
                    if (sub_url) {
                        retval["protocol"] = "";
                        retval["host"] = "";
                        retval["port"] = "";
                    }
                    
                    return retval;
                }
                
                std::string get_server_port_path(bool need_protocol = false) const {
                    std::string str_protocol = protocol;
                    std::string str_host = host;
                    std::string str_port = port;
                    
                    if (str_protocol.empty()) {
                        str_protocol = "http";
                    }
                    
                    if (str_host.empty()) {
                        return "";
                    }
                    
                    if (str_port.empty()) {
                        str_port = "80";
                    }
                    
                    
                    if (need_protocol) {
                        return str_protocol + "://" + str_host + ":" + str_port;
                    } else {
                        return /*str_protocol + "://" +*/ str_host + ":" + str_port;
                    }
                    
                }
                
                std::string get_sub_url(bool front_blank = true) const {
                    std::string retval = path + "/" + file;
                    
                    if (!parameters.empty()) {
                        retval += "?";
                        retval += parameters;
                    }
                    
                    
                    amo::string_utils::trim_left(retval, "/");
                    
                    // 如果只有一个问号，那么加不加/都没有意义
                    if (retval == "?") {
                        return "";
                    }
                    
                    if (front_blank) {
                        retval = "/" + retval;
                    }
                    
                    amo::string_utils::trim_right(retval, "/");
                    
                    return retval;
                    
                }
                
                
                
            public:
                std::string  url;		// URL地址，可以是完整的地址，也可以不是
                std::string method;		// 请求类型
                SimpleWeb::CaseInsensitiveMultimap header;  // 头部
                std::string content;	// 请求内容
                
                
                std::string protocol;	// HTTP 协议
                std::string host;		// 主机名
                std::string port;		// 端口号
                
                
                std::string path;		// URL PORT后到最file之前的路径
                std::string file;		// URL 最后一个/到？之间的路径
                std::string parameters;	// URL？及后面的内容
                
                int64_t uid;			// msgID
                int retry_count;
                
            };
            
            class HttpCView : public HttpViewBase {
            public:
                HttpCView() {
                    method = "GET";
                }
                
                HttpCView(const std::string& url_) {
                    url = url_;
                    method = "GET";
                    parse_url();
                }
                
                HttpCView(const std::string& url_, const std::string& method_) {
                    url = url_;
                    method = method_;
                    parse_url();
                }
                
                HttpCView(const HttpCView& rhs) {
                    url = rhs.url;
                    method = rhs.method;
                    header = rhs.header;
                    content = rhs.content;
                    
                    protocol = rhs.protocol;
                    host = rhs.host;
                    port = rhs.port;
                    
                    path = rhs.path;
                    file = rhs.file;
                    parameters = rhs.parameters;
                    
                    uid = rhs.uid;
                    retry_count = rhs.retry_count;
                    
                }
                
                
                
            };
            
            class HttpView : public HttpViewBase {
            public:
                HttpView() {
                    method = "GET";
                }
                
                HttpView(const std::string& content_) {
                    content = content_;
                }
                HttpView(const char* content_) {
                    content = content_;
                }
                HttpView(const HttpView& rhs) {
                    url = rhs.url;
                    method = rhs.method;
                    header = rhs.header;
                    content = rhs.content;
                    
                    protocol = rhs.protocol;
                    host = rhs.host;
                    port = rhs.port;
                    
                    path = rhs.path;
                    file = rhs.file;
                    parameters = rhs.parameters;
                    
                    uid = rhs.uid;
                    retry_count = rhs.retry_count;
                }
                
                HttpCView toCview() const {
                    HttpCView view;
                    view.url = this->url;
                    view.method = this->method;
                    view.header = this->header;
                    view.content = this->content;
                    
                    view.protocol = this->protocol;
                    view.host = this->host;
                    view.port = this->port;
                    
                    view.path = this->path;
                    view.file = this->file;
                    view.parameters = this->parameters;
                    
                    view.uid = this->uid;
                    view.retry_count = this->retry_count;
                    
                    return view;
                }
                
            };
            
        public:
            class Config {
            public:
                /// Set timeout on requests in seconds. Default value: 0 (no timeout).
                long timeout = 0;
                /// Set connect timeout in seconds. Default value: 0 (Config::timeout is then used instead).
                long timeout_connect = 0;
                /// Maximum size of response stream buffer. Defaults to architecture maximum.
                /// Reaching this limit will result in a message_size error code.
                std::size_t max_response_streambuf_size =
                    (std::numeric_limits<std::size_t>::max)();
                /// Set proxy server (server:port)
                std::string proxy_server;
            };
            
        public:
            client() {
            
            }
            client(HttpCView args) : m_args(args) {
            
            }
            
            // 同步请求
            std::shared_ptr<HttpClient::Response> request() {
                for (int i = 0; i < m_args.retry_count; ++i) {
                    try {
                        return  get_http_client()->request(m_args.method, m_args.get_sub_url(),
                                                           m_args.content,
                                                           m_args.header);
                    } catch (const std::exception& e) {
                        $err("http request error: {}", e.what());
                    }
                }
                
                return{};
            }
            
            std::shared_ptr<HttpClient::Response> request(const HttpView& http_view) {
            
                for (int i = 0; i < http_view.retry_count; ++i) {
                    try {
                        auto http_client = get_http_client();
                        
                        const auto& p = http_view;
                        
                        std::string method = p.method.empty() ? m_args.method :
                                             p.method;
                        std::string path_file = p.get_sub_url();
                        
                        if (path_file.empty()) {
                            path_file = m_args.get_sub_url();
                        }
                        
                        
                        return http_client->request(m_args.method,
                                                    path_file,
                                                    p.content,
                                                    m_args.header);
                                                    
                    } catch (const std::exception& e) {
                        $err("http request error: {}", e.what());
                        continue;
                    }
                    
                }
                
                return{};
                
                
            }
            
            
            std::shared_ptr<HttpClient::Response> request(std::istream &content) {
                for (int i = 0; i < m_args.retry_count; ++i) {
                    try {
                        return get_http_client()->request(m_args.method,
                                                          m_args.get_sub_url(), content,
                                                          m_args.header);
                                                          
                    } catch (const std::exception& e) {
                        $err("http request error: {}", e.what());
                    }
                }
                
                return{};
            }
            
            
            // 异步请求
            void request(ResponeType   request_callback) {
                for (int i = 0; i < m_args.retry_count; ++i) {
                    try {
                        auto http_client = get_http_client();
                        
                        auto response = std::bind(&client::on_http_request_calback,
                                                  this,
                                                  request_callback,
                                                  m_args.uid,
                                                  std::placeholders::_1,
                                                  std::placeholders::_2);
                        http_client->request(m_args.method, m_args.get_sub_url(), m_args.content,
                                             m_args.header,
                                             std::move(response));
                        http_client->io_service->run();
                    } catch (const std::exception& e) {
                        $err("http request error: {}", e.what());
                    }
                }
                
                return;
            }
            
            
            void request(std::istream & content,
                         ResponeType  request_callback) {
                auto fn_callback = std::bind(&client::on_http_request_calback,
                                             this,
                                             request_callback,
                                             m_args.uid,
                                             std::placeholders::_1,
                                             std::placeholders::_2);
                SimpleWeb::error_code error_code;
                SmartResponse smart_response;
                
                for (int i = 0; i < m_args.retry_count; ++i) {
                
                    try {
                    
                        auto http_client = get_http_client();
                        
                        http_client->request(m_args.method, m_args.get_sub_url(), content,
                                             m_args.header,
                        [&](SmartResponse response, const SimpleWeb::error_code & ec) {
                            smart_response = response;
                            error_code = ec;
                        });
                        http_client->io_service->run();
                        
                        if (!error_code) {
                            fn_callback(smart_response, error_code);
                        }
                        
                    } catch (const SimpleWeb::system_error & e) {
                        $err("http request error: {}", e.what());
                    }
                }
                
                fn_callback(smart_response, error_code);
                
            }
            
            void request(const std::vector<HttpView>& views,
                         ResponeType   request_callback = {}) {
                         
                if (views.empty()) {
                    return;
                }
                
                auto http_client = get_http_client();
                
                for (auto& p : views) {
                
                    std::string method = p.method.empty() ? m_args.method : p.method;
                    std::string path_file = p.get_sub_url();
                    
                    if (path_file.empty()) {
                        path_file = m_args.get_sub_url();
                    }
                    
                    std::string content = p.content;
                    SmartResponse response;
                    SimpleWeb::error_code error_code;
                    
                    for (int i = 0; i < p.retry_count; ++i) {
                        try {
                            response = http_client->request(method, path_file, content,
                                                            p.header);
                        } catch (const    SimpleWeb::system_error & e) {
                            error_code = e.code();
                        }
                        
                        if (!error_code) {
                            if (request_callback) {
                                request_callback(response, error_code, p.uid);
                            }
                            
                            break;
                        }
                    }
                    
                    if (error_code) {
                        if (request_callback) {
                            request_callback(response, error_code, p.uid);
                        }
                    }
                }
                
                
            }
            
            void async_request(const std::vector<HttpView>& views,
                               ResponeType   request_callback) {
                               
                try {
                    if (views.empty()) {
                        return;
                    }
                    
                    auto http_client = get_http_client();
                    
                    for (auto& p : views) {
                        auto fn_callback = std::bind(&client::on_http_request_calback,
                                                     this,
                                                     request_callback,
                                                     p.uid,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2);
                                                     
                        SimpleWeb::error_code error_code;
                        SmartResponse smart_response;
                        
                        
                        std::string method = p.method.empty() ? m_args.method : p.method;
                        std::string path_file = p.get_sub_url();
                        
                        if (path_file.empty()) {
                            path_file = m_args.get_sub_url();
                        }
                        
                        std::string content = p.content;
                        
                        
                        for (int i = 0; i < p.retry_count; ++i) {
                            http_client->request(m_args.method, path_file, content, m_args.header,
                            
                            [&](SmartResponse response, const SimpleWeb::error_code & ec) {
                                smart_response = response;
                                error_code = ec;
                            });
                            http_client->io_service->run();
                            
                            if (!error_code) {
                                fn_callback(smart_response, error_code);
                            }
                        }
                        
                        if (error_code) {
                            fn_callback(smart_response, error_code);
                        }
                        
                    }
                    
                    
                } catch (const std::exception& e) {
                    $err("http request error: {}", e.what());
                }
                
                
            }
            
            void async_request(const HttpView& view, ResponeType  request_callback) {
            
                std::vector<HttpView> views;
                views.push_back(view);
                return async_request(views, request_callback);
            }
            
            client& method(const std::string& val) {
                m_args.method = val;
                return *this;
            }
            
            HttpCView& view() {
                return m_args;
            }
            
            const HttpCView& view() const {
                return m_args;
            }
            
            client& url(const std::string& url) {
                m_args.url = url;
                return *this;
            }
            
            client& header(const std::unordered_map<std::string, std::string>& args) {
                for (auto& p : args) {
                    m_args.header.insert({ p.first, p.second });
                }
                
                return *this;
            }
            
            Config& config() {
                return m_config;
            }
            
            const Config& config() const {
                return m_config;
            }
        protected:
        
            std::shared_ptr< HttpClient> get_http_client() {
            
                m_args.parse_url();
                
                std::string host_port = m_args.get_server_port_path(false);
                
                if (!m_http_client || host_port != m_host_port) {
                
                    // 保存url
                    m_host_port = m_args.get_server_port_path(false);
                    m_http_client.reset(new HttpClient(m_host_port));
                    m_http_client->config.timeout = m_config.timeout;
                    m_http_client->config.timeout_connect = m_config.timeout_connect;
                    m_http_client->config.max_response_streambuf_size =
                        m_config.max_response_streambuf_size;
                    m_http_client->config.proxy_server = m_config.proxy_server;
                }
                
                
                return m_http_client;
            }
            
            
            /*std::string get_path_file() const {
                return get_path_file(m_args);
            }
            
            std::string get_path_file(const HttpView& view) const {
            
                std::string path = view.path;
                std::string file = view.file;
            
                if (path.empty() && file.empty()) {
                    return "";
                }
            
                std::string parameters = view.parameters;
                return path + file + parameters;
            }*/
            
            std::string get_method() {
                return m_args.method;
            }
            
            void on_http_request_calback(ResponeType fn, int64_t uid,
                                         SmartResponse response,
                                         const SimpleWeb::error_code & ec) {
                if (fn) {
                    fn(response, ec, uid);
                }
                
                return;
            }
        protected:
            Config m_config;
            HttpCView m_args;
            std::shared_ptr< HttpClient> m_http_client;
            std::string m_host_port;
        };
    }
}

#endif // AMO_HTTP_A29D8D02_DB5E_4810_89A2_554057E28584_HPP__

