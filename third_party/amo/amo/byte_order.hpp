#ifndef AMO_BYTE_ORDER_HPP__
#define AMO_BYTE_ORDER_HPP__


#if defined(POSIX) && !defined(__native_client__)
#include <arpa/inet.h>
#endif

#ifdef WIN32
#include <stdlib.h>
#endif

namespace amo{  

	inline void set_8(void* memory, size_t offset, uint8_t v) {
		static_cast<uint8_t*>(memory)[offset] = v;
	}

	inline uint8_t get_8(const void* memory, size_t offset) {
		return static_cast<const uint8_t*>(memory)[offset];
	}

	inline void set_be_16(void* memory, uint16_t v) {
		set_8(memory, 0, static_cast<uint8_t>(v >> 8));
		set_8(memory, 1, static_cast<uint8_t>(v >> 0));
	}

	inline void set_be_32(void* memory, uint32_t v) {
		set_8(memory, 0, static_cast<uint8_t>(v >> 24));
		set_8(memory, 1, static_cast<uint8_t>(v >> 16));
		set_8(memory, 2, static_cast<uint8_t>(v >> 8));
		set_8(memory, 3, static_cast<uint8_t>(v >> 0));
	}

	inline void set_be_64(void* memory, uint64_t v) {
		set_8(memory, 0, static_cast<uint8_t>(v >> 56));
		set_8(memory, 1, static_cast<uint8_t>(v >> 48));
		set_8(memory, 2, static_cast<uint8_t>(v >> 40));
		set_8(memory, 3, static_cast<uint8_t>(v >> 32));
		set_8(memory, 4, static_cast<uint8_t>(v >> 24));
		set_8(memory, 5, static_cast<uint8_t>(v >> 16));
		set_8(memory, 6, static_cast<uint8_t>(v >> 8));
		set_8(memory, 7, static_cast<uint8_t>(v >> 0));
	}

	inline uint16_t get_be_16(const void* memory) {
		return static_cast<uint16_t>((get_8(memory, 0) << 8) |
			(get_8(memory, 1) << 0));
	}

	inline uint32_t get_be_32(const void* memory) {
		return (static_cast<uint32_t>(get_8(memory, 0)) << 24) |
			(static_cast<uint32_t>(get_8(memory, 1)) << 16) |
			(static_cast<uint32_t>(get_8(memory, 2)) << 8) |
			(static_cast<uint32_t>(get_8(memory, 3)) << 0);
	}

	inline uint64_t get_be_64(const void* memory) {
		return (static_cast<uint64_t>(get_8(memory, 0)) << 56) |
			(static_cast<uint64_t>(get_8(memory, 1)) << 48) |
			(static_cast<uint64_t>(get_8(memory, 2)) << 40) |
			(static_cast<uint64_t>(get_8(memory, 3)) << 32) |
			(static_cast<uint64_t>(get_8(memory, 4)) << 24) |
			(static_cast<uint64_t>(get_8(memory, 5)) << 16) |
			(static_cast<uint64_t>(get_8(memory, 6)) << 8) |
			(static_cast<uint64_t>(get_8(memory, 7)) << 0);
	}

	inline void set_le_16(void* memory, uint16_t v) {
		set_8(memory, 0, static_cast<uint8_t>(v >> 0));
		set_8(memory, 1, static_cast<uint8_t>(v >> 8));
	}

	inline void set_le_32(void* memory, uint32_t v) {
		set_8(memory, 0, static_cast<uint8_t>(v >> 0));
		set_8(memory, 1, static_cast<uint8_t>(v >> 8));
		set_8(memory, 2, static_cast<uint8_t>(v >> 16));
		set_8(memory, 3, static_cast<uint8_t>(v >> 24));
	}

	inline void set_le_64(void* memory, uint64_t v) {
		set_8(memory, 0, static_cast<uint8_t>(v >> 0));
		set_8(memory, 1, static_cast<uint8_t>(v >> 8));
		set_8(memory, 2, static_cast<uint8_t>(v >> 16));
		set_8(memory, 3, static_cast<uint8_t>(v >> 24));
		set_8(memory, 4, static_cast<uint8_t>(v >> 32));
		set_8(memory, 5, static_cast<uint8_t>(v >> 40));
		set_8(memory, 6, static_cast<uint8_t>(v >> 48));
		set_8(memory, 7, static_cast<uint8_t>(v >> 56));
	}

	inline uint16_t get_le_16(const void* memory) {
		return static_cast<uint16_t>((get_8(memory, 0) << 0) |
			(get_8(memory, 1) << 8));
	}

	inline uint32_t get_le_32(const void* memory) {
		return (static_cast<uint32_t>(get_8(memory, 0)) << 0) |
			(static_cast<uint32_t>(get_8(memory, 1)) << 8) |
			(static_cast<uint32_t>(get_8(memory, 2)) << 16) |
			(static_cast<uint32_t>(get_8(memory, 3)) << 24);
	}

	inline uint64_t get_le_64(const void* memory) {
		return (static_cast<uint64_t>(get_8(memory, 0)) << 0) |
			(static_cast<uint64_t>(get_8(memory, 1)) << 8) |
			(static_cast<uint64_t>(get_8(memory, 2)) << 16) |
			(static_cast<uint64_t>(get_8(memory, 3)) << 24) |
			(static_cast<uint64_t>(get_8(memory, 4)) << 32) |
			(static_cast<uint64_t>(get_8(memory, 5)) << 40) |
			(static_cast<uint64_t>(get_8(memory, 6)) << 48) |
			(static_cast<uint64_t>(get_8(memory, 7)) << 56);
	}

	// Check if the current host is big endian.
	inline bool is_host_bigendian() {
		static const int number = 1;
		return 0 == *reinterpret_cast<const char*>(&number);
	}

	inline uint16_t host_to_network_16(uint16_t n) {
		uint16_t result;
		set_be_16(&result, n);
		return result;
	}

	inline uint32_t host_to_network_32(uint32_t n) {
		uint32_t result;
		set_be_32(&result, n);
		return result;
	}

	inline uint64_t host_to_network_64(uint64_t n) {
		uint64_t result;
		set_be_64(&result, n);
		return result;
	}

	inline uint16_t network_to_host_16(uint16_t n) {
		return get_be_16(&n);
	}

	inline uint32_t network_to_host_32(uint32_t n) {
		return get_be_32(&n);
	}

	inline uint64_t network_to_host_64(uint64_t n) {
		return get_be_64(&n);
	}
}

#endif // AMO_BYTE_ORDER_HPP__
