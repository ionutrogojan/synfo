#include <stdio.h>
#include <stdlib.h>

#define ACTIVE_COLOUR 92
#define CLEAR_SCREEN printf("\33[2J\33[3J\33[H");  // Clear entire screen
#define NEW_LINE printf("\n");

void printTitle(int colour, char *text);
void printColours(void);

int userAndHost(void);
int osName(void);
int kernel(void);
int upTime(void);
int shell(void);
int resolution(void);
int desktop(void);
// int terminalName(void);
int cpuInfo(void);
int gpuInfo(void);
int memoryAndSwap(void);

int main(void) {
    CLEAR_SCREEN
	// printColours();
    if (userAndHost() != 0 ||
             osName() != 0 ||
             kernel() != 0 ||
             upTime() != 0 ||
              shell() != 0 ||
         resolution() != 0 ||
            desktop() != 0 ||
       //terminalName() == 1 ||
            cpuInfo() != 0 ||
            gpuInfo() != 0 ||
      memoryAndSwap() != 0)
        return 1;
	NEW_LINE
	printColours();
    NEW_LINE
	return 0;
}

void printTitle(int colour, char *text) {
	printf("\33[1;%dm%s\33[0m: ", colour, text);
	fflush(stdout);
}

int userAndHost(void) {
    // User&Host user@host
    printf("\33[1;%dmUser\33[0m&\33[1;%dmHost\33[0m: ", ACTIVE_COLOUR, ACTIVE_COLOUR);
    fflush(stdout);
    return system("printf \"%s@%s\n\" \"$(whoami)\" \"$(hostname)\"");
}

int osName(void) {
    // OS: Fedora Linux 39 (Workstation Edition) x86_64    ! Missing x86_64
	printTitle(ACTIVE_COLOUR, "OS");
    return system("grep PRETTY_NAME /etc/os-release | cut -d'=' -f2 | tr -d '\"'");
}

int kernel(void) {
    // Kernel: 6.7.6-200.fc39.x86_64 
	printTitle(ACTIVE_COLOUR, "Kernel");
    return system("grep -oP 'Linux version \\K\\S+' /proc/version");
}

int upTime(void) {
    // Uptime: 6 hours, 9 mins 
	printTitle(ACTIVE_COLOUR, "Uptime");
    return system("uptime -p | sed 's/up //'");
}

/*
int packageList(void) {

    printf("Packages: %s\n", NULL);
    return 0;
}
*/

int shell(void) {
    // Shell: bash 5.2.26
	printTitle(ACTIVE_COLOUR, "Shell");
    return system("printf \"%s \" \"$(basename \"$SHELL\")\" && $SHELL --version | grep -oP '\\d+(\\.\\d+)+'");
}

int resolution(void) {
    // Resolution: 1920x1080
	printTitle(ACTIVE_COLOUR, "Resolution");
    return system("xdpyinfo | grep dimensions | cut -d' ' -f7");
}

int desktop(void) {
    // No version number or Window Manager name
	printTitle(ACTIVE_COLOUR, "Desktop");
    return system("printf \"$XDG_CURRENT_DESKTOP\n\"");
}

/*
int terminalName(void) {
    return 0;
}
*/

int cpuInfo(void) {
    // CPU: AMD Ryzen 5 5600X (12) @ 3.700GHz
	printTitle(ACTIVE_COLOUR, "CPU");
    if (system("printf \"%s \" \"$(grep 'model name' /proc/cpuinfo | head -n1 | cut -d':' -f2 | sed 's/^[[:space:]]*//' | tr -s ' ')\"") != 0) {
        perror("cpuinfo model");
        return 1;
    }
    if (system("printf \"(%s) \" \"$(grep 'siblings' /proc/cpuinfo | head -n1 | cut -d':' -f2 | sed 's/^[[:space:]]*//')\"") != 0) {
        perror("cpuinfo threads");
        return 1;
    }
    if (system("grep 'cpu MHz' /proc/cpuinfo | cut -d':' -f2 | awk '{ sum += $1; count++ } END { printf \"@ %.3fGHz\\n\", (sum/count)/1000 }'") != 0) {
        perror("cpuinfo clockSpeed");
        return 1;
    }
    return 0;
}

int gpuInfo(void) {
    // GPU: AMD ATI Radeon RX 6700/6700 XT/6750 XT / 6800M/6850M XT 
    printTitle(ACTIVE_COLOUR, "GPU");
	return system("lspci | grep -i vga | cut -d ':' -f3 | cut -d'(' -f1 | awk -F'[][]' '{print $2 \" \" $4}'");
}

int memoryAndSwap(void) {
    // Memory: 3176MiB / 31999MiB
    printTitle(ACTIVE_COLOUR, "Memory");
	if (system("awk '/MemAvailable/ {printf \"%dMiB / \", $2/1024}' /proc/meminfo") != 0) {
        perror("memory free");
        return 1;
    }
    if (system("awk '/MemTotal/ {printf \"%dMiB\\n\", $2/1024}' /proc/meminfo") != 0) {
        perror("memory total");
        return 1;
    }
    printTitle(ACTIVE_COLOUR, "Swap");
    if (system("awk '/SwapFree/ {printf \"%dMiB / \", $2/1024}' /proc/meminfo") != 0) {
        perror("swap free");
        return 1;
    }
    if (system("awk '/SwapTotal/ {printf \"%dMiB\\n\", $2/1024}' /proc/meminfo") != 0) {
        perror("swap total");
        return 1;
    }
    return 0;
}

void printColours(void) {
	printf("\33[40m   \33[41m   \33[42m   \33[43m   \33[44m   \33[45m   \33[46m   \33[47m   \33[0m\n\33[100m   \33[101m   \33[102m   \33[103m   \33[104m   \33[105m   \33[106m   \33[107m   \33[0m\n");
}
/*

Packages: 2786 (rpm), 31 (flatpak) 
WM: Mutter 
WM Theme: Adwaita 
Theme: Adwaita-dark [GTK2/3] 
Icons: Adwaita [GTK2/3] 
Terminal: BlackBox 

*/