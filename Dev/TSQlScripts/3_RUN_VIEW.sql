USE PHONEBOOK
GO

BEGIN TRY
BEGIN TRANSACTION

IF (
	EXISTS (
		SELECT 1
		FROM sys.objects
		WHERE object_id = OBJECT_ID(N'CITIES')
		AND TYPE IN (N'U')
	)
	AND EXISTS (
		SELECT 1
		FROM sys.objects
		WHERE object_id = OBJECT_ID(N'PHONE_TYPES')
		AND TYPE IN (N'U')
	)
	AND EXISTS (
		SELECT 1
		FROM sys.objects
		WHERE object_id = OBJECT_ID(N'PERSONS')
		AND TYPE IN (N'U')
	)
	AND EXISTS (
		SELECT 1
		FROM sys.objects
		WHERE object_id = OBJECT_ID(N'PHONE_NUMBERS')
		AND TYPE IN (N'U')
	)
	AND EXISTS(
		select * FROM sys.views where name = N'ALL_DATA_VIEW'
	)
)
	BEGIN
		EXEC ('SELECT * FROM ALL_DATA_VIEW')
	END
ELSE
	BEGIN
		PRINT N'No such view found, or one of the tables is missing'
	END
	COMMIT TRANSACTION
END TRY
BEGIN CATCH
	PRINT ERROR_MESSAGE()
	ROLLBACK TRANSACTION
END CATCH
GO