#define MAX_STUDENT_MARKS 4

struct Response {
	char surname[32];
	char name[32];
	int group;
	bool onBudget;

	std::string toString() {
		std::string strSurname = surname;
		std::string strName = name;
		return strSurname + " " + strName + " " + std::to_string(group) + " " + std::to_string(onBudget);
	}
};

class Student {
private:
	char surname[32];
	char name[32];
	int group;
	unsigned int marks[MAX_STUDENT_MARKS];

public:
	~Student() {}

	std::string getMarkString() {
		std::string markString = "";

		for (int i = 0; i < MAX_STUDENT_MARKS; i++) {
			markString += std::to_string(marks[i]) + " ";
		}

		return markString;
	}

	void print() {
		std::string strSurname = surname;
		std::string strName = name;

		cout << "Student: " << strSurname + " " + strName << "\n"
			<< "Group: " << group << "\n"
			<< "Marks: " << getMarkString() << "\n";
	}

	std::string getPrintString() {
		std::string strSurname = surname;
		std::string strName = name;

		return	"Student: " + strSurname + " " + strName + "\n" +
			"Group: " + std::to_string(group) + "\n";
			"Marks: " + getMarkString() + "\n";
	}

	Response getResponse() {
		unsigned int average = 0;

		for (int i = 0; i < MAX_STUDENT_MARKS; i++) {
			average += marks[i];
		}
		average /= MAX_STUDENT_MARKS;

		Response response;
		
		memcpy(response.surname, surname, sizeof(response.surname));
		memcpy(response.name, name, sizeof(response.name));
		response.group = group;
		response.onBudget = ((average >= 4) ? (true) : (false));

		return response;
	}
};