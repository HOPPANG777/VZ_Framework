@echo off
REM ============================
REM 매개변수 검증
REM ============================
IF "%~1"=="" (
    echo 대상 디렉토리가 지정되지 않았습니다.
    echo 사용법: CopyData.bat [대상 디렉토리]
    exit /b 1
)

REM ============================
REM 설정 부분: 소스 및 대상 경로 정의
REM ============================

REM 현재 배치 파일이 위치한 디렉토리를 기준으로 설정
SET "SOURCE_DIR=%~dp0"

REM 전달된 첫 번째 매개변수를 대상 디렉토리로 설정
SET "DEST_DIR=%~1\_Data"

REM ============================
REM 기존 _Data 폴더 삭제 (갱신을 위해)
REM ============================

IF EXIST "%DEST_DIR%" (
    echo 기존 _Data 폴더 삭제 중...
    rd /s /q "%DEST_DIR%"
)

REM ============================
REM _Data 폴더 생성
REM ============================

echo _Data 폴더 생성 중: %DEST_DIR%
mkdir "%DEST_DIR%"

REM ============================
REM 소스 폴더 복사
REM ============================

echo 소스 폴더 복사 중...

REM xcopy 옵션 설명:
REM /E - 모든 하위 디렉토리 및 빈 디렉토리 포함
REM /I - 대상이 디렉토리일 경우 묻지 않음
REM /Y - 기존 파일 덮어쓰기 확인하지 않음
xcopy "%SOURCE_DIR%_Data\_Textures" "%DEST_DIR%\_Textures" /E /I /Y
xcopy "%SOURCE_DIR%_Data\_Sounds" "%DEST_DIR%\_Sounds" /E /I /Y
xcopy "%SOURCE_DIR%_Data\_TileData" "%DEST_DIR%\_TileData" /E /I /Y
xcopy "%SOURCE_DIR%_Data\_Shaders" "%DEST_DIR%\_Shaders" /E /I /Y
xcopy "%SOURCE_DIR%_Data\_Fonts" "%DEST_DIR%\_Fonts" /E /I /Y

echo 복사 완료.