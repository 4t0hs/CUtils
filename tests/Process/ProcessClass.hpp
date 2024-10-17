#include <string>
#include <vector>
#include "Process/Process.h"


struct ProcessInfo {
	std::string commandLine;
	bool disableOutputCapture{ false };
private:
	std::vector<char *> _environment;
public:
	ProcessInfo() = default;
	ProcessInfo(std::string commandLine, bool disableOutputCapture = false) : commandLine(commandLine), disableOutputCapture(disableOutputCapture) {}
	void SetEnvironment(std::vector<std::string> environment) {
		_environment.clear();
		_environment.reserve(environment.size());
		for (auto &e : environment) {
			_environment.push_back((char *)e.c_str());
		}
	}
	std::vector<char *> &GetEnvironment() {
		return _environment;
	}
};

class ProcessClass : public Process {
public:
	ProcessClass() {
		Process_Init(this, nullptr);
	}
	ProcessClass(ProcessInfo &info) {
		SetStartInfo(info);
		Process_Init(this, nullptr);
	}
	~ProcessClass() {
		Process_Destroy(this);
	}

	bool HasExited() {
		return Process_HasExited(this);
	}

	std::int32_t ExitCode() {
		return Process_ExitCode(this);
	}
	double TotalProcessorTime() {
		return Process_TotalProcessorTime(this);
	}
	double UserProcessorTime() {
		return Process_UserProcessorTime(this);
	}
	double StartTime() {
		return Process_StartTime(this);
	}
	double ExitTime() {
		return Process_ExitTime(this);
	}
	pid_t Id() {
		return Process_Id(this);
	}
	std::string StandardOutput() {
		ProcessOutput output;
		Process_StandardOutput(this, &output);
		if (output.size == 0) {
			ProcessOutput_Destroy(&output);
			return "";
		}
		char *buffer = ProcessOutput_Read(&output);
		ProcessOutput_Destroy(&output);
		std::string text{buffer};
		free(buffer);
		return text;
	}
	std::string StandardError() {
		ProcessOutput output;
		Process_StandardError(this, &output);
		if (output.size == 0) {
			ProcessOutput_Destroy(&output);
			return "";
		}
		char buffer[output.size + 1];
		ProcessOutput_ReadTo(&output, buffer, output.size + 1);
		ProcessOutput_Destroy(&output);
		return std::string(buffer);
	}

	void Start() {
		Process_Start(this);
	}

	void Wait() {
		Process_Wait(this);
	}

	bool WaitFor(std::int32_t milliseconds) {
		return Process_WaitFor(this, milliseconds);
	}

	void PrintSummary() {
		Process_PrintSummary(this);
	}

	void SetStartInfo(ProcessInfo &info) {
		this->info.commandLine = info.commandLine.c_str();
		if (!info.GetEnvironment().empty()) {
			this->info.environment = info.GetEnvironment().data();
		}
		this->info.disableOutputCapture = info.disableOutputCapture;
	}
	static Process Execute(ProcessInfo &info) {
		ProcessStartInfo startInfo;
		startInfo.commandLine = info.commandLine.c_str();
		if (!info.GetEnvironment().empty()) {
			startInfo.environment = info.GetEnvironment().data();
		}
		startInfo.disableOutputCapture = info.disableOutputCapture;
		ProcessClass instance;
		Process_Execute(&instance, &startInfo);
		return instance;
	}
};

