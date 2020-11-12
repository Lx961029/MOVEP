@echo off

set keep_debug="D:\working\HTPlatform\Debug"

for /r  %%s in (.vs) do (
	if exist %%s (
		rd /s /q %%s
	)
)

for /r  %%s in (x64) do (
	if exist %%s (
		rd /s /q %%s
	)
)

for /r . %%a in (Debug) do (
	if "%%a" == %keep_debug% (
		for /r . %%s in (*.exp) do (
			if exist %%s (
				del /q %%s  
			)
		)

		for /r . %%s in (*.ilk) do (
			if exist %%s (
				del /q %%s  
			)
		)

		for /r . %%s in (*.pdb) do (
			if exist %%s (
				del /q %%s 
			)
		)
	) else (
		rd /s /q %%a
	)
)

