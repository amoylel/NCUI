#ifndef AMO_SINGLETON_HPP__
#define AMO_SINGLETON_HPP__


#include <amo/noncopyable.hpp>
#include <amo/memory.hpp>
#include <amo/object.hpp>

namespace amo {

    template<typename T>
    class singleton : public amo::noncopyable
        , public amo::object {
    public:
        static amo::shared_ptr<T> get_instance(bool empty_instance = false) {
            static amo::shared_ptr<T> m_instance;
            
            if (empty_instance) {
                m_instance.reset();
            } else {
                if (!m_instance) {
                    m_instance.reset(new T());
                    m_instance->onCreateSingleInstance();
                }
            }
            
            
            return m_instance;
            
        }
        
        static amo::shared_ptr<T> getInstance() {
            return get_instance();
        }
        
        static void releaseInstance() {
            get_instance(true);
        }
        virtual void onCreateSingleInstance() {
            return;
        }
        
    };
}

#endif // AMO_SINGLETON_HPP__