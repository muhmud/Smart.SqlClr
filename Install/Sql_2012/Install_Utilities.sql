
-- Change schema as required
create schema utils authorization dbo;
GO

-- Binary <-> Hex
create function utils.ToHex (@value varbinary(max)) returns nvarchar(max) external name [Smart.SqlClr].[Smart.SqlClr.Utils.Binary].[ToHex];
GO

-- Hex <-> Binary
create function utils.ToBytes (@value nvarchar(max)) returns varbinary(max) external name [Smart.SqlClr].[Smart.SqlClr.Utils.Binary].[ToBytes];
GO
