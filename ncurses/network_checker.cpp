#include <ncurses.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

// 检查命令是否存在
bool commandExists(const std::string &command) {
	std::string cmd = "command -v " + command + " > /dev/null 2>&1";
	return system(cmd.c_str()) == 0;
}

// 检测网络硬件是否连通
bool checkNetworkHardware() {
	int result = system("ping -c 1 8.8.8.8 > /dev/null 2>&1");
	return result == 0;
}

// 检测系统网络配置是否正常
bool checkNetworkConfig() {
	int result = system("ifconfig > /dev/null 2>&1");
	return result == 0;
}

// 检测DNS配置是否正常
bool checkDNSConfig() {
	int result = system("nslookup www.baidu.com > /dev/null 2>&1");
	return result == 0;
}

// 检测DHCP配置是否正常
bool checkDHCPConfig() {
	int result = system("ps aux | grep dhclient > /dev/null 2>&1");
	return result == 0;
}

// 检测HOSTS配置是否异常
bool checkHostsConfig() {
	int result = system("grep -q '127.0.0.1' /etc/hosts");
	return result == 0;
}

// 检测是否能够正常访问网络
bool checkInternetAccess() {
	int result = system("curl -I www.baidu.com > /dev/null 2>&1");
	return result == 0;
}

void update_label(WINDOW *win, int y, int x, bool result, const char* message) {
	wattron(win, result ? COLOR_PAIR(1) : COLOR_PAIR(2));
	mvwprintw(win, y, x, "%s - %s", result ? "正常" : "异常", message);
	wattroff(win, result ? COLOR_PAIR(1) : COLOR_PAIR(2));
}

void update_label_ignore(WINDOW *win, int y, int x, const char* command) {
	wattron(win, COLOR_PAIR(3));
	mvwprintw(win, y, x, "忽略 - 缺失命令: %s", command);
	wattroff(win, COLOR_PAIR(3));
}

void show_tooltip(WINDOW *win, const char* message) {
	int y, x;
	getyx(win, y, x);
	mvwprintw(win, y + 1, x - strlen(message) / 2, "%s", message);
}

void check_network(WINDOW *win, const std::vector<std::string> &commands) {
	bool result;
	int y = 3;
	
	result = checkNetworkHardware();
	update_label(win, y++, 1, result, "网络硬件连通性");
	
	result = checkNetworkConfig();
	update_label(win, y++, 1, result, "系统网络配置");
	
	for (const auto& command : commands) {
		if (command == "nslookup") {
			if (commandExists("nslookup")) {
				result = checkDNSConfig();
				update_label(win, y++, 1, result, "DNS配置");
			} else {
				update_label_ignore(win, y++, 1, "nslookup");
			}
		} else if (command == "dhclient") {
			if (commandExists("dhclient")) {
				result = checkDHCPConfig();
				update_label(win, y++, 1, result, "DHCP配置");
			} else {
				update_label_ignore(win, y++, 1, "dhclient");
			}
		}
	}
	
	result = checkHostsConfig();
	update_label(win, y++, 1, result, "HOSTS配置");
	
	result = checkInternetAccess();
	update_label(win, y++, 1, result, "网络访问");
	
	wrefresh(win);
}

int main() {
	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	
	WINDOW *win = newwin(15, 40, 5, 5);
	box(win, 0, 0);
	mvwprintw(win, 1, 1, "网络检测工具");
	mvwprintw(win, 2, 1, "按任意键开始检测...");
	wrefresh(win);
	
	getch();
	mvwprintw(win, 2, 1, "正在检测，请稍候...");
	wrefresh(win);
	
	std::vector<std::string> commands = {"nslookup", "dhclient"};
	check_network(win, commands);
	
	mvwprintw(win, 14, 1, "按任意键退出...");
	wrefresh(win);
	getch();
	
	endwin();
	return 0;
}

