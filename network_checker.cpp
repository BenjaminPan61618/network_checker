#include <gtk/gtk.h>
#include <cstdlib>
#include <string>
#include <iostream>

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

void update_label(GtkWidget *label, bool result, const char* message) {
    std::string markup = "<span foreground='";
    markup += result ? "green'>正常" : "red'>异常";
    markup += "</span>";
    gtk_label_set_markup(GTK_LABEL(label), (markup + message).c_str());
}

void update_label_ignore(GtkWidget *label, const char* command) {
    std::string markup = "<span foreground='#FFA500'>忽略</span>"; // 使用橙色
    gtk_label_set_markup(GTK_LABEL(label), (markup + " - 缺失命令: " + command).c_str());
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **labels = (GtkWidget **)data;
    bool result;

    result = checkNetworkHardware();
    update_label(labels[0], result, " - 网络硬件连通性");
    if (!result) return;

    result = checkNetworkConfig();
    update_label(labels[1], result, " - 系统网络配置");

    if (commandExists("nslookup")) {
        result = checkDNSConfig();
        update_label(labels[2], result, " - DNS配置");
    } else {
        update_label_ignore(labels[2], "nslookup");
    }

    if (commandExists("dhclient")) {
        result = checkDHCPConfig();
        update_label(labels[3], result, " - DHCP配置");
    } else {
        update_label_ignore(labels[3], "dhclient");
    }

    result = checkHostsConfig();
    update_label(labels[4], result, " - HOSTS配置");

    result = checkInternetAccess();
    update_label(labels[5], result, " - 网络访问");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *labels[6];

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "断网急救助手");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_widget_set_size_request(window, 300, 200);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    const char *label_texts[6] = {
        "网络硬件连通性",
        "系统网络配置",
        "DNS配置",
        "DHCP配置",
        "HOSTS配置",
        "网络访问"
    };

    for (int i = 0; i < 6; ++i) {
        labels[i] = gtk_label_new(label_texts[i]);
        gtk_box_pack_start(GTK_BOX(box), labels[i], TRUE, TRUE, 5);
    }

    button = gtk_button_new_with_label("开始检测");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 5);

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), labels);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
