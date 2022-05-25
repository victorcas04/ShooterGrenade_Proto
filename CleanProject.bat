set /P yes=Do you want to clean all temporal files?:
IF %yes%==y (
IF EXIST "./Saved" (RD /S /Q "./Saved") ELSE (ECHO SAVED already clean)
IF EXIST "./Intermediate" (RD /S /Q "./Intermediate") ELSE (ECHO INTERMEDIATE already clean)
IF EXIST "./Binaries" (RD /S /Q "./Binaries") ELSE (ECHO BINARIES already clean)
IF EXIST "./.vs" (RD /S /Q "./.vs") ELSE (ECHO .VS already clean)
del "%~dp0\ShooterGrenade_Proto.sln"
)