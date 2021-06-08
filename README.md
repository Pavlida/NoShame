# User Guide to No Shame
In order to launch the project you need to download the code and choose the outside CMakeLists.txt in Qt to construct the project. You should have Qt Widgets 5.

After building and running the project you need to load a menu by pressing "Edit Menu" in the menu bar, then "Load new menu". Your menu needs to be separated by ';' and have at least three columns: Item, Category, Calories.

After that you can sort, the menu by pressing on the headers, however if you press on the header "Category" a list of categories will appear an you can check whick categories you want to be visible. You can also search an utem by it's name, using the search bar and the button search. Alternatively, you can press "Enter". If the bar is empty and you activate the search the whole menu is shown.

You can interact with the table. By right-clicking a context menu opens up, from where you can add an item to "Your Meal", remove an item from the menu, and view nutrients of the item in a separate window.

You can also change the data of an item, despite it's category and namem, however in order for them to take placed outside the table you need to click "View item details" to see the updated nutrients or delete it from the meal and add it there again if you want your data for the meal to refresh.

By pressing the nutrients button near the total calories you get the nutrient indormation for the entire meal.

Changing the amount of an item in the meal or adding new items to it immediately changes the nutrient values of the meal so you don't have to reopen the window. If you try to add an item that already is in the meal, it will just increase it's quantity by 1 if possible.

The widget on the bottom shows the amount of time you need to walk, run or workout in order to burn all the calories from the meal you can switch the modes by pressing the tabs
