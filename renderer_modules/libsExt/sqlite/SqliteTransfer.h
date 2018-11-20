// Created by amoylel on 08/03/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SQLITETRANSFER_H__
#define AMO_SQLITETRANSFER_H__

#include <memory>
#include <functional>
#include "transfer/ClassTransfer.hpp"


#include "src/sqlite3pp.h"


namespace amo {

    /*!
     * @class	Sqlite
     *
     * @chapter extend
     *
     * @brief	Sqlite3 数据库封装类，用于操作本地数据库文件.<br>
     * 			工作线程**Renderer线程**
     */
    
    class SqliteTransfer
        : public ClassTransfer,
          public amo::singleton<SqliteTransfer> {
    public:
        // 不改变字段名
        const static int FieldNormal = 0;
        // 字段字大写
        const static int FieldUpper = 1;
        // 字段名小写
        const static int FieldLower = 2;
        
        
    public:
    
        SqliteTransfer();
        
        /*!
         * @fn	SqliteTransfer::Sqlite(const std::string& args);
         *
         * @tag constructor
         *
         * @brief	创建一个Sqlite对象.
         *
         * @param	#String 数据库文件路径，如果该数据库不存在，那么将创建一个新的数据库.
         *
         * @return  #Object Sqlite 实例对象.
         * @example
         *
        		```
        			include('Sqlite');
        			window.db = new Sqlite('test.db');
        		```
         */
        
        SqliteTransfer(const std::string& args);
        ~SqliteTransfer();
        
        
        /*!
        * @fn	virtual Any SqliteTransfer::import(IPCMessage::SmartType msg);
        * @tag sync
        * @brief	从文件中执行SQL语句.
        *
        * @param	#String 文件路径.
        *
        * @return	#Int/Undefined 如果成为返回受影响的行数，否则返回Undefined.
        *
        * @example
        *
        	```
        		include('FileStream');
        		var ofs = new FileStream('test.sql', 2);
        		ofs.write(`
        				PRAGMA foreign_keys = OFF;
        				DROP TABLE IF EXISTS "main"."table1";
        				CREATE TABLE "table1" (
        					"name"  TEXT,
        					"age"  INTEGER,
        					"score"  INTEGER,
        					"remark"  TEXT
        				);`);
        		ofs.close();
        		console.assert(db.import('test.sql') == 0);
        
        	```
        */
        
        virtual Any import(IPCMessage::SmartType msg);
        
        
        /*!
         * @fn	virtual Any SqliteTransfer::execute(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	执行原生sql.
         *
         * @param	#String 需要执行的SQL语句.
         *
         * @return	#Int/Undefined 如果成为返回受影响的行数，否则返回Undefined.
         * @example
         *
        	 ```
        		console.assert(db.execute('SELECT count(1) FROM table1') == 0);
        	 ```
         */
        
        virtual Any execute(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	virtual Any SqliteTransfer::insert(IPCMessage::SmartType msg);
        * @tag sync
        * @brief	插入数据.
        *
        * @param	#String 数据库表名.
        *
        * @param	#JsonObject 要插入的数据.
        *
        * @return	#Int/Undefined 如果成为返回id{@link getLastInsertRowID=Sqlite.getLastInsertRowID}，否则返回Undefined.
        * @example
        *
        		```
        			var val = {
        				name: '张三',
        				age:18,
        				score:99,
        				remark:'测试数据'
        			};
        			console.assert(db.insert('table1', val) ==1);
        			console.assert(db.insert('table1', {name:'李四'}) ==2);
        
        		```
        */
        
        virtual Any insert(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::update(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	更新数据.
         *
         * @param	#String 数据库表名.
         *
         * @param	#JsonObject 需要更新的数据.
         *
         * @param	#String 条件语句(如果第四个参数存在，那么使用第四个参数格式化该字符串，如果不存在，那么使用第二个参数格式化字符串).
         *
         * @param	#JsonObject=Undefined 条件语句格式化参数.
         *
         * @return	#Int/Undefined 如果成为返回受影响的行数，否则返回Undefined.
         * @example
         *
        	 ```
        	 // 直接指定条件语句
        	var retval=  db.update('table1', {remark: '李四的数据'}, 'name=李四');
        	console.log(retval);
        	 // 使用格式化参数
        	retval =  db.update('table1', {score: 77, age: 22}, 'name={name}', {name:'李四'});
        	 console.log(retval);
        	 // 使用更新数据中的参数格式化条件语句
        	 retval = db.update('table1', {score: 77, age: 22, name: '张三'}, 'name={name}');
        	 console.log(retval);
        	 ```
         */
        
        virtual Any update(IPCMessage::SmartType msg) ;
        
        /*!
         * @fn	virtual Any SqliteTransfer::backup(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	备份数据库.
         *
         * @param	#String 备份文件路径.
         *
         * @return	#Boolean true成功/false失败.
         * @example
         *
         ```
        	console.assert(db.backup('test2.db') == true);
         ```
         */
        
        virtual Any backup(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::query(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	查询数据.
         *
         * @param	#String SQL语句.
         *
         * @param	#JsonObject={} 格式化参数，可以不填.
         *
         * @param	#JsonObject=Undefined 分页参数，可以不填(不填将不会使用分页功能).<br>
         * 						 [**Int**] page 当前页码数，默认为1.<br>
         * 						 [**Int**] rows 每页条数，默认为10.<br>
         * 						 [**Int**] total 数据总系数，由程序返回<br>
         * 						 [**Int**] maxpage 总页数，由程序返回<br>
         * 						 [**Int**] startrow 开始行，<br>
         * 						 [**Boolean**] refresh 是否重新计算分页，默认为true.<br>
         *
         * @return	#JsonObject 查询到的数据.
         * 						 |#Int page 当前页码数，分页查询时返回.
         * 						 |#Int rows 每页条数，分页查询时返回.
         * 						 |#Int total 数据总系数，分页查询时返回.
         * 						 |#Int maxpage 总页数，分页查询时返回.
         * 						 |#Int startrow 开始行，分页查询时返回.
         * 						 |#Boolean refresh 是否重新计算分页，分页查询时返回.
         * 						 |#JsonArray data 查询到的数据.
         * @example
         *
         ```
        	var result = db.query('SELECT * FROM table1 WHERE name = "张三"');
        	console.log(result);
        	var result2 = db.query('SELECT * FROM table1 WHERE name = "{0}"', ['张三']);
        	console.log(result2);
        	var result3 = db.query('SELECT * FROM table1 WHERE name = "{name}" and age = "{age}" ', {name:'张三', age:'18'});
        	console.log(result3);
        	var result4 = db.query('SELECT * FROM table1 WHERE name = "{name}" and age = "{age}" ', {name:'张三', age:'18'}, {rows:10});
        	console.log(result4);
         ```
         */
        
        
        
        virtual Any query(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::remove(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	删除数据.
         *
         * @param	#String 表名.
         *
         * @param	#String 条件语句.
         *
         * @param	#JsonObject={}  条件参数，可以不填, 也可以是一个Array.
         *
         * @return	#Int/Undefined 如果成为返回受影响的行数，否则返回Undefined.
         * @example
         *
         ```
        	console.assert(db.remove('table1', '  name="{0}"', ['李四']) == 1);
        	db.insert('table1', {name:'李四'});
        	console.assert(db.remove('table1', '  name="{name}"', {name:'李四'}) == 1);
        	db.insert('table1', {name:'李四'});
        	console.assert(db.remove('table1', ' name="李四"') == 1);
        
         ```
         */
        
        virtual Any remove(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::queryCount(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	查询数据条数.
         *
         * @param	#String SQL语句.
         *
         * @param	#JsonObject={} 条件参数.
         *
         * @return	#Int 成功返回数据条数，失败返回-1.
         * @example
         *
         ```
        	db.insert('table1', {name:'李四'});
        	console.assert(db.remove('table1', 'name="{0}"', ['李四']) == 1);
        	console.assert(db.queryCount('SELECT COUNT(1) FROM table1 WHERE  name="{0}"', ['李四']) == 0);
        
         ```
         */
        
        virtual Any queryCount(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::getLastInsertRowID(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	返回最近一次SQL语句所影响的行数，如插入、删除、更新等操作后可以调用该函数获取数据.
         *
         * @return	#Int/Undefined 如果成为返回受影响的行数，否则返回Undefined.
         * @example
         *
         ```
        	var id  = db.insert('table1', {name:'李四'});
        	console.assert(db.getLastInsertRowID() == id);
         ```
         */
        
        virtual Any getLastInsertRowID(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::containsTable(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	数据库中是否存在指定的表.
         *
         * @param	#String 表名.
         *
         * @return	#Boolean true存在/false失败或不存在.
         * @example
         *
         ```
        	console.assert(db.containsTable('table1') == true);
        	console.assert(db.containsTable('table2') == false);
         ```
         */
        
        virtual Any containsTable(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::containsField(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	指定表中是否包含某字段.
         *
         * @param	#String		表名.
         * @param	#String	字段名.
         *
         * @return	#Boolean. true存在/false失败或不存在.
         * @example
         *
         ```
        	console.assert(db.containsField('table1', 'name') == true);
        	console.assert(db.containsField('table1', 'name2') == false);
         ```
         */
        
        virtual Any containsField(IPCMessage::SmartType msg);
        
        /*!
         * @fn	virtual Any SqliteTransfer::getTableFields(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取指定表中的所有字段.
         *
         * @param	#String 表名.
         *
         * @return	#Array 包含所有字段名的数组.
         * @example
         *
         ```
        	console.log(db.getTableFields('table1'));
         ```
         */
        
        virtual Any getTableFields(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(SqliteTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(import, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(execute, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(insert, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(update, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(backup, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(query, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(queryCount, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(remove, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getLastInsertRowID, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(containsTable, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(containsField, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getTableFields, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        bool isValid() const {
            return m_bValid;
        }
        void setValid(bool val) {
            m_bValid = val;
        }
    public:
        const std::string& getLastError() const;
        void setLastError(const std::string& msg);
        
        int getFieldStyle() const;
        void setFieldStyle(int val);
        std::string translateFieldName(const std::string& str);
        std::string translateFieldType(const std::string& str);
        
        virtual void onBeforeRelease() override;
        
        
    protected:
        Any executeSql(const std::string& sql);
        std::string makeSql(IPCMessage::SmartType msg);
        std::string getValuesFromJson(amo::u8json& json, const std::string& key);
        std::string makeInsertSql(IPCMessage::SmartType msg);
        std::string makeInsertSqlFromJson(const std::string& utf8TableName,
                                          amo::u8json& utf8Json);
        std::string makeRemoveSql(IPCMessage::SmartType msg);
        std::string makeUpdateSql(IPCMessage::SmartType msg);
        std::string formatArgs(IPCMessage::SmartType msg);
        
        
        
        //amo::u8string formatUpdateSql(const amo::u8string& sql, amo::u8json& json);
        
        amo::u8string formatArgsByAnsiJson(const amo::u8string& sql, amo::u8json& json);
        amo::u8string formatArgsByU8Json(const amo::u8string& sql, amo::u8json& json);
        amo::u8string formatArgsByArr(const amo::u8string& sql,
                                      std::vector<amo::u8string>& vec);
        std::string formatPagging(amo::u8json& json);
        std::vector<amo::u8string> anyToStringVec(Any& vec);
        bool queryCountImpl(const std::string& str, amo::u8json& json);
        
        /*!
        * @fn	amo::u8json Sqlite::getPaggingInfo(amo::u8json& other);
        * @ignore
        * @brief	合并分页信息.
        *
        * @param [in,out]	other	The other.
        *
        * @return	The pagging information.
        */
        
        amo::u8json getPaggingInfo(amo::u8json& other);
        
        /**
        * @fn	bool Sqlite::getTableFieldImpl(const std::string& table);
        * @ignore
        * @brief	获取数据库表的所有字段.
        *
        * @param	table	The table.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        std::vector<std::string>& getTableFieldImpl(const std::string& table);
        
        /**
        * @fn	bool Sqlite::containsFieldImpl(const std::string& table, const std::string& field);
        * @ignore
        * @brief	判断所给字段是否存在于表中.
        *
        * @param	table	The table.
        * @param	field	The field.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool containsFieldImpl(const std::string& table, const std::string& field);
        
        
    private:
        /*! @brief	当前数据库连接是否有效. */
        bool m_bValid;
        std::string m_strLastError;
        std::string m_strDBPath;
        std::shared_ptr< sqlite3pp::database> m_pDB;
        std::unordered_map<std::string, std::vector<std::string> > m_tableFieldMap;
        std::vector<std::string> m_emptyFields;
        int nfieldStyle;
    };
}

#endif // AMO_SQLITETRANSFER_H__
