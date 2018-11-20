# Sqlite

  Sqlite3 数据库封装类，用于操作本地数据库文件.<br>工作线程**Renderer线程**
  
## Sqlite &nbsp;<span class="label label-constructor">构造</span> 

  创建一个Sqlite对象.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>数据库文件路径，如果该数据库不存在，那么将创建一个新的数据库.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Object Sqlite 实例对象. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
include('Sqlite');
window.db = new Sqlite('test.db');

```


<div class="adoc" id="div_Sqlite"></div>


## import &nbsp;<span class="label label-sync">同步</span> 

  从文件中执行SQL语句.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>文件路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int /Undefined 如果成为返回受影响的行数，否则返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
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


<div class="adoc" id="div_import"></div>


## execute &nbsp;<span class="label label-sync">同步</span> 

  执行原生sql.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>需要执行的SQL语句.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int /Undefined 如果成为返回受影响的行数，否则返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
console.assert(db.execute('SELECT count(1) FROM table1') == 0);

```


<div class="adoc" id="div_execute"></div>


## insert &nbsp;<span class="label label-sync">同步</span> 

  插入数据.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>数据库表名.</td>
</tr><tr>
	<td>JsonObject </td>
	<td></td>
	<td>要插入的数据.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int/Undefined 如果成为返回id，否则返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var val = {
    name: '张三',
    age:18,
    score:99,
    remark:'测试数据'
};
console.assert(db.insert('table1', val) ==1);
console.assert(db.insert('table1', {name:'李四'}) ==2);


```


<div class="adoc" id="div_insert"></div>


## update &nbsp;<span class="label label-sync">同步</span> 

  更新数据.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>数据库表名.</td>
</tr><tr>
	<td>JsonObject </td>
	<td></td>
	<td>需要更新的数据.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>条件语句(如果第四个参数存在，那么使用第四个参数格式化该字符串，如果不存在，那么使用第二个参数格式化字符串).</td>
</tr><tr>
	<td>JsonObject</td>
	<td>Undefined </td>
	<td>条件语句格式化参数.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int /Undefined 如果成为返回受影响的行数，否则返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
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


<div class="adoc" id="div_update"></div>


## backup &nbsp;<span class="label label-sync">同步</span> 

  备份数据库.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>备份文件路径.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true成功/false失败. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
console.assert(db.backup('test2.db') == true);

```


<div class="adoc" id="div_backup"></div>


## query &nbsp;<span class="label label-sync">同步</span> 

  查询数据.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>SQL语句.</td>
</tr><tr>
	<td>JsonObject</td>
	<td>{} </td>
	<td>格式化参数，可以不填.</td>
</tr><tr>
	<td>JsonObject</td>
	<td>Undefined </td>
	<td>分页参数，可以不填(不填将不会使用分页功能).<br>[**Int**] page 当前页码数，默认为1.<br>[**Int**] rows 每页条数，默认为10.<br>[**Int**] total 数据总系数，由程序返回<br>[**Int**] maxpage 总页数，由程序返回<br>[**Int**] startrow 开始行，<br>[**Boolean**] refresh 是否重新计算分页，默认为true.<br></td>
</tr>
	</tbody>
</table>

* **返回值**
  JsonObject 查询到的数据.
	* **Int** page  当前页码数，分页查询时返回.
	* **Int** rows  每页条数，分页查询时返回.
	* **Int** total  数据总系数，分页查询时返回.
	* **Int** maxpage  总页数，分页查询时返回.
	* **Int** startrow  开始行，分页查询时返回.
	* **Boolean** refresh  是否重新计算分页，分页查询时返回.
	* **JsonArray** data  查询到的数据.
 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var result = db.query('SELECT * FROM table1 WHERE name = "张三"');
console.log(result);
var result2 = db.query('SELECT * FROM table1 WHERE name = "{0}"', ['张三']);
console.log(result2);
var result3 = db.query('SELECT * FROM table1 WHERE name = "{name}" and age = "{age}" ', {name:'张三', age:'18'});
console.log(result3);
var result4 = db.query('SELECT * FROM table1 WHERE name = "{name}" and age = "{age}" ', {name:'张三', age:'18'}, {rows:10});
console.log(result4);

```


<div class="adoc" id="div_query"></div>


## remove &nbsp;<span class="label label-sync">同步</span> 

  删除数据.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>表名.</td>
</tr><tr>
	<td>String </td>
	<td></td>
	<td>条件语句.</td>
</tr><tr>
	<td>JsonObject</td>
	<td>{} </td>
	<td>条件参数，可以不填, 也可以是一个Array.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int /Undefined 如果成为返回受影响的行数，否则返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
console.assert(db.remove('table1', '  name="{0}"', ['李四']) == 1);
db.insert('table1', {name:'李四'});
console.assert(db.remove('table1', '  name="{name}"', {name:'李四'}) == 1);
db.insert('table1', {name:'李四'});
console.assert(db.remove('table1', ' name="李四"') == 1);


```


<div class="adoc" id="div_remove"></div>


## queryCount &nbsp;<span class="label label-sync">同步</span> 

  查询数据条数.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>SQL语句.</td>
</tr><tr>
	<td>JsonObject</td>
	<td>{} </td>
	<td>条件参数.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Int 成功返回数据条数，失败返回-1. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
db.insert('table1', {name:'李四'});
console.assert(db.remove('table1', 'name="{0}"', ['李四']) == 1);
console.assert(db.queryCount('SELECT COUNT(1) FROM table1 WHERE  name="{0}"', ['李四']) == 0);


```


<div class="adoc" id="div_queryCount"></div>


## getLastInsertRowID &nbsp;<span class="label label-sync">同步</span> 

  返回最近一次SQL语句所影响的行数，如插入、删除、更新等操作后可以调用该函数获取数据.
  
* **函数参数**  无

* **返回值**
  Int /Undefined 如果成为返回受影响的行数，否则返回Undefined. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
var id  = db.insert('table1', {name:'李四'});
console.assert(db.getLastInsertRowID() == id);

```


<div class="adoc" id="div_getLastInsertRowID"></div>


## containsTable &nbsp;<span class="label label-sync">同步</span> 

  数据库中是否存在指定的表.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>表名.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean true存在/false失败或不存在. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
console.assert(db.containsTable('table1') == true);
console.assert(db.containsTable('table2') == false);

```


<div class="adoc" id="div_containsTable"></div>


## containsField &nbsp;<span class="label label-sync">同步</span> 

  指定表中是否包含某字段.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String	</td>
	<td></td>
	<td>表名.</td>
</tr><tr>
	<td>String	</td>
	<td></td>
	<td>字段名.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Boolean . true存在/false失败或不存在. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
console.assert(db.containsField('table1', 'name') == true);
console.assert(db.containsField('table1', 'name2') == false);

```


<div class="adoc" id="div_containsField"></div>


## getTableFields &nbsp;<span class="label label-sync">同步</span> 

  获取指定表中的所有字段.
  
* **函数参数**

<table class="table table-hover table-bordered ">
	<thead>
		<tr>
			<th class="col-xs-1">类型</th>
			<th class="col-xs-1">默认值</th>
			<th>说明</th>
		</tr>
	</thead>
	<tbody>
		<tr>
	<td>String </td>
	<td></td>
	<td>表名.</td>
</tr>
	</tbody>
</table>

* **返回值**
  Array 包含所有字段名的数组. 

* **示例&nbsp;&nbsp;&nbsp;&nbsp;**

```html
console.log(db.getTableFields('table1'));

```


<div class="adoc" id="div_getTableFields"></div>


