﻿USE PHONEBOOK
GO

DECLARE @VAR_PHONE_NUMBER NVARCHAR(32);
DECLARE @VAR_NEW_PHONE_TYPE NVARCHAR(32);
DECLARE @VAR_NEW_PHONE_NUMBER NVARCHAR(32);

SET @VAR_PHONE_NUMBER = N'+35927036265'
                   
SET @VAR_NEW_PHONE_TYPE = N'Мобилен'
SET @VAR_NEW_PHONE_NUMBER = N'+35927036265'

IF NOT EXISTS (
	SELECT *
	FROM PHONE_TYPES
	WHERE PHONE_TYPE = @VAR_NEW_PHONE_TYPE
)
PRINT N'No such phone type found'

IF (
	EXISTS (
		SELECT *
		FROM PHONE_NUMBERS
		WHERE PHONE_NUMBER = @VAR_NEW_PHONE_NUMBER
	)
	AND @VAR_NEW_PHONE_NUMBER != @VAR_PHONE_NUMBER
)
PRINT N'Phone number is not unique'

BEGIN TRY
BEGIN TRANSACTION

UPDATE PHONE_NUMBERS
SET PHONE_TYPE_ID = (
	SELECT ID
	FROM PHONE_TYPES 
	WHERE PHONE_TYPE = @VAR_NEW_PHONE_TYPE
	)
FROM PHONE_NUMBERS
WHERE PHONE_NUMBER = @VAR_PHONE_NUMBER

UPDATE PHONE_NUMBERS
SET PHONE_NUMBER = @VAR_NEW_PHONE_NUMBER
FROM PHONE_NUMBERS
WHERE PHONE_NUMBER = @VAR_PHONE_NUMBER

	COMMIT TRANSACTION
END TRY
BEGIN CATCH
	PRINT ERROR_MESSAGE()
	ROLLBACK TRANSACTION
END CATCH
GO