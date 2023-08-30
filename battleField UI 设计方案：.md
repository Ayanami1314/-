battleField UI 设计方案：

顶部栏： 回合，谁的回合

右侧边栏： 取消选择键，攻击键（普通攻击，法师法术选择），随着选中对象不同变化，结束回合键

1. 鼠标选中

2. 渲染可移动点数（右侧边栏），渲染可移动范围地块（bfs）

3. 取消机制（右侧栏点击）

4. 点击范围内地块移动，扣除相应移动点数并渲染（进阶：移动分步，寻路）

   （进阶二：鼠标悬停不点击，移动点数产生 "-> 后果" 虚影，点击后消失）

5. 移动点数为0时无法移动
6. 攻击键选中
7. 渲染可攻击对象
8. 点击对象攻击
9. 被攻击对象消失



对象重叠，人物盖在环境上，使用图层

其他内部逻辑和命令行的没区别。

顶部栏，右侧边栏：专门编写按钮， 内部状态是否被点击（类似React useState）

bfs渲染，写一个通用函数，

 `void renderTile(int x, int y, int maxdis, <vector<Type>> unreachable, int renderCode )`  // 注意其他单位是unreachable

鼠标悬停和选中：写一个辅助函数

bool whichBlockChoose()

点击用 click和该函数一起的条件



移动寻路：带路径bfs，先搜一遍，回溯，反转vector，得到路径，使用fElapseTime, 指定速度（但感觉不需要）





  // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects

  // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg

  // -1, -2, -3, -4 blue

  // 10 plain, 11 water, 12 mountains, 13 abyss, 19 fire

  // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack





第一次更新：

一堆“按钮”（状态）蛮笨重的。

其实感觉还有种写法是设置一个输入栈，将鼠标点击分成 棋盘选中和各个按钮 这些类型

例如移动  pos1--> 移动按钮-->pos2

这个逻辑可以变成：移动按钮入栈时，等待下一个入栈的棋盘选中，之后不断pop直到按钮前栈中的第一个棋盘选中，移动



状态的一大坏处就是会丢失时间序列信息，只能在代码之中弄出对应的时间序列信息，对多对象、复杂逻辑比较无力



（项目中用状态实现move的奇技淫巧：存一个长期的lastClick，每次点击棋盘更新时将lastClick和currentClick同步，由于move键在棋盘外，所以点击move不会改变lastClick。 只有当lastClick 和 moveButton.clicked ==true时，才move到currentClick。如果先点其他地方，lastClick会和currentClick同步，无法”实质上避免“ pos1-->pos2-->buttonclick这种可能性，但是这样会自己移动到自己的位置，实际无影响）

