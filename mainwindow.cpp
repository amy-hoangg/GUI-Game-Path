/* Path game
 *
 * Description:
 * Path is a wooden puzzle game.The game requires the player to move green
 * and red buttons to the opposite positions than their starting positions.
 * The buttons cannot be moved over each other, but only along an empty path.
 * The minimum number of moves to reach the final position is 31.
 * The game has legality checks implemented in the template code,
 * so the player does not need to worry about legal moves.
 *
 * Minimum requirements:
 * display the game grid
 * choose a piece to be moved and its new location
 * tell the amount of moves made
 * start the game again (reset)
 * tell the time used so far
 *
 * Extra features:
 * choose colours of the game pieces
 * add icons to the buttons
 * background changes when complete goals
 * set goals
 * pause the time
 * undo the moves
 *
 * Program author
 * Name: My Hoang
 * Student number: 151395127
 * UserID: vcmyho
 * E-Mail: my.hoang@tuni.fi
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game_piece_color1(Qt::red)
    , game_piece_color2(Qt::darkGreen)
{
    ui->setupUi(this);

    main_layout = new QVBoxLayout(this);
    QWidget *central_widget = new QWidget(this);
    central_widget->setLayout(main_layout);
    setCentralWidget(central_widget);

    QLabel *game_name = new QLabel("Path Game");
    game_name->setStyleSheet("font-size: 40px; font-weight: bold; color: red;");

    init_boards();

    quit_button = new QPushButton("Quit");
    connect(quit_button, &QPushButton::clicked, this, &MainWindow::handle_quit_button);
    quit_button->setFixedSize(120, 50);
    quit_button->setStyleSheet("background-color: white; border-radius: 10px;");

    start_button = new QPushButton("Start");
    connect(start_button, &QPushButton::clicked, this, &MainWindow::handle_start_button);
    start_button->setFixedSize(120, 50);
    start_button->setStyleSheet("background-color: white; border-radius: 10px;");


    // Create the timer and connect its timeout signal to a slot
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer_label);


    // Create a QLabel to display the timer value
    time_name_label->setStyleSheet("font-size: 15px; font-weight: bold; color: darkgreen;");
    timer_label = new QLabel("00:00");
    timer_label->setStyleSheet("font-size: 15px; font-weight: bold; color: darkgreen;");

    moves_name_label->setStyleSheet("font-size: 15px; font-weight: bold; color: darkgreen;");
    number_of_moves = new QLabel("0");
    number_of_moves->setStyleSheet("font-size: 15px; font-weight: bold; color: darkgreen;");

    goals_name->setStyleSheet("font-size: 15px; font-weight: bold; color: darkgreen;");
    goals_spinbox->setMinimum(31);
    set_goal_button->setFixedSize(30,30);
    connect(set_goal_button, &QPushButton::clicked, this, &MainWindow::handle_set_goal_button);

    reset_button = new QPushButton("Reset");
    connect(reset_button, &QPushButton::clicked, this, &MainWindow::handle_reset_button);
    reset_button->setFixedSize(120, 50);
    reset_button->setStyleSheet("background-color: white; border-radius: 10px;");

    elapsed_seconds = 0;

    setting_color_button = new QPushButton(" Setting Colors");
    connect(setting_color_button, &QPushButton::clicked, this, &MainWindow::handle_setting_color_button);
    setting_color_button->setFixedSize(120, 50);
    setting_color_button->setStyleSheet("background-color: white; border-radius: 10px;");


    pause_and_resume_button = new QPushButton("Pause");
    connect(pause_and_resume_button, &QPushButton::clicked, this, &MainWindow::handle_pause_and_resume_button);
    pause_and_resume_button->setFixedSize(120, 50);
    pause_and_resume_button->setStyleSheet("background-color: white; border-radius: 10px;");


    undo_button = new QPushButton("Undo");
    connect(undo_button, &QPushButton::clicked, this, &MainWindow::handle_undo_button);
    undo_button->setFixedSize(120, 50);
    undo_button->setStyleSheet("background-color: white; border-radius: 10px;");

    setting_icon_button = new QPushButton("Setting Icons");
    connect(setting_icon_button, &QPushButton::clicked, this, &MainWindow::handle_setting_icon_button);
    setting_icon_button->setFixedSize(120, 50);
    setting_icon_button->setStyleSheet("background-color: white; border-radius: 10px;");

    topmost_layout->addWidget(game_name);
    main_layout->addLayout(topmost_layout);

    top_layout->addStretch();
    top_layout->addWidget(moves_name_label);
    top_layout->addWidget(number_of_moves);
    top_layout->addStretch();
    top_layout->addWidget(time_name_label);
    top_layout->addWidget(timer_label);
    top_layout->addStretch();
    top_layout->addWidget(goals_name);
    top_layout->addWidget(goals_spinbox);
    top_layout->addWidget(set_goal_button);
    top_layout->addWidget(goal_display);
    top_layout->addStretch();
    top_layout->setSpacing(5);
    main_layout->addLayout(top_layout);

    inside_middle_layout->addWidget(quit_button);
    inside_middle_layout->addWidget(reset_button);
    inside_middle_layout->addWidget(pause_and_resume_button);
    inside_middle_layout->addWidget(undo_button);
    inside_middle_layout->addWidget(setting_icon_button);
    inside_middle_layout->addWidget(setting_color_button);
    outside_middle_layout->addLayout(inside_middle_layout);
    main_layout->addLayout(outside_middle_layout);

    topmost_layout->setAlignment(game_name, Qt::AlignHCenter);

    bottom_layout->addWidget(start_button);
    main_layout->addLayout(bottom_layout);

    top_layout->setAlignment(number_of_moves, Qt::AlignCenter);
    top_layout->setAlignment(timer_label, Qt::AlignCenter);
    bottom_layout->setAlignment(start_button, Qt::AlignBottom | Qt::AlignHCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Initialize the game board layout and add buttons to it
 * based on the game board data.
 */
void MainWindow::init_boards()
{
    // Create a new widget to serve as the container for the layout.
    QWidget *game_board_widget = new QWidget(this);
    game_board_widget->setFixedSize(500, 500); //style

    // Create a new grid layout.
    game_board_layout = new QGridLayout(game_board_widget);
    game_board_layout->setContentsMargins(10, 10, 10, 10);
    game_board_layout->setSpacing(5);

    game_board = new GameBoard(); // Allocate game_board_ on the heap

    total_moves = 0;

    game_board_data_ = game_board->get_game_board();


    for (size_t i = 0; i < game_board_data_.size(); ++i) {
        for (size_t j = 0; j < game_board_data_[i].size(); ++j) {
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(50, 50);

            // Set the UI color and border radius for the button based on the enum color.
            switch (game_board_data_[i][j])
            {
                case GREEN:
                    button->setStyleSheet(QString("background-color: %1; border-radius: 10px;").arg(game_piece_color2.name()));
                    break;
                case RED:
                    button->setStyleSheet(QString("background-color: %1; border-radius: 10px;").arg(game_piece_color1.name()));
                    break;
                case UNUSED:
                    button->hide();
                    break;
                case EMPTY:
                    button->setStyleSheet("background-color: white; border-radius: 10px;");
                    break;
            }

            // Connect the clicked() signal of the button to the handle_button_clicks() slot
            connect(button, &QPushButton::clicked, this, &MainWindow::handle_button_clicks);

            // Debug output to confirm that the connect() function call is being called
            qDebug() << "Connected button " << i << ", " << j;

            // Add the button to the layout.
            game_board_layout->addWidget(button, i, j);
            button->setEnabled(false);
        }
    }

    // Set the layout as the layout for the widget.
    game_board_widget->setLayout(game_board_layout);

    // Set the widget as the central widget of the main window.
    outside_middle_layout->addWidget(game_board_widget);
}

/**
 * @brief Handle button clicks on the game board.
 * If the game is over, display a message box indicating
 * the result and stop the timer.
 * If two buttons have been clicked, validate the move,
 * update the game board and UI, and reset the buttons.
 * If one button has been clicked, set it as the first
 * button for the move.
 */
void MainWindow::handle_button_clicks()
{
    // Check if the game is over
    if (game_board->is_game_over()) {
        if(isSetGoal_== true && goal_<total_moves)
        {
            QMessageBox::warning(this, "Game Over!", "Game Over! Fail Goal! Total Move: "+ QString::number(total_moves));
            this->setStyleSheet("background-color: lightgray;");
        }
        else if(isSetGoal_== true && goal_>=total_moves)
        {
            QMessageBox::warning(this, "Game Over!", "Game Over! Complete Goal! Total Move: "+ QString::number(total_moves));
            this->setStyleSheet("background-color: pink;");
        }
        else
        {
            QMessageBox::warning(this, "Game Over!", "Game Over! Total Move: "+ QString::number(total_moves));
        }

        timer->stop();
        for(size_t i= 0; i < game_board_data_.size(); i++)
        {
            for(size_t j = 0; j < game_board_data_[i].size(); j++)
            {
                QPushButton *button = qobject_cast<QPushButton *>(game_board_layout->itemAtPosition(i, j)->widget());
                button->setEnabled(false);
                isPauseClicked = true;
            }
        }
        return;
    }

    // Get the button that was clicked
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    //get the parent widget of the button -> layout widget
    QWidget *layoutWidget = button->parentWidget();
    //get the layout of the widget layout
    QGridLayout *layout = qobject_cast<QGridLayout*>(layoutWidget->layout());

    // Get the index of the button in the layout
    int index = layout->indexOf(button);

    // Get the row and column numbers of the index
    int row, col, rowSpan, colSpan;
    layout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);

    if (!button1_)
    {
        button1_ = button;
        p1_ = {col, row};
        qDebug() << "Button 1 set at row:" << row << "col:" << col;
        return;
    }
    // If button2_ is not set, set it to the clicked button
    if (!button2_) {
        button2_ = button;
        qDebug() << "Button 2 set at row:" << row << "col:" << col;
        p2_ = {col, row};

        Slot_type slot_p1 = game_board->which_slot(p1_);
        Slot_type slot_p2 = game_board->which_slot(p2_);
        qDebug() << "Move from:" << p1_.x << "," << p1_.y << "to:" << p2_.x << "," << p2_.y;


        // Check if the starting point is valid
        if (slot_p1 != GREEN && slot_p1 != RED) {
            qDebug() << "Invalid starting point selected. Slot type is:" << slot_p1;
            QMessageBox::warning(this, "Invalid Move", "Invalid starting point selected.");
            button1_ = nullptr;  // Reset button1_ to allow a new move to start
            button2_ = nullptr;  // Reset button2_ to allow a new move to start
            return;
        }
        // Check if the destination point is valid
        if (!game_board->is_valid_point(p2_)) {
            qDebug() << "Invalid destination point selected.";
            QMessageBox::warning(this, "Invalid Move", "Invalid destination point selected.");
            button1_ = nullptr;  // Reset button1_ to allow a new move to start
            button2_ = nullptr;  // Reset button2_ to allow a new move to start
            return;
        }
        // Check if the destination point is empty
        if (slot_p2 != EMPTY) {
            qDebug() << "Destination point is not empty. Slot type is:" << slot_p2;
            QMessageBox::warning(this, "Invalid Move", "Destination point is not empty.");
            button1_ = nullptr;  // Reset button1_ to allow a new move to start
            button2_ = nullptr;  // Reset button2_ to allow a new move to start
            return;
        }

        qDebug() << "Before move: button1 position:" << p1_.x << "," << p1_.y << "button2 position:" << p2_.x << "," << p2_.y;
        GameBoard current_board = *game_board;
        game_board_history_.push_back(current_board);


        // Make the move and update the UI
        if (game_board->move(p1_, p2_))
        {
            qDebug() << "After move: button1 position:" << p1_.x << "," << p1_.y << "button2 position:" << p2_.x << "," << p2_.y;
            update_ui();
            ++total_moves;
            number_of_moves->setText(QString::number(total_moves));

        }
        else
        {
            QMessageBox::warning(this, "Invalid Move", "Cannot move to destination point.");
        }
        // Reset button1_ and button2_ to allow a new move to start
        button1_ = nullptr;
        button2_ = nullptr;
    }
}

/**
 * @brief This function handles the action of quitting
 * the game by closing the MainWindow.
 */
void MainWindow::handle_quit_button()
{
    close();
}

/**
 * @brief Handles the start button click event and starts the game.
 * This function sets the 'isStarted' flag to true and enables all the buttons in the game board.
 * It also starts the timer with a 1 second interval and disables the start button.
 * Additionally, it outputs debugging information about the timer's status and interval.
 */
void MainWindow::handle_start_button()
{
    isStarted = true;

    qDebug() << "Start button clicked";
    qDebug() << "Timer is active: " << timer->isActive();
    qDebug() << "Timer interval: " << timer->interval();

    // Enable all the buttons in the game board
    for (int i = 0; i < game_board_layout->count(); ++i) {
        QPushButton *button = qobject_cast<QPushButton*>(game_board_layout->itemAt(i)->widget());
        if (button)
        {
            button->setEnabled(true);
        }
    }
    start_button->setEnabled(false);

    timer->start(1000); // Start the timer with a 1 second interval

    qDebug() << "Timer is active: " << timer->isActive();
}

/**
 * @brief Slot function to update the timer label
 * every second. This function increments the elapsed_seconds
 * counter and updates the timer label with the new elapsed time.
 * The timer label displays the elapsed time in minutes and seconds.
 */
void MainWindow::update_timer_label()
{
    qDebug() << "Update timer label";
    qDebug() << "Elapsed seconds:" << elapsed_seconds;

    ++elapsed_seconds;
    int minutes = elapsed_seconds / 60;
    int seconds = elapsed_seconds % 60;

    QString text = QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));
    timer_label->setText(text);

    qDebug() << "Timer label text:" << text;
}

/**
 * @brief This function is called when the reset button is clicked. It resets the game state by
 * stopping and restarting the timer, creating a new game board object, resetting the number of moves
 * and elapsed time counters, and updating the user interface.
 */
void MainWindow::handle_reset_button() {
    timer->stop();
    timer->start(1000);
    timer_label->setText("00:00");


    // Delete the old game_board object
    delete game_board;

    // Create a new game_board object and get the game board data
    game_board = new GameBoard();
    game_board_data_ = game_board->get_game_board();

    // Reset the number of moves to 0
    total_moves = 0;
    // Reset the elapsed seconds to 0
    elapsed_seconds = 0;

    update_ui();


    // Reset the timer label and number of moves label
    number_of_moves->setText("0");
}

/**
* @brief This function is called when the user clicks the "Set Colors" button. It opens a color
* dialog to allow the user to select two colors for the game pieces. The selected colors are stored
* in the `game_piece_color1` and `game_piece_color2` member variables, and the user interface is
* updated to reflect the new colors.
*/
void MainWindow::handle_setting_color_button()
{
    QColorDialog color_dialog;
    color_dialog.setOptions(QColorDialog::ColorDialogOptions(
                              QColorDialog::ShowAlphaChannel |
                              QColorDialog::DontUseNativeDialog));
    color_dialog.setCurrentColor(game_piece_color1);
    color_dialog.setWindowTitle("Select Two Colors");

    if (color_dialog.exec() == QDialog::Accepted) {
        QColor color1 = color_dialog.currentColor();
        color_dialog.setCurrentColor(game_piece_color2);
        color_dialog.setWindowTitle("Select Second Color");

        if (color_dialog.exec() == QDialog::Accepted) {
            QColor color2 = color_dialog.currentColor();

            game_piece_color1 = color1;
            game_piece_color2 = color2;
            isIconsSet = false;

            // Debug output to check the selected colors
            qDebug() << "Selected colors: " << game_piece_color1 << game_piece_color2;
            update_ui();
        }
    }
}

/**
 * @brief Update the user interface (UI) to match the current game state.
 * This function updates the color and icon of the buttons in the game board based on the current game state.
 * If the icons have not been set yet, the colors will be updated to reflect the selected colors.
 * If the icons have already been set, the buttons will display the corresponding icons.
*/
void MainWindow::update_ui()
{
    if(!isIconsSet)
    {
        qDebug() << "game_piece_color1: " << game_piece_color1;
        qDebug() << "game_piece_color2: " << game_piece_color2;

        // Loop through the buttons in the game board and update their colors.
        for (size_t i = 0; i < game_board->get_game_board().size(); ++i) {
            for (size_t j = 0; j < game_board->get_game_board()[i].size(); ++j)
            {
                // Access the button in the layout.
                QPushButton *button = qobject_cast<QPushButton*>(game_board_layout->itemAtPosition(i, j)->widget());

                qDebug() << "Updating button at position (" << i << "," << j << ")";

                // Set the UI color and border radius for the button based on the enum color.
                switch (game_board->get_game_board()[i][j])
                {
                case GREEN:
                    button->setIcon(QIcon()); // Clear the icon
                    button->setStyleSheet(QString("background-color: %1; border-radius: 10px;").arg(game_piece_color2.name()));
                    break;
                case RED:
                    button->setIcon(QIcon()); // Clear the icon
                    button->setStyleSheet(QString("background-color: %1; border-radius: 10px;").arg(game_piece_color1.name()));
                    break;
                case UNUSED:
                    button->hide();
                    break;
                case EMPTY:
                    button->setStyleSheet("background-color: white; border-radius: 10px;");
                    break;
                }
            }
        }
    }
    else
    {
        // Loop through the buttons in the game board and update their icons.
        for (size_t i = 0; i < game_board->get_game_board().size(); ++i) {
            for (size_t j = 0; j < game_board->get_game_board()[i].size(); ++j)
            {
                // Access the button in the layout.
                QPushButton *button = qobject_cast<QPushButton*>(game_board_layout->itemAtPosition(i, j)->widget());

                qDebug() << "Updating button at position (" << i << "," << j << ")";

                // Set the UI color and border radius for the button based on the enum color.
                switch (game_board->get_game_board()[i][j])
                {
                case GREEN:
                    button->setStyleSheet("background-color: white; border-radius: 10px;");
                    button->setIcon(icon1_);
                    break;
                case RED:
                    button->setStyleSheet("background-color: white; border-radius: 10px;");
                    button->setIcon(icon2_);
                    break;
                case UNUSED:
                    button->hide();
                    break;
                case EMPTY:
                    button->setStyleSheet("background-color: white; border-radius: 10px;");
                    button->setIcon(QIcon()); // Clear the icon
                    break;
                }
            }
        }
    }

}

/**
 * @brief Handles the click of the pause and resume button.
 * If the game is started, the function stops the timer and disables all the buttons on the game board
 * by setting them to be disabled. The text of the button is set to "Resume".
 * If the game is paused, the function starts the timer and enables all the buttons on the game board
 * by setting them to be enabled. The text of the button is set to "Pause".
*/
void MainWindow::handle_pause_and_resume_button()
{
    if(isStarted)
    {
        if(!isPauseClicked)
        {
            timer->stop();
            for(size_t i= 0; i < game_board_data_.size(); i++)
            {
                for(size_t j = 0; j < game_board_data_[i].size(); j++)
                {
                    QPushButton *button = qobject_cast<QPushButton *>(game_board_layout->itemAtPosition(i, j)->widget());
                    button->setEnabled(false);
                    isPauseClicked = true;
                }
            }
            pause_and_resume_button->setText("Resume");
        }
        else
        {
            timer->start();
            for(size_t i= 0; i < game_board_data_.size(); i++)
            {
                for(size_t j = 0; j < game_board_data_[i].size(); j++)
                {
                    QPushButton *button = qobject_cast<QPushButton *>(game_board_layout->itemAtPosition(i, j)->widget());
                    button->setEnabled(true);
                    isPauseClicked = false;
                }
            }
            pause_and_resume_button->setText("Pause");
        }
    }
}

/**
 * @brief Handles the "Undo" button click event by restoring the previous game board state if available.
 * If the game board history stack is not empty, this function retrieves the previous game board state,
 * pops it from the history stack, sets it as the current game board state, updates the user interface,
 * decrements the total number of moves, and updates the displayed number of moves.
*/
void MainWindow::handle_undo_button()
{
    if(!game_board_history_.empty())
    {
        GameBoard previous_game_board = game_board_history_.back();
        game_board_history_.pop_back();

        *game_board = previous_game_board;
        update_ui();
        total_moves--;
        number_of_moves->setText(QString::number(total_moves));
    }
}

/**
 * @brief This function handles the event where the "Setting Icon" button is clicked. It opens a dialog
 * box where the user can select two icons to use in the game. The selected icons are then stored
 * two QPixmap variables and used to display the corresponding images on the game board.
 */
void MainWindow::handle_setting_icon_button()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Select Icons");
    dialog.setFixedSize(400, 200);

    // Create the labels for the icons
    const int numIcons = 6;
    QLabel* icons[numIcons];
    for (int i = 0; i < numIcons; i++)
    {
        // Create the label for the icon
        icons[i] = new QLabel(&dialog);
        QPixmap pixmap(QString("/Users/amyo/vcmyho/student/12/path_gui/icons/icon%1.png").arg(i+1));
        icons[i]->setPixmap(pixmap.scaledToWidth(50));
        icons[i]->setGeometry(20 + i * 70, 20, 50, 50);

        // Create the button for selecting the icon
        QPushButton* selectButton = new QPushButton("Select", &dialog);
        selectButton->setGeometry(20 + i * 70, 80, 50, 20);

        // Connect the button's clicked() signal to a slot
        connect(selectButton, &QPushButton::clicked, [=]()
        {
            // Get the pixmap from the associated label
            QPixmap pixmap = icons[i]->pixmap();

            // Save the pixmap to the corresponding variable
            if (!icon1_.isNull() && !icon2_.isNull())
            {
                // Both icons have been selected, so shift the saved values
                icon1_ = icon2_;
                icon2_ = pixmap;
            }
            else if (icon1_.isNull())
            {
                // Save the first selected icon
                icon1_ = pixmap;
            }
            else
            {
                // Save the second selected icon
                icon2_ = pixmap;
            }
        });
    }

    // Create OK and Cancel buttons
    QPushButton* okButton = new QPushButton("OK", &dialog);
    okButton->setGeometry(150, 120, 80, 30);
    QPushButton* cancelButton = new QPushButton("Cancel", &dialog);
    cancelButton->setGeometry(240, 120, 80, 30);

    // Connect the OK and Cancel buttons to slots that close the dialog box
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Show the dialog box and wait for the user to select an option
    if (dialog.exec() == QDialog::Accepted)
    {
        // Handle the user's selection
        if (!icon1_.isNull() && !icon2_.isNull())
        {
            isIconsSet = true;
            update_ui();
        }
        else
        {
            QMessageBox::warning(this, "Not enough icons", "You need to select 2 icons.");
        }
    }
}

/**
 * @brief This function is called when the user clicks the "Set Goal" button.
 *  It sets the goal for the game and updates the UI to reflect the new goal.
 *  It also sets the isSetGoal_ variable to true, indicating that a goal has been set.
 *  The goal value is obtained from the goals_spinbox, a QSpinBox widget.
 *  The goal_display QLabel is updated to display the new goal value using QString::number().
 */
void MainWindow::handle_set_goal_button()
{
    isSetGoal_ = true;
    goal_ = goals_spinbox->value();
    goal_display->setText(QString::number(goal_)); // Convert the integer to a QString
}
