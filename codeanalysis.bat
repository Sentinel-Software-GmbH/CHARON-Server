cppcheck --enable=all --inline-suppr src/ port/windows/src port/stm32f4-discovery/Src --suppress=variableScope --inconclusive --xml --xml-version=2 --force -i port/stm32f4-discovery/Src/stm32f4xx_hal_msp.c -i port/stm32f4-discovery/Src/stm32f4xx_it.c -i port/stm32f4-discovery/Src/system_stm32f4xx.c 2> cppcheck.xml
py toolchain/htmlreport/cppcheck-htmlreport --file="cppcheck.xml" --report-dir=build/cppCheckReport
