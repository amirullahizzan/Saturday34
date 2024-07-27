::============================================================================
:: .buildディレクトリを削除
::============================================================================
@echo off
@setlocal enabledelayedexpansion
chcp 65001 > nul

pushd "%~dp0"

:: 削除済の場合はスキップ
if not exist .build goto finish

::-------------------------------------------------------------------------------
:: 削除
::-------------------------------------------------------------------------------
set CURRENT_DIRECTORY=%cd%

:: .buildがファイルになっている場合？があるため、ファイルとしての削除を試行
del /f /s /q .build > nul 2>&1

:: 削除済の場合はスキップ
if not exist .build goto finish

cd .build > nul 2>&1

:: ディレクトリ移動できなかった場合を検出
:: シンボリックリンクのリンク先が消滅している場合に発生
if "%cd%"=="%CURRENT_DIRECTORY%" (
	rd /s /q .build
	goto finish
)

:: 全ディレクトリを削除
for /f "usebackq delims=" %%a in (`dir /b /ad`)  do (
	rd /s /q "%%a"
)
:: 全ファイルを削除
del /q *.* > nul 2>&1

:: 隠しファイル
del /q /a:h *.* > nul 2>&1

:: 完全削除ではなくwarm deleteの場合は.buildの削除はしない
if "%1"=="warm" goto finish

::-------------------------------------------------------------------------------
:: .buildの中が空になったら.buildを削除
:: シンボリックリンクで結合している場合の対策で、参照先を全削除してからリンクを削除
::-------------------------------------------------------------------------------
set EXIST_ANY_FILE=0

:: 検索
for /f "usebackq delims=" %%a in (`dir /b /a`)  do (
	set EXIST_ANY_FILE=1
)

:: 空ならディレクトリを削除
if %EXIST_ANY_FILE%==0 (
	cd..
	rd .build
)

:finish

popd

::============================================================================
:: .ccccディレクトリを削除
::============================================================================
pushd "%~dp0"
if exist .cccc (
	rd /s /q .cccc
)
popd

::============================================================================
:: doxygenの生成ディレクトリを削除
::============================================================================
pushd "%~dp0document"
if exist html (
	rd /s /q html
)
if exist latex (
	rd /s /q latex
)
popd

@endlocal
