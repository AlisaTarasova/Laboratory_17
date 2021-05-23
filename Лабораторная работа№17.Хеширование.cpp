#include <iostream>
#include <vector>
using namespace std;

struct Date
{
	int Day, Month, Year;

	bool operator==(const Date& t)
	{
		return (Day == t.Day && Month == t.Month && Year == t.Year);
	}
};

struct Address
{
	int Home;
	string Street;
};

struct Human
{
	string FIO;
	Date DateOfBirth;
	Address Address1;

	Human()
	{
		string names[] = { "Андрей", "Михаил", "Сергей", "Артём", "Дмитрий" };
		string surnames[] = { "Петров" , "Соловьёв", "Иванов","Гончаров","Пушкин" };
		string patronymic[] = { "Андреевич", "Владимирович", "Максимович", "Кирилович", "Ильич" };

		string street[] = { "Ленина","Екатерининская","Революция","Карла Маркса","Куйбышева" };

		FIO = surnames[rand() % 5] + " " + names[rand() % 5] + " " + patronymic[rand() % 5];
		
		DateOfBirth.Day = 1 + rand() % 28;
		DateOfBirth.Month = 1 + rand() % 12;
		DateOfBirth.Year = 1800 + rand() % 220;
		
		Address1.Home = 1 + rand() % 50;
		Address1.Street = street[rand() % 5];
	}

	void Print()
	{
		cout << "ФИО: " << FIO << endl;
		cout << "Дата рождения: " << DateOfBirth.Day << "." << DateOfBirth.Month << "." << DateOfBirth.Year << endl;
		cout << "Адрес: " << Address1.Street << ", " << Address1.Home << endl << endl;
	}
};

void Printmas(Human*& hum, int n)
{
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			cout << "Элемент " << i + 1 << ": " << endl;
			hum[i].Print();
		}
	}
	else
		cout << "Массив пуст.";
}

struct Hash_Table_El
{
	Human* info;
	Hash_Table_El* next;

	Hash_Table_El(Human* info, Hash_Table_El* next)
	{
		this->info = info;
		this->next = next;
	}
};

struct Function_Hash
{
	int operator()(Date value, int table_size) const
	{
		int key = value.Year * 10000 + value.Month * 100 + value.Day;
		key = key % table_size;
		return key;
	}
};
 
template <typename T>
struct Hash_Table
{

	int table_size;
	Hash_Table_El** hash_table;

	Hash_Table(int Tsize)
	{
		this->table_size = Tsize;
		this->hash_table = new Hash_Table_El * [Tsize];
		for (int i = 0; i < table_size; i++) 
		{
			this->hash_table[i] = NULL;
		}
	}

	void Adding(Human*& value, const T& t = T()) 
	{
		int index = t(value->DateOfBirth, this->table_size);
		Hash_Table_El* item = new Hash_Table_El(value, NULL);

		if (this->hash_table[index] == NULL)
		{
			this->hash_table[index] = item;
			return;
		}

		Hash_Table_El* place = this->hash_table[index];
		while (place->next != NULL) 
		{
			place = place->next;
		}
		place->next = item;
	}

	Human* Find(Date value, const T& t = T())
	{
		int index = t(value, this->table_size);

		if (this->hash_table[index] == NULL) 
		{
			return NULL;
		}

		Hash_Table_El* item = this->hash_table[index];
		while (item != NULL) 
		{
			if (item->info->DateOfBirth == value) 
			{
				return item->info;
			}
			item = item->next;
		}
		return NULL;
	}

	void Print() {
		for (int i = 0; i < this->table_size; i++) 
		{
			cout << "Индекс: " << i << endl;

			if (this->hash_table[i] == NULL) 
			{
				cout << "Элементов нет.";
			}
			else 
			{
				Hash_Table_El* item = this->hash_table[i];
				while (item != NULL) 
				{
					cout << item->info->FIO << endl;
					item = item->next;
				}
			}

			cout << endl << endl;
		}
	}
};


int main()
{
	setlocale(0,"");
	system("color F0");

	Hash_Table<Function_Hash>* table = new Hash_Table<Function_Hash>(31);

	// Заполнение
	for (int i = 0; i < 100; i++)
	{
		Human* human = new Human();
		table->Adding(human);
	}

	//Вывод
	cout << "Хеш-таблица: "<<endl;
	table->Print();

	// Коллизии
	int k = 0;
	for (int i = 0; i < table->table_size; i++) 
	{
		if (table->hash_table[i] != NULL && table->hash_table[i]->next != NULL)
		{
			k++;
		}
	}
	cout << "Количество коллизий: " << k<<endl;

	// Поиск
	Date key;
	cout << "Введите дату рождения для поиска: ";
	cin >> key.Day >> key.Month >> key.Year;

	Human* found = table->Find(key);
	if (found != NULL) 
	{
		found->Print();
	}
	else 
	{
		cout << "Данной даты рождения нет." << endl;
	}

	return 0;
}

