#ifndef AMO_NONCOPYABLE_HPP__
#define AMO_NONCOPYABLE_HPP__
 
namespace amo
{
	class noncopyable 
	{
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:   
		noncopyable( const noncopyable& );
		noncopyable& operator=( const noncopyable& );
	};
}


#endif // AMO_NONCOPYABLE_HPP__
