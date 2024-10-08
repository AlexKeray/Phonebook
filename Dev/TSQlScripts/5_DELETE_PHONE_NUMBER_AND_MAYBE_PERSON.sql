USE PHONEBOOK
GO

DECLARE @VAR_PHONE_NUMBER NVARCHAR(32);
DECLARE @VAR_PERSON_ID INT;
DECLARE @VAR_PERSON_FIRST_NAME NVARCHAR(64);
DECLARE @VAR_PERSON_LAST_NAME NVARCHAR(64);
DECLARE @VAR_PERSON_UCN NVARCHAR(32);

SET @VAR_PHONE_NUMBER = N'02 5 03 39                      ';

IF NOT EXISTS (
	SELECT *
	FROM PHONE_NUMBERS
	WHERE PHONE_NUMBER = @VAR_PHONE_NUMBER
)
PRINT N'The phone number ' + @VAR_PHONE_NUMBER + N' wanst found'

BEGIN TRY
BEGIN TRANSACTION

	SET @VAR_PERSON_ID = 
	(
		SELECT TOP 1 PERSON_ID
		FROM PHONE_NUMBERS
		WHERE PHONE_NUMBER = @VAR_PHONE_NUMBER
	)

	SET @VAR_PERSON_FIRST_NAME = 
		(
			SELECT TOP 1 FIRST_NAME 
			FROM PERSONS
			WHERE ID = @VAR_PERSON_ID
		)

	SET @VAR_PERSON_LAST_NAME = 
		(
			SELECT TOP 1 LAST_NAME 
			FROM PERSONS
			WHERE ID = @VAR_PERSON_ID
		)

	SET @VAR_PERSON_UCN = 
		(
			SELECT TOP 1 UCN 
			FROM PERSONS
			WHERE ID = @VAR_PERSON_ID
		)

	DELETE FROM PHONE_NUMBERS
	WHERE PHONE_NUMBER = @VAR_PHONE_NUMBER

	IF NOT EXISTS (
		SELECT *
		FROM PHONE_NUMBERS
		WHERE PERSON_ID = @VAR_PERSON_ID
	)
	BEGIN
		DELETE FROM PERSONS
		WHERE ID = @VAR_PERSON_ID
		PRINT N'The person ' + RTRIM(@VAR_PERSON_FIRST_NAME) + N' ' +
			RTRIM(@VAR_PERSON_LAST_NAME) + N' with the UCN: ' + 
			RTRIM(@VAR_PERSON_UCN) + N' was deleted'
	END

COMMIT TRANSACTION

END TRY
BEGIN CATCH
	PRINT ERROR_MESSAGE()
	ROLLBACK TRANSACTION
END CATCH
GO