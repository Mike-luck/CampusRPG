#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <fstream>
using namespace std;

class Player;

// ==================== 工具函数 ====================
int inputInt()
{
    int x;
    while (!(cin >> x))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "输入错误，请输入整数：";
    }
    return x;
}

// ==================== 物品基类 ====================
class Item
{
protected:
    int id;
    string name;
    string type;
    int price;
    string description;

public:
    Item(int i, string n, string t, int p, string d)
        : id(i), name(n), type(t), price(p), description(d)
    {
    }

    virtual void use(Player& player) = 0;

    virtual ~Item()
    {
    }

    int getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }

    string getType() const
    {
        return type;
    }

    int getPrice() const
    {
        return price;
    }

    void showInfo() const
    {
        cout << "名称：" << name << endl;
        cout << "类型：" << type << endl;
        cout << "价格：" << price << endl;
        cout << "说明：" << description << endl;
    }
};

// ==================== 角色类 ====================
class Player
{
private:
    string name;
    int level;
    int hp;
    int maxHp;
    int exp;
    int gold;
    int attack;

public:
    Player(string n = "Player")
    {
        name = n;
        level = 1;
        hp = 100;
        maxHp = 100;
        exp = 0;
        gold = 100;
        attack = 20;
    }

    void showInfo() const
    {
        cout << "\n===== 角色信息 =====" << endl;
        cout << "角色名称：" << name << endl;
        cout << "等级：" << level << endl;
        cout << "生命值：" << hp << "/" << maxHp << endl;
        cout << "攻击力：" << attack << endl;
        cout << "经验值：" << exp << endl;
        cout << "金币：" << gold << endl;
    }

    void addExp(int value)
    {
        exp += value;

        while (exp >= 100)
        {
            exp -= 100;
            level++;
            maxHp += 20;
            hp = maxHp;
            attack += 5;

            cout << "角色升级！当前等级：" << level << endl;
            cout << "最大生命值增加，攻击力提升。" << endl;
        }
    }

    void addGold(int value)
    {
        gold += value;
    }

    bool spendGold(int value)
    {
        if (gold < value)
        {
            return false;
        }

        gold -= value;
        return true;
    }

    void takeDamage(int damage)
    {
        hp -= damage;

        if (hp < 0)
        {
            hp = 0;
        }
    }

    void heal(int value)
    {
        int oldHp = hp;
        hp += value;

        if (hp > maxHp)
        {
            hp = maxHp;
        }

        cout << "恢复了 " << hp - oldHp << " 点生命值。" << endl;
    }

    void increaseAttack(int value)
    {
        attack += value;
        cout << "攻击力增加了 " << value << " 点。" << endl;
    }

    bool isAlive() const
    {
        return hp > 0;
    }

    int getAttack() const
    {
        return attack;
    }

    int getGold() const
    {
        return gold;
    }

    string getName() const
    {
        return name;
    }

    int getLevel() const
    {
        return level;
    }

    int getHp() const
    {
        return hp;
    }

    int getMaxHp() const
    {
        return maxHp;
    }

    int getExp() const
    {
        return exp;
    }

    void setData(string n, int lv, int h, int mh, int e, int g, int atk)
    {
        name = n;
        level = lv;
        hp = h;
        maxHp = mh;
        exp = e;
        gold = g;
        attack = atk;
    }
};

// ==================== 物品派生类 ====================
class Food : public Item
{
private:
    int recoverHp;

public:
    Food(int i, string n, int p, string d, int value)
        : Item(i, n, "食物", p, d)
    {
        recoverHp = value;
    }

    void use(Player& player) override
    {
        cout << "使用食物：" << name << endl;
        player.heal(recoverHp);
    }
};

class Medicine : public Item
{
private:
    int recoverHp;

public:
    Medicine(int i, string n, int p, string d, int value)
        : Item(i, n, "药品", p, d)
    {
        recoverHp = value;
    }

    void use(Player& player) override
    {
        cout << "使用药品：" << name << endl;
        player.heal(recoverHp);
    }
};

class Equipment : public Item
{
private:
    int attackBonus;

public:
    Equipment(int i, string n, int p, string d, int value)
        : Item(i, n, "装备", p, d)
    {
        attackBonus = value;
    }

    void use(Player& player) override
    {
        cout << "装备物品：" << name << endl;
        player.increaseAttack(attackBonus);
    }
};

// ==================== 物品工厂函数 ====================
shared_ptr<Item> createItemById(int id)
{
    if (id == 1)
    {
        return make_shared<Food>(1, "面包", 10, "恢复20点生命值", 20);
    }
    else if (id == 2)
    {
        return make_shared<Medicine>(2, "治疗药水", 30, "恢复50点生命值", 50);
    }
    else if (id == 3)
    {
        return make_shared<Equipment>(3, "木剑", 50, "增加10点攻击力", 10);
    }
    else if (id == 4)
    {
        return make_shared<Medicine>(4, "高级药水", 60, "恢复100点生命值", 100);
    }
    else if (id == 5)
    {
        return make_shared<Equipment>(5, "铁剑", 120, "增加25点攻击力", 25);
    }

    return nullptr;
}

// ==================== 背包类 ====================
class Backpack
{
private:
    vector<shared_ptr<Item>> items;

public:
    void addItem(shared_ptr<Item> item)
    {
        if (item == nullptr)
        {
            return;
        }

        items.push_back(item);
        cout << "获得物品：" << item->getName() << endl;
    }

    void showItems() const
    {
        cout << "\n===== 背包物品 =====" << endl;

        if (items.empty())
        {
            cout << "背包为空。" << endl;
            return;
        }

        for (int i = 0; i < static_cast<int>(items.size()); i++)
        {
            cout << "\n物品编号：" << i + 1 << endl;
            items[i]->showInfo();
        }
    }

    void useItem(int number, Player& player)
    {
        int index = number - 1;

        if (index < 0 || index >= static_cast<int>(items.size()))
        {
            cout << "物品编号错误。" << endl;
            return;
        }

        items[index]->use(player);
        items.erase(items.begin() + index);
        cout << "该物品已从背包中移除。" << endl;
    }

    void deleteItem(int number)
    {
        int index = number - 1;

        if (index < 0 || index >= static_cast<int>(items.size()))
        {
            cout << "物品编号错误。" << endl;
            return;
        }

        cout << "删除物品：" << items[index]->getName() << endl;
        items.erase(items.begin() + index);
    }

    shared_ptr<Item> getItem(int number) const
    {
        int index = number - 1;

        if (index < 0 || index >= static_cast<int>(items.size()))
        {
            return nullptr;
        }

        return items[index];
    }

    bool removeItem(int number)
    {
        int index = number - 1;

        if (index < 0 || index >= static_cast<int>(items.size()))
        {
            return false;
        }

        items.erase(items.begin() + index);
        return true;
    }

    bool isEmpty() const
    {
        return items.empty();
    }

    vector<int> getItemIds() const
    {
        vector<int> ids;

        for (auto item : items)
        {
            ids.push_back(item->getId());
        }

        return ids;
    }

    void clear()
    {
        items.clear();
    }
};

// ==================== 商店类 ====================
class Shop
{
private:
    unordered_map<int, shared_ptr<Item>> products;

public:
    void addProduct(int number, shared_ptr<Item> item)
    {
        products[number] = item;
    }

    void showProducts() const
    {
        cout << "\n===== 商店商品 =====" << endl;

        for (auto pair : products)
        {
            cout << "\n商品编号：" << pair.first << endl;
            pair.second->showInfo();
        }
    }

    void buyItem(int number, Player& player, Backpack& backpack)
    {
        if (products.find(number) == products.end())
        {
            cout << "商品编号错误。" << endl;
            return;
        }

        shared_ptr<Item> item = products[number];

        if (!player.spendGold(item->getPrice()))
        {
            cout << "金币不足，无法购买。" << endl;
            return;
        }

        backpack.addItem(createItemById(item->getId()));

        cout << "购买成功：" << item->getName() << endl;
        cout << "剩余金币：" << player.getGold() << endl;
    }

    void sellItem(int number, Player& player, Backpack& backpack)
    {
        shared_ptr<Item> item = backpack.getItem(number);

        if (item == nullptr)
        {
            cout << "物品编号错误。" << endl;
            return;
        }

        int sellPrice = item->getPrice() / 2;

        player.addGold(sellPrice);
        backpack.removeItem(number);

        cout << "出售成功：" << item->getName() << endl;
        cout << "获得金币：" << sellPrice << endl;
        cout << "当前金币：" << player.getGold() << endl;
    }
};

// ==================== 任务类 ====================
class Task
{
private:
    string name;
    string description;
    int target;
    int progress;
    int rewardExp;
    int rewardGold;

    bool accepted;
    bool completed;
    bool rewarded;

public:
    Task(string n = "", string d = "", int t = 1, int expReward = 0, int goldReward = 0)
        : name(n), description(d), target(t), progress(0),
        rewardExp(expReward), rewardGold(goldReward),
        accepted(false), completed(false), rewarded(false)
    {
    }

    void showInfo() const
    {
        cout << "\n任务名称：" << name << endl;
        cout << "任务描述：" << description << endl;
        cout << "完成条件：" << progress << "/" << target << endl;
        cout << "奖励：经验 " << rewardExp << "，金币 " << rewardGold << endl;

        cout << "状态：";

        if (rewarded)
        {
            cout << "奖励已领取" << endl;
        }
        else if (completed)
        {
            cout << "已完成，等待领取奖励" << endl;
        }
        else if (accepted)
        {
            cout << "进行中" << endl;
        }
        else
        {
            cout << "未接受" << endl;
        }
    }

    void accept()
    {
        if (accepted)
        {
            cout << "该任务已经接受。" << endl;
            return;
        }

        accepted = true;
        cout << "成功接受任务：" << name << endl;
    }

    void addProgress(int value)
    {
        if (!accepted || completed)
        {
            return;
        }

        progress += value;

        if (progress >= target)
        {
            progress = target;
            completed = true;
            cout << "任务完成：" << name << endl;
        }
        else
        {
            cout << "任务进度更新：" << progress << "/" << target << endl;
        }
    }

    void claimReward(Player& player)
    {
        if (!completed)
        {
            cout << "任务尚未完成，不能领取奖励。" << endl;
            return;
        }

        if (rewarded)
        {
            cout << "任务奖励已经领取。" << endl;
            return;
        }

        player.addExp(rewardExp);
        player.addGold(rewardGold);
        rewarded = true;

        cout << "领取任务奖励成功！" << endl;
        cout << "获得经验：" << rewardExp << endl;
        cout << "获得金币：" << rewardGold << endl;
    }

    bool isAccepted() const
    {
        return accepted;
    }

    bool isCompleted() const
    {
        return completed;
    }

    bool isRewarded() const
    {
        return rewarded;
    }

    int getProgress() const
    {
        return progress;
    }

    void setState(bool a, bool c, bool r, int p)
    {
        accepted = a;
        completed = c;
        rewarded = r;
        progress = p;
    }
};

// ==================== 敌人类 ====================
class Enemy
{
private:
    string name;
    int hp;
    int attack;
    int expReward;
    int goldReward;
    int difficulty;

public:
    Enemy(string n = "", int h = 1, int atk = 1, int exp = 0, int gold = 0, int diff = 1)
    {
        name = n;
        hp = h;
        attack = atk;
        expReward = exp;
        goldReward = gold;
        difficulty = diff;
    }

    void showInfo() const
    {
        cout << "敌人：" << name
            << " | 生命值：" << hp
            << " | 攻击力：" << attack
            << " | 经验奖励：" << expReward
            << " | 金币奖励：" << goldReward
            << endl;
    }

    string getName() const
    {
        return name;
    }

    int getHp() const
    {
        return hp;
    }

    int getAttack() const
    {
        return attack;
    }

    int getExpReward() const
    {
        return expReward;
    }

    int getGoldReward() const
    {
        return goldReward;
    }

    int getDifficulty() const
    {
        return difficulty;
    }

    void takeDamage(int damage)
    {
        hp -= damage;

        if (hp < 0)
        {
            hp = 0;
        }
    }

    bool isAlive() const
    {
        return hp > 0;
    }
};

// ==================== 游戏管理类 ====================
class Game
{
private:
    Player player;
    Backpack backpack;
    Shop shop;
    vector<Task> tasks;
    vector<Enemy> enemies;

public:
    void init()
    {
        string name;

        cout << "===== Campus RPG =====" << endl;
        cout << "请输入角色名称：";
        cin >> name;

        player = Player(name);

        shop.addProduct(1, createItemById(1));
        shop.addProduct(2, createItemById(2));
        shop.addProduct(3, createItemById(3));
        shop.addProduct(4, createItemById(4));
        shop.addProduct(5, createItemById(5));

        tasks.push_back(Task("校园除害", "击败2只校园怪物", 2, 80, 50));
        tasks.push_back(Task("新手训练", "完成1次战斗", 1, 40, 20));

        enemies.push_back(Enemy("操场史莱姆", 60, 10, 40, 20, 1));
        enemies.push_back(Enemy("图书馆幽灵", 90, 15, 60, 30, 2));
        enemies.push_back(Enemy("实验楼机械怪", 130, 22, 100, 60, 3));

        sort(enemies.begin(), enemies.end(), [](const Enemy& a, const Enemy& b) {
            return a.getDifficulty() < b.getDifficulty();
            });
    }

    void showMenu() const
    {
        cout << "\n========== 主菜单 ==========" << endl;
        cout << "1. 查看角色信息" << endl;
        cout << "2. 查看背包" << endl;
        cout << "3. 查看商店" << endl;
        cout << "4. 购买商品" << endl;
        cout << "5. 使用物品" << endl;
        cout << "6. 出售物品" << endl;
        cout << "7. 任务系统" << endl;
        cout << "8. 战斗系统" << endl;
        cout << "9. 保存游戏" << endl;
        cout << "10. 读取存档" << endl;
        cout << "0. 退出游戏" << endl;
        cout << "请选择：";
    }

    void taskMenu()
    {
        cout << "\n===== 任务系统 =====" << endl;

        for (int i = 0; i < static_cast<int>(tasks.size()); i++)
        {
            cout << "\n任务编号：" << i + 1 << endl;
            tasks[i].showInfo();
        }

        cout << "\n1. 接受任务" << endl;
        cout << "2. 领取奖励" << endl;
        cout << "0. 返回主菜单" << endl;
        cout << "请选择：";

        int choice = inputInt();

        if (choice == 1)
        {
            cout << "请输入要接受的任务编号：";
            int number = inputInt();

            if (number < 1 || number > static_cast<int>(tasks.size()))
            {
                cout << "任务编号错误。" << endl;
                return;
            }

            tasks[number - 1].accept();
        }
        else if (choice == 2)
        {
            cout << "请输入要领取奖励的任务编号：";
            int number = inputInt();

            if (number < 1 || number > static_cast<int>(tasks.size()))
            {
                cout << "任务编号错误。" << endl;
                return;
            }

            tasks[number - 1].claimReward(player);
        }
    }

    void battle()
    {
        cout << "\n===== 战斗系统 =====" << endl;

        for (int i = 0; i < static_cast<int>(enemies.size()); i++)
        {
            cout << "敌人编号：" << i + 1 << " ";
            enemies[i].showInfo();
        }

        cout << "请选择敌人编号：";
        int number = inputInt();

        if (number < 1 || number > static_cast<int>(enemies.size()))
        {
            cout << "敌人编号错误。" << endl;
            return;
        }

        Enemy enemy = enemies[number - 1];

        cout << "\n战斗开始！你遇到了：" << enemy.getName() << endl;

        while (player.isAlive() && enemy.isAlive())
        {
            cout << "\n你攻击敌人，造成 " << player.getAttack() << " 点伤害。" << endl;
            enemy.takeDamage(player.getAttack());

            if (!enemy.isAlive())
            {
                break;
            }

            cout << enemy.getName() << " 反击，造成 " << enemy.getAttack() << " 点伤害。" << endl;
            player.takeDamage(enemy.getAttack());

            cout << "你的剩余生命值：" << player.getHp() << "/" << player.getMaxHp() << endl;
            cout << "敌人剩余生命值：" << enemy.getHp() << endl;
        }

        if (player.isAlive())
        {
            cout << "\n战斗胜利！" << endl;
            cout << "获得经验：" << enemy.getExpReward() << endl;
            cout << "获得金币：" << enemy.getGoldReward() << endl;

            player.addExp(enemy.getExpReward());
            player.addGold(enemy.getGoldReward());

            for (int i = 0; i < static_cast<int>(tasks.size()); i++)
            {
                tasks[i].addProgress(1);
            }
        }
        else
        {
            cout << "\n战斗失败，生命值为0。" << endl;
            cout << "系统恢复角色部分生命值，避免游戏无法继续。" << endl;
            player.heal(50);
        }
    }

    void saveGame()
    {
        ofstream fout("save.txt");

        if (!fout)
        {
            cout << "存档失败。" << endl;
            return;
        }

        fout << player.getName() << endl;
        fout << player.getLevel() << " "
            << player.getHp() << " "
            << player.getMaxHp() << " "
            << player.getExp() << " "
            << player.getGold() << " "
            << player.getAttack() << endl;

        vector<int> ids = backpack.getItemIds();
        fout << ids.size() << endl;

        for (int id : ids)
        {
            fout << id << " ";
        }

        fout << endl;

        fout << tasks.size() << endl;

        for (Task& task : tasks)
        {
            fout << task.isAccepted() << " "
                << task.isCompleted() << " "
                << task.isRewarded() << " "
                << task.getProgress() << endl;
        }

        fout.close();

        cout << "游戏保存成功。" << endl;
    }

    void loadGame()
    {
        ifstream fin("save.txt");

        if (!fin)
        {
            cout << "没有找到存档文件。" << endl;
            return;
        }

        string name;
        int level, hp, maxHp, exp, gold, attack;

        fin >> name;
        fin >> level >> hp >> maxHp >> exp >> gold >> attack;

        player.setData(name, level, hp, maxHp, exp, gold, attack);

        backpack.clear();

        int itemCount;
        fin >> itemCount;

        for (int i = 0; i < itemCount; i++)
        {
            int id;
            fin >> id;
            backpack.addItem(createItemById(id));
        }

        int taskCount;
        fin >> taskCount;

        for (int i = 0; i < taskCount && i < static_cast<int>(tasks.size()); i++)
        {
            bool accepted, completed, rewarded;
            int progress;

            fin >> accepted >> completed >> rewarded >> progress;
            tasks[i].setState(accepted, completed, rewarded, progress);
        }

        fin.close();

        cout << "读取存档成功。" << endl;
    }

    void run()
    {
        int choice;

        do
        {
            showMenu();
            choice = inputInt();

            switch (choice)
            {
            case 1:
                player.showInfo();
                break;

            case 2:
                backpack.showItems();
                break;

            case 3:
                shop.showProducts();
                break;

            case 4:
            {
                shop.showProducts();
                cout << "\n请输入要购买的商品编号：";
                int number = inputInt();
                shop.buyItem(number, player, backpack);
                break;
            }

            case 5:
            {
                if (backpack.isEmpty())
                {
                    cout << "背包为空，无法使用物品。" << endl;
                    break;
                }

                backpack.showItems();
                cout << "\n请输入要使用的物品编号：";
                int number = inputInt();
                backpack.useItem(number, player);
                break;
            }

            case 6:
            {
                if (backpack.isEmpty())
                {
                    cout << "背包为空，无法出售物品。" << endl;
                    break;
                }

                backpack.showItems();
                cout << "\n请输入要出售的物品编号：";
                int number = inputInt();
                shop.sellItem(number, player, backpack);
                break;
            }

            case 7:
                taskMenu();
                break;

            case 8:
                battle();
                break;

            case 9:
                saveGame();
                break;

            case 10:
                loadGame();
                break;

            case 0:
                cout << "游戏结束。" << endl;
                break;

            default:
                cout << "输入错误，请重新选择。" << endl;
            }

        } while (choice != 0);
    }
};

// ==================== 主函数 ====================
int main()
{
    Game game;
    game.init();
    game.run();

    return 0;
}
