﻿USE PHONEBOOK;
GO


IF EXISTS(
	SELECT 1
	FROM sys.foreign_keys
	WHERE OBJECT_NAME(sys.foreign_keys.parent_object_id) = N'PERSONS'
	AND sys.foreign_keys.NAME = N'FK_PERSON_CITY_ID'
)
BEGIN
	ALTER TABLE PERSONS
	DROP CONSTRAINT FK_PERSON_CITY_ID
END

IF EXISTS(
	SELECT 1
	FROM sys.foreign_keys
	WHERE OBJECT_NAME(sys.foreign_keys.parent_object_id) = N'PHONE_NUMBERS'
	AND sys.foreign_keys.NAME = N'FK_PHONE_NUMBER_PERSON_ID'
)
BEGIN
	ALTER TABLE PHONE_NUMBERS
	DROP CONSTRAINT FK_PHONE_NUMBER_PERSON_ID
END

IF EXISTS(
	SELECT 1
	FROM sys.foreign_keys
	WHERE OBJECT_NAME(sys.foreign_keys.parent_object_id) = N'PHONE_NUMBERS'
	AND sys.foreign_keys.NAME = N'FK_PHONE_NUMBER_TYPE_ID'
)
BEGIN
	ALTER TABLE PHONE_NUMBERS
	DROP CONSTRAINT FK_PHONE_NUMBER_TYPE_ID
END


IF EXISTS (
	SELECT 1
	FROM sys.objects
	WHERE object_id = OBJECT_ID(N'CITIES')
	AND TYPE IN (N'U')
)
BEGIN
	DROP TABLE CITIES
END

CREATE TABLE CITIES (
	ID INT IDENTITY(1,1) NOT NULL,
	UPDATE_COUNTER INT NOT NULL,
	CITY_NAME NCHAR(64) NOT NULL,
	AREA NCHAR (64) NOT NULL
	CONSTRAINT PK_CITY_ID PRIMARY KEY(ID)
)

CREATE UNIQUE NONCLUSTERED INDEX UX_CITIES_CITY_NAME_AREA
	ON CITIES (CITY_NAME, AREA);

IF EXISTS (
	SELECT 1
	FROM sys.objects
	WHERE object_id = OBJECT_ID(N'PHONE_TYPES')
	AND TYPE IN (N'U')
)
BEGIN
	DROP TABLE PHONE_TYPES;
END

CREATE TABLE PHONE_TYPES (
	ID INT IDENTITY(1,1) NOT NULL,
	UPDATE_COUNTER INT NOT NULL,
	PHONE_TYPE NCHAR(32) NOT NULL,
	CONSTRAINT PK_PHONE_TYPE_ID PRIMARY KEY(ID)
)

CREATE UNIQUE INDEX IX_PHONE_TYPES_PHONE_TYPE
	ON PHONE_TYPES (PHONE_TYPE)

IF EXISTS (
	SELECT 1
	FROM sys.objects
	WHERE object_id = OBJECT_ID(N'PERSONS')
	AND TYPE IN (N'U')
)
BEGIN
	DROP TABLE PERSONS;
END

CREATE TABLE PERSONS (
	ID INT IDENTITY(1,1) NOT NULL,
	UPDATE_COUNTER INT NOT NULL,
	FIRST_NAME NCHAR(64) NOT NULL,
	MIDDLE_NAME NCHAR(64) NOT NULL,
	LAST_NAME NCHAR(64) NOT NULL,
	UCN NCHAR(32) NOT NULL,
	CITY_ID INT NOT NULL,
	PERSON_ADDRESS NCHAR(512)

	CONSTRAINT PK_PERSON_ID
		PRIMARY KEY(ID),
	CONSTRAINT FK_PERSON_CITY_ID
		FOREIGN KEY (CITY_ID)
		REFERENCES CITIES(ID)
)

CREATE UNIQUE INDEX IX_PERSON_UCN
	ON PERSONS (UCN)

IF EXISTS (
	SELECT 1
	FROM sys.indexes
	WHERE NAME = 'IX_PERSONS_CITY_ID'
	AND object_id = OBJECT_ID(N'PERSONS')
)
BEGIN
	DROP INDEX IX_PERSONS_CITY_ID
	ON PERSONS
END

CREATE INDEX IX_PERSONS_CITY_ID
	ON PERSONS(CITY_ID)


IF EXISTS (
	SELECT 1
	FROM sys.objects
	WHERE object_id = OBJECT_ID(N'PHONE_NUMBERS')
	AND TYPE IN (N'U')
)
BEGIN
	DROP TABLE PHONE_NUMBERS;
END

CREATE TABLE PHONE_NUMBERS (
	ID INT IDENTITY(1,1) NOT NULL,
	UPDATE_COUNTER INT NOT NULL,
	PERSON_ID INT NOT NULL,
	PHONE_TYPE_ID INT NOT NULL,
	PHONE_NUMBER CHAR(32) NOT NULL

	CONSTRAINT PK_PHONE_NUMBER_ID 
		PRIMARY KEY(ID),
	CONSTRAINT FK_PHONE_NUMBER_PERSON_ID
		FOREIGN KEY(PERSON_ID) 
		REFERENCES PERSONS(ID),
	CONSTRAINT FK_PHONE_NUMBER_TYPE_ID
		FOREIGN KEY(PHONE_TYPE_ID) 
		REFERENCES PHONE_TYPES(ID)
);

CREATE UNIQUE INDEX IX_PHONE_NUMBERS_PHONE_NUMBER
	ON PHONE_NUMBERS (PHONE_NUMBER)

IF EXISTS (
	SELECT 1
	FROM sys.indexes
	WHERE NAME = 'IX_PHONE_NUMBER_PERSON_ID'
	AND object_id = OBJECT_ID(N'PHONE_NUMBERS')
)
BEGIN
	DROP INDEX IX_PHONE_NUMBER_PERSON_ID
	ON PHONE_NUMBERS
END

CREATE INDEX IX_PHONE_NUMBER_PERSON_ID
	ON PHONE_NUMBERS(PERSON_ID)

IF EXISTS (
	SELECT 1
	FROM sys.indexes
	WHERE NAME = 'IX_PHONE_NUMBER_TYPE_ID'
	AND object_id = OBJECT_ID(N'PHONE_NUMBERS')
)
BEGIN
	DROP INDEX IX_PHONE_NUMBER_TYPE_ID
	ON PHONE_NUMBERS
END

CREATE INDEX IX_PHONE_NUMBER_TYPE_ID
	ON PHONE_NUMBERS(PHONE_TYPE_ID)

PRINT N'Created tables: CITIES, PERSONS, PHONE_TYPES, PHONE_NUMBERS'
PRINT FORMAT(GETDATE(), 'dd-MM-yyyy HH:mm:ss')