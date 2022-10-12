import pyperclip
import sys


def switch_language_standard_keyboard(buffer: str) -> str:
    rus = 'ё1234567890-=йцукенгшщзхъфывапролджэячсмитьбю.'
    cap_rus = 'Ё!"№;%:?*()_+ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ,'
    eng = '`1234567890-=qwertyuiop[]asdfghjkl;\'zxcvbnm,./'
    cap_eng = '~!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>?'
    return switch_language(rus, cap_rus, eng, cap_eng, buffer)


def switch_language_apple_keyboard(buffer: str):
    rus = '>1234567890-=йцукенгшщзхъфывапролджэё]ячсмитьбю/'
    cap_rus = '<!"№%:,.;()_+ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЁ[ЯЧСМИТЬБЮ?'
    eng = '§1234567890-=qwertyuiop[]asdfghjkl;\'\\`zxcvbnm,./'
    cap_eng = '±!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:"|~ZXCVBNM<>?'
    return switch_language(rus, cap_rus, eng, cap_eng, buffer)


def switch_language(rus: str, cap_rus: str, eng: str, cap_eng: str, buffer: str) -> str:
    converted = [' '] * len(buffer)
    for i in range(len(buffer)):
        if buffer[i] in eng:
            converted[i] = rus[eng.index(buffer[i])]
        elif buffer[i] in rus:
            converted[i] = eng[rus.index(buffer[i])]
        elif buffer[i] in cap_eng:
            converted[i] = cap_rus[cap_eng.index(buffer[i])]
        elif buffer[i] in cap_rus:
            converted[i] = cap_eng[cap_rus.index(buffer[i])]
    return ''.join(converted)


if __name__ == '__main__':
    print("Program transforms the text that is already in the buffer")
    if sys.platform == 'darwin':
        pyperclip.copy(switch_language_apple_keyboard(pyperclip.paste()))
    else:
        pyperclip.copy(switch_language_standard_keyboard(pyperclip.paste()))