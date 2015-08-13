
-- Change the schema as required
create schema types authorization dbo;
GO

-- List
create type types.List external name [Smart.SqlClr.Types].[Smart.SqlClr.Types.List];
GO

create aggregate types.ListCreator(@value sql_variant) returns types.List external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.ListCreator];
create aggregate types.ListCreatorZ(@value sql_variant) returns types.List external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.ListCreatorZ];

-- Sorted List
create aggregate types.SortedListCreator(@value sql_variant) returns types.List external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.SortedListCreator];
create aggregate types.SortedListCreatorZ(@value sql_variant) returns types.List external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.SortedListCreatorZ];
GO

-- Dictionary
create type types.Dictionary external name [Smart.SqlClr.Types].[Smart.SqlClr.Types.Dictionary];
GO

create aggregate types.DictionaryCreator(@key sql_variant, @value sql_variant) returns types.Dictionary external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.DictionaryCreator];
create aggregate types.DictionaryCreatorZ(@key sql_variant, @value sql_variant) returns types.Dictionary external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.DictionaryCreatorZ];

-- Sorted Dictionary
create aggregate types.SortedDictionaryCreator(@key sql_variant, @value sql_variant) returns types.Dictionary external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.SortedDictionaryCreator];
create aggregate types.SortedDictionaryCreatorZ(@key sql_variant, @value sql_variant) returns types.Dictionary external name [Smart.SqlClr.Types].[Smart.SqlClr.Aggregates.SortedDictionaryCreatorZ];

-- Enumerator
create type types.Enumerator external name [Smart.SqlClr.Types].[Smart.SqlClr.Types.Enumerator];
GO

-- Tuple
create type types.Tuple external name [Smart.SqlClr.Types].[Smart.SqlClr.Types.Tuple];
GO

create type types.TupleSet external name [Smart.SqlClr.Types].[Smart.SqlClr.Types.TupleSet];
GO

-- Variant
create type types.Variant external name [Smart.SqlClr.Types].[Smart.SqlClr.Types.Variant];
GO