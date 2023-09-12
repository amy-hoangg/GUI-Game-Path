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

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTimer>
#include <QUndoStack>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handle_button_clicks();
    void handle_pause_and_resume_button();
    void handle_quit_button();
    void handle_reset_button();
    void handle_setting_color_button();
    void handle_setting_icon_button();
    void handle_set_goal_button();
    void handle_start_button();
    void handle_undo_button();
    void update_timer_label();

private:
    Ui::MainWindow *ui;

    const int maxSelectedIcons = 2;
    unsigned int goal_;
    unsigned int total_moves = 0;
    int elapsed_seconds = 0;

    Point p1_;
    Point p2_;

    QPixmap icon1_;
    QPixmap icon2_;

    QColor game_piece_color1;
    QColor game_piece_color2;

    QVBoxLayout *main_layout;
    QHBoxLayout *topmost_layout = new QHBoxLayout();
    QHBoxLayout *top_layout = new QHBoxLayout();
    QHBoxLayout *outside_middle_layout = new QHBoxLayout();
    QVBoxLayout *inside_middle_layout = new QVBoxLayout();
    QHBoxLayout *bottom_layout = new QHBoxLayout();
    QGridLayout *game_board_layout;

    GameBoard *game_board;
    vector<vector<Slot_type>> game_board_data_;
    vector<GameBoard> game_board_history_;

    QPushButton* button1_ = nullptr;
    QPushButton* button2_ = nullptr;
    QPushButton *start_button;
    QPushButton *quit_button;
    QPushButton *pause_and_resume_button;
    QPushButton *undo_button;
    QPushButton *setting_icon_button;
    QPushButton *set_goal_button = new QPushButton("Ok");
    QLabel *goals_name = new QLabel("Setting Goals");
    QSpinBox *goals_spinbox = new QSpinBox();
    QLabel *moves_name_label = new QLabel("Number of moves: ");
    QLabel *time_name_label = new QLabel("Time: ");
    QLabel *goal_display = new QLabel("No Goal");
    QTimer *timer;
    QLabel *timer_label;
    QLabel *number_of_moves;
    QPushButton *reset_button;
    QPushButton *setting_color_button;

    void disableButtons();
    void init_boards();
    void update_ui();

    bool isPauseClicked;
    bool isStarted = false;
    bool isIconsSet = false;
    bool isSetGoal_ = false;

};
#endif // MAINWINDOW_HH
