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

#### List of Data
```
-- Type of the List is inferred from the elements
select types.ListCreator(a).ToSimpleString()
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
	   @list.IndexOf(9.9), -- -1
	   @list.Count; -- Count of items in the list

-- Add a list to another list
declare @list2 types.List = types.List::NewOf('decimal');
set @list2.AddItem(10.10);
set @list2.AddItem(2.11);
set @list.AddFromList(@list2);

set @list.Sort(); -- Sort the list
select @list.ToSimpleString();

-- Null handling
declare @list types.List = types.List::NewOf('string')
set @list.AddItem(null);
set @list.AddItem('abc');
select @list.ToSimpleString()
```

### Dictionary

#### Default Dictionary (string -> sql_variant)
```
declare @map types.Dictionary = types.Dictionary::New();
set @map.AddEntry('hello', 'world');
set @map.AddEntry('world', 2);
select @map.ToSimpleString();
```

#### Typed Dictionary
```
declare @map types.Dictionary = types.Dictionary::NewOf('int', 'varchar');
set @map.AddEntry(1, 'hello');
set @map.AddEntry(2, 'world');
select @map.ToSimpleString();
```

#### Dictionary of Data
```
select types.DictionaryCreator(k, v).ToSimpleString()
from (
	values (1, 2), (3, 4), (5, 6)
) t (k, v);
```

#### Compressed Dictionary
```
select types.DictionaryCreatorZ(object_id, name)
from sys.objects;
```

#### Nested Dictionary
```
declare @map types.Dictionary = types.Dictionary::NewNestedOf('string');
declare @nestedMap types.Dictionary = types.Dictionary::NewOf('string', 'int');
set @nestedMap.AddEntry('world', 2);
set @map.AddNestedEntry('hello', @nestedMap);
select @map.ToSimpleString()
```

#### Sorted Dictionary
```
declare @map types.Dictionary = types.Dictionary::NewSortedOf('int', 'string');
set @map.AddEntry(1, 'hello');
set @map.AddEntry(10, '!');
set @map.AddEntry(2, 'world');
select @map.ToSimpleString();

-- Nested & Sorted
declare @map types.Dictionary = types.Dictionary::NewSortedAndNestedOf('string');
declare @nestedMap types.Dictionary = types.Dictionary::NewOf('int', 'datetime');
set @nestedMap.AddEntry(3, getdate());
set @map.AddNestedEntry('world', @nestedMap);
set @map.AddNestedEntry('hello', @nestedMap);
select @map.ToSimpleString()
```

#### Dictionary Operations
```
declare @map types.Dictionary = types.Dictionary::NewOf('string', 'float');
set @map.AddEntry('abc', cast(5.4 as float));
set @map.AddEntry('def', cast(3.4 as float));
set @map.AddEntry('ghi', cast(7.6 as float));

set @map.AddEntryIfNotExists('ghi', cast(9.9 as float)); -- Add if not exists
set @map.UpdateEntry('def', cast(9.9 as float)); -- Update an entry
set @map.MergeEntry('jkl', cast(19.19 as float)); -- Add or else update
set @map.RemoveEntry('abc'); -- Remove an entry

declare @map2 types.Dictionary = types.Dictionary::NewOf('string', 'float');
set @map2.AddEntry('mno', cast(8.9 as float));

-- Add all entries from a second map, skipping duplicate keys
-- Also, AddFromDictionary() (fails on duplicates),
-- UpdateDictionary(), RemoveDictionary() and MergeDictionary()
set @map.AddDictionary(@map2);

select @map.ToSimpleString(), -- Simple representation of the map
	   @map.ContainsKey('def'), -- Check for a key
	   @map.Get('ghi'), -- Get a key (use GetDictionary() for a nested Dictionary)
	   @map.GetIfExists('lll'); -- Get a key if it exists

select @map.Keys.ToSimpleString(), -- The Keys in the Dictionary
	   @map.[Values].ToSimpleString(), -- The Values in the Dictionary
	   @map.KeyType, -- The type of the keys
	   @map.ValueType, -- The type of the values
	   @map.Count, -- Number of entries in the Dictionary
	   @map.IsCompressed, -- Whether or not the Dictionary is compressed
	   @map.IsSorted, -- Whether or not the Dictionary is sorted
	   @map.IsNested -- Whether or not the Dictionary is nested
```

### Enumerator

#### Iterate over list
```
declare @list types.List = types.List::NewOf('uniqueidentifier');
set @list.AddItem(newid());
set @list.AddItem(newid());
set @list.AddItem(newid());
declare @enumerator types.Enumerator = @list.Items;
while (@enumerator.HasNext = 1)
begin
	select @enumerator.CurrentItem;
	set @enumerator.MoveNext();
end
```
