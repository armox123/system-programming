#include <iostream>
#include <windows.h>
#include <string>
#include <io.h>
#include <fcntl.h>

int main() {
    // НАСТРОЙКА КОДИРОВКИ
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    std::wcout << L"Управление процессами в Windows" << std::endl;
    std::wcout << std::endl;

    // ========================================================
    // ПОДГОТОВКА СТРУКТУР ДЛЯ CreateProcess
    // ========================================================
    // Вам нужно:
    //   1) Объявить структуру STARTUPINFO с именем si
    //      - Очистить её через ZeroMemory(&si, sizeof(si))
    //      - Заполнить поле si.cb = sizeof(si)
    //   2) Объявить структуру PROCESS_INFORMATION с именем pi
    //      - Очистить её через ZeroMemory(&pi, sizeof(pi))
    //
    // Напишите код здесь:
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));


    // ========================================================
    // СОЗДАНИЕ ДОЧЕРНЕГО ПРОЦЕССА
    // ========================================================
    // Вам нужно:
    //   1) Подготовить строку с путём к программе.
    //      Важно: CreateProcess может изменять строку, поэтому
    //      нельзя использовать const. Объявите массив:
    //      wchar_t commandLine[] = L"notepad.exe";
    //
    //   2) Вызвать функцию CreateProcess со следующими параметрами:
    //      - lpApplicationName: NULL (используем commandLine)
    //      - lpCommandLine: commandLine (НЕ const!)
    //      - lpProcessAttributes: NULL
    //      - lpThreadAttributes: NULL
    //      - bInheritHandles: FALSE
    //      - dwCreationFlags: 0
    //      - lpEnvironment: NULL
    //      - lpCurrentDirectory: NULL
    //      - lpStartupInfo: &si
    //      - lpProcessInformation: &pi
    //
    //   3) Если функция вернула FALSE:
    //      - Вывести сообщение об ошибке и код GetLastError()
    //      - Завершить программу через return 1
    //      Если TRUE:
    //      - Вывести PID процесса (pi.dwProcessId)
    //
    // Напишите код здесь:
    wchar_t commandLine[] = L"notepad.exe";
    BOOL result = CreateProcess(
        NULL,
        commandLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
    if (result == FALSE) {
        std::wcout << L"Ошибка запуска процесса, код: " << GetLastError() << std::endl;
        return 1;
    }
    else {
        std::wcout << L"Процесс запущен, PID: " << pi.dwProcessId << std::endl;
    }

    std::wcout << L"Ожидаем завершения процесса (таймаут 30 секунд)..." << std::endl;
    std::wcout << L"Закройте блокнот вручную или дождитесь таймаута." << std::endl;
    std::wcout << std::endl;

    // ========================================================
    // ОЖИДАНИЕ ЗАВЕРШЕНИЯ И ПОЛУЧЕНИЕ КОДА ВЫХОДА
    // ========================================================
    // Вам нужно:
    //   1) Вызвать WaitForSingleObject для ожидания завершения:
    //      - hHandle: pi.hProcess (дескриптор процесса)
    //      - dwMilliseconds: 30000 (30 секунд)
    //   2) Проверить результат:
    //      - WAIT_OBJECT_0 - процесс завершился успешно
    //      - WAIT_TIMEOUT - истёк таймаут
    //      - WAIT_FAILED - ошибка ожидания
    //   3) Если процесс завершился (WAIT_OBJECT_0):
    //      - Объявить переменную DWORD exitCode
    //      - Вызвать GetExitCodeProcess(pi.hProcess, &exitCode)
    //      - Вывести код завершения
    //   4) Если таймаут - вывести сообщение и принудительно
    //      завершить процесс через TerminateProcess(pi.hProcess, 1)
    //
    // Напишите код здесь:
    DWORD waitResult = WaitForSingleObject(pi.hProcess, 30000);
    if (waitResult == WAIT_OBJECT_0) {
        DWORD exitCode = 0;
        if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
            DWORD err = GetLastError();
            std::wcout << L"Ошибка кода выхода: " << err << std::endl;
            return 1;
        }
        std::wcout << L"Процесс завершился. Код выхода: " << exitCode << std::endl;
    }
    else if (waitResult == WAIT_TIMEOUT) {
        std::wcout << L"Процесс не завершился за 30 секунд. Принудительное завершение." << std::endl;
        if (!TerminateProcess(pi.hProcess, 1)) {
            DWORD err = GetLastError();
            std::wcout << L"Ошибка TerminateProcess: " << err << std::endl;
            return 1;
        }
        WaitForSingleObject(pi.hProcess, 5000);
        std::wcout << L"Процесс принудительно завершен" << std::endl;
    }
    else if (waitResult == WAIT_FAILED) {
        DWORD err = GetLastError();
        std::wcout << L"Ошибка ожидания: " << err << std::endl;
        return 1;
    }

    // ========================================================
    // ОСВОБОЖДЕНИЕ РЕСУРСОВ
    // ========================================================
    // Вам нужно:
    //   Закрыть оба дескриптора через CloseHandle, иначе будут утечки ресурсов ядра ОС:
    //   - pi.hProcess
    //   - pi.hThread
    //
    // Напишите код здесь:
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::wcout << L"\nПрограмма завершена. Нажмите Enter для выхода..." << std::endl;
    std::wcin.get();
    return 0;
}