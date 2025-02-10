class Human {
private:
	std::string name;
	int age;
	int weight;

public:
	Human(std::string content) {
		cout << "Applying \"" << content << "\" to human!" << endl;
		std::istringstream iss(content);
		iss >> name >> age >> weight;
	}
	~Human() {}

	void print() {
		cout << "Name: " << name << "\n"
			 << "Age: " << age << "\n"
			 << "Weight: " << weight << "\n";
	}

	std::string getPrintString() {
		return	"Name: " + name + "\n" +
				"Age: " + std::to_string(age) + "\n" +
				"Weight: " + std::to_string(weight) + "\n";
	}
};