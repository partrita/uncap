/**
@file
@mainpage

Caps Lock 키를 Escape 키로, 또는 모든 키를 원하는 키로 매핑합니다.

The MIT License (MIT)
---------------------
Copyright (c) 2015-2021 Susam Pal

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <tlhelp32.h>


/** 프로그램 버전. */
#define VERSION "0.4.0-dev"

/** 프로그램 저자. */
#define AUTHOR "Susam Pal"

/** 저작권 고지. */
#define COPYRIGHT "Copyright (c) 2015-2021 " AUTHOR

/** 라이선스 사본 URL. */
#define LICENSE_URL "<https://susam.github.io/licenses/mit.html>"

/** 이슈 보고 URL. */
#define SUPPORT_URL "<https://github.com/susam/uncap/issues>"

/** 읽어들일 명령줄 인수의 최대 길이. */
#define MAX_ARG_LEN 16

/** 오류 메시지의 최대 허용 길이. */
#define MAX_ERR_LEN 256

/** Uncap에 의해 주입된 키보드 입력을 식별하는 매직 넘버. */
#define UNCAP_INFO (WM_APP + 3195) /* 35963 */

#ifndef LLKHF_LOWER_IL_INJECTED
/** MinGW winuser.h에 없는 정의에 대한 해결 방법. **/
#define LLKHF_LOWER_IL_INJECTED 0x00000002
#endif

/**
두 개의 null로 끝나는 바이트 문자열이 같은지 확인합니다.

@param a null로 끝나는 바이트 문자열에 대한 포인터. (타입: const char *)
@param b null로 끝나는 바이트 문자열에 대한 포인터. (타입: const char *)

@return 두 문자열이 같으면 1, 그렇지 않으면 0.
*/
#define streq(a, b) (strcmp(a, b) == 0)


/**
null로 끝나는 바이트 문자열을 문자 배열에 복사합니다.

@param a 문자열을 복사할 문자 배열에 대한 포인터. (타입: char *)
@param b 복사할 null로 끝나는 바이트 문자열에 대한 포인터. (타입: const char *)
@param c 복사할 최대 문자 수. (타입: size_t)

@return a
*/
#define strcp(a, b, c) (a[0] = '\0', strncat(a, b, c - 1))


/**
문자열의 모든 문자를 소문자로 변환합니다.

@param s null로 끝나는 바이트 문자열에 대한 포인터.

@return s
*/
char *strlower(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        s[i] = (char) tolower(s[i]);
    return s;
}


/**
지정된 경로에서 리프 디렉터리 또는 파일의 이름을 반환합니다.

백슬래시와 슬래시 모두 경로 구분자로 처리됩니다. 마지막 슬래시(제외)와
문자열 끝 사이의 하위 문자열 시작 부분에 대한 포인터가 기본 이름으로
반환됩니다. 따라서 지정된 경로가 슬래시로 끝나면 빈 문자열이 반환됩니다.

@param path 경로 문자열.

@return 지정된 경로의 리프 디렉터리 또는 파일 이름.
*/
const char *basename(const char *path)
{
    const char *base;
    if ((base = strrchr(path, '\\')) != NULL)
        return base + 1;
    else if ((base = strrchr(path, '/')) != NULL)
        return base + 1;
    else
        return path;
}


/**
함수가 성공 또는 실패를 나타내기 위해 반환하는 값입니다.

이러한 반환 코드는 함수의 작업 성공 또는 실패 및 다음 조치를
나타내기 위해 함수에서 반환될 수 있습니다.
*/
enum action {
    GOOD, /**< 성공적인 작업; 프로그램은 계속되어야 합니다. */
    EXIT, /**< 성공적인 작업; 프로그램은 정상적으로 종료되어야 합니다. */
    FAIL  /**< 실패한 작업; 프로그램은 오류와 함께 종료되어야 합니다. */
};


/**
이 프로그램의 전역 상태.
*/
struct state {
    char name[MAX_ARG_LEN];  /**< 프로그램 이름. */
    WORD keymap[256];        /**< 키 매핑. */
    HHOOK hook;              /**< 키보드 후크 프로시저에 대한 핸들. */
    int console;             /**< 콘솔 모드가 활성화되었는지 여부. */
    int debug;               /**< 상세 모드가 활성화되었는지 여부. */
    FILE *file;              /**< 상세 로그를 쓸 파일. */
    char error[MAX_ERR_LEN]; /**< 실패한 작업에 대한 오류 메시지. */
} my; /**< 이 프로그램의 전역 상태. */


/**
표준 오류 스트림에 오류 메시지를 출력합니다.

@param format printf용 형식 문자열.
@param ...    추가 인수.

@return EXIT_FAILURE; 이 함수를 호출한 쪽은 프로그램의 비정상적인
        종료를 나타내기 위해 이 코드를 반환할 수 있습니다.
*/
int error(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "%s: ", my.name);
    vfprintf(stderr, format, ap);
    return EXIT_FAILURE;
}


/** 로그 형식 문자열. */
#define LOG_FMT \
    "%-10s %3d %5lu %3lu " \
    "%3lu (%#04lx) %3lu (%#04lx) " \
    "[%s%s%s%s%s%s%s]\n"


/**
키 입력 세부 정보를 지정된 파일에 기록합니다.

@param file 쓸 파일. (타입: FILE *)
*/
#define logKeyTo(file) \
            fprintf(file, LOG_FMT, \
                    wParamStr, nCode, p->dwExtraInfo, p->flags, \
                    p->scanCode, p->scanCode, p->vkCode, p->vkCode, \
                    vkStr, upStr, extStr, altStr, lowStr, injStr, uncapStr)

/**
키 입력의 세부 정보를 기록합니다.

@param nCode  메시지 처리 방법을 결정하는 데 사용되는 코드.
@param wParam 키보드 메시지의 식별자.
@param lParam KBDLLHOOKSTRUCT 구조체에 대한 포인터.
*/
void logKey(int nCode, WPARAM wParam, LPARAM lParam)
{
    const KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;

    char wParamStr[16] = "UNKNOWN";
    char vkStr[16];
    char extStr[16];
    char lowStr[16];
    char injStr[16];
    char altStr[16];
    char upStr[16];
    char uncapStr[16];

    /* 키보드 메시지 식별자를 문자열 표기법으로 변환합니다. */
    switch (wParam) {
    case WM_KEYDOWN:
        strcpy(wParamStr, "KEYDOWN");
        break;

    case WM_KEYUP:
        strcpy(wParamStr, "KEYUP");
        break;

    case WM_SYSKEYDOWN:
        strcpy(wParamStr, "SYSKEYDOWN");
        break;

    case WM_SYSKEYUP:
        strcpy(wParamStr, "SYSKEYUP");
        break;
    }

    /* 가상 키 코드를 문자열 표기법으로 변환합니다. */
    if (p->vkCode == VK_RETURN)
        sprintf(vkStr, "RETURN");
    else if (p->vkCode == VK_CAPITAL)
        sprintf(vkStr, "CAPITAL");
    else if (p->vkCode == VK_ESCAPE)
        sprintf(vkStr, "ESCAPE");
    else if (p->vkCode == VK_LCONTROL)
        sprintf(vkStr, "LCONTROL");
    else if (p->vkCode == VK_RCONTROL)
        sprintf(vkStr, "RCONTROL");
    else if (p->vkCode == VK_LMENU)
        sprintf(vkStr, "LMENU");
    else if (p->vkCode == VK_RMENU)
        sprintf(vkStr, "RMENU");
    else if (p->vkCode == VK_LWIN)
        sprintf(vkStr, "LWIN");
    else if (p->vkCode == VK_RWIN)
        sprintf(vkStr, "RWIN");
    else if ((p->vkCode >= '0' && p->vkCode <= '9') ||
             (p->vkCode >= 'A' && p->vkCode <= 'Z'))
        sprintf(vkStr, "%c", p->vkCode);
    else if (p->vkCode >= VK_NUMPAD0 && p->vkCode <= VK_NUMPAD9)
        sprintf(vkStr, "NUMPAD%d", p->vkCode - VK_NUMPAD0);
    else if (p->vkCode >= VK_F1 && p->vkCode <= VK_F24)
        sprintf(vkStr, "F%d", p->vkCode - VK_F1 + 1);
    else
        sprintf(vkStr, "%#x", p->vkCode);

    /* 각 플래그를 문자열 표기법으로 변환합니다. */
    strcpy(extStr, p->flags & LLKHF_EXTENDED ? " EXT" : "");
    strcpy(lowStr, p->flags & LLKHF_LOWER_IL_INJECTED ? " LOW": "");
    strcpy(injStr, p->flags & LLKHF_INJECTED ? " INJ" : "");
    strcpy(altStr, p->flags & LLKHF_ALTDOWN ? " ALT" : "");
    strcpy(upStr, p->flags & LLKHF_UP ? " UP" : " DN");
    strcpy(uncapStr, p->dwExtraInfo == UNCAP_INFO ? " UNCAP" : "");

    /* 상세 모드가 활성화된 경우 표준 오류 스트림에 키를 기록합니다. */
    if (my.debug)
        logKeyTo(stderr);

    /* 파일 로깅이 활성화된 경우 사용자 지정 파일에 키를 기록합니다. */
    if (my.file != NULL)
        logKeyTo(my.file);

    fflush(NULL);
}


/**
하나의 키를 다른 키에 매핑합니다.

@param nCode  메시지 처리 방법을 결정하는 데 사용되는 코드.
@param wParam 키보드 메시지의 식별자.
@param lParam KBDLLHOOKSTRUCT 구조체에 대한 포인터.

@return 키보드 메시지가 처리되면 1, 즉 키가 다른 키에 매핑되면 1,
        그렇지 않으면 CallNextHookEx를 호출하고 반환된 값을 반환합니다.
*/
LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;
    WORD keyCode;
    WORD mapCode;

    if (nCode < 0)
        return CallNextHookEx(my.hook, nCode, wParam, lParam);

    keyCode = (WORD) p->vkCode;
    mapCode = (keyCode < 256) ? my.keymap[keyCode] : keyCode;

    if (my.debug || my.file) {
        logKey(nCode, wParam, lParam);
    }

    if (mapCode == 0) {
        /* 눌린 키가 매핑되지 않은 경우 키 누름을 비활성화합니다. */
        return 1;
    } else if (keyCode != mapCode && p->dwExtraInfo != UNCAP_INFO) {
        /* 키가 매핑된 경우 매핑된 키로 변환합니다. */
        INPUT inputs[1];
        PKEYBDINPUT ki = &inputs[0].ki;

        inputs[0].type = INPUT_KEYBOARD;
        ki->time = ki->wScan = 0;

        ki->wVk = mapCode;
        ki->dwFlags = (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
                      ? KEYEVENTF_KEYUP : 0;
        ki->dwExtraInfo = UNCAP_INFO;

        SendInput(1, inputs, sizeof *inputs);
        return 1;
    }

    return CallNextHookEx(my.hook, nCode, wParam, lParam);
}


/**
이 프로그램의 다른 실행 중인 인스턴스를 종료합니다.

@return 이 함수가 다른 실행 중인 인스턴스를 종료할 수 있었는지 여부에
        따라 취할 다음 조치입니다. 이 함수가 적어도 하나의 다른 실행 중인
        인스턴스를 종료하지 못하면 FAIL이 반환됩니다. 이 함수가 다른 모든
        실행 중인 인스턴스를 성공적으로 종료했거나 다른 실행 중인
        인스턴스가 없는 경우 EXIT가 반환됩니다.
*/
enum action kill(void)
{
    char myExeFile[MAX_ARG_LEN + 4];
    char *dot;
    PROCESSENTRY32 entry;
    HANDLE snapshotHandle;
    int failure = 0;

    /* 시스템에서 실행 중인 모든 프로세스의 스냅샷을 찍습니다. */
    snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshotHandle == NULL) {
        sprintf(my.error, "Cannot take snapshot of processes; "
                          "error: %lu.", GetLastError());
        return FAIL;
    }

    /* 스냅샷의 각 프로세스를 반복하기 시작합니다. */
    entry.dwSize = sizeof entry;
    if (!Process32First(snapshotHandle, &entry)) {
        sprintf(my.error, "Cannot retrieve process from snapshot; "
                          "error: %lu.", GetLastError());
        return FAIL;
    }

    /* 종료할 프로세스의 이름. */
    strcpy(myExeFile, my.name);
    strlower(myExeFile);
    dot = strrchr(myExeFile, '.');
    if (dot == NULL || !streq(dot, ".exe"))
        strcat(myExeFile, ".exe");

    /* 스냅샷의 각 프로세스를 반복하여 종료할 프로세스를 찾아 종료합니다. */
    do {
        HANDLE processHandle;

        /* 현재 프로세스는 무시합니다. */
        if (entry.th32ProcessID == GetCurrentProcessId())
            continue;

        /* 다른 프로그램은 무시합니다. */
        if (!streq(strlower(entry.szExeFile), myExeFile))
            continue;

        /* 이 프로그램의 다른 인스턴스를 엽니다. */
        processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0,
                                    entry.th32ProcessID);
        if (processHandle == NULL) {
            error("Cannot open process \"%s\" (PID %lu); error %lu.",
                  entry.szExeFile, entry.th32ProcessID, GetLastError());
            failure = 1;
            continue;
        }

        /* 이 프로그램의 다른 인스턴스를 종료합니다. */
        if (TerminateProcess(processHandle, 0)) {
            printf("Terminated %s (PID %lu).\n",
                   entry.szExeFile, entry.th32ProcessID, GetLastError());
        } else {
            error("Cannot terminate %s (PID %lu); error %lu.\n",
                  entry.szExeFile, entry.th32ProcessID, GetLastError());
            failure = 1;
        }
        CloseHandle(processHandle);

    } while (Process32Next(snapshotHandle, &entry));

    CloseHandle(snapshotHandle);

    if (failure) {
        sprintf(my.error, "Failed to terminate all running instances "
                          "of %s.", myExeFile);
        return FAIL;
    }

    return EXIT;
}


/**
이 프로그램의 사용법 및 도움말 세부 정보를 표시합니다.
*/
void showHelp(void)
{
    const char *usage =
"Usage: %s [-k] [-c] [-d] [-f FILE] [-h] [-v] [[MAP_KEY:TO_KEY]...]\n\n";

    const char *summary =
"Map Caps Lock key to Escape key, or any key to any key.\n\n";

    const char *description1 =
"Caps Lock key is mapped to Escape key by default. This may be\n"
"overridden by specifying a new mapping for Caps Lock key. Any key\n"
"may be mapped to any key with one or more MAP_KEY:TO_KEY arguments.\n"
"Each argument is a colon separated pair of virtual-key codes from\n"
"<https://msdn.microsoft.com/library/windows/desktop/dd375731.aspx>.\n\n"

"The virtual-key code may be specified either as hexadecimal integer\n"
"as mentioned in the above URL or its equivalent decimal notation.\n\n";

    const char *description2 =
"If MAP_KEY equals TO_KEY, then no mapping occurs for it. If TO_KEY\n"
"equals 0, then the key mapped to 0 is disabled.\n\n";

    const char *details =
"Options:\n"
"  -k, --kill       Kill other instances of uncap.\n"
"  -c, --console    Run silently in console.\n"
"  -d, --debug      Run verbosely in console.\n"
"  -f, --file FILE  Write verbose logs to file.\n"
"  -h, --help       Show this help and exit.\n"
"  -v, --version    Show version and exit.\n\n"

"Arguments:\n"
"  MAP_KEY          Virtual-key code of key to map.\n"
"  TO_KEY           Virtual-key code of key to map to.\n\n"

"Report bugs to " SUPPORT_URL ".\n";

    printf(usage, my.name);
    printf(summary);
    printf(description1);
    printf(description2);
    printf(details);
}


/**
이 프로그램의 버전 및 저작권 정보를 표시합니다.
*/
void showVersion(void)
{
    char name[MAX_ARG_LEN];

    const char *s =
    "%s " VERSION "\n"
    COPYRIGHT "\n\n"

    "This is free and open source software. You can use, copy, modify,\n"
    "merge, publish, distribute, sublicense, and/or sell copies of it,\n"
    "under the terms of the MIT License. You can obtain a copy of the\n"
    "MIT License at " LICENSE_URL ".\n\n"

    "This software is provided \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n"
    "express or implied. See the MIT License for details.\n";

    strcpy(name, my.name);
    name[0] = (char) toupper(name[0]);
    printf(s, name);
}


/**
사랑을 출력합니다.
*/
void qtpi(void)
{
    const int show[] = {'.', '@', ' ', '\n'};
    const int love[] = {
        252, 7, 252, 7, 72, 6, 29, 6, 36, 6, 29,
        6,  72, 7, 60, 6,  17, 22, 17, 6, 12, 6,

              17, 22,           17,  6,
           60, 7, 52, 6,     13,  46,  13,
         6, 13, 46, 13, 6, 52, 7, 48, 6, 13,
        62, 5, 62, 13,  6, 48,  7, 48, 6, 13,
        18, 1, 70, 13,  6, 48,  7, 48, 6, 13,
        126, 13, 6, 48, 7, 52, 6, 13,  38, 2,
         38, 13, 6,  52, 7,  60, 6,  17, 94,
           17, 6,  60, 7,  72, 6,  17, 22,
              3, 18,  17, 6, 72, 7, 84,
                 6, 17,  46,  17, 6,
                    84, 7, 96, 6,
                       17, 22,
                         17,

        6, 96, 7, 108, 6,  13, 6, 13, 6, 108, 7,
        120,  6, 5,  6, 120, 7,  252, 7, 252, 7,
    };
    const char *say[] = {"", "Cutie Pai,", "I love you!", "-- Susam"};
    size_t i;
    int j;
    for (i = 0; i < sizeof love / sizeof *love; i++) {
        if (love[i] < 4)
            printf(say[love[i]]);
        else
            for (j = 0; j < love[i] / 4; j++)
                putchar(show[love[i] % 4]);
    }
}


/**
명령줄 인수를 구문 분석합니다.

@param argc 인수 개수
@param argv 인수 벡터

@return 명령줄 인수 구문 분석 성공 여부에 따라 취할 다음 조치.
        명령줄 인수가 성공적으로 구문 분석되고 프로그램이 계속되어야 하는 경우
        GOOD이 반환됩니다. 명령줄 인수가 성공적으로 구문 분석 및 처리되고
        프로그램이 종료되어야 하는 경우 EXIT가 반환됩니다. 명령줄 인수에서
        오류가 발견되면 FAIL이 반환됩니다.
*/
enum action parseArguments(int argc, const char **argv)
{
    int i;

    /* 기본값을 설정합니다. */
    strcp(my.name, basename(argv[0]), sizeof my.name);

    for (i = 0; i < (int) (sizeof my.keymap / sizeof *my.keymap); i++)
        my.keymap[i] = (WORD) i;

    my.keymap[VK_CAPITAL] = VK_ESCAPE;

    my.console = 0;
    my.debug = 0;
    my.file = NULL;

    /* 명령줄 옵션을 구문 분석합니다. */
    i = 1;
    while (i < argc) {
        const char *arg = argv[i];
        if (streq(arg, "-h") || streq(arg, "--help")) {
            showHelp();
            return EXIT;
        } else if (streq(arg, "-v") || streq(arg, "--version")) {
            showVersion();
            return EXIT;
        } else if (streq(arg, "--qtpi")) {
            qtpi();
            return EXIT;
        } else if (streq(arg, "-c") || streq(arg, "--console")) {
            my.console = 1;
            ++i;
        } else if (streq(arg, "-d") || streq(arg, "--debug")) {
            my.debug = 1;
            ++i;
        } else if (streq(arg, "-f") || streq(arg, "--file")) {
            if (i == argc - 1) {
                sprintf(my.error, "Option '%.*s' must be followed by "
                                  "file path", MAX_ARG_LEN, arg);
                return FAIL;
            }
            arg = argv[++i];
            if ((my.file = fopen(arg, "a")) == NULL) {
                sprintf(my.error, "Cannot open %.*s.", MAX_ARG_LEN, arg);
                return FAIL;
            }
            ++i;
        } else if (streq(arg, "-k") || streq(arg, "--kill")) {
            return kill();
        } else if (streq(arg, "--")) {
            ++i;
            break;
        } else if (arg[0] == '-' && arg[1] != 0) {
            sprintf(my.error, "Unknown option '%.*s'.", MAX_ARG_LEN, arg);
            return FAIL;
        } else {
            break;
        }
    }

    /* 명령줄 인수를 구문 분석합니다. */
    while (i < argc) {
        const char *arg = argv[i];
        char *colon;
        unsigned long int mapKey;
        unsigned long int toKey;

        if ((colon = strchr(arg, ':')) == NULL) {
            sprintf(my.error, "Colon is missing from argument '%.*s'.",
                    MAX_ARG_LEN, arg);
            return FAIL;
        }

        mapKey = strtoul(arg, NULL, 0);
        toKey = strtoul(colon + 1, NULL, 0);

        if (mapKey < 1 || mapKey > 254 || toKey > 254) {
            sprintf(my.error, "Invalid key code in argument '%.*s'.",
                    MAX_ARG_LEN, arg);
            return FAIL;
        }

        my.keymap[mapKey] = (WORD) toKey;
        ++i;
    }

    /* 명령줄 인수가 성공적으로 구문 분석되었습니다. */
    return GOOD;
}


/**
프로그램을 시작합니다.

@param argc 인수 개수.
@param argv 인수 벡터.

@return 프로그램이 정상적으로 종료되면 EXIT_SUCCESS,
        오류가 발생하면 EXIT_FAILURE.
*/
int main(int argc, char **argv)
{
    enum action a;
    MSG msg;

    /* 명령줄 인수를 구문 분석합니다. */
    if ((a = parseArguments(argc, (const char **) argv)) == FAIL)
        return error(my.error);
    else if (a == EXIT)
        return EXIT_SUCCESS;

    /* 콘솔의 가시성을 설정합니다. */
    if (!my.console && !my.debug) {
        HWND h = GetConsoleWindow();
        if (h != NULL)
            ShowWindow(h, SW_HIDE);
        else
            error("Cannot find console window; error %lu.", GetLastError());
    }

    /* 하위 수준 키보드 입력 이벤트를 모니터링하기 위해 후크를 설치합니다. */
    my.hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, NULL, 0);
    if (my.hook == NULL)
        return error("Cannot install hook; error %lu.", GetLastError());

    /* 메시지 전달을 시작합니다. */
    if (GetMessage(&msg, NULL, 0, 0) == -1)
        return error("Cannot retrieve message; error %lu.", GetLastError());

    return EXIT_SUCCESS;
}
