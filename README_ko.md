Uncap
=====

Windows 시스템에서 Caps Lock 키를 Escape 키로, 또는 모든 키를 원하는 키로 매핑합니다.

[![다운로드][Download SVG]][Download URL]
[![MIT 라이선스][License Badge]][L]
[![트위터][Twitter SVG]][Twitter URL]

이 README 파일은 Uncap이 Windows에서 어떻게 작동하는지 문서화하는 것 외에도, Linux 및 Mac에서 키를 리매핑하는 방법에 대한 자세한 문서도 포함하고 있습니다.

[Download SVG]: https://img.shields.io/badge/download-uncap.exe-brightgreen.svg
[Download URL]: https://github.com/susam/uncap/releases/download/0.3.0/uncap.exe
[License Badge]: https://img.shields.io/badge/license-MIT-blue.svg
[Twitter SVG]: https://img.shields.io/badge/twitter-%40susam-%231da1f2
[Twitter URL]: https://twitter.com/susam


목차
--------

* [기능](#기능)
* [시작하기](#시작하기)
* [사용자 지정 매핑](#사용자-지정-매핑)
* [사용법](#사용법)
* [Windows 10의 Microsoft Edge 관련 알려진 문제](#windows-10의-microsoft-edge-관련-알려진-문제)
* [대안](#대안)
  * [Windows: 스캔 코드 맵 레지스트리 값](#windows-스캔-코드-맵-레지스트리-값)
  * [Windows: AutoHotkey](#windows-autohotkey)
  * [Linux: setxkbmap](#linux-setxkbmap)
  * [Linux: xmodmap](#linux-xmodmap)
  * [Linux: loadkeys](#linux-loadkeys)
  * [Mac: 시스템 환경설정](#mac-시스템-환경설정)
  * [Mac: Seil](#mac-seil)
* [자료](#자료)
* [라이선스](#라이선스)
* [지원](#지원)


기능
--------

다음 목록은 Uncap의 몇 가지 기능을 간략하게 설명합니다.

 1. 거의 노력 없이 새로운 Windows 시스템에서 Caps Lock 키를 Escape 키로 매핑합니다. 그냥 다운로드하고 더블 클릭하세요. 이것이 Uncap이 작성된 주된 이유입니다. 이 도구는 편의를 위해 Caps Lock 키를 Escape 키로 매핑하려는 vi/Vim 편집기 사용자를 위해 작성되었습니다.
 2. 명령줄 인수를 사용하여 모든 키를 모든 키로, 여러 키를 다른 키로 매핑하거나 키를 비활성화합니다. 추가 파일이나 스크립트가 필요하지 않습니다.
 3. Windows 시스템을 재부팅하지 않고도 키 매핑을 활성화, 비활성화 또는 수정할 수 있습니다.
 4. Uncap을 중지하여 키 매핑을 쉽게 비활성화할 수 있습니다.
 5. 단일 파일 실행 파일입니다. 매우 가볍습니다. 실행 파일의 크기는 약 100KB에 불과합니다. 실행 중에는 1MB 미만의 메모리와 무시할 수 있는 CPU를 차지합니다.

Windows에서 한 키를 다른 키로 매핑하는 다른 여러 방법과 도구가 있습니다. 그러나 위에 열거된 다섯 가지 기능을 모두 갖춘 것은 없는 것 같습니다. 예를 들어, [스캔 코드 맵](#windows-스캔-코드-맵-레지스트리-값) 레지스트리 값을 편집하려면 변경할 때마다 Windows 시스템을 재부팅해야 합니다. 반면에 [AutoHotkey](#windows-autohotkey)는 추가 스크립트 파일을 작성해야 합니다. 따라서 Uncap은 위에 설명된 다섯 가지 기능을 모두 지원하도록 작성되었습니다.


시작하기
------------

Uncap은 단일 파일 실행 파일입니다: [`uncap.exe`][Download URL].

가장 간단한 실행 방법은 다운로드하여 더블 클릭하는 것입니다. 출력 창이 표시되지 않습니다. 백그라운드에서 실행됩니다. 실행되면 Caps Lock 키를 누를 때마다 Uncap에 의해 Escape 키로 변환됩니다.


사용자 지정 매핑
--------------

기본 매핑인 Caps Lock 키를 Escape 키로 매핑하는 것을 사용자 지정 매핑으로 재정의할 수 있지만, 그렇게 하려면 명령 프롬프트나 Windows 실행 대화 상자에서 인수를 사용하여 Uncap을 실행해야 합니다. 다음 단계는 uncap.exe를 사용하여 Caps Lock 키를 왼쪽 Control 키로 매핑하는 방법을 보여줍니다.

 1. `uncap.exe`를 Windows `PATH` 변수에 지정된 디렉터리로 복사합니다. `C:\Windows`는 이 파일을 복사하기에 매우 편리한 위치입니다.

 2. `Windows 로고 키` + `R`을 눌러 Windows 실행 대화 상자를 시작합니다.

 3. 다음 명령을 입력합니다.

        uncap 0x14:0xa2

 4. 확인 버튼을 누릅니다. 그러면 Uncap이 시작됩니다. 출력 창이 표시되지 않습니다. Uncap은 백그라운드에서 실행됩니다. 이제 Caps Lock 키를 누를 때마다 Uncap이 왼쪽 Control 키로 변환합니다.

 5. Uncap을 종료하려면 다음 명령을 실행하십시오.

        uncap -k

위의 단계에서 Caps Lock 키는 `0x14:0xa2`를 Uncap에 인수로 지정하여 왼쪽 Control 키에 매핑됩니다. Caps Lock의 가상 키 코드는 `0x14`이고 Control 키의 가상 키 코드는 `0xa2`입니다. 따라서 인수 `0x14:0xa2`는 Caps Lock 키를 Control 키에 매핑합니다.

모든 키에 대한 가상 키 코드 목록은 <https://msdn.microsoft.com/library/windows/desktop/dd375731.aspx>에서 찾을 수 있습니다.


사용법
-----

Uncap이 인수 없이 실행되면 Caps Lock 키를 Escape 키에 매핑합니다. 인수를 사용하여 Caps Lock 키를 모든 키에, 모든 키를 모든 키에 매핑하거나 모든 키를 비활성화할 수 있습니다.

각 인수는 <https://msdn.microsoft.com/library/windows/desktop/dd375731.aspx>의 가상 키 코드를 콜론으로 구분한 쌍입니다.

가상 키 코드는 위 URL에 언급된 16진수 정수 또는 이에 상응하는 10진수 표기법으로 지정할 수 있습니다. 이러한 세부 정보는 아래 예제 목록에 설명되어 있습니다.

 1. Caps Lock 키를 Escape 키로 매핑합니다.

        uncap

 2. Caps Lock 키를 Escape 키와 바꿉니다.

        uncap 0x1b:0x14

    Caps Lock 키는 기본적으로 이미 Escape 키에 매핑되어 있습니다. 위의 명령은 Escape 키를 Caps Lock 키에 매핑하여 두 키를 효과적으로 바꿉니다.

 3. 16진수 정수는 대소문자를 구분하지 않으므로 다음 명령은 이전 명령과 동일합니다.

        uncap 0x1B:0x14

 4. 가상 키 코드는 10진수 표기법으로 지정할 수 있으므로 다음 명령은 이전 명령과 동일합니다.

        uncap 27:20

 5. Caps Lock 키를 왼쪽 Control 키로 매핑합니다.

        uncap 0x14:0xa2

 6. Caps Lock 키와 왼쪽 Control 키를 바꿉니다.

        uncap 0x14:0xa2 0xa2:0x14

    이 예는 여러 키를 다른 키에 매핑하기 위해 둘 이상의 인수를 지정할 수 있음을 보여줍니다.

 7. 다음은 여러 키를 다른 키에 매핑하는 또 다른 예입니다. 다음 명령은 Caps Lock 키를 왼쪽 Control 키에, 왼쪽 Control 키를 Escape 키에, Escape 키를 Caps Lock 키에 매핑합니다.

        uncap 0x14:0xa2 0xa2:0x1b 0x1b:0x14

 8. 키가 자신에게 매핑되면 매핑이 발생하지 않습니다. 이것은 Caps Lock 키를 Escape 키에 매핑하는 기본 동작을 재정의하고 다른 키를 일부 키에 매핑하는 동안 매핑되지 않은 상태로 두는 데 사용할 수 있습니다. 다음은 Caps Lock 키를 매핑 해제하고 F1 키를 Escape 키에 매핑하는 예입니다.

        uncap 0x14:0x14 0x70:0x1b

    키가 다른 키에 매핑되고 해당 키를 누르면 Uncap은 새 키 입력을 합성하여 시스템에 주입하여 매핑된 키로 변환합니다. 그러나 키가 자신에게 매핑되고 해당 키를 누르면 무시되므로 새 키 입력의 변환이나 합성이 발생하지 않습니다.

 9. 키가 0에 매핑되면 키가 비활성화됩니다. 다음은 Caps Lock 키를 완전히 비활성화하는 예입니다.

        uncap 0x14:0

10. Uncap은 기본적으로 콘솔을 표시하지 않고 실행됩니다. 실행 중일 수 있는 다른 Uncap 인스턴스를 종료하려면 `-k` 또는 `--kill` 옵션을 사용하십시오.

        uncap -k

    위의 명령은 콘솔 유무에 관계없이 실행 중인 다른 모든 Uncap 인스턴스를 종료합니다.

11. 콘솔에서 Uncap을 실행하려면 `-c` 또는 `--console` 옵션을 사용하십시오.

        uncap -c

    이와 같이 콘솔에서 Uncap이 실행 중일 때 `Ctrl` + `C`를 눌러 종료할 수 있습니다.

12. 디버그 모드에서 Uncap을 실행하려면 `-d` 또는 `--debug` 옵션을 사용하십시오.

        uncap -d

    디버그 출력에는 키를 누르거나 놓을 때마다 세부 정보가 포함된 한 줄의 출력이 포함됩니다.

13. 키 입력 세부 정보를 파일에 기록하려면 `-f` 또는 `--file` 옵션을 사용하십시오.

        uncap -f C:\keys.txt

    이 옵션만 사용하면 Uncap은 백그라운드에서 실행되고 키 입력 세부 정보를 지정된 파일에 기록합니다. 이 옵션은 `-c` 또는 `-d`와 결합하여 각각 콘솔에서 Uncap을 실행하거나 콘솔에 디버그 출력을 인쇄할 수 있습니다.

14. 지원되는 옵션과 함께 전체 사용 세부 정보를 참조하십시오.

        uncap -h


Windows 10의 Microsoft Edge 관련 알려진 문제
---------------------------------------------

Windows 10의 Microsoft Edge 웹 브라우저를 사용하여 Uncap을 다운로드하면 키 매핑에 실패하는 경우가 있습니다.

다음 조치는 문제를 해결하는 데 *성공하지 못한* 것으로 밝혀졌습니다.

  - `uncap.exe`를 마우스 오른쪽 버튼으로 클릭한 다음 *속성*을 선택하고 *차단 해제*를 선택한 다음 *확인*을 클릭해도 문제가 해결되지 않습니다.

  - `uncap.exe`를 마우스 오른쪽 버튼으로 클릭한 다음 *속성*을 선택하고 *보안* 탭으로 이동한 다음 *편집*을 클릭하고 다음 두 계정을 제거해도 문제가 해결되지 않습니다.

        알 수 없는 계정(S-1-15-2-3624051433-2125758914-1423191267-1740899205-1073925389-3782572162-737981194)
        알 수 없는 계정(S-1-15-3-3624051433-2125758914-1423191267-1740899205-1073925389-3782572162-737981194)

    이러한 계정은 Microsoft Edge로 다운로드한 파일에 자동으로 추가됩니다.

다음 해결 방법은 문제를 성공적으로 해결하는 것으로 밝혀졌습니다.

  - `uncap.exe`를 동일한 디렉터리의 새 파일에 복사합니다. 새 파일을 실행하면 키가 성공적으로 다시 매핑됩니다. 마지막으로 다운로드한 `uncap.exe`를 삭제한 다음 새 파일의 이름을 `uncap.exe`로 바꾸고 정상적으로 사용합니다.

  - `uncap.exe`를 다른 디렉터리로 복사합니다. 새로 복사한 파일을 실행하면 키가 성공적으로 다시 매핑됩니다.

이 문제는 Windows 10 Pro의 Microsoft Edge Microsoft Edge 44.17763.1.0에서 관찰되었습니다.


대안
------------

Windows, Linux 및 Mac에서 한 키를 다른 키로 매핑하는 다른 도구와 방법이 있습니다. 이 섹션에서는 몇 가지 이러한 도구와 방법을 설명합니다.


### Windows: 스캔 코드 맵 레지스트리 값 ###

추가 소프트웨어를 사용하지 않고 Windows에서 한 키를 다른 키로 매핑하는 한 가지 방법은 레지스트리를 편집하여 "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" 레지스트리 키에 "Scancode Map"이라는 값을 추가하는 것입니다.

예를 들어, Caps Lock 키를 Escape 키에 매핑하려면 등록 항목 파일, 즉 확장자가 reg인 파일(예: uncap.reg)을 다음 내용으로 만듭니다.

    REGEDIT4
    [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout]
    "Scancode Map"=hex:00,00,00,00,00,00,00,00,02,00,00,00,01,00,3a,00,00,00,00,00

그런 다음 파일을 저장합니다. 그런 다음 이 파일을 더블 클릭하여 Windows 레지스트리에 추가합니다. 그런 다음 Windows 시스템을 재부팅합니다. 재부팅 후 Caps Lock 키를 누를 때마다 Escape 키처럼 작동합니다.

명령 프롬프트에서 다음 명령을 실행하여 동일한 레지스트리 값을 추가할 수 있습니다.

    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" /v "Scancode Map" /t REG_BINARY /d 00000000000000000200000001003a0000000000

레지스트리 값의 16진수 코드는 스캔 코드 맵입니다. 스캔 코드 맵의 형식은 <https://msdn.microsoft.com/library/windows/hardware/jj128267.aspx#scan_code_mapper_for_keyboards>에 자세히 설명되어 있습니다.

다음 명령을 사용하여 이 레지스트리 값을 볼 수 있습니다.

    reg query "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" /v "Scancode Map"

다음 명령을 사용하여 이 레지스트리 값을 제거할 수 있습니다.

    reg delete "HKLM\SYSTEM\CurrentControlSet\Control\Keyboard Layout" /v "Scancode Map"

이 방법은 추가 소프트웨어가 필요 없다는 장점이 있지만, 레지스트리를 변경할 때마다 시스템을 재부팅해야 한다는 단점이 있습니다.

이 레지스트리 값을 관리하기 위한 그래픽 사용자 인터페이스가 있는 [SharpKeys][]라는 도구가 있습니다. 그러나 SharpKeys로 변경할 때마다 재부팅이 필요합니다.

[SharpKeys]: https://sharpkeys.codeplex.com/


### Windows: AutoHotkey ###

Windows용으로 매우 정교한 스크립팅 언어인 [AutoHotkey][]가 있으며, 이를 사용하여 한 키를 다른 키로 매핑하는 등 여러 가지 작업을 수행할 수 있습니다.

AutoHotkey를 사용하여 한 키를 다른 키로 매핑하는 것은 매우 간단합니다. 예를 들어, AutoHotkey를 설치한 후 Caps Lock 키를 Escape 키에 매핑하려면 AutoHotkey 스크립트, 즉 확장자가 ahk인 파일(예: uncap.ahk)을 다음 내용으로 만듭니다.

    CapsLock::Esc

그런 다음 파일을 저장합니다. 그런 다음 이 파일을 더블 클릭하여 이 스크립트로 AutoHotkey를 실행합니다. 이렇게 하면 Caps Lock 키를 누를 때마다 Escape 키로 변환됩니다. 알림 영역(시스템 트레이)에 AutoHotkey 아이콘이 나타납니다. 중지하려면 아이콘을 마우스 오른쪽 버튼으로 클릭하고 '종료'를 선택합니다.

이 방법은 시스템을 재부팅할 필요가 없다는 장점이 있습니다. 그러나 AutoHotkey는 단축키(바로 가기 키), 핫스트링(자동 교체), 매크로(스크립트된 작업) 등을 정의하는 등 여러 가지 다른 작업을 수행하는 데 사용할 수 있는 스크립팅 언어입니다. 한 키를 다른 키로 매핑하려는 경우에만 이것은 마치 망치로 파리를 잡는 것과 같이 느껴질 수 있습니다.

[AutoHotkey]: http://www.autohotkey.com/


### Linux: setxkbmap ###

리눅스에서는 `setxkbmap` 명령어를 사용하여 X 윈도우 시스템에서 키를 다른 키로 매핑할 수 있습니다. 예를 들어, 다음 명령어는 Caps Lock 키를 Escape 키로 매핑합니다.

    setxkbmap -option caps:escape

각 옵션에 대한 간단한 설명과 함께 옵션 목록은 현재 XKB 규칙과 동일한 이름을 가진 .lst 파일의 `/usr/share/X11/xkb/rules`에서 찾을 수 있습니다. 현재 XKB 규칙과 현재 매핑 옵션은 이 명령어의 출력에서 찾을 수 있습니다.

    setxkbmap -query

예를 들어, 'evdev'가 현재 규칙으로 표시되면 사용 가능한 옵션 목록과 각 옵션에 대한 간단한 설명은 `/usr/share/X11/xkb/rules/evdev.lst`를 참조하십시오.

위 명령어의 출력에서 'rules:'로 시작하는 줄은 현재 XKB 규칙을 보여줍니다. 키 매핑 옵션은 위 명령어의 출력에서 'option:'으로 시작하는 줄에 표시됩니다. 이러한 줄이 없으면 키 매핑 옵션이 설정되지 않은 것입니다.

다음 명령어는 모든 키 매핑 옵션을 제거합니다.

    setxkbmap -option


### Linux: xmodmap ###

리눅스에는 X 윈도우 시스템에서 한 키를 다른 키로 매핑하는 데 사용할 수 있는 `xmodmap`이라는 또 다른 명령어가 있습니다. 그러나 이 명령어는 이제 구식으로 간주됩니다. 특히 키 매핑을 제거할 때 사용하기가 약간 번거롭습니다. 이전 섹션에서 설명한 `setxkbmap` 명령어는 X 윈도우 시스템에서 키를 매핑하는 데 선호되는 방법입니다. 그러나 이 방법은 점차 사라지고 있지만 여전히 사용되고 있기 때문에 이 섹션에서는 `xmodmap`을 사용하여 한 키를 다른 키로 매핑하는 방법을 설명합니다. 예로서, 이 섹션에서는 Caps Lock 키를 Escape 키에 매핑하는 방법을 설명합니다.

키 매핑을 수행하기 전에 X 윈도우 시스템에서 Caps Lock 키의 키 코드를 찾아야 합니다. 이것은 키 매핑을 되돌릴 때 유용합니다. 다음은 Caps Lock 키의 키 코드를 얻는 명령어입니다.

    xmodmap -pke | grep Caps_Lock

PC 키보드의 경우 키 코드는 일반적으로 `66`입니다.

다음 명령어는 Caps Lock 키를 Escape 키에 매핑합니다.

    xmodmap -e "remove Lock = Caps_Lock" -e "keysym Caps_Lock = Escape"

다음은 Caps Lock의 키 코드가 66인 경우 Caps Lock 키를 Escape 키에 매핑하는 또 다른 명령어입니다. 이 명령어는 `keysym` 대신 `keycode`를 사용합니다.

    xmodmap -e "remove Lock = Caps_Lock" -e "keycode 66 = Escape"

다음 두 명령어는 현재 세부 정보를 표시합니다.

    xmodmap -pm -pke

다음 명령어는 매핑을 취소합니다.

    xmodmap -e "keycode 66 = Caps_Lock" -e "add Lock = Caps_Lock"

위 명령어에서 `keycode`가 사용되었습니다. 위 명령어에서 `keysym Escape = Caps_Lock`을 대신 사용하면 Caps Lock 키를 Caps Lock처럼 작동하도록 되돌리는 것과 함께 Escape 키의 동작도 Caps Lock처럼 작동하도록 변경됩니다. `keycode 66 = Caps_Lock`을 사용하면 Caps Lock 키의 동작만 복원됩니다.

### Linux: loadkeys

리눅스에서는 `loadKeys` 명령어를 사용하여 콘솔에서 한 키를 다른 키로 매핑할 수 있습니다. 이 섹션에서는 Caps Lock 키를 Escape 키에 매핑하는 방법을 설명합니다. 이 섹션의 모든 명령어는 루트 또는 슈퍼유저로 실행해야 합니다.

키 매핑을 수행하기 전에 콘솔에서 Caps Lock 키의 키 코드를 찾아야 합니다. 이것은 키 매핑을 되돌릴 때도 유용합니다.

다음은 콘솔에서 Caps Lock 키의 키 코드를 찾는 빠른 방법입니다.

    dumpkeys | grep -E "CtrlL_Lock|Caps_Lock"

이 섹션에서는 Caps Lock 키의 키 코드가 `58`이고 `Caps_Lock` 키에 매핑되어 있다고 가정합니다. 이는 대부분의 시스템에서 사실입니다. Debian 기반 시스템에서는 키 코드 `58`이 Debian 버그 [514464][] 및 커널 버그 [7746][]을 해결하기 위해 `CtrlL_Lock`에 매핑될 수 있습니다.

다음 명령어는 Caps Lock 키를 Escape 키에 매핑합니다.

    (dumpkeys | grep keymaps; echo keycode 58 = Escape) | loadkeys

다음 명령어는 매핑을 취소합니다.

    (dumpkeys | grep keymaps; echo keycode 58 = Caps_Lock) | loadkeys

[514464]: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=514464
[7746]: https://bugzilla.kernel.org/show_bug.cgi?id=7746


### Mac: 시스템 환경설정

macOS Sierra 버전 10.12.1부터 *시스템 환경설정*을 통해 Caps Lock을 Escape에 쉽게 매핑할 수 있습니다. 그렇게 하려면 다음 단계를 수행하십시오.

 1. Apple 메뉴 > *시스템 환경설정* > *키보드*로 이동합니다.
 2. *보조 키*를 클릭합니다.
 3. *Caps Lock 키*를 *Escape*로 설정합니다.
 4. *확인*을 클릭합니다.


### Mac: Seil

OS X에서는 *시스템 환경설정*을 통해 사용할 수 있는 매핑 옵션이 매우 제한적입니다. 예를 들어, Caps Lock을 Escape에 매핑할 수 없습니다. 따라서 일반적으로 OS X에서 키를 매핑하려면 추가 소프트웨어를 설치해야 합니다.

OS X에서 한 키를 다른 키로 매핑하는 데 널리 사용되는 도구는 [Seil][]입니다. 다음은 제 아름다운 여자 친구이자 아내가 Seil을 사용하여 OS X에서 Caps Lock을 Escape에 매핑하는 방법을 설명하는 기사입니다. <http://sunainapai.in/blog/map-caps-lock-to-escape-on-mac-os-x/>.

[Seil]: https://pqrs.org/osx/karabiner/seil.html


자료
---------

다음은 이 프로젝트에 대한 유용한 링크 목록입니다.

- [다운로드][Download URL]
- [소스 코드](https://github.com/susam/uncap)
- [이슈 트래커](https://github.com/susam/uncap/issues)
- [변경 로그](https://github.com/susam/uncap/blob/master/CHANGES.md)


라이선스
-------

이것은 자유 및 오픈 소스 소프트웨어입니다. MIT 라이선스 조건에 따라 사용, 복사, 수정, 병합, 게시, 배포, 하위 라이선스 부여 및/또는 사본을 판매할 수 있습니다. 자세한 내용은 [LICENSE.md][L]를 참조하십시오.

이 소프트웨어는 명시적이든 묵시적이든 어떠한 종류의 보증도 없이 "있는 그대로" 제공됩니다. 자세한 내용은 [LICENSE.md][L]를 참조하십시오.

[L]: LICENSE.md


지원
-------

버그를 보고하거나, 개선 사항을 제안하거나, 질문하려면 <https://github.com/susam/uncap/issues>를 방문하십시오.
