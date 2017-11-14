#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <conio.h>
#include <inttypes.h>
#include <locale.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Типы пользовательских данных-------------------------------------------------------------------------------------

typedef struct {
	char* type;
	char* number;
} TPhones;

typedef struct {
	char* street;
	uint8_t house;
	uint8_t room;
} TAddress;

typedef struct {
	char* fname;
	char* lname;
} TName;

typedef struct {
	/*char* fname;
	char* lname;*/
	TName* name;
	uint8_t phNum;
	TPhones* phones;
	TAddress* address;
} TContact;

//------------------------------------------------------------------------------------------------------------------

//Меню и прочее-----------------------------------------------------------------------------------------------------

uint8_t menu() {
	uint8_t ch = 0;

	system("cls");

	printf("Меню:\n");
	printf("1 - Добавить контакт.\n");
	printf("2 - Печать контактов.\n");
	printf("3 - Очистить список контактов.\n");
	printf("4 - Удалить контакт.\n");
	printf("5 - Редактировать контакт.\n");
	printf("6 - Просмотреть контакт.\n");
	printf("7 - Вызвать контакт.\n");
	printf("8 - Сортировать контакты.\n");
	printf("9 - Запись в файл.\n");
	printf("10 - Чтение из файла.\n");
	printf("11 - Поиск по имени.\n");
	printf("12 - Поиск по номеру телефона.\n");
	printf("13 - Поиск по параметрам.\n");
	printf("14 - Выход.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

uint8_t menuCalling() {
	uint8_t ch = 0;

	system("cls");

	printf("Меню:\n");
	printf("1 - Выбрать контакт.\n");
	printf("2 - Набрать номер.\n");
	printf("3 - Назад.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

uint8_t menuYesNo(const char* mes) {
	uint8_t ch = 0;

	system("cls");

	printf(mes);
	printf("1 - Да.\n");
	printf("2 - Нет.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

void pass() {
	while (getchar() != '\n');
}

//------------------------------------------------------------------------------------------------------------------

//Ввод--------------------------------------------------------------------------------------------------------------
//Ввод строки-------------------------------------------------------------------------------------------------------

char* createLine(FILE* fl) {
	char* line = NULL;
	char ch = 0;
	uint16_t ln = 0;
	uint16_t ind = 0;

	while (!feof(fl)) {
		ch = getc(fl);

		if (ln == ind) {
			ln++;
			line = (char*)realloc(line, (ln *= 2) * sizeof(char));
		}

		if (ch == '\n' || ch == EOF || ch == '\0') {
			if (ind == 0) {
				free(line);
				line = NULL;
				break;
			}

			*(line + ind) = '\0';
			line = (char*)realloc(line, (ind + 1) * sizeof(char));
			break;
		}
		else {
			*(line + ind++) = ch;
		}
	}

	return line;
}

//------------------------------------------------------------------------------------------------------------------
//Ввод телефонов----------------------------------------------------------------------------------------------------

TPhones* createPhones(uint8_t* num) {
	TPhones* phn = NULL;
	char* line = NULL;
	*num = 0;

	printf("Введите номер телефона или пустую строку для выхода: ");
	while (line = createLine(stdin)) {
		phn = (TPhones*)realloc(phn, (*num + 1)*sizeof(TPhones));
		(phn + *num)->number = line;
		

		printf("Введите тип номера: ");
		if (line = createLine(stdin)) {
			(phn + *num)->type = line;
		}
		else {
			(phn + *num)->type = NULL;
			break;
		}
		++*num;
		printf("Введите номер телефона или пустую строку для выхода: ");
	}

	return phn;
}

//------------------------------------------------------------------------------------------------------------------
//Ввод адреса-------------------------------------------------------------------------------------------------------

TAddress* createAddress() {
	TAddress* adr = NULL;
	char* line = NULL;
	uint8_t h = 0;
	uint8_t r = 0;

	printf("Введите улицу или пустую строку для выхода: ");
	if (line = createLine(stdin)) {
		adr = (TAddress*)calloc(1, sizeof(TAddress));
		adr->street = line;

		printf("Введите номер дома: ");
		if (fscanf(stdin, "%hhu", &h) && adr) {
			adr->house = h;
		}

		printf("Введите номер квартиры: ");
		if (fscanf(stdin, "%hhu", &r) && adr) {
			adr->room = r;
		}
		pass();
	}

	return adr;
}

//------------------------------------------------------------------------------------------------------------------
//Ввод имени--------------------------------------------------------------------------------------------------------

TName* createName() {
	TName* nm = NULL;
	char* line = NULL;

	printf("Введите имя или пустую строку для выхода: ");
	if (line = createLine(stdin)) {
		nm = (TName*)calloc(1, sizeof(TName));
		nm->fname = line;

		printf("Введите фамилию: ");
		if ((line = createLine(stdin)) && (nm)) {
			nm->lname = line;
		}
	}

	return nm;
}

//------------------------------------------------------------------------------------------------------------------
//Ввод контакта-----------------------------------------------------------------------------------------------------

TContact* createContact() {
	TContact* cnt = NULL;
	TName* nm = NULL;
	char* line = NULL;

	if (nm = createName(stdin)) {
		cnt = (TContact*)calloc(1, sizeof(TContact));

		cnt->name = nm;
		cnt->phNum = 0;
		cnt->phones = createPhones(&(cnt->phNum));
		cnt->address = createAddress();

		if (cnt->phones == NULL && cnt->address == NULL) {
			free(cnt);
			cnt = NULL;
		}
	}

	return cnt;
}

//------------------------------------------------------------------------------------------------------------------


//Печать------------------------------------------------------------------------------------------------------------
//Печать телефонов--------------------------------------------------------------------------------------------------

void printPhones(TPhones* phn, uint8_t num) {
	printf("Телефны:\n");
	for (uint8_t i = 0; i < num; i++) {
		printf("%s: %s\n", (phn + i)->type, (phn + i)->number);
	}
}

//------------------------------------------------------------------------------------------------------------------
//Печать адресов----------------------------------------------------------------------------------------------------

void printAddress(TAddress* adr) {
	printf("Адрес: ");
	printf("st. %s, h. %hhu, r. %hhu.\n", adr->street, adr->house, adr->room);
}

//------------------------------------------------------------------------------------------------------------------
//Печать контакта---------------------------------------------------------------------------------------------------

void printContact(TContact* cnt) {
	printf("Имя: %s.\n", cnt->name->fname);
	printf("Фамилия: %s.\n", cnt->name->lname);

	if (cnt->phones) {
		printPhones(cnt->phones, cnt->phNum);
	}
	
	if (cnt->address) {
		printAddress(cnt->address);
	}

	putc('\n', stdout);
}

//------------------------------------------------------------------------------------------------------------------

//Удаление----------------------------------------------------------------------------------------------------------
//Удаление телефонов------------------------------------------------------------------------------------------------

TPhones* deletePhones(TPhones* phn, uint8_t num) {
	for (int8_t i = num - 1; i >= 0; i--) {
		free((phn + i)->type);
		free((phn + i)->number);
		phn = (TPhones*)realloc(phn, num * sizeof(TPhones));
	}

	return NULL;
}

//------------------------------------------------------------------------------------------------------------------
//Удаление контакта--------------------------------------------------------------------------------------------------

TContact* deleteContact(TContact* cnt) {
	free(cnt->name->fname);
	free(cnt->name->lname);
	deletePhones(cnt->phones, cnt->phNum);
	free(cnt->address->street);

	return NULL;
}

//------------------------------------------------------------------------------------------------------------------


int main(void) {
	setlocale(LC_ALL, "Russian");

	TContact** noteBook = NULL;
	uint8_t notes = 0;

	uint8_t ch = 0;
	
	while ((ch = menu()) != 14) {
		switch (ch) {
		//добавление контакта
		case 1: {
			TContact* nt = NULL;

			pass();
			while (nt = createContact()) {
				noteBook = (TContact**)realloc(noteBook, (notes + 1) * sizeof(TContact*));
				*(noteBook + notes++) = nt;
				putc('\n', stdout);
			}
			break;
		}

		//печать контактов
		case 2:
			printf("\nВсего контактов: %hhu.\n\n", notes);
			for (uint8_t i = 0; i < notes; i++) {
				printContact(*(noteBook + i));
			}
			break;

		//очистить список контактов
		case 3:
			for (int8_t i = notes - 1; i >= 0; i--) {
				deleteContact(*(noteBook + i));
				noteBook = (TContact**)realloc(noteBook, i * sizeof(TContact*));
			}
			noteBook = NULL;
			notes = 0;
			break;

		//удалить контакт
		case 4: {
			uint8_t num = 0;
			printf("Введите номер удаляемой записи: ");
			scanf("%hhu", &num);

			for (uint8_t i = num - 1; i < notes - 1; i++) {
				*(noteBook + i) = *(noteBook + i + 1);
			}
			if (notes > 0) {
				noteBook = (TContact**)realloc(noteBook, (--notes) * sizeof(TContact*));
			}

			break;
		}

		//редактировать контакт
		case 5: {
			uint8_t num = 0;
			TContact* rec = NULL;
			printf("Введите номер редактируемой записи: ");
			scanf("%hhu", &num);
			pass();

			printContact(*(noteBook + num - 1));

			while ((rec = createContact()) == NULL) {
				printf("Ошибка при редактировании. Пожалуйста повторите попытку.\n");
			}
			deleteContact(*(noteBook + num - 1));
			*(noteBook + num - 1) = rec;

			break;
		}

		//просмотреть контакт
		case 6: {
			uint8_t num = 0;
			printf("Введите номер просматриваемой записи: ");
			scanf("%hhu", &num);
			pass();

			if (num <= notes) {
				printContact(*(noteBook + num - 1));
			}
			
			break;
		}

		//вызвать контакт
		case 7: {
			uint8_t ch = 0;
			while ((ch = menuCalling()) != 3) {
				switch (ch) {
				case 1: {
					uint8_t num = 0;
					printf("Введите номер вызываемого контакта: ");
					scanf("%hhu", &num);
					pass();

					const TContact* cnt = *(noteBook + num - 1);

					if (cnt->phNum == 1) {
						num = 1;
					}
					else {
						for (uint8_t i = 0; i < cnt->phNum; i++) {
							printf("%hhu - %s: %s.\n", i + 1, (cnt->phones + i)->type, (cnt->phones + i)->number);
						}
						printf("Ваш выбор: ");
						scanf("%hhu", &num);
						pass();
					}

					printf("%s: %s. ", (cnt->phones + num - 1)->type, (cnt->phones + num - 1)->number);
					printf("Вызов %s %s.\n", cnt->name->fname, cnt->name->lname);
					break;
				}

				case 2: {
					pass();
					printf("Введите номер: ");
					char* phnm = createLine(stdin);
					bool found = false;

					for (uint8_t i = 0; i < notes; i++) {
						TContact* cnt = *(noteBook + i);
						for (uint8_t j = 0; j < cnt->phNum; j++) {
							if (!strcmp(phnm, (cnt->phones + j)->number)) {
								found = true;
								printf("%s: %s. ", (cnt->phones + j)->type, (cnt->phones + j)->number);
								printf("Вызов %s %s.\n", cnt->name->fname, cnt->name->lname);
								//_getch();
								break;
							}
						}

						if (found) {
							break;
						}
					}

					if (!found) {
						printf("Вызов %s.\n", phnm);
						printf("Нажмите любую клавишу.\n");
						_getch();
					}
					
					//добавление в телефонную книгу
					if (!found && menuYesNo("Желаете добавить контакт:\n") == 1) {
						pass();
						
						TContact* cnt = NULL;
						TName* nm = NULL;

						if (nm = createName()) {
							cnt = (TContact*)calloc(1, sizeof(TContact));

							cnt->name = nm;
							cnt->phNum = 0;
							
							ungetc('\n', stdin);
							ungetc(' ', stdin);
							cnt->phones = createPhones(&(cnt->phNum));
							cnt->phones->number = phnm;

							cnt->address = createAddress();

							if (cnt->phones == NULL && cnt->address == NULL) {
								free(cnt);
								cnt = NULL;
							}
						}

						if (cnt) {
							noteBook = (TContact**)realloc(noteBook, (notes + 1) * sizeof(TContact*));
							*(noteBook + notes++) = cnt;
						}

					}

					break;
				}

				default:
					pass();
					printf("Неправильный ввод.\n");
					break;
				}
				printf("Нажмите любую клавишу.\n");
				_getch();
			}
			
			break;
		}//case 7

		//сортировка контактов
		case 8:
			for (uint8_t i = 0; i < notes - 1; i++) {
				for (uint8_t j = i + 1; j < notes; j++) {
					if ( strcmp((*(noteBook + i))->name->lname, (*(noteBook + j))->name->lname) > 0 ) {
						TContact* cnt = *(noteBook + i);
						*(noteBook + i) = *(noteBook + j);
						*(noteBook + j) = cnt;
					}
				}
			}
			break;

		//запись в файл
		case 9: {
			FILE* out = NULL;

			pass();
			printf("Введите имя файла: ");
			char* fname = createLine(stdin);

			if (fname == NULL || (out = fopen(fname, "wb")) == NULL) {
				printf("Ошибка открытия файла.\n");
			}
			else {
				free(fname);
				fname = NULL;
				fwrite(&notes, sizeof(notes), 1, out);
				for (uint8_t i = 0; i < notes; i++) {
					bool flag = false;

					fwrite((*(noteBook + i))->name->fname, strlen((*(noteBook + i))->name->fname) + 1, 1, out);
					fwrite((*(noteBook + i))->name->lname, strlen((*(noteBook + i))->name->lname) + 1, 1, out);

					fwrite(&((*(noteBook + i))->phNum), sizeof(uint8_t), 1, out);

					for (uint8_t j = 0; j < (*(noteBook + i))->phNum; j++) {
						fwrite(((*(noteBook + i))->phones + j)->type, strlen(((*(noteBook + i))->phones + j)->type) + 1, 1, out);
						fwrite(((*(noteBook + i))->phones + j)->number, strlen(((*(noteBook + i))->phones + j)->number) + 1, 1, out);
					}

					if ((*(noteBook + i))->address) {
						flag = true;

						fwrite(&flag, sizeof(bool), 1, out);

						fwrite((*(noteBook + i))->address->street, strlen((*(noteBook + i))->address->street) + 1, 1, out);
						fwrite(&((*(noteBook + i))->address->house), sizeof(uint8_t), 1, out);
						fwrite(&((*(noteBook + i))->address->room), sizeof(uint8_t), 1, out);
					}
					else {
						fwrite(&flag, sizeof(bool), 1, out);
					}

					fflush(out);
				}
			}

			if (out) {
				fclose(out);
			}

			break;
		}

		//чтение файла
		case 10: {
			FILE* in = NULL;

			pass();
			printf("Введите имя файла: ");
			char* fname = createLine(stdin);

			if (fname == NULL || (in = fopen(fname, "rb")) == NULL) {
				printf("Ошибка открытия файла.\n");
			}
			else {
				free(fname);
				fname = NULL;

				uint8_t cont = 0;
				fread(&cont, sizeof(cont), 1, in);
				noteBook = (TContact**)realloc(noteBook, (notes + cont) * sizeof(TContact*));

				for (uint8_t i = 0; i < cont; i++) {
					*(noteBook + notes + i) = (TContact*)calloc(1, sizeof(TContact));
					(*(noteBook + notes + i))->name = (TName*)calloc(1, sizeof(TName));
					(*(noteBook + notes + i))->name->fname = createLine(in);
					(*(noteBook + notes + i))->name->lname = createLine(in);

					fread(&((*(noteBook + notes + i))->phNum), sizeof(uint8_t), 1, in);
					(*(noteBook + notes + i))->phones = (TPhones*)calloc((*(noteBook + notes + i))->phNum, sizeof(TPhones));
					for (uint8_t j = 0; j < (*(noteBook + notes + i))->phNum; j++) {
						((*(noteBook + notes + i))->phones + j)->type = createLine(in);
						((*(noteBook + notes + i))->phones + j)->number = createLine(in);
					}

					bool flag = false;
					fread(&flag, sizeof(bool), 1, in);
					if (flag) {
						(*(noteBook + notes + i))->address = (TAddress*)calloc(1, sizeof(TAddress));
						(*(noteBook + notes + i))->address->street = createLine(in);
						fread(&((*(noteBook + notes + i))->address->house), sizeof(uint8_t), 1, in);
						fread(&((*(noteBook + notes + i))->address->room), sizeof(uint8_t), 1, in);
					}
				}
				notes += cont;
			}

			if (in) {
				fclose(in);
			}

			break;
		}

		//поиск по имени
		case 11: {
			pass();
			printf("Введите имя: ");
			char* line = createLine(stdin);
			bool flag = true;
			putc('\n', stdin);
			if (line) {
				for (uint8_t i = 0; i < notes; i++) {
					if (!strcmp(line, (*(noteBook + i))->name->lname)) {
						printContact(*(noteBook + i));
						flag = false;
					}
				}
			}

			if (flag) {
				printf("Абонент не найден.\n");
			}

			break;
		}

		//поиск по номеру телефона
		case 12: {
			pass();
			printf("Ведите номер телефона: ");
			char* line = createLine(stdin);
			bool flag = true;
			putc('\n', stdin);

			if (line) {
				for (uint8_t i = 0; i < notes; i++) {
					for (uint8_t j = 0; j < (*(noteBook+i))->phNum; j++) {
						if (!strcmp(line, ((*(noteBook + i))->phones + j)->number)) {
							printContact(*(noteBook + i));
							flag = false;
						}
					}
				}
			}

			if (flag) {
				printf("Абонент не найден.\n");
			}

			break;
		}

		//поиск по параметрам
		case 13: {
			pass();
			printf("Введите улицу или Ввод для возврата: ");
			char* line = createLine(stdin);
			bool flag = true;

			if (line) {
				printf("Введите номмер дома: ");
				uint8_t hn = 0;
				scanf("%hhu", &hn);
				if (hn == 0) {
					for (uint8_t i = 0; i < notes; i++) {
						if (!strcmp(line, (*(noteBook + i))->address->street)) {
							printContact(*(noteBook + i));
							flag = false;
						}
					}
					if (flag) {
						printf("Поиск не дал результатов.\n");
					}
				}//if (hn == 0)
				else {
					printf("Введите номер квартиры: ");
					uint8_t rn = 0;
					scanf("%hhu", &rn);
					uint8_t N = 0;

					if (rn == 0) {
						printf("Введите глубину поиска по домам: ");
						scanf("%hhu", &N);
						uint8_t bgn = (hn - N > 0) ? hn - N : 1;
						uint8_t end = hn + N;

						flag = true;
						for (uint8_t i = 0; i < notes; i++) {
							if (!strcmp(line, (*(noteBook + i))->address->street) && (*(noteBook + i))->address->house >= bgn && (*(noteBook + i))->address->house <= end) {
								printContact(*(noteBook + i));
								flag = false;
							}
						}

						if (flag) {
							printf("Поиск не дал результатов.\n");
						}
					}//if (rn == 0)
					else {
						printf("Введите глубину поиска по квартирам: ");
						scanf("%hhu", &N);

						uint8_t bgn = (rn - N > 0) ? rn - N : 1;
						uint8_t end = rn + N;

						flag = true;
						for (uint8_t i = 0; i < notes; i++) {
							if (!strcmp(line, (*(noteBook + i))->address->street) && (*(noteBook + i))->address->house == hn && (*(noteBook + i))->address->room >= bgn && (*(noteBook + i))->address->room <= end) {
								printContact(*(noteBook + i));
								flag = false;
							}
						}

						if (flag) {
							printf("Поиск не дал результатов.\n");
						}
					}
				}//else (hn == 0)
			}

			break;
		}

		default:
			pass();
			printf("Неправильный ввод.\n");
			break;
		}
		printf("Нажмите любую клавишу.\n");
		_getch();
	}

	return 1;
}