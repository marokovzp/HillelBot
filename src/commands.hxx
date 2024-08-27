#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <sstream>
#include <fstream>

class AdbstractCommand {
public:
	virtual std::string execute(std::string cmd) = 0;
};

class Echo : public AdbstractCommand {
public:
	std::string execute(std::string cmd) override;
};

class Hello : public AdbstractCommand {
public:
	std::string execute(std::string cmd) override;
};

class AddFunc : public AdbstractCommand {
public:
	std::string execute(std::string cmd) override;
};

class DoNothing : public AdbstractCommand {
public:
	std::string execute(std::string cmd) override;
};

class Joke : public AdbstractCommand {
public:
	std::string execute(std::string cmd) override;
};

namespace Commands {
	std::unordered_map<std::string, AdbstractCommand*> commands();

	std::string* is_valid_command(const std::string& maybe_command);

	AdbstractCommand* get_command(const std::string maybe_command_name);
};