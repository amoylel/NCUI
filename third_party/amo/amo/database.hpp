#ifndef AMO_DATABASE_HPP__
#define AMO_DATABASE_HPP__

#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#include <amo/config.hpp>
#include <amo/stdint.hpp>
#include <amo/smart_ptr.hpp>
#include <amo/singleton.hpp>
   

#include "database_impl.hpp"

namespace amo{ namespace db{
 
	class database : public amo::singleton < database >
	{
	public:
		const static uint64_t error_code = 0xffffffffffffffffULL;

		static amo::shared_ptr<database> GetInstance()
		{
			return get_instance();
		}

	public:
		database(void){}
		~database(void){}

		bool connect(const std::string& connection_str, uint32_t connections = 1)
		{
			amo::unique_lock<amo::mutex> lock(m_mutex);

			if (connections <= 0) return false;
			for (uint32_t i = 0; i < connections; ++i)
			{
				amo::shared_ptr<database_impl> db(new database_impl());
				if (!db->connect(connection_str)) return false;
				m_db_vec.push_back(db); 
			}

			return true;
		}

		amo::shared_ptr<database_impl> get_db()
		{
			amo::unique_lock<amo::mutex> lock(m_mutex);
			for (auto& p : m_db_vec)
			{
				if (!p->is_locked()) return p;
			}

			return m_db_vec[0];
		}

		uint64_t execute(std::string sql)
		{
			return get_db()->execute(sql);
		}

		template<typename T>
		bool add(amo::shared_ptr<T> val)
		{
			return get_db()->add<T>(val);
		}

		template<typename T>
		bool add_list(std::vector<amo::shared_ptr<T> > val)
		{
			return get_db()->add_list<T>(val);
		}

		template<typename T>
		bool remove(amo::shared_ptr<T> val)
		{
			return get_db()->remove<T>(val);
		}

		template<typename T>
		bool remove_list(std::vector<amo::shared_ptr<T> > val)
		{
			return get_db()->remove_list<T>(val);
		}

		template<typename T>
		bool update(amo::shared_ptr<T> val)
		{
			return get_db()->update<T>(val);
		}


		template<typename T>
		bool update_list(std::vector<amo::shared_ptr<T> > val)
		{
			return get_db()->update_list<T>(val);
		}

		template<typename T>
		std::vector<amo::shared_ptr<T> > query_list(odb::query<T> condition = odb::query<T>())
		{
			return get_db()->query_list<T>(condition);
		}

		template<typename T>
		amo::shared_ptr<T>  query(odb::query<T> condition)
		{
			return get_db()->query<T>(condition);
		}

		template<typename T>
		amo::shared_ptr<T> load(int64_t id)
		{
			return get_db()->load<T>(id);
		}

		template<typename T>
		std::vector<amo::shared_ptr<T> > load_list(std::vector<typename T::id_type> val)
		{
			return get_db()->load_list<T>(val);
		}
		 
	private:
		amo::mutex m_mutex;

		std::vector<amo::shared_ptr<database_impl> > m_db_vec;
	};
} }


#endif // AMO_DATABASE_HPP__
