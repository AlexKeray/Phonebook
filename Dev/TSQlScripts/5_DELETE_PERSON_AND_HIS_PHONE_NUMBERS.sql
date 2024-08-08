USE PHONEBOOK
GO

DECLARE @VAR_UCN NVARCHAR(32);
SET @VAR_UCN = N'1815035454';

IF NOT EXISTS (
	SELECT *
	FROM PERSONS
	WHERE UCN = @VAR_UCN
)
PRINT N'The UCN ' + @VAR_UCN + N' wanst found'

BEGIN TRY
BEGIN TRANSACTION

	DELETE FROM PHONE_NUMBERS
	WHERE PERSON_ID = (
		SELECT ID
		FROM PERSONS
		WHERE UCN = @VAR_UCN
	)

	DELETE FROM PERSONS
	WHERE UCN = @VAR_UCN

COMMIT TRANSACTION

END TRY
BEGIN CATCH
	PRINT ERROR_MESSAGE()
	ROLLBACK TRANSACTION
END CATCH
GO