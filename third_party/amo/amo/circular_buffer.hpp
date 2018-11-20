#ifndef AMO_CIRCULAR_BUFFER_HPP__
#define AMO_CIRCULAR_BUFFER_HPP__

#include <algorithm>
#include <stdexcept>
#include <amo/thread.hpp>
#include <amo/chrono.hpp>

namespace amo
{
	// circular buffer class
	template<class T>
	class circular_buffer
	{
	public:
		// initialize buffer with a fixed capacity, default is 20
		circular_buffer(int capacity = 20)
			: buffer(new T[capacity]), _capacity(capacity),
			_size(0), _front(-1), _back(-1)
		{
		}

		// copy constructor
		circular_buffer(const circular_buffer& other)
			: buffer(new T[other._capacity]), _capacity(other._capacity),
			_size(other._size), _front(other._front), _back(other._back)
		{
			for (int i = _front; i != _back; i = (i + 1) % _capacity)
				buffer[i] = other.buffer[i];
			buffer[_back] = other.buffer[_back];
		}

		// move constructor
		circular_buffer(circular_buffer&& other)
			: buffer(other.buffer), _capacity(other._capacity),
			_size(other._size), _front(other._front), _back(other._back)
		{
			other.buffer = nullptr;
		}

		// copy assignment operator
		circular_buffer& operator=(const circular_buffer& other)
		{
			if (&other != this) 
			{
				if (_capacity != other._capacity) 
				{
					_capacity = other._capacity;
					delete[] buffer;
					buffer = new T[other._capacity];
				}
				_size = other._size;
				_front = other._front;
				_back = other._back;
				for (int i = _front; i != _back; i = (i + 1) % _capacity)
					buffer[i] = other.buffer[i];
				buffer[_back] = other.buffer[_back];
			}
			return *this;
		}

		// move assignment operator
		circular_buffer&  operator=(circular_buffer&& other)
		{
			if (_capacity != other._capacity)
			{
				_capacity = other._capacity;
				delete[] buffer;
				buffer = other.buffer;
			}
			_size = other._size;
			_front = other._front;
			_back = other._back;
			other.buffer = nullptr;
			return *this;
		}

		// delete pointer to buffer
		~circular_buffer()
		{
			delete[] buffer;
		}


		// print buffer information
		void print_buffer()
		{
			std::cout << "buffer size: " << _size << "/" << _capacity << std::endl;
			// skip detail information if buffer is empty
			if (_size == 0)
			{
				std::cout << "buffer is empty\n";
				return;
			}

			std::cout << "buffer content: \n";
			for (int i = _front; i != _back; i = (i + 1) % _capacity)
				std::cout << "buffer[" << i << "] = " << buffer[i] << std::endl;
			std::cout << "buffer[" << _back << "] = " << buffer[_back] << std::endl;
			std::cout << "front element: " << buffer[_front] << std::endl;
			std::cout << "back element: " << buffer[_back] << std::endl;
		}

		// add new data at the end
		void push_back(const T& value)
		{
			_back = (_back + 1) % _capacity;
			if (_front == -1 || _size == _capacity)
				_front = (_front + 1) % _capacity;
			buffer[_back] = value;
			if (_size < _capacity)
				_size++;
		}

		// return and remove the oldest data at the front
		T pop_front()
		{
			if (_size == 0)
				throw std::out_of_range("\nERROR: buffer is empty! no more item to pop.\n");
			T value = buffer[_front];
			buffer[_front] = T();
			if (--_size == 0)
				_front = _back = -1;
			else
			{
				_front = (_front + 1) % _capacity;
			}
			return value;
		}

		// clear the buffer and reset all fields 
		void clear()
		{
			for (size_t i = 0; i < _size;i++)
				buffer[i] = T();
			_size = 0;
			_front = _back = -1;
		}

		// check if the buffer is empty
		inline bool is_empty() const
		{
			return _size == 0;
		}

		// return buffer capacity 
		inline size_t capacity() const
		{
			return _capacity;
		}

		// return buffer size
		inline size_t size() const
		{
			return _size;
		}
	private:
		T* buffer;
		size_t _size;
		size_t _capacity;
		int _front;
		int _back;
	};


	//不可靠的循环缓冲区,不保证数据插入成功.
	template <>   
	class circular_buffer<uint8_t> 
	{   
	public:      
		typedef std::vector<uint8_t> container_type;      
		typedef container_type::size_type size_type;      
		typedef container_type::value_type value_type;      
		typedef container_type::pointer pointer;
		explicit circular_buffer(size_type capacity = 100)
			: m_container(capacity, 0), start_pos(-1), insert_pos(0) 
		{
			is_cleared = false;
		}

		bool push(uint8_t* buffer, int64_t buffer_size)
		{

			if(buffer_size <= 0) return false;
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
			if(unreceptive(buffer_size) ) 
			{
				std::cout << "too many data caches, discard." << std::endl;
				return false;
			}
			int64_t container_size =  static_cast<int64_t>(m_container.size());
			m_not_full.wait(lock, std::bind(&circular_buffer::is_not_full, this, buffer_size));


			// 这里已经判断了有足够的空间进行插入运算,如果start_pos > insert_pos,
			// 那么m_container.size - insert_pos 一定大于buffer_size.
			// (start_pos-insert_pos) 一定大大于buffer_size的,
			// 

			int64_t odd_pos = buffer_size - (container_size - insert_pos);
			if(odd_pos < 0)
			{
				memcpy(&m_container[insert_pos], buffer, buffer_size);
			}
			else
			{
				memcpy(&m_container[insert_pos], buffer, container_size - insert_pos);
				if(odd_pos >0)
					memcpy(&m_container[0], &buffer[ container_size - insert_pos], odd_pos);
			}

			if(start_pos == -1)
				start_pos = insert_pos;
			insert_pos += buffer_size;
			insert_pos %=  container_size;
			m_not_empty.notify_all();

			return true;
		}

		bool push(uint8_t const* buffer, int64_t buffer_size)
		{
			if(buffer_size <= 0) return false;
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
			if(unreceptive(buffer_size) ) 
			{
				std::cout << "too many data caches, discard." << std::endl;
				return false;
			}
			int64_t container_size =  static_cast<int64_t>(m_container.size());
			m_not_full.wait(lock, std::bind(&circular_buffer::is_not_full, this, buffer_size)); 
			// 这里已经判断了有足够的空间进行插入运算,如果start_pos > insert_pos,
			// 那么m_container.size - insert_pos 一定大于buffer_size.
			// (start_pos-insert_pos) 一定大大于buffer_size的,
			// 

			int64_t odd_pos = buffer_size - (container_size - insert_pos);
			if(odd_pos < 0)
			{
				memcpy(&m_container[insert_pos], buffer, buffer_size);
			}
			else
			{
				memcpy(&m_container[insert_pos], buffer, container_size - insert_pos);
				if(odd_pos >0)
					memcpy(&m_container[0], &buffer[ container_size - insert_pos], odd_pos);
			}

			if(start_pos == -1)
				start_pos = insert_pos;
			insert_pos += buffer_size;
			insert_pos %=  container_size;
			m_not_empty.notify_all();


			return true;

		}
		int64_t pop(uint8_t* buffer, int64_t buffer_size) 
		{        
			if(buffer_size <= 0) return 0;
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);         
			/*if(!m_not_empty.wait_for(lock, amo::chrono::milliseconds(10), std::bind(&circular_buffer::is_not_empty, this)) )
			{
				return 0;
			}*/
			m_not_empty.wait(lock, std::bind(&circular_buffer::is_not_empty_or_cleared, this)); 
			if(is_cleared)
			{
				return 0;
			}
		 

			int64_t rec_size = buffer_size;
			int64_t container_size =  static_cast<int64_t>(m_container.size());

			//如果保存总数小于需求总数,返回最大值
			int64_t total_size = 0;
			if(insert_pos > start_pos)
				total_size = insert_pos - start_pos;
			else if(insert_pos == start_pos)
				total_size = container_size;
			else
				total_size = container_size - (start_pos - insert_pos);


			if(rec_size > total_size)
				rec_size = total_size;


			int64_t odd_pos = rec_size + start_pos - container_size;
			if(odd_pos < 0)
			{
				memcpy(buffer, &m_container[start_pos], rec_size);
			}
			else
			{
				memcpy(buffer, &m_container[start_pos], container_size - start_pos);
				if(odd_pos != 0)
					memcpy(&buffer[container_size - start_pos], &m_container[0], odd_pos);
			}


			start_pos += rec_size;
			start_pos %= container_size;
			if(start_pos == insert_pos)
			{
				start_pos = -1;
				insert_pos = 0;
			}

			m_not_full.notify_all();
			return rec_size;
		}

		pointer top()
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex);         
			m_not_empty.wait(lock, std::bind(&circular_buffer::is_not_empty, this));      
			m_not_empty.notify_all();
			return &m_container[start_pos];
		}

		void clear()
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
			m_container.clear();
			
			start_pos = -1;
			insert_pos = 0;
			is_cleared = true;
			m_not_empty.notify_all();
		}

		void clear_data()
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
			memset(&m_container[0], 0, m_container.size());
			start_pos = -1;
			insert_pos = 0;
		}

		bool is_not_empty() 
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
			return start_pos > -1; 
		}   

		bool is_not_empty_or_cleared() 
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
			return start_pos > -1 || is_cleared; 
		}   

		// 		bool is_not_empty(int64_t buffer_size)
		// 		{
		// 			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
		// 			return start_pos > -1; 
		// 			return false;
		// 		}

		//容器是否能接受当前大小的数据,所给的数据大小大于缓冲区大小
		inline bool unreceptive(int64_t buffer_size)
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
			return m_container.size() < buffer_size;
		}

		bool is_not_full(int64_t buffer_size)
		{
			amo::unique_lock<amo::recursive_mutex> lock(m_mutex); 
			int64_t container_size = static_cast<int64_t>(m_container.size());
			if(insert_pos == start_pos)
				return false;
			if(start_pos > insert_pos)
				return start_pos - insert_pos >= buffer_size;
			else
				return container_size - insert_pos >= buffer_size;
		}

	private:      
		circular_buffer(const circular_buffer&);              // 禁止复制构造函数      
		circular_buffer& operator = (const circular_buffer&); // 禁止赋值操作符      


		amo::recursive_mutex  m_mutex;      
		amo::condition_variable_any m_not_empty;      
		amo::condition_variable_any m_not_full;


		std::vector<uint8_t> m_container;
		int64_t start_pos;		//起始起始位置
		int64_t insert_pos;		//插入位置
		bool is_cleared;		


	};
}

#endif // AMO_CIRCULAR_BUFFER_HPP__
