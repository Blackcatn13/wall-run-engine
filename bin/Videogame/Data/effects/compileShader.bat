for /r %%i in (*.fx) do ("%DXSDK_DIR%"Utilities\bin\x86\fxc /LD /Tfx_2_0 /Fo%%~ni.fxc %%i)
pause