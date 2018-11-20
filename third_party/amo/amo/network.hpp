/*
 * Copyright (c) 2014, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef AMO_NETWORK_HPP
#define AMO_NETWORK_HPP
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <amo/config.hpp>
#include <amo/stdint.hpp>
#include <iomanip>
#include <amo/utility.hpp>


namespace amo {


#define TYP_INIT 0
#define TYP_SMLE 1
#define TYP_BIGE 2

    /// Convert 64 bit value to network byte order
    /**
     * This method is prefixed to avoid conflicts with operating system level
     * macros for this functionality.
     *
     * TODO: figure out if it would be beneficial to use operating system level
     * macros for this.
     *
     * @param src The integer in host byte order
     * @return src converted to network byte order
     */
    inline uint64_t _htonll(uint64_t src) {
        static int typ = TYP_INIT;
        unsigned char c;
        union {
            uint64_t ull;
            unsigned char c[8];
        } x;
        
        if (typ == TYP_INIT) {
            x.ull = 0x01;
            typ = (x.c[7] == 0x01ULL) ? TYP_BIGE : TYP_SMLE;
        }
        
        if (typ == TYP_BIGE) {
            return src;
        }
        
        x.ull = src;
        c = x.c[0];
        x.c[0] = x.c[7];
        x.c[7] = c;
        c = x.c[1];
        x.c[1] = x.c[6];
        x.c[6] = c;
        c = x.c[2];
        x.c[2] = x.c[5];
        x.c[5] = c;
        c = x.c[3];
        x.c[3] = x.c[4];
        x.c[4] = c;
        return x.ull;
    }
    
    /// Convert 64 bit value to host byte order
    /**
     * This method is prefixed to avoid conflicts with operating system level
     * macros for this functionality.
     *
     * TODO: figure out if it would be beneficial to use operating system level
     * macros for this.
     *
     * @param src The integer in network byte order
     * @return src converted to host byte order
     */
    inline uint64_t _ntohll(uint64_t src) {
        return _htonll(src);
    }
    
    
    
    // Looks for first MAC address of any network device, any size.
    static bool get_any_mac(std::vector<unsigned char> &_node) {
        $windows({
            PIP_ADAPTER_INFO pAdapterInfo;
            PIP_ADAPTER_INFO pAdapter = 0;
            ULONG len = sizeof(IP_ADAPTER_INFO);
            pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
            
            // Make an initial call to GetAdaptersInfo to get
            // the necessary size into len
            DWORD rc = GetAdaptersInfo(pAdapterInfo, &len);
            
            if (rc == ERROR_BUFFER_OVERFLOW) {
                delete[] reinterpret_cast<char*>(pAdapterInfo);
                pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
            } else if (rc != ERROR_SUCCESS) {
                return ("cannot get network adapter list"), false;
            }
            
            bool found = false, gotten = false;
            
            if (GetAdaptersInfo(pAdapterInfo, &len) == NO_ERROR) {
                gotten = true;
                
                pAdapter = pAdapterInfo;
                
                while (pAdapter && !found) {
                    if (pAdapter->Type == MIB_IF_TYPE_ETHERNET && pAdapter->AddressLength > 0) {
                        _node.resize(pAdapter->AddressLength);
                        std::memcpy(_node.data(), pAdapter->Address, _node.size());
                        found = true;
                    }
                    
                    pAdapter = pAdapter->Next;
                }
            }
            
            delete[] reinterpret_cast<char*>(pAdapterInfo);
            
            if (!gotten)
                return ("cannot get network adapter list"), false;
                
            if (!found)
                return ("no Ethernet adapter found"), false;
                
            return true;
        })
        
        $bsd({
            struct ifaddrs* ifaphead;
            int rc = getifaddrs(&ifaphead);
            
            if (rc) return ("cannot get network adapter list"), false;
            
            bool foundAdapter = false;
            for (struct ifaddrs* ifap = ifaphead; ifap; ifap = ifap->ifa_next) {
                if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_LINK) {
                    struct sockaddr_dl* sdl = reinterpret_cast<struct sockaddr_dl*>(ifap->ifa_addr);
                    caddr_t ap = (caddr_t)(sdl->sdl_data + sdl->sdl_nlen);
                    int alen = sdl->sdl_alen;
                    
                    if (ap && alen > 0) {
                        _node.resize(alen);
                        std::memcpy(_node.data(), ap, _node.size());
                        foundAdapter = true;
                        break;
                    }
                }
            }
            freeifaddrs(ifaphead);
            
            if (!foundAdapter) return ("cannot determine MAC address (no suitable network adapter found)"), false;
            return true;
        })
        
        $osx({
            struct ifaddrs* ifaphead;
            int rc = getifaddrs(&ifaphead);
            
            if (rc) return ("cannot get network adapter list"), false;
            
            bool foundAdapter = false;
            for (struct ifaddrs* ifap = ifaphead; ifap; ifap = ifap->ifa_next) {
                if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_LINK) {
                    struct sockaddr_dl* sdl = reinterpret_cast<struct sockaddr_dl*>(ifap->ifa_addr);
                    caddr_t ap = (caddr_t)(sdl->sdl_data + sdl->sdl_nlen);
                    int alen = sdl->sdl_alen;
                    
                    if (ap && alen > 0) {
                        _node.resize(alen);
                        std::memcpy(_node.data(), ap, _node.size());
                        foundAdapter = true;
                        break;
                    }
                }
            }
            freeifaddrs(ifaphead);
            
            if (!foundAdapter) return ("cannot determine MAC address (no suitable network adapter found)"), false;
            return true;
        })
        
        $linux({
            struct ifreq ifr;
            
            int s = ::socket(PF_INET, SOCK_DGRAM, 0);
            
            if (s == -1) return ("cannot open socket"), false;
            
            std::strcpy(ifr.ifr_name, "eth0");
            int rc = ioctl(s, SIOCGIFHWADDR, &ifr);
            close(s);
            if (rc < 0) return ("cannot get MAC address"), false;
            struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&ifr.ifr_addr);
            _node.resize(sizeof(sa->sa_data));
            std::memcpy(_node.data(), sa->sa_data, _node.size());
            return true;
        })
        
        $unix({
            char name[MAXHOSTNAMELEN];
            
            if (gethostname(name, sizeof(name)))
                return ("cannot get host name"), false;
                
            struct hostent* pHost = gethostbyname(name);
            if (!pHost) return ("cannot get host IP address"), false;
            
            int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (s == -1) return ("cannot open socket"), false;
            
            struct arpreq ar;
            std::memset(&ar, 0, sizeof(ar));
            struct sockaddr_in* pAddr = reinterpret_cast<struct sockaddr_in*>(&ar.arp_pa);
            pAddr->sin_family = AF_INET;
            std::memcpy(&pAddr->sin_addr, *pHost->h_addr_list, sizeof(struct in_addr));
            int rc = ioctl(s, SIOCGARP, &ar);
            close(s);
            if (rc < 0) return ("cannot get MAC address"), false;
            _node.resize(sizeof(ar.arp_ha.sa_data));
            std::memcpy(_node.data(), ar.arp_ha.sa_data, _node.size());
            return true;
        })
    }
    
    // Looks for first MAC address of any network device, size truncated to 48bits.
    static uint64_t get_any_mac48() {
        std::vector<unsigned char> node;
        
        if (get_any_mac(node)) {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            node.resize(6);
            
            for (unsigned i = 0; i < 6; ++i) {
                ss << std::setw(2) << int(node[i]);
            }
            
            uint64_t t;
            
            if (ss >> t) {
                return t;
            }
        }
        
        return 0;
    }
    
    
    static bool is_valid_ip(const std::string& ip) {
        return true;
        /*using namespace boost::xpressive;
        cregex reg_ip = cregex::compile("(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])");
        
        return regex_match(ip.c_str(), reg_ip);*/
    }
    
    static bool is_empty_ip(const std::string& ip) {
        return ip == std::string("0.0.0.0");
    }
    
    
    
    
#ifdef WIN32
    typedef struct tagIpAdapterInfo {
        std::string name;			//网卡名称
        std::string description;	//网卡描述
        uint32_t type;				//网卡类型
        std::string addr;			//IP地址
        std::string mac;			//MAC
        std::string mask;			//子网掩码
        std::string gateway;		//网关
        
        bool active() {
            return is_valid_ip(gateway) && !is_empty_ip(gateway);
            //return !gateway.empty();
        }
        
    } IpAdapterInfo;
    
    static std::vector<IpAdapterInfo> get_adapters() {
        std::vector<IpAdapterInfo> rec;
        //PIP_ADAPTER_INFO结构体指针存储本机网卡信息
        PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
        //得到结构体大小,用于GetAdaptersInfo参数
        unsigned long stSize = sizeof(IP_ADAPTER_INFO);
        //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
        int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
        //记录网卡数量
        int netCardNum = 0;
        //记录每张网卡上的IP地址数量
        int IPnumPerNetCard = 0;
        
        if (ERROR_BUFFER_OVERFLOW == nRel) {
            //如果函数返回的是ERROR_BUFFER_OVERFLOW
            //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
            //这也是说明为什么stSize既是一个输入量也是一个输出量
            //释放原来的内存空间
            delete pIpAdapterInfo;
            //重新申请内存空间用来存储所有网卡信息
            pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
            //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
            nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
        }
        
        if (ERROR_SUCCESS == nRel) {
            //输出网卡信息
            //可能有多网卡,因此通过循环去判断
            while (pIpAdapterInfo) {
                IpAdapterInfo adapter_info;
                adapter_info.name = pIpAdapterInfo->AdapterName;
                adapter_info.description = pIpAdapterInfo->Description;
                adapter_info.type = pIpAdapterInfo->Type;
                
                for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++) {
                    if (i < pIpAdapterInfo->AddressLength - 1) {
                        char tmp[4] = { 0 };
                        ::sprintf(tmp, "%02x-", pIpAdapterInfo->Address[i]);
                        adapter_info.mac += tmp;
                        //adapter_info.mac += (boost::format("%02x-") % pIpAdapterInfo->Address[i]).str();
                    } else {
                        char tmp[4] = { 0 };
                        ::sprintf(tmp, "%02x", pIpAdapterInfo->Address[i]);
                        adapter_info.mac += tmp;
                        //adapter_info.mac += (boost::format("%02x") % pIpAdapterInfo->Address[i]).str();
                    }
                }
                
                //可能网卡有多IP,因此通过循环去判断
                IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
                
                do {
                    adapter_info.addr = pIpAddrString->IpAddress.String;
                    adapter_info.mask = pIpAddrString->IpMask.String;
                    adapter_info.gateway = pIpAdapterInfo->GatewayList.IpAddress.String;
                    
                    pIpAddrString = pIpAddrString->Next;
                } while (pIpAddrString);
                
                rec.push_back(adapter_info);
                pIpAdapterInfo = pIpAdapterInfo->Next;
                
            }
            
        }
        
        //释放内存空间
        if (pIpAdapterInfo) {
            delete pIpAdapterInfo;
        }
        
        return rec;
    }
    
    
    class IpAdapterInfos : public std::vector < IpAdapterInfo > {
    public:
        IpAdapterInfos() {
            std::vector<IpAdapterInfo> vecs = get_adapters();
            std::copy(vecs.begin(), vecs.end(), std::back_inserter(*this));
        }
        
        
        iterator get_best_adapter() {
            IpAdapterInfos::iterator iter = begin();
            
            for (; iter != end(); ++iter) {
                if (iter->active()) {
                    break;
                }
            }
            
            return iter;
        }
        
    };
#endif
    
    
    
    
    static int32_t address_to_number(const std::string& strIP) {
        //IP转化为数值
        //没有格式检查
        //返回值就是结果
        
        int32_t a[4];
        std::string IP = strIP;
        std::string strTemp;
        size_t pos;
        size_t i = 3;
        
        do {
            pos = IP.find(".");
            
            if (pos != std::string::npos) {
                strTemp = IP.substr(0, pos);
                a[i] = atoi(strTemp.c_str());
                i--;
                IP.erase(0, pos + 1);
            } else {
                strTemp = IP;
                a[i] = atoi(strTemp.c_str());
                break;
            }
            
        } while (1);
        
        int32_t nResult = (a[3] << 24) + (a[2] << 16) + (a[1] << 8) + a[0];
        return nResult;
    }
    
    static std::string number_to_address(const int32_t& nValue) {
        //数值转化为IP
        //没有格式检查
        //返回值就是结果
        
        char strTemp[20];
        ::sprintf(strTemp, "%d.%d.%d.%d",
                  (nValue & 0xff000000) >> 24,
                  (nValue & 0x00ff0000) >> 16,
                  (nValue & 0x0000ff00) >> 8,
                  (nValue & 0x000000ff));
        return std::string(strTemp);
    }
    
    static std::pair<int32_t, int32_t> get_ip_scope(const std::string& ip,
            const std::string& mask) {
        int32_t ip_number = 0;
        int32_t net_mask_number = 0, ip_count = 0;
        
        ip_number = address_to_number(ip);
        
        if (mask.empty()) {
            return std::pair<int32_t, int32_t>(ip_number, ip_number);
        }
        
        net_mask_number = address_to_number(mask);
        ip_count = address_to_number("255.255.255.255") - net_mask_number;
        int32_t net_ip = ip_number & net_mask_number;
        int32_t host_scope = net_ip + ip_count;
        return std::pair<int32_t, int32_t>(net_ip, host_scope);
    }
    
} // amo


#endif // AMO_NETWORK_HPP
