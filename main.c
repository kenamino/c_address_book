#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义联系人结构体
typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

// 函数声明
void display_menu();
void add_contact(Contact contacts[], int *count);
void view_contacts(const Contact contacts[], int count);
void search_contact(const Contact contacts[], int count);
void delete_contact(Contact contacts[], int *count);
void save_contacts(const Contact contacts[], int count);
void load_contacts(Contact contacts[], int *count);

// 全局常量
#define MAX_CONTACTS 100
#define DATA_FILE "contacts.dat"

int main() {
    Contact contacts[MAX_CONTACTS];
    int count = 0;
    int choice;

    // 尝试加载现有数据
    load_contacts(contacts, &count);

    do {
        display_menu();
        printf("请输入您的选择: ");
        // 使用 if (scanf("%d", &choice) != 1) 来检查输入是否成功
        if (scanf("%d", &choice) != 1) {
            choice = 0; // 设置一个无效值
        }
        // 清除输入缓冲区，防止后续gets或fgets读取到换行符
        while (getchar() != '\n'); 

        switch (choice) {
            case 1:
                add_contact(contacts, &count);
                break;
            case 2:
                view_contacts(contacts, count);
                break;
            case 3:
                search_contact(contacts, count);
                break;
            case 4:
                delete_contact(contacts, &count);
                break;
            case 5:
                save_contacts(contacts, count);
                printf("数据已保存。程序退出。\n");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
        }
        printf("\n");
    } while (choice != 5);

    return 0;
}

// ------------------- 功能实现 -------------------

/**
 * @brief 显示主菜单
 */
void display_menu() {
    printf("========== 命令行通讯录管理系统 ==========\n");
    printf("1. 添加联系人\n");
    printf("2. 查看所有联系人\n");
    printf("3. 搜索联系人\n");
    printf("4. 删除联系人\n");
    printf("5. 保存并退出\n");
    printf("==========================================\n");
}

/**
 * @brief 添加新的联系人
 * @param contacts 联系人数组
 * @param count 当前联系人数量指针
 */
void add_contact(Contact contacts[], int *count) {
    if (*count >= MAX_CONTACTS) {
        printf("通讯录已满，无法添加新的联系人。\n");
        return;
    }

    printf("--- 添加联系人 ---\n");
    printf("姓名: ");
    // 使用fgets读取一行，比scanf更安全
    if (fgets(contacts[*count].name, sizeof(contacts[*count].name), stdin) == NULL) return;
    // 移除末尾的换行符
    contacts[*count].name[strcspn(contacts[*count].name, "\n")] = 0;

    printf("电话: ");
    if (fgets(contacts[*count].phone, sizeof(contacts[*count].phone), stdin) == NULL) return;
    contacts[*count].phone[strcspn(contacts[*count].phone, "\n")] = 0;

    printf("邮箱: ");
    if (fgets(contacts[*count].email, sizeof(contacts[*count].email), stdin) == NULL) return;
    contacts[*count].email[strcspn(contacts[*count].email, "\n")] = 0;

    (*count)++;
    printf("联系人添加成功！\n");
}

/**
 * @brief 查看所有联系人
 * @param contacts 联系人数组
 * @param count 当前联系人数量
 */
void view_contacts(const Contact contacts[], int count) {
    if (count == 0) {
        printf("通讯录为空。\n");
        return;
    }

    printf("--- 所有联系人 (%d) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("ID: %d\n", i + 1);
        printf("  姓名: %s\n", contacts[i].name);
        printf("  电话: %s\n", contacts[i].phone);
        printf("  邮箱: %s\n", contacts[i].email);
        printf("------------------------\n");
    }
}

/**
 * @brief 搜索联系人
 * @param contacts 联系人数组
 * @param count 当前联系人数量
 */
void search_contact(const Contact contacts[], int count) {
    if (count == 0) {
        printf("通讯录为空，无法搜索。\n");
        return;
    }

    char search_term[50];
    printf("--- 搜索联系人 ---\n");
    printf("请输入要搜索的姓名或电话: ");
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) return;
    search_term[strcspn(search_term, "\n")] = 0;

    int found_count = 0;
    for (int i = 0; i < count; i++) {
        // 使用strstr进行模糊匹配
        if (strstr(contacts[i].name, search_term) != NULL || strstr(contacts[i].phone, search_term) != NULL) {
            printf("--- 找到联系人 (ID: %d) ---\n", i + 1);
            printf("  姓名: %s\n", contacts[i].name);
            printf("  电话: %s\n", contacts[i].phone);
            printf("  邮箱: %s\n", contacts[i].email);
            printf("---------------------------\n");
            found_count++;
        }
    }

    if (found_count == 0) {
        printf("未找到匹配的联系人。\n");
    } else {
        printf("共找到 %d 个匹配的联系人。\n", found_count);
    }
}

/**
 * @brief 删除联系人
 * @param contacts 联系人数组
 * @param count 当前联系人数量指针
 */
void delete_contact(Contact contacts[], int *count) {
    if (*count == 0) {
        printf("通讯录为空，无法删除。\n");
        return;
    }

    int id_to_delete;
    printf("--- 删除联系人 ---\n");
    printf("请输入要删除的联系人ID (1 到 %d): ", *count);
    if (scanf("%d", &id_to_delete) != 1) {
        printf("输入无效。\n");
        while (getchar() != '\n'); // 清除输入缓冲区
        return;
    }
    while (getchar() != '\n'); // 清除输入缓冲区

    if (id_to_delete < 1 || id_to_delete > *count) {
        printf("ID无效。\n");
        return;
    }

    // ID是1-based，数组索引是0-based
    int index_to_delete = id_to_delete - 1;

    // 移动后续元素覆盖被删除的元素
    for (int i = index_to_delete; i < *count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    (*count)--;
    printf("联系人 (ID: %d) 删除成功！\n", id_to_delete);
}

/**
 * @brief 将联系人数据保存到文件
 * @param contacts 联系人数组
 * @param count 当前联系人数量
 */
void save_contacts(const Contact contacts[], int count) {
    FILE *file = fopen(DATA_FILE, "wb"); // 以二进制写入模式打开
    if (file == NULL) {
        perror("无法打开文件进行保存");
        return;
    }

    // 写入联系人数量
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        perror("写入联系人数量失败");
        fclose(file);
        return;
    }
    // 写入所有联系人数据
    if (fwrite(contacts, sizeof(Contact), count, file) != count) {
        perror("写入联系人数据失败");
        fclose(file);
        return;
    }

    fclose(file);
}

/**
 * @brief 从文件加载联系人数据
 * @param contacts 联系人数组
 * @param count 当前联系人数量指针
 */
void load_contacts(Contact contacts[], int *count) {
    FILE *file = fopen(DATA_FILE, "rb"); // 以二进制读取模式打开
    if (file == NULL) {
        // 文件不存在或无法打开，这是正常情况，不报错
        *count = 0;
        return;
    }

    // 读取联系人数量
    if (fread(count, sizeof(int), 1, file) != 1) {
        // 读取失败，可能文件为空或损坏
        *count = 0;
        fclose(file);
        return;
    }

    // 读取所有联系人数据
    if (fread(contacts, sizeof(Contact), *count, file) != *count) {
        // 读取失败
        *count = 0;
        fclose(file);
        return;
    }

    fclose(file);
    printf("成功加载 %d 个联系人。\n", *count);
}
