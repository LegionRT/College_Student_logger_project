CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -fexec-charset=CP866 -finput-charset=UTF-8

TARGET_APP = StudentApp.exe
TARGET_DLL = StudentDLL.dll

all: app

app:
	@echo [1/2] Сборка StudentDLL.dll...
	$(CXX) $(CXXFLAGS) -shared -DSTUDENTDLL_EXPORTS -o $(TARGET_DLL) StudentDLL/StudentDLL.cpp StudentDLL/dllmain.cpp StudentCore/StudentCore.cpp
	@echo [2/2] Сборка StudentApp.exe...
	$(CXX) $(CXXFLAGS) -o $(TARGET_APP) StudentApp/StudentApp.cpp -L. -lStudentDLL -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic
	@echo === Готово! ===
	@del /q *.o *.a *.lib 2>nul

run:
	$(TARGET_APP)

clean:
	@del /q *.o *.a *.dll *.lib *.exe 2>nul
	@del /q StudentCore\*.o 2>nul
	@del /q StudentDLL\*.o 2>nul
	@del /q StudentApp\*.o 2>nul

.PHONY: all run clean