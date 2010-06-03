@echo off
if "%1" == "" goto error

3rdParty\CodeFormatter\astyle  --indent=spaces=4 --indent-switches --indent-namespaces --indent-labels --indent-preprocessor --indent-col1-comments --max-instatement-indent=80 --pad-header --unpad-paren --convert-tabs --align-pointer=middle --suffix=none --lineend=windows --recursive "%1"
goto end

:error
echo "Usage FormatCpp.bat HPPFile"

:end
