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

#### ListCreator aggregate function
```
-- Type of the List is inferred from the elements
select types.ListCreator(a).ToString()
from (
	values (1), (2), (3)
) t(a);
```

#### Compressed Lists
```
select type, types.ListCreatorZ(object_id) as all_object_ids
from sys.objects
group by type;
```

#### List Operations
```
declare @list types.List = types.List::NewOf('decimal');
set @list.AddItem(4.5);
set @list.AddIfNotExists(3.4); -- Set-like operation
set @list.AddItem(5.6);
set @list.AddItem(9.9);

set @list.Remove(9.9); 
set @list.RemoveIfExists(9.9); -- Removal of non-existent item throws

set @list.InsertAt(1, 7.3); -- Positional insert/removal
set @list.RemoveAt(1);

set @list.SetCompression(1); -- Set/Unset compression

select @list.ToSimpleString(), -- String representation of the list
	   @list.ContainsValue(3.4), -- 1 (True)
	   @list.Get(0), -- 3.4,
	   @list.IndexOf(5.6), -- 2
	   @list.IndexOf(9.9); -- -1

set @list.Sort(); -- Sort the list
select @list.ToSimpleString();
```
