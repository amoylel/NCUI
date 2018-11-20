#ifndef AMO_VARIANT_HPP__
#define AMO_VARIANT_HPP__




#include <amo/any.hpp>
namespace amo
{
	template<typename T, typename P>
	class variant
	{
	public:
		struct base {
			virtual ~base() {}
			virtual amo::shared_ptr<base> clone() const = 0;
		};

	

		template<typename T>
		struct object : base {
			//static_assert(std::is_copy_constructible<typename std::decay<T>::type>(), "Concept violation: T is not CopyConstructible");
			template<typename U>
			object(U&& v): value(std::forward<U>(v)) {}
			T value;

			amo::shared_ptr<base> clone() const {
				return amo::shared_ptr<base>(new object<T>(value));
			}
		};

		amo::shared_ptr<base> clone() const {
			return obj ? obj->clone() : nullptr;
		}

	public:
		//template<typename Param> variant(const Param& val_) : val(val_);
		 
	explicit variant(T&& actual) : obj(new(std::nothrow) object<typename amo::decay<T>::type >(std::forward<T>(actual))) {}
	 
	explicit variant(P&& actual) : obj(new(std::nothrow) object<typename amo::decay<P>::type >(std::forward<P>(actual))) {}
 
    variant(const variant& other): obj(other.clone()) {}

    
    variant(variant&& other): obj(std::move(other.obj)) {}

    
    variant& operator=(const variant& other) {
        if(obj == other.obj)
            return *this;

        obj = other.clone();
        return *this;
    }

    
    variant& operator=(variant&& other) {
        if(obj == other.obj)
            return *this;

        obj.swap(other.obj);
        return *this;
    }

 
	template<typename T>
	bool is() const {
		auto ptr = dynamic_cast<object<typename amo::decay<T>::type>*>(obj.get());
		return ptr;
	}

    
	

	private:
			amo::shared_ptr<base> obj;
	};
}

#endif // AMO_VARIANT_HPP__
