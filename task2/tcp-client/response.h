#define MAX_STUDENT_MARKS 4

class ServerResponse {
private:
	char surname[32];
	char name[32];
	int group;
	bool onBudget;

public:
	~ServerResponse() {}

	std::string getMessage() {
		std::string strSurname = surname, strName = name,
		budgetMsg = (onBudget) ?
						"accepted for the budget." :
						"not allowed for the budget.";

		return "Student " + strSurname + " " + strName +
			" of group " + std::to_string(group) + " " + budgetMsg;
	}
};

struct Request {
	char surname[32];
	char name[32];
	int group;
	int marks[MAX_STUDENT_MARKS] = {0, 0, 0, 0};

	void input() {
		cin >> surname >> name >> group;
		for (int i = 0; i < MAX_STUDENT_MARKS; i++) cin >> marks[i];
	}

	void print() {
		cout << surname << " " << name << " " << group << " ";
		for (int i = 0; i < MAX_STUDENT_MARKS; i++) cout << marks[i] << " ";
		cout << endl;
	}
};