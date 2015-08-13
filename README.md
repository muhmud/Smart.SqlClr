# Smart.SqlClr
SQL Server SqlClr.net library with types &amp; utility functions

## Installation

Simply execute the ```Install_Assemblies.sql``` script from the ```Install``` folder for your version of SQL server. 

If you want types then proceed to execute the ```Install_Types.sql``` script and for utilities, the ```Install_Utilities.sql``` script. The schema and type/function/aggregate names in these files can be tailored to how you would like them.

## Examples

### List

#### List of sql_variant
```
declare @list types.List = types.List::New();
set @list.AddItem(1);
set @list.AddItem(2);
set @list.AddItem('Hello');

-- ToSimpleString() provides a simple representation of the elements in the collection.
-- ToString() uses an Xml representation
select @list.ToSimpleString();
```

#### Typed List
```
declare @list types.List = types.List::NewOf('int')
set @list.AddItem(1);
set @list.AddItem(2);
set @list.AddItem(3);
select @list.ToSimpleString();
```
