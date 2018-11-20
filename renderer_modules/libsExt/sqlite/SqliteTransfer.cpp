#include "stdafx.h"
#include "sqlite/SqliteTransfer.h"

#include <iostream>
#include <map>
#include <amo/json.hpp>
#include <amo/string.hpp>
#include <amo/format.hpp>
#include <amo/json.hpp>
#include <amo/logger.hpp>
#include <amo/filestream.hpp>



#pragma comment(lib, "sqlite3.lib")


const static std::string SQLITE_INVALID_CONNECTION =
    "invalid database connection";
const static std::string SQLITE_EMPTY_SQL = "empty SQL";
const static std::string SQLITE_INVALID_SQL = "invalid SQL";

namespace amo {



    SqliteTransfer::SqliteTransfer()
        : ClassTransfer("Sqlite") {
        
    }
    
    SqliteTransfer::SqliteTransfer(const std::string& args)
        : ClassTransfer("Sqlite") {
        try {
            m_strDBPath = args;
            m_pDB.reset(new sqlite3pp::database(args.c_str()));
            setValid(true);
        } catch (std::exception& e) {
            setLastError(e.what());
            setValid(false);
            m_pDB.reset();
        }
        
    }
    
    SqliteTransfer::~SqliteTransfer() {
    
    }
    
    
    Any SqliteTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string strPath = args->getString(0);
        
        if (strPath.empty()) {
            return Undefined();
        }
        
        std::shared_ptr<SqliteTransfer> pTransfer;
        pTransfer = ClassTransfer::createTransfer<SqliteTransfer>(strPath);
        /*   pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());
           pTransfer->setDefaultTriggerEventFunc(this->getDefaultTriggerEventFunc());
           pTransfer->setModuleName(this->getModuleName());*/
        return  pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    
    
    Any SqliteTransfer::execute(IPCMessage::SmartType msg) {
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return Undefined();
        }
        
        std::string sql = makeSql(msg);
        
        if (sql.empty()) {
            setLastError(SQLITE_EMPTY_SQL);
            return Undefined();
        }
        
        sqlite3pp::database& db = *m_pDB;
        
        std::shared_ptr<sqlite3pp::transaction> transaction;
        
        
        
        try {
            transaction.reset(new sqlite3pp::transaction(db, true));
            
            int ret = db.execute(sql.c_str());
            ret = db.changes();
            transaction->commit();
            return ret;
            
        } catch (std::exception& e) {
            setLastError(e.what());
            
            if (transaction) {
                transaction->rollback();
            }
        }
        
        return Undefined();
    }
    
    Any SqliteTransfer::import(IPCMessage::SmartType msg) {
        amo::u8string str(msg->getArgumentList()->getString(0), true);
        amo::u8path p(str);
        
        if (!p.exists()) {
            return Undefined();
        }
        
        amo::filestream ifs(p.c_str());
        std::string sql = ifs.read_all();
        auto cloneMsg = msg->clone();
        cloneMsg->getArgumentList()->setValue(0, sql);
        return execute(cloneMsg);
        
    }
    
    Any SqliteTransfer::executeSql(const std::string& sql) {
    
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return Undefined();
        }
        
        if (sql.empty()) {
            setLastError(SQLITE_EMPTY_SQL);
            return Undefined();
        }
        
        sqlite3pp::database& db = *m_pDB;
        std::shared_ptr<sqlite3pp::transaction> transaction;
        
        
        
        try {
            transaction.reset(new sqlite3pp::transaction(db, true));
            int ret = db.execute(sql.c_str());
            ret = db.changes();
            transaction->commit();
            return ret;
            
        } catch (std::exception& e) {
            setLastError(e.what());
            
            if (transaction) {
                transaction->rollback();
            }
        }
        
        return Undefined();
    }
    
    
    Any SqliteTransfer::insert(IPCMessage::SmartType msg) {
        std::string sql = makeInsertSql(msg);
        Any ret;
        
        if (sql.empty()) {
        
            ret = execute(msg);
        } else {
            ret = executeSql(sql);
        }
        
        if (!ret.is<Undefined>()) {
            return getLastInsertRowID(msg);
        }
        
        return Undefined();
    }
    
    Any SqliteTransfer::update(IPCMessage::SmartType msg) {
        std::string sql = makeUpdateSql(msg);
        amo::u8string ansiSql(sql, true);
        Any ret = Undefined();
        
        if (sql.empty()) {
        
            ret = execute(msg);
        } else {
            ret = executeSql(sql);
        }
        
        return ret;
    }
    
    
    Any SqliteTransfer::backup(IPCMessage::SmartType msg) {
        std::shared_ptr< sqlite3pp::database> pDB;
        
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return false;
        }
        
        try {
            std::string args = msg->getArgumentList()->getString(0);
            pDB.reset(new sqlite3pp::database(args.c_str()));
            return m_pDB->backup(*pDB.get()) != 0;
        } catch (std::exception& e) {
            m_strLastError = e.what();
            pDB.reset();
        }
        
        return false;
    }
    
    Any SqliteTransfer::query(IPCMessage::SmartType msg) {
    
        amo::u8json queryJson;
        // 获取数据
        amo::u8json jsonArr;
        jsonArr.set_array();
        
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            queryJson.put("data", jsonArr);
            return queryJson;
        }
        
        
        bool bNeedPagging = false;
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        // 如果第三个参数是一个JSON,那么认为是分页信息
        if (args->getValue(2).type() == AnyValueType<amo::u8json>::value) {
            bNeedPagging = true;
            queryJson = args->getJson(2);
            queryJson = getPaggingInfo(queryJson);
        }
        
        // 生成SQL语句
        std::string sql = makeSql(msg);
        
        if (sql.empty()) {
            setLastError(SQLITE_EMPTY_SQL);
            queryJson.put("data", jsonArr);
            return queryJson;
        }
        
        if (bNeedPagging) {
            // 查询分页
            bool bOk = queryCountImpl(sql, queryJson);
            
            if (!bOk) {
                queryJson.put("data", jsonArr);
                return queryJson;
            }
            
            sql += formatPagging(queryJson);
        }
        
        
        try {
        
            sqlite3pp::database& db = *m_pDB;
            
            {
                sqlite3pp::query qry(db, sql.c_str());
                
                std::vector<std::string> keys;
                std::vector<std::string> types;
                
                bool bQueryData = true;
                
                // 获取类型名称、字段名
                for (int i = 0; i < qry.column_count(); ++i) {
                    const char* columnName = qry.column_name(i);
                    
                    if (columnName != NULL) {
                        keys.push_back(translateFieldName(columnName));
                    } else {
                        bQueryData = false;
                        break;
                    }
                    
                    const char* columnType = qry.column_decltype(i);
                    
                    if (columnType != NULL) {
                    
                        types.push_back(translateFieldType(columnType));
                    } else {
                        bQueryData = false;
                        types.push_back("TEXT");
                        amo::cwarn << func_orient << "unknown type of table:" << columnName <<
                                   amo::endl;
                        //break;
                    }
                }
                
                if (!bQueryData) {
                    //TODO: 处理
                    //return Undefined();
                }
                
                
                auto ccoutn = qry.column_count();
                
                for (sqlite3pp::query::iterator iter = qry.begin();
                        iter != qry.end();
                        ++iter) {
                    amo::u8json json;
                    
                    for (int j = 0; j < qry.column_count(); ++j) {
                    
                    
                        const char* data = (*iter).get<const char*>(j);
                        
                        if (types.at(j) == "Boolean") {
                            if (data != NULL) {
                                json.put(keys.at(j), (*iter).get<int>(j) != 0);
                            }
                        } else if (types.at(j) == "INTEGER"
                                   || types.at(j) == "SMALLINT"
                                   || types.at(j) == "DECIMAL") {
                            if (data != NULL) {
                                int val32 = (*iter).get<int>(j);
                                int64_t val64 = (*iter).get<int64_t>(j);
                                
                                if (val32 == val64) {
                                    json.put(keys.at(j), val32);
                                } else {
                                    json.put(keys.at(j), val64);
                                }
                            }
                            
                        } else if (types.at(j) == "UNSIGNED INT"
                                   || types.at(j) == "UNSIGNED") {
                            if (data != NULL) {
                                json.put(keys.at(j), (uint32_t)(*iter).get<int64_t>(j));
                            }
                        } else if (types.at(j) == "TEXT"
                                   || types.at(j) == "VARCHAR"
                                   || types.at(j) == "CHAR"
                                   || types.at(j) == "GRAPHIC"
                                   || types.at(j) == "VARGRAPHIC"
                                   || types.at(j).find("CHAR") == 0
                                   || types.at(j).find("VARCHAR") == 0) {
                            int bytes = (*iter).column_bytes(j);
                            
                            //int len = strlen(cc);
                            //std::string sb(cc, bytes);
                            //
                            //// 如果不相等，说明字符串中的\0;
                            //if (len != bytes) {
                            //
                            //}
                            //
                            json.put(keys.at(j), data, bytes);
                            /*  std::string sss3 = json.to_string();
                              std::string sssss  = json.getString(keys.at(j));
                              size_t cc1 = sssss.size();
                              size_t cd1 = sssss.length();*/
                            
                            //json.put(keys.at(j), (*iter).get<std::string>(j));
                        } else if (types.at(j) == "REAL"
                                   || types.at(j) == "FLOAT"
                                   || types.at(j) == "DOUBLE") {
                            if (data != NULL) {
                                json.put(keys.at(j), (*iter).get<double>(j));
                            }
                        } else if (types.at(j) == "BLOB") {
                            int bytes = (*iter).column_bytes(j);
                            const char* dataBlob = (const char*)(*iter).get<const void*>(j);
                            
                            if (dataBlob != NULL) {
                                json.put(keys.at(j), dataBlob, bytes);
                            }
                            
                        } else if (types.at(j) == "DATE") {
                            if (data != NULL) {
                                json.put(keys.at(j), (*iter).get<std::string>(j));
                            }
                        } else if (types.at(j) == "TIME") {
                            if (data != NULL) {
                                json.put(keys.at(j), (*iter).get<std::string>(j));
                            }
                        } else if (types.at(j) == "DATETIME") {
                            if (data != NULL) {
                                json.put(keys.at(j), (*iter).get<std::string>(j));
                            }
                        } else if (types.at(j) == "TIMESTAMP") {
                            if (data != NULL) {
                                json.put(keys.at(j), (*iter).get<std::string>(j));
                            }
                        }
                        
                    }
                    
                    jsonArr.push_back(json);
                }
                
                qry.reset();
                queryJson.put("data", jsonArr);
                return queryJson;
            }
        } catch (std::exception& e) {
            setLastError(e.what());
        }
        
        queryJson.put("data", jsonArr);
        return queryJson;
        
    }
    
    
    Any SqliteTransfer::remove(IPCMessage::SmartType msg) {
        std::string sql = makeRemoveSql(msg);
        Any ret = Undefined();
        
        if (sql.empty()) {
            ret = execute(msg);
        } else {
            ret = executeSql(sql);
        }
        
        return ret;
    }
    
    Any SqliteTransfer::getLastInsertRowID(IPCMessage::SmartType msg) {
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return Undefined();
        }
        
        return m_pDB->last_insert_rowid();
    }
    
    Any SqliteTransfer::containsTable(IPCMessage::SmartType msg) {
        std::string tableName = msg->getArgumentList()->getString(0);
        
        // 这个变量有毒， 不能直接设置到msg里面去，Clone一下就好了，为啥？
        // 因为Debug 模式下，MTD初始libNCUI释放了sql变量，该值设置为Any后只能在libExt里面释放
        // 其他地方也是一样
        //
        std::string sql =
            "select count(1) from sqlite_master where type='table' and name='" + tableName +
            "';";
            
        auto cloneMsg = msg->clone();
        
        cloneMsg->getArgumentList()->setValue(0, sql);
        int nCount = queryCount(cloneMsg);
        
        if (nCount == 0) {
            return false;
        }
        
        return true;
    }
    
    Any SqliteTransfer::containsField(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        std::string tableName = args->getString(0);
        std::string fieldName = args->getString(1);
        
        if (tableName.empty() || fieldName.empty()) {
            return false;
        }
        
        return containsFieldImpl(tableName, fieldName);
    }
    
    Any SqliteTransfer::getTableFields(IPCMessage::SmartType msg) {
        std::vector<Any> retval;
        auto args = msg->getArgumentList();
        std::string tableName = args->getString(0);
        
        if (tableName.empty()) {
            return retval;
            
        }
        
        auto vec =  getTableFieldImpl(tableName);
        
        for (auto& p : vec) {
            retval.push_back(p);
        }
        
        return retval;
    }
    
    std::string SqliteTransfer::makeSql(IPCMessage::SmartType msg) {
        if (!msg) {
            return "";
        }
        
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string sql = args->getString(0);
        
        if (args->isValid(1)) {
            sql = formatArgs(msg);
        }
        
        return sql;
        
        
    }
    
    
    std::string SqliteTransfer::getValuesFromJson(amo::u8json& json,
            const std::string& key) {
        std::stringstream stream;
        
        if (json.is_bool(key)) {
            bool val = json.getBool(key);
            
            if (val) {
                stream << 1;
            } else {
                stream << 0;
            }
        } else if (json.is_int(key)) {
            stream << json.getInt(key);
        } else if (json.is_double(key)) {
            stream << json.getDouble(key);
        } else if (json.is_int64(key)) {
            stream << json.getInt64(key);
        } else if (json.is_uint(key)) {
            stream << json.getUint(key);
        } else if (json.is_uint64(key)) {
            stream << json.getUint(key);
        } else if (json.is_string(key)) {
            stream << "'" << json.getString(key) << "'";
        }
        
        return stream.str();
        
    }
    
    std::string SqliteTransfer::makeInsertSql(IPCMessage::SmartType msg) {
    
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string utf8TableName = args->getString(0);
        amo::u8string ansiTableName(utf8TableName, true);
        ansiTableName.trim_left(amo::u8string(" ", true));
        ansiTableName.trim_right(amo::u8string(" ", true));
        std::vector<amo::u8string> tables = ansiTableName.split(amo::u8string(" ",
                                            true));
                                            
        // 如果拆分出来不只一项,那么认为不是一个表名
        if (tables.size() > 1) {
        
            return "";
        }
        
        if (args->getValue(1).is<std::vector<Any>>()) {
            std::vector<Any> arr = args->getValue(1);
            std::string sql;
            
            for (auto & p : arr) {
                amo::u8json& utf8Json = p.As<amo::u8json>();
                std::string s = makeInsertSqlFromJson(utf8TableName, utf8Json);
                
                if (!s.empty()) {
                    sql += s;
                    sql += ";";
                }
            }
            
            return sql;
        } else if (args->getValue(1).is<amo::u8json>()) {
            amo::u8json utf8Json = args->getJson(1);
            return makeInsertSqlFromJson(utf8TableName, utf8Json);
        }
        
        return "";
    }
    
    
    std::string SqliteTransfer::makeInsertSqlFromJson(const std::string&
            utf8TableName, amo::u8json& utf8Json) {
        // 如果不是一个合法的JSON，返回""
        if (!utf8Json.is_valid() || utf8Json.size() <= 0) {
            return "";
        }
        
        std::string sssb = utf8Json.to_string();
        std::vector<std::string> keys = utf8Json.keys();
        std::stringstream streamKeys;
        std::stringstream streamValues;
        
        streamKeys << "(";
        streamValues << "(";
        
        for (size_t i = 0; i < keys.size(); ++i) {
            // 如果所给字段在表中不存在，那么跳过此字段，这里字符集有点乱
            if (!containsFieldImpl(utf8TableName, keys[i])) {
                continue;
            }
            
            streamKeys << keys[i];
            streamValues << getValuesFromJson(utf8Json, keys[i]);
            
            if (i < keys.size() - 1) {
                streamKeys << ", ";
                streamValues << ", ";
            } else {
                streamKeys << ") ";
                streamValues << ") ";
            }
        }
        
        std::string sql = "INSERT INTO " + utf8TableName + " ";
        sql += streamKeys.str();
        sql += " VALUES ";
        sql += streamValues.str();
        return sql;
    }
    
    std::string SqliteTransfer::makeRemoveSql(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string utf8TableName = args->getString(0);
        amo::u8string ansiTableName(utf8TableName, true);
        ansiTableName.trim_left(amo::u8string(" ", true));
        ansiTableName.trim_right(amo::u8string(" ", true));
        std::vector<amo::u8string> tables = ansiTableName.split(amo::u8string(" ",
                                            true));
                                            
        // 如果拆分出来不只一项,那么认为不是一个表名
        if (tables.size() > 1) {
        
            return "";
        }
        
        std::string utf8WhereString = args->getString(1);
        
        // 如果不是一个合法的JSON，返回""
        if (utf8WhereString.empty()) {
            return "";
        }
        
        //std::vector<std::string> keys = utf8Json.keys();
        std::stringstream stream;
        std::string sql = " DELETE FROM " + utf8TableName;
        stream << sql;
        
        
        if (args->isValid(1)) {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            Any val = args->getValue(2);
            amo::u8string sqlWhere(args->getString(1), true);
            
            if (val.is<amo::u8json>()) {
                amo::u8json json = val;
                sqlWhere = formatArgsByU8Json(sqlWhere, json);
                
            } else if (val.is<std::vector<Any> >()) {
                std::vector<amo::u8string> vec = anyToStringVec(val);
                sqlWhere = formatArgsByArr(sqlWhere, vec);
            }
            
            
            if (sqlWhere.size() > 0) {
                amo::u8string tmp = sqlWhere;
                
                if (tmp.to_upper().find("WHERE") == -1) {
                    stream << " WHERE ";
                }
                
                stream   << sqlWhere.to_utf8();
            }
            
        }
        
        return stream.str();
        
    }
    
    std::string SqliteTransfer::makeUpdateSql(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string utf8TableName = args->getString(0);
        amo::u8string ansiTableName(utf8TableName, true);
        ansiTableName.trim_left(amo::u8string(" ", true));
        ansiTableName.trim_right(amo::u8string(" ", true));
        std::vector<amo::u8string> tables = ansiTableName.split(amo::u8string(" ",
                                            true));
                                            
        // 如果拆分出来不只一项,那么认为不是一个表名
        if (tables.size() > 1) {
        
            return "";
        }
        
        amo::u8json utf8Json = args->getJson(1);
        
        // 如果不是一个合法的JSON，返回""
        if (!utf8Json.is_valid() || !utf8Json.is_object()) {
            return "";
        }
        
        std::vector<std::string> keys = utf8Json.keys();
        std::stringstream stream;
        std::string sql = " UPDATE " + utf8TableName + " SET ";
        stream << sql;
        
        stream << " ";
        
        for (size_t i = 0; i < keys.size(); ++i) {
            // 如果所给字段在表中不存在，那么跳过此字段，这里字符集有点乱
            if (!containsFieldImpl(utf8TableName, keys[i])) {
                continue;
            }
            
            stream << keys[i];
            stream << " = ";
            stream << getValuesFromJson(utf8Json, keys[i]);
            
            if (i < keys.size() - 1) {
                stream << ", ";
            } else {
                stream << "  ";
            }
        }
        
        std::stringstream whereStream;
        
        // 解析 WHERE
        if (args->isValid(2)) {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            Any val = args->getValue(3);
            
            
            if (val.is<amo::u8json>()) {
                amo::u8string sqlWhere(args->getString(2), true);
                amo::u8json json = val;
                whereStream << formatArgsByU8Json(sqlWhere, json).to_utf8();
            } else if (val.is<std::vector<Any> >()) {
                std::vector<amo::u8string> vec = anyToStringVec(val);
                amo::u8string sqlWhere(args->getString(2), true);
                whereStream << formatArgsByArr(sqlWhere, vec).to_utf8();
            } else if (!val.isValid() || val.is<Nil>()) {
                // 如果格式化参数不存在，那么使用第二个参数
                amo::u8string sqlWhere(args->getString(2), true);
                amo::u8json json = args->getJson(1);
                amo::u8string jsonString(json.to_string(), true);
                json = amo::u8json(jsonString);
                whereStream << sqlWhere.format(json).to_utf8();
            }
            
        }
        
        std::string whereString = whereStream.str();
        
        
        
        amo::u8string retval(stream.str(), true);
        retval.trim_right(amo::u8string(" ", true));
        retval.trim_right(amo::u8string(",", true));
        retval += " ";
        
        if (!whereString.empty()) {
            retval += " WHERE ";
            retval += amo::u8string(whereString, true);
        }
        
        return retval.to_utf8();
    }
    
    std::string SqliteTransfer::formatArgs(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        Any val = args->getValue(1);
        
        
        if (val.type() == AnyValueType<amo::u8json>::value) {
            amo::u8string sql(args->getString(0), true);
            amo::u8json json = val;
            return formatArgsByU8Json(sql, json).to_utf8();
            
        } else if (val.type() == AnyValueType<std::vector<Any> >::value) {
            std::vector<amo::u8string> vec = anyToStringVec(val);
            amo::u8string sql(args->getString(0), true);
            return formatArgsByArr(sql, vec).to_utf8();
        }
        
        // 返回原始SQL
        return args->getString(0);
    }
    
    amo::u8string SqliteTransfer::formatArgsByAnsiJson(const amo::u8string& sql,
            amo::u8json& json) {
            
        return sql.format(json);
    }
    
    amo::u8string SqliteTransfer::formatArgsByU8Json(const amo::u8string& sql,
            amo::u8json& json) {
        amo::u8string jsonString(json.to_string(), true);
        amo::u8json ansiJson = amo::u8json(jsonString);
        return  formatArgsByAnsiJson(sql, ansiJson);
    }
    
    amo::u8string SqliteTransfer::formatArgsByArr(const amo::u8string& sql,
            std::vector<amo::u8string>& vec) {
        std::vector<std::string> fmtArgsList;
        
        for (size_t i = 0; i < vec.size(); ++i) {
            fmtArgsList.push_back(vec[i].str());
        }
        
        std::string retval = sql.to_utf8();
        
        
        switch (fmtArgsList.size()) {
        case 0:
            return sql;
            
        case 1:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0]);
            break;
            
        case 2:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1]);
            break;
            
        case 3:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2]);
            break;
            
        case 4:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3]);
            break;
            
        case 5:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3],
                                 fmtArgsList[4]);
            break;
            
        case 6:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3],
                                 fmtArgsList[4],
                                 fmtArgsList[5]);
            break;
            
        case 7:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3],
                                 fmtArgsList[4],
                                 fmtArgsList[5],
                                 fmtArgsList[6]);
            break;
            
        case 8:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3],
                                 fmtArgsList[4],
                                 fmtArgsList[5],
                                 fmtArgsList[6],
                                 fmtArgsList[7]);
            break;
            
        case 9:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3],
                                 fmtArgsList[4],
                                 fmtArgsList[5],
                                 fmtArgsList[6],
                                 fmtArgsList[7],
                                 fmtArgsList[8]);
            break;
            
        case 10:
            retval = amo::format(sql.str(),
                                 fmtArgsList[0],
                                 fmtArgsList[1],
                                 fmtArgsList[2],
                                 fmtArgsList[3],
                                 fmtArgsList[4],
                                 fmtArgsList[5],
                                 fmtArgsList[6],
                                 fmtArgsList[7],
                                 fmtArgsList[8],
                                 fmtArgsList[9]);
            break;
            
        default:
            break;
        }
        
        return amo::u8string(retval, true);
    }
    
    std::string SqliteTransfer::formatPagging(amo::u8json& json) {
    
        amo::u8string sql(" limit({rows}) offset({startrow})", true);
        
        int startrow = json.getInt("rows") * (json.getInt("page") - 1);
        json.put("startrow", startrow);
        return sql.format(json).to_utf8();
        
    }
    
    std::vector<amo::u8string> SqliteTransfer::anyToStringVec(Any& val) {
        std::vector<Any> vec = val;
        std::vector<amo::u8string> retval;
        
        for (size_t i = 0; i < vec.size(); ++i) {
            retval.push_back(amo::u8string(vec[i].As<std::string>(), true));
        }
        
        return retval;
    }
    
    bool SqliteTransfer::queryCountImpl(const std::string& str, amo::u8json& json) {
    
        std::string str2 = json.to_string();
        
        // 不重新计算，直接返回
        if (!json.getBool("refresh")) {
            return true;
        }
        
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return false;
        }
        
        std::regex args("([S|s][E|e][L|l][E|e][C|c][T|t]).*?([F|f][R|r][O|o][M|m])",
                        std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch m;
        std::string sql;
        amo::u8string myStr(str, true);
        myStr = myStr.replace(amo::u8string("\n", true), amo::u8string(" ", true));
        sql = myStr.to_utf8();
        bool bCountSql = false;
        
        while (std::regex_search(sql, m, args)) {
            std::string key = m[0].str();
            
            /* std::regex e(key, std::regex_constants::ECMAScript | std::regex_constants::icase);
            sql = std::regex_replace(sql, e, " SELECT count(*) FROM ");*/
            std::string::size_type pos = 0;
            pos = sql.find(key, pos);
            std::string strDst = " SELECT count(*) FROM ";
            
            if (pos != std::string::npos) {
                sql.replace(pos, key.size(), strDst);
                bCountSql = true;
                break;
            }
            
            setLastError("invalid SELECT COUNT(*) sql");
            return false;
        }
        
        // 判断是否为Select语句
        if (!bCountSql) {
            setLastError("invalid SELECT COUNT(*) sql");
            return false;
        }
        
        try {
        
            sqlite3pp::database& db = *m_pDB;
            sqlite3pp::query qry(db, sql.c_str());
            
            for (sqlite3pp::query::iterator iter = qry.begin();
                    iter != qry.end();
                    ++iter) {
                    
                for (int i = 0; i < qry.column_count(); ++i) {
                    int bytes = (*iter).column_bytes(i);
                    const char* data = (*iter).get<const char*>(i);
                    
                    int total = (*iter).get<int>(i);
                    int page = json.getInt("page");
                    int rows = json.getInt("rows");
                    
                    if (rows < 1) {
                        rows = 1;
                    }
                    
                    int maxpage = ceil(total / (double)rows); //重新计算总页数
                    
                    // 重新计算当前页
                    if (page < 1) {
                        page = 1;
                    }
                    
                    
                    if (page > maxpage) {
                        page = maxpage;
                    }
                    
                    // 更新分页数据
                    json.put("page", page);
                    json.put("rows", rows);
                    json.put("total", (*iter).get<int>(i));
                    json.put("maxpage", maxpage);
                    
                    return true;
                }
                
            }
            
            qry.reset();
            
            
        } catch (std::exception& e) {
            setLastError(e.what());
            return false;
        }
        
        return false;
    }
    
    Any SqliteTransfer::queryCount(IPCMessage::SmartType msg) {
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return -1;
        }
        
        // 生成SQL语句
        std::string sql = makeSql(msg);
        
        if (sql.empty()) {
            // -1 表示获取失败。
            setLastError(SQLITE_EMPTY_SQL);
            return -1;
        }
        
        try {
        
            sqlite3pp::database& db = *m_pDB;
            sqlite3pp::query qry(db, sql.c_str());
            
            for (sqlite3pp::query::iterator iter = qry.begin();
                    iter != qry.end();
                    ++iter) {
                // 只能有一列，还只能有一行
                if (qry.column_count() != 1) {
                    return -1;
                }
                
                for (int i = 0; i < qry.column_count(); ++i) {
                    int total = (*iter).get<int>(i);
                    return total;
                }
            }
            
            qry.reset();
            
            
        } catch (std::exception& e) {
            setLastError(e.what());
            return -1;
        }
        
        return -1;
    }
    
    amo::u8json SqliteTransfer::getPaggingInfo(amo::u8json& other) {
        std::string ss = other.to_string();
        amo::u8json json;
        json.put("page", 1); // 当前页码数
        json.put("rows", 10);		// 每页条数
        json.put("total", 0);		// 数据总条数
        json.put("maxpage", 0);		// 总页数
        json.put("startrow", 0);	// 开始行
        json.put("refresh", true);  //重新计算分页
        
        if (other.contains_key("page")) {
            json.put("page", other.getInt("page"));
        }
        
        if (other.contains_key("rows")) {
            json.put("rows", other.getInt("rows"));
        }
        
        if (other.contains_key("total")) {
            json.put("total", other.getInt("total"));
        }
        
        if (other.contains_key("maxpage")) {
            json.put("maxpage", other.getInt("maxpage"));
        }
        
        if (other.contains_key("startrow")) {
            json.put("startrow", other.getInt("startrow"));
        }
        
        if (other.contains_key("refresh")) {
            json.put("refresh", other.getBool("refresh"));
        }
        
        return json;
    }
    
    std::vector<std::string>& SqliteTransfer::getTableFieldImpl(
        const std::string& table) {
        if (!m_pDB) {
            setLastError(SQLITE_INVALID_CONNECTION);
            return m_emptyFields;
        }
        
        auto iter = m_tableFieldMap.find(table);
        
        if (iter != m_tableFieldMap.end()) {
            return iter->second;
        }
        
        std::string sql = "PRAGMA table_info( " + table + " )";
        
        
        
        
        try {
        
            sqlite3pp::database& db = *m_pDB;
            sqlite3pp::query qry(db, sql.c_str());
            
            
            std::vector<std::string> keys;
            std::vector<std::string> names;
            
            bool bQueryData = true;
            
            // 获取类型名称、字段名
            for (int i = 0; i < qry.column_count(); ++i) {
                const char* columnName = qry.column_name(i);
                
                if (columnName != NULL) {
                    keys.push_back(columnName);
                } else {
                    bQueryData = false;
                    continue;;
                }
                
            }
            
            
            
            for (sqlite3pp::query::iterator iter = qry.begin();
                    iter != qry.end();
                    ++iter) {
                    
                for (int j = 0; j < qry.column_count(); ++j) {
                    if (keys[j] == "name") {
                        names.push_back((*iter).get<std::string>(j));
                        break;
                    }
                }
                
            }
            
            m_tableFieldMap[table] = names;
            return  m_tableFieldMap[table];
            
        } catch (std::exception& e) {
            setLastError(e.what());
            return m_emptyFields;
        }
        
    }
    
    bool SqliteTransfer::containsFieldImpl(const std::string& table,
                                           const std::string& field) {
        auto& vec = getTableFieldImpl(table);
        
        for (auto& p : vec) {
            if (amo::u8string(p, true).to_upper() == amo::u8string(field,
                    true).to_upper()) {
                return true;
            }
        }
        
        return false;
    }
    
    const std::string& SqliteTransfer::getLastError() const {
        return m_strLastError;
    }
    
    void SqliteTransfer::setLastError(const std::string& msg) {
        m_strLastError = msg;
    }
    
    int SqliteTransfer::getFieldStyle() const {
        return nfieldStyle;
    }
    
    void SqliteTransfer::setFieldStyle(int val) {
        nfieldStyle = val;
    }
    
    std::string SqliteTransfer::translateFieldName(const std::string& str) {
    
        switch (nfieldStyle) {
        case FieldUpper:
            return amo::u8string(str, true).to_upper().to_utf8();
            
        case FieldLower:
            return amo::u8string(str, true).to_lower().to_utf8();
            
        case FieldNormal:
        default:
            break;
        }
        
        return str;
    }
    
    std::string SqliteTransfer::translateFieldType(const std::string& str) {
        amo::u8string val(str, true);
        val.to_upper();
        
        std::unordered_map<std::string, std::string> map {
            { "INT", "INTEGER"},
            {"LONG", "INTEGER"},
            {"LONG LONG", "INTEGER"},
            {"SHORT", "INTEGER"},
            {"SMALLINT", "INTEGER"}
        };
        auto iter = map.find(val);
        
        if (iter != map.end()) {
            return iter->second;
        }
        
        return val;
    }
    
    void SqliteTransfer::onBeforeRelease() {
        if (m_pDB) {
            m_pDB.reset();
        }
    }
    
}

