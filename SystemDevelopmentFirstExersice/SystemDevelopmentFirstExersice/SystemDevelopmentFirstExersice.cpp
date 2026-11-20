#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <io.h>
#include <fcntl.h>

int main() {
    // НАСТРОЙКА КОДИРОВКИ
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wcout << L"Системная информация" << std::endl;
    std::wcout << std::endl;

    // ========================================================
    // ПОЛУЧЕНИЕ ИМЕНИ КОМПЬЮТЕРА
    // ========================================================
    // Вам нужно:
    //   1) Объявить буфер для хранения имени.
    //      Тип: массив TCHAR, размер: MAX_COMPUTERNAME_LENGTH + 1
    //   2) Объявить переменную DWORD с именем size,
    //      инициализировать её размером буфера в символах.
    //   3) Вызвать функцию GetComputerName(буфер, &size)
    //   4) Если функция вернула TRUE - вывести имя через std::wcout
    //      Если FALSE - вывести сообщение об ошибке и код GetLastError()
    // Напишите код здесь:

    TCHAR eshkere[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD size = _countof(eshkere);
    
    if (GetComputerName(eshkere, &size)) {
        std::wcout << L"Your PC Name: " << eshkere;
    }
    else {
        std::wcout << L"Error: " << GetLastError();
    }
    
    
    std::wcout << std::endl;

    // ========================================================
    // ПОЛУЧЕНИЕ ИНФОРМАЦИИ ОБ ОПЕРАТИВНОЙ ПАМЯТИ
    // ========================================================
    // Вам нужно:
    //   1) Объявить переменную типа MEMORYSTATUSEX с именем memStatus
    //   2) Заполнить поле memStatus.dwLength размером структуры
    //   3) Вызвать функцию GlobalMemoryStatusEx(&memStatus)
    //   4) Если успешно - вычислить и вывести:
    //      - Общий объём ОЗУ в ГБ (поле ullTotalPhys)
    //      - Доступный объём ОЗУ в ГБ (поле ullAvailPhys)
    //      - Процент загрузки (поле dwMemoryLoad)
    //   5) Если ошибка - вывести код GetLastError()
    // Напишите код здесь:

    MEMORYSTATUSEX memStatus = {};
    memStatus.dwLength = sizeof(memStatus);
    double allRam, freeRam, loadCoef;
    if (GlobalMemoryStatusEx(&memStatus)) {
        allRam = memStatus.ullTotalPhys / (1024 * 1024);
        freeRam = memStatus.ullAvailPhys / (1024 * 1024);
        loadCoef = memStatus.dwMemoryLoad;
        std::wcout << L"All RAM: " << allRam << L" MB" << std::endl;
        std::wcout << L"Free RAM: " << freeRam << L" MB" << std::endl;
        std::wcout << L"Load coefficient: " << loadCoef << L"%";

    }
    else {
        std::wcout << "Error: " << GetLastError();
    }


    std::wcout << std::endl;

    // ========================================================
    // ФОРМИРОВАНИЕ СТРОКИ ДЛЯ ДИАЛОГОВОГО ОКНА
    // ========================================================
    // Вам нужно:
    //   1) Создать переменную std::wstring с именем message
    //   2) Собрать в неё всю полученную информацию в формате:
    //      Системная информация:
    //      Компьютер: <имя>
    //      Всего ОЗУ: <число> ГБ
    //      Свободно ОЗУ: <число> ГБ
    //      Загрузка: <число>%
    // Напишите код здесь:

    std::wstringstream ss;
    ss << L"System Information: \n"
        << L"Computer: " << eshkere << L"\n"
        << L"All RAM: " << allRam << L" MB\n"
        << L"Free RAM: " << freeRam << L" MB\n"
        << L"Download: " << loadCoef << L"%" << std::endl;
    std::wstring message = ss.str();

    // ========================================================
    // ВЫВОД ДИАЛОГОВОГО ОКНА
    // ========================================================
    // Вам нужно:
    //   Вызвать функцию MessageBox, чтобы показать окно с:
    //   - Родительское окно: NULL
    //   - Текст: ваша строка message (подсказка: .c_str())
    //   - Заголовок: L"Лабораторная работа №1"
    //   - Флаги: кнопка OK + значок информации
    // Напишите код здесь:
    MessageBox(NULL, message.c_str(), L"Lab №1", MB_OK | MB_ICONINFORMATION);


    std::wcout << L"\nГотово! Нажмите Enter для выхода..." << std::endl;
    std::wcin.get();
    return 0;
}
