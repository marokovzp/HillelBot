#include <commands.hxx>
#include <numeric>
#include <cctype>
#include <random>


std::string Echo::execute(std::string cmd) {
	cmd.erase(0, strlen("/echo "));		// стереть начало "/echo"

	return cmd;
}

std::string Hello::execute(std::string cmd) {
	return std::string("Welcome to the bot!!!");
}

bool isInteger(const std::string& str) {
	if (str.empty()) return false;
	for (char c : str) {
		if (!std::isdigit(c) && c != '-' && c != '+') {
			return false;
		}
	}
	return true;
}

std::string AddFunc::execute(std::string cmd) {
	cmd.erase(0, strlen("/add"));

	std::istringstream iss2(cmd);
	std::vector<std::string> characters;
	std::string character;

	while (iss2 >> character) {
		characters.push_back(character);
	}

	if (characters.size() < 1) {
		cmd = "0";
		return cmd;
	}

	int index = -1;
	for (size_t i = 0; i < characters.size(); ++i) {
		if (!isInteger(characters[i])) {
			index = i;
			break;
		}
	}

	if (index != -1) {

		cmd = characters[index];
		cmd += " is not an integer number.";

		return cmd;
	}

	std::istringstream iss(cmd);
	std::vector<int> numbers;
	int number;
	while (iss >> number) {
		numbers.push_back(number);
	}
	int sum = std::accumulate(numbers.begin(), numbers.end(), 0);

	cmd = "Summ of integers ";
	cmd += std::to_string(sum);

	return cmd;
}

std::string DoNothing::execute(std::string cmd) {
	return std::string();
}

int generate_randome_number(int from, int to) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(from, to);

	return distrib(gen);
}

std::string Joke::execute(std::string cmd) {
	int i;
	std::vector<std::string> joke = {
	"I told my wife she should embrace her mistakes.She gave me a hug.",
	"Parallel lines have so much in common. It's a shame they'll never meet.",
	"I'm reading a book on anti-gravity. It's impossible to put down!",
	"Why don't skeletons fight each other? They don't have the guts.",
	"I used to play piano by ear, but now I use my hands.",
	"I'm reading a book about anti-gravity. It's really uplifting!",
	"Why did the scarecrow win an award ? Because he was outstanding in his field!",
	"I told my computer I needed a break and now it won't stop giving me Kit Kats.",
	"Why was the math book sad ? Because it had too many problems.",
	"I used to be a baker, but I couldn't make enough dough."
	};
	i = generate_randome_number(0, 9);

	std::cout << joke[i] << std::endl;

	return joke[i];
}

std::unordered_map<std::string, AdbstractCommand*> Commands::commands() {
	std::unordered_map<std::string, AdbstractCommand*> cmds = {
{ "/echo ", new Echo() },
{ "/hello", new Hello() },
{ "/add", new AddFunc() },
{ "/joke", new Joke() }
	};
	return cmds;
}

std::string* Commands::is_valid_command(const std::string& maybe_command) {
	for (auto& pair : commands()) {
		if (maybe_command.starts_with(pair.first)) {
			return new std::string(pair.first);
		}
	}
	return nullptr;
}

AdbstractCommand* Commands::get_command(const std::string maybe_command_name) {
	std::cout << "GetCommand" << std::endl;
	std::string* command_name = is_valid_command(maybe_command_name);
	if (command_name) {
		//std::cout << "Found correct command " << maybe_command_name << std::endl;
		return commands()[*command_name];
	}
	else {
		return new DoNothing();
	}
}