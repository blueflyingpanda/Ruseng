#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "get_next_line.h"

int replaceSym(wchar_t *layout, wchar_t *ptr) {
	int i = 0;
	while (*ptr != layout[i])
		i++;
	return i;
}

void switchLang(wchar_t **str2convert) {
	int i = 0;
	wchar_t *ptr;
	wchar_t rus[] = L"йцукенгшщзхъфывапролджэё]ячсмитьбю/>";
	wchar_t capRus[] = L"ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЁ[ЯЧСМИТЬБЮ\"№%:,.;";
	wchar_t eng[] = L"qwertyuiop[]asdfghjkl;'\\`zxcvbnm,./§";
	wchar_t capEng[] = L"QWERTYUIOP{}ASDFGHJKL:\"|~ZXCVBNM<>@#$%^&*";

	while (*str2convert && (*str2convert)[i]){
		if ((ptr = wcschr(rus, (*str2convert)[i])))
			(*str2convert)[i] = eng[replaceSym(rus, ptr)];
		else if ((ptr = wcschr(eng, (*str2convert)[i])))
			(*str2convert)[i] = rus[replaceSym(eng, ptr)];
		else if ((ptr = wcschr(capRus, (*str2convert)[i])))
			(*str2convert)[i] = capEng[replaceSym(capRus, ptr)];
		else if ((ptr = wcschr(capEng, (*str2convert)[i])))
			(*str2convert)[i] = capRus[replaceSym(capEng, ptr)];
		i++;
	}
}

void fromInput() {
	char *str2convert;


	write(1, "Enter your string:\n", strlen("Enter your string:\n"));
	if (get_next_line(0, &str2convert) != -1){
		wchar_t *toPrint = malloc(sizeof(wchar_t) * strlen(str2convert));
		mbstowcs(toPrint, str2convert, sizeof(wchar_t) * strlen(str2convert) + 1);
		switchLang(&toPrint);
		printf("\nConverted string is:\n%ls\n", toPrint);
		free(str2convert);
		free(toPrint);
	}
	else
		write(2, "Something went wrong\n", strlen("Something went wrong\n"));
}

void fromFile(char *file) {
	char *str2convert;
	wchar_t *toPrint;
	char newFile[] = "temp";
	int fd = -2;
	int filedes = -2;

	if ((fd = open(file, O_RDONLY)) < 0)
		write(2, "Can't open the file\n", strlen("Can't open the file\n"));
	if ((filedes = open(newFile,O_TRUNC | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
		write(2, "Can't create the file\n", strlen("Can't create the file\n"));

	if (fd > -1 && filedes > -1){
		write(1, "Converting your file...", strlen("Converting your file..."));
		while (get_next_line(fd, &str2convert) > 0){
			toPrint = malloc(sizeof(wchar_t) * strlen(str2convert));
			mbstowcs(toPrint, str2convert, sizeof(wchar_t) * strlen(str2convert) + 1);
			switchLang(&toPrint);
			dprintf(filedes, "%ls\n", toPrint);
			free(str2convert);
			free(toPrint);
		}
		if (str2convert){
			toPrint = malloc(sizeof(wchar_t) * strlen(str2convert));
			mbstowcs(toPrint, str2convert, sizeof(wchar_t) * strlen(str2convert) + 1);
			switchLang(&toPrint);
			dprintf(filedes, "%ls", toPrint);
			free(str2convert);
			free(toPrint);
		}
		close(fd);
		close(filedes);
		if (remove(file))
			write(2, "Can't delete the file\n", strlen("Can't delete the file\n"));
		else if (rename(newFile, file))
			write(2, "Can't rename the file\n", strlen("Can't rename the file\n"));
		else
			write(1, "Done!\n", strlen("Done!\n"));
	}
}

int main(int argc, char **argv) {
	setlocale(LC_CTYPE,"UTF-8");
	if (argc == 1)
		fromInput();
	else if (argc == 2){
		fromFile(argv[1]);
	}
	else
		write(2, "Too many arguments\n", strlen("Too many arguments\n"));
	return 0;
}
