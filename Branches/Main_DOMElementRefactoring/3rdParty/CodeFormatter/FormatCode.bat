set COMMON_OPTIONS=--indent=spaces=4 --indent-switches --indent-namespaces --indent-labels --indent-preprocessor --indent-col1-comments --max-instatement-indent=80 --pad-header --unpad-paren --convert-tabs --align-pointer=middle --suffix=none --lineend=windows --recursive
astyle --brackets=break $COMMON_OPTIONS *.h
astyle $COMMON_OPTIONS *.cpp

