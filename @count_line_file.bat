:: 全ソースの行数を数える
@echo off
@setlocal enabledelayedexpansion

cd /d "%~dp0"
echo 【ソースコード行数】

pushd "%~dp0src"

set hoge=0

for /r %%i in (*.c *.h *.cpp *.hpp *.cxx *.hxx *.inl) do (
	echo %%i
	set lines=
	for /f %%a in ('type %%i ^| find /c /v ""') do (
		set /a lines=%%a
	)
	echo !lines!
	
	set /a hoge+=lines
)

echo %hoge%

pause

goto end

:end











