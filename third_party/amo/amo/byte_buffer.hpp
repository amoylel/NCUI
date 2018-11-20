#ifndef AMO_BYTE_BUFFER_HPP__
#define AMO_BYTE_BUFFER_HPP__


#include <utility>

#include <amo/byte_order.hpp>


namespace amo
{
	
	class byte_buffer 
	{
	public:
		static const int DEFAULT_SIZE = 4096;

		enum byte_order 
		{
			ORDER_NETWORK = 0,  // Default, use network byte order (big endian).
			ORDER_HOST,         // Use the native order of the host.
		};


		class ReadPosition {
			friend class byte_buffer;
			ReadPosition(size_t start, int version)
				: start_(start), version_(version) { }
			size_t start_;
			int version_;
		};
		// |byte_order_| defines order of bytes in the buffer.

		

		byte_buffer() 
		{
			construct(NULL, DEFAULT_SIZE, ORDER_NETWORK);
		}

		byte_buffer(byte_order byte_order_) {
			construct(NULL, DEFAULT_SIZE, byte_order_);
		}

		byte_buffer(const char* bytes, size_t len) {
			construct(bytes, len, ORDER_NETWORK);
		}

		byte_buffer(const char* bytes, size_t len, byte_order byte_order_) {
			construct(bytes, len, byte_order_);
		}

		byte_buffer(const char* bytes) {
			construct(bytes, strlen(bytes), ORDER_NETWORK);
		}

	
		~byte_buffer() {
			delete[] bytes_;
		}

		const char* data() const { return bytes_ + start_; }
		size_t length() const { return end_ - start_; }
		size_t capacity() const { return size_ - start_; }
		byte_order order() const { return byte_order__; }

		bool read(uint8_t* val) {
			if (!val) return false;

			return read_bytes(reinterpret_cast<char*>(val), 1);
		}

		bool read(uint16_t* val) {
			if (!val) return false;

			uint16_t v;
			if (!read_bytes(reinterpret_cast<char*>(&v), 2)) {
				return false;
			} else {
				*val = (byte_order__ == ORDER_NETWORK) ? amo::network_to_host_16(v) : v;
				return true;
			}
		}

		bool read_uint_24(uint32_t* val) {
			if (!val) return false;

			uint32_t v = 0;
			char* read_into = reinterpret_cast<char*>(&v);
			if (byte_order__ == ORDER_NETWORK || amo::is_host_bigendian()) {
				++read_into;
			}

			if (!read_bytes(read_into, 3)) {
				return false;
			} else {
				*val = (byte_order__ == ORDER_NETWORK) ? amo::network_to_host_32(v) : v;
				return true;
			}
		}

		bool read(uint32_t* val) {
			if (!val) return false;

			uint32_t v;
			if (!read_bytes(reinterpret_cast<char*>(&v), 4)) {
				return false;
			} else {
				*val = (byte_order__ == ORDER_NETWORK) ? amo::network_to_host_32(v) : v;
				return true;
			}
		}

		bool read(uint64_t* val) {
			if (!val) return false;

			uint64_t v;
			if (!read_bytes(reinterpret_cast<char*>(&v), 8)) {
				return false;
			} else {
				*val = (byte_order__ == ORDER_NETWORK) ? amo::network_to_host_64(v) : v;
				return true;
			}
		}

		bool read(std::string* val, size_t len) {
			if (!val) return false;

			if (len > length()) {
				return false;
			} else {
				val->append(bytes_ + start_, len);
				start_ += len;
				return true;
			}
		}

		bool read_bytes(char* val, size_t len) {
			if (len > length()) {
				return false;
			} else {
				memcpy(val, bytes_ + start_, len);
				start_ += len;
				return true;
			}
		}

		void write(uint8_t val) {
			write(reinterpret_cast<const char*>(&val), 1);
		}

		void write(uint16_t val) {
			uint16_t v = (byte_order__ == ORDER_NETWORK) ? amo::host_to_network_16(val) : val;
			write(reinterpret_cast<const char*>(&v), 2);
		}

		void write_uint_24(uint32_t val) {
			uint32_t v = (byte_order__ == ORDER_NETWORK) ? amo::host_to_network_32(val) : val;
			char* start = reinterpret_cast<char*>(&v);
			if (byte_order__ == ORDER_NETWORK || amo::is_host_bigendian()) {
				++start;
			}
			write(start, 3);
		}

		void write(uint32_t val) {
			uint32_t v = (byte_order__ == ORDER_NETWORK) ? amo::host_to_network_32(val) : val;
			write(reinterpret_cast<const char*>(&v), 4);
		}

		void write(uint64_t val) {
			uint64_t v = (byte_order__ == ORDER_NETWORK) ? amo::host_to_network_64(val) : val;
			write(reinterpret_cast<const char*>(&v), 8);
		}

		void write(const std::string& val) {
			write(val.c_str(), val.size());
		}

		void write(const char* val, size_t len) {
			memcpy(reserve_write_buffer(len), val, len);
		}

		char* reserve_write_buffer(size_t len) {
			if (length() + len > capacity())
				resize(length() + len);

			char* start = bytes_ + end_;
			end_ += len;
			return start;
		}

		void resize(size_t size) {
			size_t len = std::min<size_t>(end_ - start_, size);
			if (size <= size_) {
				// Don't reallocate, just move data backwards
				memmove(bytes_, bytes_ + start_, len);
			} else {
				// Reallocate a larger buffer.
				size_ = std::max<size_t>(size, 3 * size_ / 2);
				char* new_bytes = new char[size_];
				memcpy(new_bytes, bytes_ + start_, len);
				delete [] bytes_;
				bytes_ = new_bytes;
			}
			start_ = 0;
			end_ = len;
			++version_;
		}

		bool consume(size_t size) {
			if (size > length())
				return false;
			start_ += size;
			return true;
		}

		ReadPosition get_read_position() const {
			return ReadPosition(start_, version_);
		}

		bool set_read_position(const ReadPosition &position) {
			if (position.version_ != version_) {
				return false;
			}
			start_ = position.start_;
			return true;
		}

		void clear() {
			memset(bytes_, 0, size_);
			start_ = end_ = 0;
			++version_;
		}
		
	private:
		byte_buffer(const byte_buffer& );
		void construct(const char* bytes, size_t len,
			byte_order byte_order_) {
				version_ = 0;
				start_ = 0;
				size_ = len;
				byte_order__ = byte_order_;
				bytes_ = new char[size_];

				if (bytes) {
					end_ = len;
					memcpy(bytes_, bytes, end_);
				} else {
					end_ = 0;
				}
		}
	private:
		char* bytes_;
		size_t size_;
		size_t start_;
		size_t end_;
		int version_;
		byte_order byte_order__;
	};

}

#endif // AMO_BYTE_BUFFER_HPP__